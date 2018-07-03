
#include "compat.h"
#include "mzd_additional.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const size_t mzd_local_t_size = (sizeof(mzd_local_t) + 0x1f) & ~0x1f;
static_assert(((sizeof(mzd_local_t) + 0x1f) & ~0x1f) == 32, "sizeof mzd_local_t not supported");

static const unsigned int align_bound = 128 / (8 * sizeof(word));

static uint32_t calculate_rowstride(uint32_t width) {
  // As soon as we hit the AVX bound, use 32 byte alignment. Otherwise use 16
  // byte alignment for SSE.
  if (width > align_bound) {
    return ((width * sizeof(word) + 31) & ~31) / sizeof(word);
  } else {
    return ((width * sizeof(word) + 15) & ~15) / sizeof(word);
  }
}

// Notes on the memory layout: mzd_init allocates multiple memory blocks (one
// for mzd_local_t, one for rows and multiple for the buffers). We use one memory
// block for mzd_local_t, rows and the buffer. This improves memory locality and
// requires less calls to malloc.
//
// In mzd_local_init_multiple we do the same, but store n mzd_local_t instances in one
// memory block.

mzd_local_t* mzd_local_init_ex(uint32_t r, uint32_t c, bool clear) {
  const uint32_t width     = (c + 64 - 1) / 64;
  const uint32_t rowstride = calculate_rowstride(width);

  const size_t buffer_size = r * rowstride * sizeof(word);

  unsigned char* buffer = aligned_alloc(32, (mzd_local_t_size + buffer_size + 31) & ~31);

  mzd_local_t* A = (mzd_local_t*)buffer;
  buffer += mzd_local_t_size;

  if (clear) {
    memset(buffer, 0, buffer_size);
  }

  // assign in order
  A->nrows     = r;
  A->ncols     = c;
  A->width     = width;
  A->rowstride = rowstride;

  return A;
}

void mzd_local_free(mzd_local_t* v) {
  aligned_free(v);
}

void mzd_local_init_multiple_ex(mzd_local_t** dst, size_t n, uint32_t r, uint32_t c, bool clear) {
  const uint32_t width     = (c + 64 - 1) / 64;
  const uint32_t rowstride = calculate_rowstride(width);

  const size_t buffer_size   = r * rowstride * sizeof(word);
  const size_t size_per_elem = (mzd_local_t_size + buffer_size + 31) & ~31;

  unsigned char* full_buffer = aligned_alloc(32, size_per_elem * n);

  for (size_t s = 0; s < n; ++s, full_buffer += size_per_elem) {
    unsigned char* buffer = full_buffer;
    mzd_local_t* A        = (mzd_local_t*)buffer;
    dst[s]                = A;

    buffer += mzd_local_t_size;

    if (clear) {
      memset(buffer, 0, buffer_size);
    }

    // assign in order
    A->nrows     = r;
    A->ncols     = c;
    A->width     = width;
    A->rowstride = rowstride;
  }
}

void mzd_local_free_multiple(mzd_local_t** vs) {
  if (vs) {
    aligned_free(vs[0]);
  }
}

mzd_local_t* mzd_local_copy(mzd_local_t* dst, mzd_local_t const* src) {
  if (dst == src) {
    return dst;
  }

  if (!dst) {
    dst = mzd_local_init(src->nrows, src->ncols);
  }

  memcpy(ASSUME_ALIGNED(FIRST_ROW(dst), 32), ASSUME_ALIGNED(CONST_FIRST_ROW(src), 32),
         src->nrows * sizeof(word) * src->rowstride);
  return dst;
}

void mzd_local_clear(mzd_local_t* c) {
  memset(ASSUME_ALIGNED(FIRST_ROW(c), 32), 0, c->nrows * sizeof(word) * c->rowstride);
}

void mzd_shift_right(mzd_local_t* res, mzd_local_t const* val, unsigned count) {
  if (!count) {
    mzd_local_copy(res, val);
    return;
  }

  const unsigned int nwords     = val->width;
  const unsigned int left_count = 8 * sizeof(word) - count;

  word* resptr       = FIRST_ROW(res);
  word const* valptr = CONST_FIRST_ROW(val);

  for (unsigned int i = nwords - 1; i; --i, ++resptr) {
    const word tmp = *valptr >> count;
    *resptr        = tmp | (*++valptr << left_count);
  }
  *resptr = *valptr >> count;
}

void mzd_shift_left(mzd_local_t* res, mzd_local_t const* val, unsigned count) {
  if (!count) {
    mzd_local_copy(res, val);
    return;
  }

  const unsigned int nwords      = val->width;
  const unsigned int right_count = 8 * sizeof(word) - count;

  word* resptr       = FIRST_ROW(res) + nwords - 1;
  word const* valptr = CONST_FIRST_ROW(val) + nwords - 1;

  for (unsigned int i = nwords - 1; i; --i, --resptr) {
    const word tmp = *valptr << count;
    *resptr        = tmp | (*--valptr >> right_count);
  }
  *resptr = *valptr << count;
}


mzd_local_t* mzd_and(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {

  unsigned int width    = first->width;
  word* resptr          = FIRST_ROW(res);
  word const* firstptr  = CONST_FIRST_ROW(first);
  word const* secondptr = CONST_FIRST_ROW(second);

  while (width--) {
    *resptr++ = *firstptr++ & *secondptr++;
  }

  return res;
}


mzd_local_t* mzd_xor(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  return mzd_xor_general(res, first, second);
}

mzd_local_t* mzd_xor_general(mzd_local_t* res, mzd_local_t const* first,
                             mzd_local_t const* second) {
  unsigned int width    = first->width;
  word* resptr          = FIRST_ROW(res);
  word const* firstptr  = CONST_FIRST_ROW(first);
  word const* secondptr = CONST_FIRST_ROW(second);

  while (width--) {
    *resptr++ = *firstptr++ ^ *secondptr++;
  }

  return res;
}

mzd_local_t* mzd_mul_v(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  if (At->nrows != v->ncols) {
    // number of columns does not match
    return NULL;
  }

  mzd_local_clear(c);
  return mzd_addmul_v(c, v, At);
}

mzd_local_t* mzd_mul_v_general(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {

  if (At->nrows != v->ncols) {
    // number of columns does not match
    return NULL;
  }

  mzd_local_clear(c);
  return mzd_addmul_v_general(c, v, At);
}


mzd_local_t* mzd_addmul_v(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  if (A->ncols != c->ncols || A->nrows != v->ncols) {
    // number of columns does not match
    return NULL;
  }


  return mzd_addmul_v_general(c, v, A);
}

mzd_local_t* mzd_addmul_v_general(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {

  const unsigned int len       = A->width;
  const unsigned int rowstride = A->rowstride;
  word* cptr                   = FIRST_ROW(c);
  word const* vptr             = CONST_FIRST_ROW(v);
  const unsigned int width     = v->width;

  for (unsigned int w = 0; w < width; ++w, ++vptr) {
    word idx = *vptr;

    word const* Aptr = CONST_ROW(A, w * sizeof(word) * 8);
    while (idx) {
      if (idx & 0x1) {
        for (unsigned int i = 0; i < len; ++i) {
          cptr[i] ^= Aptr[i];
        }
      }

      Aptr += rowstride;
      idx >>= 1;
    }
  }

  return c;
}

bool mzd_local_equal(mzd_local_t const* first, mzd_local_t const* second) {
  if (first == second) {
    return true;
  }
  if (first->ncols != second->ncols || first->nrows != second->nrows) {
    return false;
  }

  const unsigned int rows  = first->nrows;
  const unsigned int width = first->width;

  for (unsigned int r = 0; r < rows; ++r) {
    if (memcmp(ASSUME_ALIGNED(CONST_ROW(first, r), 32), ASSUME_ALIGNED(CONST_ROW(second, r), 32),
               sizeof(word) * width) != 0) {
      return false;
    }
  }

  return true;
}

static void xor_comb(const unsigned int len, word* Brow, mzd_local_t const* A,
                     unsigned int r_offset, unsigned comb) {
  while (comb) {
    const word* Arow = CONST_ROW(A, r_offset);
    if (comb & 0x1) {
      for (unsigned int i = 0; i < len; ++i) {
        Brow[i] ^= Arow[i];
      }
    }

    comb >>= 1;
    ++r_offset;
  }
}

/**
 * Pre-compute matrices for faster mzd_addmul_v computions.
 *
 */
mzd_local_t* mzd_precompute_matrix_lookup(mzd_local_t const* A) {
  mzd_local_t* B = mzd_local_init_ex(32 * A->nrows, A->ncols, true);

  const unsigned int len = A->width;

  for (unsigned int r = 0; r < B->nrows; ++r) {
    const unsigned int comb     = r & 0xff;
    const unsigned int r_offset = (r >> 8) << 3;
    if (!comb) {
      continue;
    }

    xor_comb(len, ROW(B, r), A, r_offset, comb);
  }

  return B;
}


mzd_local_t* mzd_mul_vl(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  if (A->nrows != 32 * v->ncols) {
    // number of columns does not match
    return NULL;
  }

  mzd_local_clear(c);
  return mzd_addmul_vl(c, v, A);
}

mzd_local_t* mzd_mul_vl_general(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  mzd_local_clear(c);
  return mzd_addmul_vl_general(c, v, A);
}

mzd_local_t* mzd_addmul_vl(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  if (A->ncols != c->ncols || A->nrows != 32 * v->ncols) {
    // number of columns does not match
    return NULL;
  }

  return mzd_addmul_vl_general(c, v, A);
}

mzd_local_t* mzd_addmul_vl_general(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const unsigned int len   = A->width;
  word* cptr               = FIRST_ROW(c);
  word const* vptr         = CONST_FIRST_ROW(v);
  const unsigned int width = v->width;

  for (unsigned int w = 0; w < width; ++w, ++vptr) {
    word idx         = *vptr;
    unsigned int add = 0;

    while (idx) {
      const word comb = idx & 0xff;

      word const* Aptr = CONST_ROW(A, w * sizeof(word) * 8 * 32 + add + comb);
      for (unsigned int i = 0; i < len; ++i) {
        cptr[i] ^= Aptr[i];
      }

      idx >>= 8;
      add += 256;
    }
  }

  return c;
}
