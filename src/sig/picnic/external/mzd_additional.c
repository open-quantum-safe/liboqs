/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "compat.h"
#include "mzd_additional.h"

#if !defined(_MSC_VER)
#include <stdalign.h>
#endif
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#if !defined(_MSC_VER) && !defined(static_assert)
#define static_assert _Static_assert
#endif

static_assert(((sizeof(mzd_local_t) + 0x1f) & ~0x1f) == 32, "sizeof mzd_local_t not supported");

#if defined(WITH_OPT)
#include "simd.h"

#if defined(WITH_SSE2)
#define ATTR_TARGET_S128 ATTR_TARGET_SSE2
#else
#define ATTR_TARGET_S128
#endif

#if defined(WITH_POPCNT)
#include <nmmintrin.h>

#if !defined(__x86_64__) && !defined(_M_X64)
ATTR_TARGET("popcnt") ATTR_CONST static inline uint64_t parity64_popcnt(uint64_t in) {
  return (_mm_popcnt_u32(in >> 32) ^ _mm_popcnt_u32(in)) & 0x1;
}
#else
ATTR_TARGET("popcnt") ATTR_CONST static inline uint64_t parity64_popcnt(uint64_t in) {
  return _mm_popcnt_u64(in) & 0x1;
}
#endif
#endif
#endif
static const unsigned int align_bound = 128 / (8 * sizeof(word));

static uint32_t calculate_rowstride(uint32_t width) {
  // As soon as we hit the AVX bound, use 32 byte alignment. Otherwise use 16
  // byte alignment for SSE2 and 128 bit vectors.
  if (width > align_bound) {
    return ((width * sizeof(word) + 31) & ~31) / sizeof(word);
  } else {
    return ((width * sizeof(word) + 15) & ~15) / sizeof(word);
  }
}

static uint32_t calculate_width(uint32_t c) {
  return (c + sizeof(word) * 8 - 1) / (sizeof(word) * 8);
}

// Notes on the memory layout: mzd_init allocates multiple memory blocks (one
// for mzd_local_t, one for rows and multiple for the buffers). We use one memory
// block for mzd_local_t, rows and the buffer. This improves memory locality and
// requires less calls to malloc.
//
// In mzd_local_init_multiple we do the same, but store n mzd_local_t instances in one
// memory block.

mzd_local_t* oqs_sig_picnic_mzd_local_init_ex(uint32_t r, uint32_t c, bool clear) {
  const uint32_t width     = calculate_width(c);
  const uint32_t rowstride = calculate_rowstride(width);

  const size_t buffer_size = r * rowstride * sizeof(word);
  const size_t alloc_size = (buffer_size + 31) & ~31;

  /* We always align mzd_local_ts to 32 bytes. Thus the first row is always
   * aligned to 32 bytes as well. For 128 bit and SSE all other rows are then
   * aligned to 16 bytes. */
  unsigned char* buffer = aligned_alloc(32, alloc_size);
  if (clear) {
    memset(buffer, 0, alloc_size);
  }

  return (mzd_local_t*)buffer;
}

void oqs_sig_picnic_mzd_local_free(mzd_local_t* v) {
  aligned_free(v);
}

void oqs_sig_picnic_mzd_local_init_multiple_ex(mzd_local_t** dst, size_t n, uint32_t r, uint32_t c, bool clear) {
  const uint32_t width     = calculate_width(c);
  const uint32_t rowstride = calculate_rowstride(width);

  const size_t buffer_size   = r * rowstride * sizeof(word);
  const size_t size_per_elem = (buffer_size + 31) & ~31;

  unsigned char* full_buffer = aligned_alloc(32, size_per_elem * n);
  if (clear) {
    memset(full_buffer, 0, size_per_elem * n);
  }

  for (size_t s = 0; s < n; ++s, full_buffer += size_per_elem) {
    unsigned char* buffer = full_buffer;
    dst[s] = (mzd_local_t*)buffer;
  }
}

void oqs_sig_picnic_mzd_local_free_multiple(mzd_local_t** vs) {
  if (vs) {
    aligned_free(vs[0]);
  }
}

#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
ATTR_TARGET_S128
void oqs_sig_picnic_mzd_xor_s128_128(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  block_t* rblock       = BLOCK(res, 0);
  const block_t* fblock = CONST_BLOCK(first, 0);
  const block_t* sblock = CONST_BLOCK(second, 0);

  rblock->w128[0] = mm128_xor(fblock->w128[0], sblock->w128[0]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_xor_s128_blocks(block_t* rblock, const block_t* fblock, const block_t* sblock,
                         unsigned int count) {
  for (; count; --count, ++rblock, ++fblock, ++sblock) {
    rblock->w128[0] = mm128_xor(fblock->w128[0], sblock->w128[0]);
    rblock->w128[1] = mm128_xor(fblock->w128[1], sblock->w128[1]);
  }
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_xor_s128_256(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s128_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 1);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_xor_s128_640(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s128_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 2);
  BLOCK(res, 2)->w128[0] =
      mm128_xor(CONST_BLOCK(first, 2)->w128[0], CONST_BLOCK(second, 2)->w128[0]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_xor_s128_896(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s128_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 3);
  BLOCK(res, 3)->w128[0] =
      mm128_xor(CONST_BLOCK(first, 3)->w128[0], CONST_BLOCK(second, 3)->w128[0]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_xor_s128_1024(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s128_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 4);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_xor_s128_1152(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s128_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 4);
  BLOCK(res, 4)->w128[0] =
      mm128_xor(CONST_BLOCK(first, 4)->w128[0], CONST_BLOCK(second, 4)->w128[0]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_xor_s128_1280(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s128_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 5);
}
#endif

#if defined(WITH_AVX2)
ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_xor_s256_128(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  block_t* rblock       = BLOCK(res, 0);
  const block_t* fblock = CONST_BLOCK(first, 0);
  const block_t* sblock = CONST_BLOCK(second, 0);

  rblock->w128[0] = mm128_xor(fblock->w128[0], sblock->w128[0]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_xor_s256_blocks(block_t* rblock, const block_t* fblock, const block_t* sblock,
                         unsigned int count) {
  for (; count; --count, ++rblock, ++fblock, ++sblock) {
    rblock->w256 = mm256_xor(fblock->w256, sblock->w256);
  }
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_xor_s256_256(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 1);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_xor_s256_768(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 3);
}

void oqs_sig_picnic_mzd_xor_s256_1024(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 4);
}

void oqs_sig_picnic_mzd_xor_s256_1280(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 5);
}
#endif
#endif

static void oqs_sig_picnic_mzd_xor_uint64_block(block_t* rblock, const block_t* fblock, const block_t* sblock,
                                 const unsigned int len) {
  for (unsigned int i = 0; i < len; ++i) {
    rblock->w64[i] = fblock->w64[i] ^ sblock->w64[i];
  }
}

static void oqs_sig_picnic_mzd_xor_uint64_blocks(block_t* rblock, const block_t* fblock, const block_t* sblock,
                                 const unsigned int len) {
  for (unsigned int i = len; i; --i, ++rblock, ++fblock, ++sblock) {
    oqs_sig_picnic_mzd_xor_uint64_block(rblock, fblock, sblock, 4);
  }
}

void oqs_sig_picnic_mzd_xor_uint64_128(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_uint64_block(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 2);
}

void oqs_sig_picnic_mzd_xor_uint64_192(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_uint64_block(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 3);
}

void oqs_sig_picnic_mzd_xor_uint64_256(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_uint64_block(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 4);
}

void oqs_sig_picnic_mzd_xor_uint64_576(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_uint64_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 2);
  oqs_sig_picnic_mzd_xor_uint64_block(BLOCK(res, 2), CONST_BLOCK(first, 2), CONST_BLOCK(second, 2), 1);
}

void oqs_sig_picnic_mzd_xor_uint64_640(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_uint64_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 2);
  oqs_sig_picnic_mzd_xor_uint64_block(BLOCK(res, 2), CONST_BLOCK(first, 2), CONST_BLOCK(second, 2), 2);
}

void oqs_sig_picnic_mzd_xor_uint64_896(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_uint64_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 3);
  oqs_sig_picnic_mzd_xor_uint64_block(BLOCK(res, 3), CONST_BLOCK(first, 3), CONST_BLOCK(second, 3), 2);
}

void oqs_sig_picnic_mzd_xor_uint64_960(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_uint64_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 3);
  oqs_sig_picnic_mzd_xor_uint64_block(BLOCK(res, 3), CONST_BLOCK(first, 3), CONST_BLOCK(second, 3), 3);
}

void oqs_sig_picnic_mzd_xor_uint64_1152(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_uint64_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 4);
  oqs_sig_picnic_mzd_xor_uint64_block(BLOCK(res, 4), CONST_BLOCK(first, 4), CONST_BLOCK(second, 4), 2);
}

void oqs_sig_picnic_mzd_xor_uint64_1216(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) {
  oqs_sig_picnic_mzd_xor_uint64_blocks(BLOCK(res, 0), CONST_BLOCK(first, 0), CONST_BLOCK(second, 0), 4);
  oqs_sig_picnic_mzd_xor_uint64_block(BLOCK(res, 4), CONST_BLOCK(first, 4), CONST_BLOCK(second, 4), 3);
}

void oqs_sig_picnic_mzd_mul_v_parity_uint64_128_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  cblock->w64[0] = 0;

  word res = 0;
  for (unsigned int i = 15; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 15 - i);
    const word parity1 =
        parity64_uint64((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]));
    const word parity2 =
        parity64_uint64((vblock->w64[0] & Ablock->w64[2]) ^ (vblock->w64[1] & Ablock->w64[3]));
    res |= (parity1 | (parity2 << 1)) << (64 - (2 * i));
  }
  cblock->w64[1] = res;
}

void oqs_sig_picnic_mzd_mul_v_parity_uint64_192_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  for (unsigned int j = 0; j < 2; j++) {
    cblock->w64[j] = 0;
  }

  word res = 0;
  for (unsigned int i = 30; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 30 - i);
    const word parity =
        parity64_uint64((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]) ^
                        (vblock->w64[2] & Ablock->w64[2]));
    res |= parity << (64 - i);
  }
  cblock->w64[2] = res;
}

void oqs_sig_picnic_mzd_mul_v_parity_uint64_256_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  for (unsigned int j = 0; j < 3; j++) {
    cblock->w64[j] = 0;
  }

  word res = 0;
  for (unsigned int i = 30; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 30 - i);
    const word parity =
        parity64_uint64((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]) ^
                        (vblock->w64[2] & Ablock->w64[2]) ^ (vblock->w64[3] & Ablock->w64[3]));
    res |= parity << (64 - i);
  }
  cblock->w64[3] = res;
}

void oqs_sig_picnic_mzd_mul_v_parity_uint64_128_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  cblock->w64[0] = 0;

  const block_t* Ablock1 = CONST_BLOCK(At, 0);
  const block_t* Ablock2 = CONST_BLOCK(At, 1);

  const word parity1 =
      parity64_uint64((vblock->w64[0] & Ablock1->w64[0]) ^ (vblock->w64[1] & Ablock1->w64[1]));
  const word parity2 =
      parity64_uint64((vblock->w64[0] & Ablock1->w64[2]) ^ (vblock->w64[1] & Ablock1->w64[3]));
  const word parity3 =
      parity64_uint64((vblock->w64[0] & Ablock2->w64[0]) ^ (vblock->w64[1] & Ablock2->w64[1]));

  cblock->w64[1] = (parity1 | (parity2 << 1) | (parity3 << 2)) << 61;
}

void oqs_sig_picnic_mzd_mul_v_parity_uint64_192_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  for (unsigned int j = 0; j < 3; j++) {
    cblock->w64[j] = 0;
  }

  word res = 0;
  for (unsigned int i = 3; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 3 - i);
    const word parity =
        parity64_uint64((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]) ^
                        (vblock->w64[2] & Ablock->w64[2]));
    res |= parity << (64 - i);
  }
  cblock->w64[2] = res;
}

void oqs_sig_picnic_mzd_mul_v_parity_uint64_256_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  for (unsigned int j = 0; j < 3; j++) {
    cblock->w64[j] = 0;
  }

  word res = 0;
  for (unsigned int i = 3; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 3 - i);
    const word parity =
        parity64_uint64((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]) ^
                        (vblock->w64[2] & Ablock->w64[2]) ^ (vblock->w64[3] & Ablock->w64[3]));
    res |= parity << (64 - i);
  }
  cblock->w64[3] = res;
}

#if defined(WITH_OPT)
#if defined(WITH_POPCNT)
ATTR_TARGET("popcnt")
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_128_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  cblock->w64[0] = 0;

  word res = 0;
  for (unsigned int i = 15; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 15 - i);
    const word parity1 =
        parity64_popcnt((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]));
    const word parity2 =
        parity64_popcnt((vblock->w64[0] & Ablock->w64[2]) ^ (vblock->w64[1] & Ablock->w64[3]));
    res |= (parity1 | (parity2 << 1)) << (64 - (2 * i));
  }
  cblock->w64[1] = res;
}

ATTR_TARGET("popcnt")
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_192_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  for (unsigned int j = 0; j < 2; j++) {
    cblock->w64[j] = 0;
  }

  word res = 0;
  for (unsigned int i = 30; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 30 - i);
    const word parity =
        parity64_popcnt((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]) ^
                        (vblock->w64[2] & Ablock->w64[2]));
    res |= parity << (64 - i);
  }
  cblock->w64[2] = res;
}

ATTR_TARGET("popcnt")
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_256_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  for (unsigned int j = 0; j < 3; j++) {
    cblock->w64[j] = 0;
  }

  word res = 0;
  for (unsigned int i = 30; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 30 - i);
    const word parity =
        parity64_popcnt((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]) ^
                        (vblock->w64[2] & Ablock->w64[2]) ^ (vblock->w64[3] & Ablock->w64[3]));
    res |= parity << (64 - i);
  }
  cblock->w64[3] = res;
}

ATTR_TARGET("popcnt")
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_128_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  cblock->w64[0] = 0;

  const block_t* Ablock1 = CONST_BLOCK(At, 0);
  const block_t* Ablock2 = CONST_BLOCK(At, 1);

  const word parity1 =
      parity64_popcnt((vblock->w64[0] & Ablock1->w64[0]) ^ (vblock->w64[1] & Ablock1->w64[1]));
  const word parity2 =
      parity64_popcnt((vblock->w64[0] & Ablock1->w64[2]) ^ (vblock->w64[1] & Ablock1->w64[3]));
  const word parity3 =
      parity64_popcnt((vblock->w64[0] & Ablock2->w64[0]) ^ (vblock->w64[1] & Ablock2->w64[1]));

  cblock->w64[1] = (parity1 | (parity2 << 1) | (parity3 << 2)) << 61;
}

ATTR_TARGET("popcnt")
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_192_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  for (unsigned int j = 0; j < 3; j++) {
    cblock->w64[j] = 0;
  }

  word res = 0;
  for (unsigned int i = 3; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 3 - i);
    const word parity =
        parity64_popcnt((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]) ^
                        (vblock->w64[2] & Ablock->w64[2]));
    res |= parity << (64 - i);
  }
  cblock->w64[2] = res;
}

ATTR_TARGET("popcnt")
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_256_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);

  for (unsigned int j = 0; j < 3; j++) {
    cblock->w64[j] = 0;
  }

  word res = 0;
  for (unsigned int i = 3; i; --i) {
    const block_t* Ablock = CONST_BLOCK(At, 3 - i);
    const word parity =
        parity64_popcnt((vblock->w64[0] & Ablock->w64[0]) ^ (vblock->w64[1] & Ablock->w64[1]) ^
                        (vblock->w64[2] & Ablock->w64[2]) ^ (vblock->w64[3] & Ablock->w64[3]));
    res |= parity << (64 - i);
  }
  cblock->w64[3] = res;
}
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
ATTR_TARGET_S128 ATTR_CONST static inline word128 mm128_compute_mask(const word idx,
                                                                     const size_t bit) {
#if defined(WITH_SSE2)
  return _mm_set1_epi64x(-((idx >> bit) & 1));
#else
  return vdupq_n_u64(-((idx >> bit) & 1));
#endif
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_v_s128_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero, mm128_zero};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 4, idx >>= 4, Ablock += 2) {
      cval[0] = mm128_xor_mask(cval[0], Ablock[0].w128[0], mm128_compute_mask(idx, 0));
      cval[1] = mm128_xor_mask(cval[1], Ablock[0].w128[1], mm128_compute_mask(idx, 1));
      cval[0] = mm128_xor_mask(cval[0], Ablock[1].w128[0], mm128_compute_mask(idx, 2));
      cval[1] = mm128_xor_mask(cval[1], Ablock[1].w128[1], mm128_compute_mask(idx, 3));
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[1]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_v_s128_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {cblock->w128[0], mm128_zero, mm128_zero,
                                                    mm128_zero};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 4, idx >>= 4, Ablock += 2) {
      cval[0] = mm128_xor_mask(cval[0], Ablock[0].w128[0], mm128_compute_mask(idx, 0));
      cval[1] = mm128_xor_mask(cval[1], Ablock[0].w128[1], mm128_compute_mask(idx, 1));
      cval[0] = mm128_xor_mask(cval[0], Ablock[1].w128[0], mm128_compute_mask(idx, 2));
      cval[1] = mm128_xor_mask(cval[1], Ablock[1].w128[1], mm128_compute_mask(idx, 3));
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[1]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_v_s128_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero, mm128_zero};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 2, idx >>= 2, Ablock += 2) {
      mm128_xor_mask_region(&cval[0], Ablock[0].w128, mm128_compute_mask(idx, 0), 2);
      mm128_xor_mask_region(&cval[2], Ablock[1].w128, mm128_compute_mask(idx, 1), 2);
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_v_s128_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {cblock->w128[0], cblock->w128[1], mm128_zero,
                                                    mm128_zero};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 2, idx >>= 2, Ablock += 2) {
      mm128_xor_mask_region(&cval[0], Ablock[0].w128, mm128_compute_mask(idx, 0), 2);
      mm128_xor_mask_region(&cval[2], Ablock[1].w128, mm128_compute_mask(idx, 1), 2);
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_v_s128_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero, mm128_zero};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 2, idx >>= 2, Ablock += 2) {
      mm128_xor_mask_region(&cval[0], Ablock[0].w128, mm128_compute_mask(idx, 0), 2);
      mm128_xor_mask_region(&cval[2], Ablock[1].w128, mm128_compute_mask(idx, 1), 2);
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_v_s128_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {cblock->w128[0], cblock->w128[1], mm128_zero,
                                                    mm128_zero};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 2, idx >>= 2, Ablock += 2) {
      mm128_xor_mask_region(&cval[0], Ablock[0].w128, mm128_compute_mask(idx, 0), 2);
      mm128_xor_mask_region(&cval[2], Ablock[1].w128, mm128_compute_mask(idx, 1), 2);
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_v_s128_128_640(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[5] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero, mm128_zero,
                                                    mm128_zero};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 1, idx >>= 1, Ablock += 3) {
      const word128 mask = mm128_compute_mask(idx, 0);
      mm128_xor_mask_region(&cval[0], Ablock[0].w128, mask, 2);
      mm128_xor_mask_region(&cval[2], Ablock[1].w128, mask, 2);
      cval[4] = mm128_xor_mask(cval[4], Ablock[2].w128[0], mask);
    }
  }

  block_t* cblock1 = BLOCK(c, 0);
  block_t* cblock2 = BLOCK(c, 1);
  block_t* cblock3 = BLOCK(c, 2);
  cblock1->w128[0] = cval[0];
  cblock1->w128[1] = cval[1];
  cblock2->w128[0] = cval[2];
  cblock2->w128[1] = cval[3];
  cblock3->w128[0] = cval[4];
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_v_s128_192_896(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[7] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero, mm128_zero,
                                                    mm128_zero, mm128_zero, mm128_zero};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 1, idx >>= 1, Ablock += 4) {
      const word128 mask = mm128_compute_mask(idx, 0);
      mm128_xor_mask_region(&cval[0], Ablock[0].w128, mask, 2);
      mm128_xor_mask_region(&cval[2], Ablock[1].w128, mask, 2);
      mm128_xor_mask_region(&cval[4], Ablock[2].w128, mask, 2);
      cval[6] = mm128_xor_mask(cval[6], Ablock[3].w128[0], mask);
    }
  }

  block_t* cblock1 = BLOCK(c, 0);
  block_t* cblock2 = BLOCK(c, 1);
  block_t* cblock3 = BLOCK(c, 2);
  block_t* cblock4 = BLOCK(c, 3);
  cblock1->w128[0] = cval[0];
  cblock1->w128[1] = cval[1];
  cblock2->w128[0] = cval[2];
  cblock2->w128[1] = cval[3];
  cblock3->w128[0] = cval[4];
  cblock3->w128[1] = cval[5];
  cblock4->w128[0] = cval[6];
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_v_s128_192_1024(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[8] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero, mm128_zero,
                                                    mm128_zero, mm128_zero, mm128_zero, mm128_zero};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 1, idx >>= 1, Ablock += 4) {
      const word128 mask = mm128_compute_mask(idx, 0);
      mm128_xor_mask_region(&cval[0], Ablock[0].w128, mask, 2);
      mm128_xor_mask_region(&cval[2], Ablock[1].w128, mask, 2);
      mm128_xor_mask_region(&cval[4], Ablock[2].w128, mask, 2);
      mm128_xor_mask_region(&cval[6], Ablock[3].w128, mask, 2);
    }
  }

  block_t* cblock1 = BLOCK(c, 0);
  block_t* cblock2 = BLOCK(c, 1);
  block_t* cblock3 = BLOCK(c, 2);
  block_t* cblock4 = BLOCK(c, 3);
  cblock1->w128[0] = cval[0];
  cblock1->w128[1] = cval[1];
  cblock2->w128[0] = cval[2];
  cblock2->w128[1] = cval[3];
  cblock3->w128[0] = cval[4];
  cblock3->w128[1] = cval[5];
  cblock4->w128[0] = cval[6];
  cblock4->w128[1] = cval[7];
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_v_s128_256_1152(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[9] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero,
                                                    mm128_zero, mm128_zero, mm128_zero,
                                                    mm128_zero, mm128_zero, mm128_zero};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 1, idx >>= 1, Ablock += 5) {
      const word128 mask = mm128_compute_mask(idx, 0);
      mm128_xor_mask_region(&cval[0], Ablock[0].w128, mask, 2);
      mm128_xor_mask_region(&cval[2], Ablock[1].w128, mask, 2);
      mm128_xor_mask_region(&cval[4], Ablock[2].w128, mask, 2);
      mm128_xor_mask_region(&cval[6], Ablock[3].w128, mask, 2);
      cval[8] = mm128_xor_mask(cval[8], Ablock[4].w128[0], mask);
    }
  }

  block_t* cblock1 = BLOCK(c, 0);
  block_t* cblock2 = BLOCK(c, 1);
  block_t* cblock3 = BLOCK(c, 2);
  block_t* cblock4 = BLOCK(c, 3);
  block_t* cblock5 = BLOCK(c, 4);
  cblock1->w128[0] = cval[0];
  cblock1->w128[1] = cval[1];
  cblock2->w128[0] = cval[2];
  cblock2->w128[1] = cval[3];
  cblock3->w128[0] = cval[4];
  cblock3->w128[1] = cval[5];
  cblock4->w128[0] = cval[6];
  cblock4->w128[1] = cval[7];
  cblock5->w128[0] = cval[8];
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_v_s128_256_1280(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[10] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero, mm128_zero,
                                                     mm128_zero, mm128_zero, mm128_zero, mm128_zero,
                                                     mm128_zero, mm128_zero};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 1, idx >>= 1, Ablock += 5) {
      const word128 mask = mm128_compute_mask(idx, 0);
      mm128_xor_mask_region(&cval[0], Ablock[0].w128, mask, 2);
      mm128_xor_mask_region(&cval[2], Ablock[1].w128, mask, 2);
      mm128_xor_mask_region(&cval[4], Ablock[2].w128, mask, 2);
      mm128_xor_mask_region(&cval[6], Ablock[3].w128, mask, 2);
      mm128_xor_mask_region(&cval[8], Ablock[4].w128, mask, 2);
    }
  }

  block_t* cblock1 = BLOCK(c, 0);
  block_t* cblock2 = BLOCK(c, 1);
  block_t* cblock3 = BLOCK(c, 2);
  block_t* cblock4 = BLOCK(c, 3);
  block_t* cblock5 = BLOCK(c, 4);
  cblock1->w128[0] = cval[0];
  cblock1->w128[1] = cval[1];
  cblock2->w128[0] = cval[2];
  cblock2->w128[1] = cval[3];
  cblock3->w128[0] = cval[4];
  cblock3->w128[1] = cval[5];
  cblock4->w128[0] = cval[6];
  cblock4->w128[1] = cval[7];
  cblock5->w128[0] = cval[8];
  cblock5->w128[1] = cval[9];
}
#endif

#if defined(WITH_AVX2)
ATTR_TARGET_AVX2
ATTR_CONST static inline word256 mm256_compute_mask(const word idx, const size_t bit) {
  return _mm256_set1_epi64x(-((idx >> bit) & 1));
}

ATTR_TARGET_AVX2
ATTR_CONST static inline word256 mm256_compute_mask_2(const word idx, const size_t bit) {
  const uint64_t m1 = -((idx >> bit) & 1);
  const uint64_t m2 = -((idx >> (bit + 1)) & 1);
  return _mm256_set_epi64x(m2, m2, m1, m1);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_v_s256_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {_mm256_castsi128_si256(cblock->w128[0]),
                                                    _mm256_setzero_si256()};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 8, idx >>= 8, Ablock += 4) {
      cval[0] = mm256_xor_mask(cval[0], Ablock[0].w256, mm256_compute_mask_2(idx, 0));
      cval[1] = mm256_xor_mask(cval[1], Ablock[1].w256, mm256_compute_mask_2(idx, 2));
      cval[0] = mm256_xor_mask(cval[0], Ablock[2].w256, mm256_compute_mask_2(idx, 4));
      cval[1] = mm256_xor_mask(cval[1], Ablock[3].w256, mm256_compute_mask_2(idx, 6));
    }
  }
  cval[0] = _mm256_xor_si256(cval[0], cval[1]);
  cblock->w128[0] =
      _mm_xor_si128(_mm256_extractf128_si256(cval[0], 0), _mm256_extractf128_si256(cval[0], 1));
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_v_s256_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {_mm256_setzero_si256(), _mm256_setzero_si256()};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 8, idx >>= 8, Ablock += 4) {
      cval[0] = mm256_xor_mask(cval[0], Ablock[0].w256, mm256_compute_mask_2(idx, 0));
      cval[1] = mm256_xor_mask(cval[1], Ablock[1].w256, mm256_compute_mask_2(idx, 2));
      cval[0] = mm256_xor_mask(cval[0], Ablock[2].w256, mm256_compute_mask_2(idx, 4));
      cval[1] = mm256_xor_mask(cval[1], Ablock[3].w256, mm256_compute_mask_2(idx, 6));
    }
  }
  cval[0] = _mm256_xor_si256(cval[0], cval[1]);
  cblock->w128[0] =
      _mm_xor_si128(_mm256_extractf128_si256(cval[0], 0), _mm256_extractf128_si256(cval[0], 1));
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_v_s256_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {cblock->w256, _mm256_setzero_si256()};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 4, idx >>= 4, Ablock += 4) {
      cval[0] = mm256_xor_mask(cval[0], Ablock[0].w256, mm256_compute_mask(idx, 0));
      cval[1] = mm256_xor_mask(cval[1], Ablock[1].w256, mm256_compute_mask(idx, 1));
      cval[0] = mm256_xor_mask(cval[0], Ablock[2].w256, mm256_compute_mask(idx, 2));
      cval[1] = mm256_xor_mask(cval[1], Ablock[3].w256, mm256_compute_mask(idx, 3));
    }
  }
  cblock->w256 = _mm256_xor_si256(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_v_s256_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {_mm256_setzero_si256(), _mm256_setzero_si256()};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 4, idx >>= 4, Ablock += 4) {
      cval[0] = mm256_xor_mask(cval[0], Ablock[0].w256, mm256_compute_mask(idx, 0));
      cval[1] = mm256_xor_mask(cval[1], Ablock[1].w256, mm256_compute_mask(idx, 1));
      cval[0] = mm256_xor_mask(cval[0], Ablock[2].w256, mm256_compute_mask(idx, 2));
      cval[1] = mm256_xor_mask(cval[1], Ablock[3].w256, mm256_compute_mask(idx, 3));
    }
  }
  cblock->w256 = _mm256_xor_si256(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_v_s256_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {cblock->w256, _mm256_setzero_si256()};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 4, idx >>= 4, Ablock += 4) {
      cval[0] = mm256_xor_mask(cval[0], Ablock[0].w256, mm256_compute_mask(idx, 0));
      cval[1] = mm256_xor_mask(cval[1], Ablock[1].w256, mm256_compute_mask(idx, 1));
      cval[0] = mm256_xor_mask(cval[0], Ablock[2].w256, mm256_compute_mask(idx, 2));
      cval[1] = mm256_xor_mask(cval[1], Ablock[3].w256, mm256_compute_mask(idx, 3));
    }
  }
  cblock->w256 = _mm256_xor_si256(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_v_s256_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {_mm256_setzero_si256(), _mm256_setzero_si256()};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 4, idx >>= 4, Ablock += 4) {
      cval[0] = mm256_xor_mask(cval[0], Ablock[0].w256, mm256_compute_mask(idx, 0));
      cval[1] = mm256_xor_mask(cval[1], Ablock[1].w256, mm256_compute_mask(idx, 1));
      cval[0] = mm256_xor_mask(cval[0], Ablock[2].w256, mm256_compute_mask(idx, 2));
      cval[1] = mm256_xor_mask(cval[1], Ablock[3].w256, mm256_compute_mask(idx, 3));
    }
  }
  cblock->w256 = _mm256_xor_si256(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_v_s256_128_768(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[3] ATTR_ALIGNED(alignof(word256)) = {mm256_zero, mm256_zero, mm256_zero};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 1, idx >>= 1) {
      const word256 mask = mm256_compute_mask(idx, 0);
      for (unsigned int j = 0; j < 3; ++j, ++Ablock) {
        cval[j] = mm256_xor_mask(cval[j], Ablock->w256, mask);
      }
    }
  }

  for (unsigned int j = 0; j < 3; ++j) {
    BLOCK(c, j)->w256 = cval[j];
  }
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_v_s256_192_1024(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[4] ATTR_ALIGNED(alignof(word256)) = {mm256_zero, mm256_zero, mm256_zero, mm256_zero};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 1, idx >>= 1) {
      const word256 mask = mm256_compute_mask(idx, 0);
      for (unsigned int j = 0; j < 4; ++j, ++Ablock) {
        cval[j] = mm256_xor_mask(cval[j], Ablock->w256, mask);
      }
    }
  }

  for (unsigned int j = 0; j < 4; ++j) {
    BLOCK(c, j)->w256 = cval[j];
  }
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_v_s256_256_1280(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[5] ATTR_ALIGNED(alignof(word256)) = {mm256_zero, mm256_zero, mm256_zero, mm256_zero,
                                                    mm256_zero};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 1, idx >>= 1) {
      const word256 mask = mm256_compute_mask(idx, 0);
      for (unsigned int j = 0; j < 5; ++j, ++Ablock) {
        cval[j] = mm256_xor_mask(cval[j], Ablock->w256, mask);
      }
    }
  }

  for (unsigned int j = 0; j < 5; ++j) {
    BLOCK(c, j)->w256 = cval[j];
  }
}
#endif
#endif

static void clear_uint64_block(block_t* block, const unsigned int idx) {
  for (unsigned int i = 0; i < idx; ++i) {
    block->w64[i] = 0;
  }
}

static void clear_uint64_blocks(block_t* block, unsigned int len) {
  for (; len; --len, ++block) {
    clear_uint64_block(block, 4);
  }
}

static void oqs_sig_picnic_mzd_xor_mask_uint64_block(block_t* rblock, const block_t* fblock, const word mask,
                                      const unsigned int idx) {
  for (unsigned int i = 0; i < idx; ++i) {
    rblock->w64[i] ^= fblock->w64[i] & mask;
  }
}

void oqs_sig_picnic_mzd_addmul_v_uint64_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; i -= 2, idx >>= 2, Ablock += 1) {
      const uint64_t mask1 = -(idx & 1);
      const uint64_t mask2 = -((idx >> 1) & 1);
      cblock->w64[0] ^= (Ablock->w64[0] & mask1) ^ (Ablock->w64[2] & mask2);
      cblock->w64[1] ^= (Ablock->w64[1] & mask1) ^ (Ablock->w64[3] & mask2);
    }
  }
}

void oqs_sig_picnic_mzd_mul_v_uint64_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  clear_uint64_block(BLOCK(c, 0), 2);
  oqs_sig_picnic_mzd_addmul_v_uint64_128(c, v, A);
}

void oqs_sig_picnic_mzd_addmul_v_uint64_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; --i, idx >>= 1, ++Ablock) {
      const uint64_t mask = -(idx & 1);
      oqs_sig_picnic_mzd_xor_mask_uint64_block(cblock, Ablock, mask, 3);
    }
  }
}

void oqs_sig_picnic_mzd_mul_v_uint64_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  clear_uint64_block(BLOCK(c, 0), 3);
  oqs_sig_picnic_mzd_addmul_v_uint64_192(c, v, A);
}

void oqs_sig_picnic_mzd_addmul_v_uint64_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;

    for (unsigned int i = sizeof(word) * 8; i; --i, idx >>= 1, ++Ablock) {
      const uint64_t mask = -(idx & 1);
      oqs_sig_picnic_mzd_xor_mask_uint64_block(cblock, Ablock, mask, 4);
    }
  }
}

void oqs_sig_picnic_mzd_mul_v_uint64_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  clear_uint64_block(BLOCK(c, 0), 4);
  oqs_sig_picnic_mzd_addmul_v_uint64_256(c, v, A);
}

void oqs_sig_picnic_mzd_mul_v_uint64_128_576(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  clear_uint64_blocks(BLOCK(c, 0), 2);
  clear_uint64_block(BLOCK(c, 2), 1);

  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; --i, idx >>= 1, ++Ablock) {
      const uint64_t mask = -(idx & 1);
      for (unsigned int j = 0; j < 2; ++j, ++Ablock) {
        oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, j), Ablock, mask, 4);
      }
      oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, 2), Ablock, mask, 1);
    }
  }
}

void oqs_sig_picnic_mzd_mul_v_uint64_128_640(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  clear_uint64_blocks(BLOCK(c, 0), 2);
  clear_uint64_block(BLOCK(c, 2), 2);

  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; --i, idx >>= 1, ++Ablock) {
      const uint64_t mask = -(idx & 1);
      for (unsigned int j = 0; j < 2; ++j, ++Ablock) {
        oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, j), Ablock, mask, 4);
      }
      oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, 2), Ablock, mask, 2);
    }
  }
}

void oqs_sig_picnic_mzd_mul_v_uint64_192_896(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  clear_uint64_blocks(BLOCK(c, 0), 3);
  clear_uint64_block(BLOCK(c, 3), 2);

  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; --i, idx >>= 1, ++Ablock) {
      const uint64_t mask = -(idx & 1);
      for (unsigned int j = 0; j < 3; ++j, ++Ablock) {
        oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, j), Ablock, mask, 4);
      }
      oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, 3), Ablock, mask, 2);
    }
  }
}

void oqs_sig_picnic_mzd_mul_v_uint64_192_960(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  clear_uint64_blocks(BLOCK(c, 0), 3);
  clear_uint64_block(BLOCK(c, 3), 3);

  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; --i, idx >>= 1, ++Ablock) {
      const uint64_t mask = -(idx & 1);
      for (unsigned int j = 0; j < 3; ++j, ++Ablock) {
        oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, j), Ablock, mask, 4);
      }
      oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, 3), Ablock, mask, 3);
    }
  }
}

void oqs_sig_picnic_mzd_mul_v_uint64_256_1152(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  clear_uint64_blocks(BLOCK(c, 0), 4);
  clear_uint64_block(BLOCK(c, 4), 2);

  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; --i, idx >>= 1, ++Ablock) {
      const uint64_t mask = -(idx & 1);
      for (unsigned int j = 0; j < 4; ++j, ++Ablock) {
        oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, j), Ablock, mask, 4);
      }
      oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, 4), Ablock, mask, 2);
    }
  }
}

void oqs_sig_picnic_mzd_mul_v_uint64_256_1216(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  const word* vptr      = CONST_BLOCK(v, 0)->w64;
  const block_t* Ablock = CONST_BLOCK(A, 0);

  clear_uint64_blocks(BLOCK(c, 0), 4);
  clear_uint64_block(BLOCK(c, 4), 3);

  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int i = sizeof(word) * 8; i; --i, idx >>= 1, ++Ablock) {
      const uint64_t mask = -(idx & 1);
      for (unsigned int j = 0; j < 4; ++j, ++Ablock) {
        oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, j), Ablock, mask, 4);
      }
      oqs_sig_picnic_mzd_xor_mask_uint64_block(BLOCK(c, 4), Ablock, mask, 3);
    }
  }
}

#if defined(MUL_M4RI)
#include <stdio.h>

static void xor_comb(const unsigned int len, const unsigned int rowstride, block_t* Bblock,
                     mzd_local_t const* A, unsigned int r_offset, unsigned comb) {
  for (; comb; comb >>= 1, ++r_offset) {
    if (comb & 0x1) {
      const block_t* Ablock = CONST_BLOCK(A, r_offset * rowstride / 4);
      unsigned int i        = 0;
      unsigned int j        = len;
      for (; i < len / 4; ++i, j -= 4) {
        oqs_sig_picnic_mzd_xor_uint64_block(&Bblock[i], &Bblock[i], &Ablock[i], 4);
      }
      oqs_sig_picnic_mzd_xor_uint64_block(&Bblock[i], &Bblock[i], &Ablock[i], j);
    }
  }
}

static void xor_comb_128(block_t* Bblock, const unsigned int boffset, mzd_local_t const* A,
                         unsigned int r_offset, unsigned comb) {
  for (; comb; comb >>= 1, ++r_offset) {
    if (comb & 0x1) {
      const block_t* Ablock      = CONST_BLOCK(A, r_offset / 2);
      const unsigned int aoffset = (r_offset & 0x1) << 1;

      Bblock->w64[boffset] ^= Ablock->w64[aoffset];
      Bblock->w64[boffset + 1] ^= Ablock->w64[aoffset + 1];
    }
  }
}

/**
 * Pre-compute matrices for faster oqs_sig_picnic_mzd_addmul_v computions.
 */
mzd_local_t* oqs_sig_picnic_mzd_precompute_matrix_lookup(mzd_local_t const* A, unsigned int rows, unsigned int cols) {
  mzd_local_t* B = oqs_sig_picnic_mzd_local_init_ex(32 * rows, cols, true);

  const unsigned int len = calculate_width(cols);
  const unsigned int rowstride = calculate_rowstride(len);

  for (unsigned int r = 0; r < 32 * rows; ++r) {
    const unsigned int comb     = r & 0xff;
    const unsigned int r_offset = (r >> 8) << 3;
    if (!comb) {
      continue;
    }

    if (len == 2) {
      /* 128 columns are special. they have two rows per block */
      xor_comb_128(BLOCK(B, r / 2), (r & 0x1) << 1, A, r_offset, comb);
    } else {
      xor_comb(len, rowstride, BLOCK(B, r * rowstride / 4), A, r_offset, comb);
    }
  }

  return B;
}

#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_vl_s128_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 128;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word128 cval[2] ATTR_ALIGNED(alignof(word128)) = {cblock->w128[0], mm128_zero};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      cval[0] = mm128_xor(cval[0], Ablock[(idx >> 1) & 0x7f].w128[idx & 0x1]);
      Ablock += moff2;
      cval[1] = mm128_xor(cval[1], Ablock[(idx >> 9) & 0x7f].w128[(idx >> 8) & 0x1]);
      Ablock += moff2;
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[1]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_vl_s128_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 128;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word128 cval[2] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      cval[0] = mm128_xor(cval[0], Ablock[(idx >> 1) & 0x7f].w128[idx & 0x1]);
      Ablock += moff2;
      cval[1] = mm128_xor(cval[1], Ablock[(idx >> 9) & 0x7f].w128[(idx >> 8) & 0x1]);
      Ablock += moff2;
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[1]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_vl_s128_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {cblock->w128[0], cblock->w128[1], mm128_zero,
                                                    mm128_zero};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      mm128_xor_region(&cval[0], Ablock[(idx >> 0) & 0xff].w128, 2);
      Ablock += moff2;
      mm128_xor_region(&cval[2], Ablock[(idx >> 8) & 0xff].w128, 2);
      Ablock += moff2;
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_vl_s128_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero, mm128_zero};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      mm128_xor_region(&cval[0], Ablock[(idx >> 0) & 0xff].w128, 2);
      Ablock += moff2;
      mm128_xor_region(&cval[2], Ablock[(idx >> 8) & 0xff].w128, 2);
      Ablock += moff2;
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_vl_s128_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {cblock->w128[0], cblock->w128[1], mm128_zero,
                                                    mm128_zero};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      mm128_xor_region(&cval[0], Ablock[(idx >> 0) & 0xff].w128, 2);
      Ablock += moff2;
      mm128_xor_region(&cval[2], Ablock[(idx >> 8) & 0xff].w128, 2);
      Ablock += moff2;
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_vl_s128_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {mm128_zero, mm128_zero, mm128_zero, mm128_zero};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      mm128_xor_region(&cval[0], Ablock[(idx >> 0) & 0xff].w128, 2);
      Ablock += moff2;
      mm128_xor_region(&cval[2], Ablock[(idx >> 8) & 0xff].w128, 2);
      Ablock += moff2;
    }
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_vl_s128_128_640(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256 * 3;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  for (unsigned int j = 0; j < 2; ++j) {
    BLOCK(c, j)->w128[0] = BLOCK(c, j)->w128[1] = mm128_zero;
  }
  BLOCK(c, 2)->w128[0] = mm128_zero;

  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      const block_t* Ablock1 = &Ablock[((idx >> 0) & 0xff) * 3];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock1, 2);
      cblock[2].w128[0] = mm128_xor(cblock[2].w128[0], Ablock1[2].w128[0]);
      Ablock += moff2;

      const block_t* Ablock2 = &Ablock[((idx >> 8) & 0xff) * 3];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock2, 2);
      cblock[2].w128[0] = mm128_xor(cblock[2].w128[0], Ablock2[2].w128[0]);
      Ablock += moff2;
    }
  }
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_vl_s128_192_896(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256 * 4;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  for (unsigned int j = 0; j < 3; ++j) {
    BLOCK(c, j)->w128[0] = BLOCK(c, j)->w128[1] = mm128_zero;
  }
  BLOCK(c, 3)->w128[0] = mm128_zero;

  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      const block_t* Ablock1 = &Ablock[((idx >> 0) & 0xff) * 4];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock1, 3);
      cblock[3].w128[0] = mm128_xor(cblock[3].w128[0], Ablock1[3].w128[0]);
      Ablock += moff2;

      const block_t* Ablock2 = &Ablock[((idx >> 8) & 0xff) * 4];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock2, 3);
      cblock[3].w128[0] = mm128_xor(cblock[3].w128[0], Ablock2[3].w128[0]);
      Ablock += moff2;
    }
  }
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_vl_s128_192_1024(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256 * 4;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  for (unsigned int j = 0; j < 4; ++j) {
    BLOCK(c, j)->w128[0] = BLOCK(c, j)->w128[1] = mm128_zero;
  }

  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      const block_t* Ablock1 = &Ablock[((idx >> 0) & 0xff) * 4];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock1, 4);
      Ablock += moff2;

      const block_t* Ablock2 = &Ablock[((idx >> 8) & 0xff) * 4];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock2, 4);
      Ablock += moff2;
    }
  }
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_vl_s128_256_1152(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256 * 5;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  for (unsigned int j = 0; j < 4; ++j) {
    BLOCK(c, j)->w128[0] = BLOCK(c, j)->w128[1] = mm128_zero;
  }
  BLOCK(c, 4)->w128[0] = mm128_zero;

  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      const block_t* Ablock1 = &Ablock[((idx >> 0) & 0xff) * 5];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock1, 4);
      cblock[4].w128[0] = mm128_xor(cblock[4].w128[0], Ablock1[4].w128[0]);
      Ablock += moff2;

      const block_t* Ablock2 = &Ablock[((idx >> 8) & 0xff) * 5];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock2, 4);
      cblock[4].w128[0] = mm128_xor(cblock[4].w128[0], Ablock2[4].w128[0]);
      Ablock += moff2;
    }
  }
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_mul_vl_s128_256_1280(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256 * 5;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  for (unsigned int j = 0; j < 5; ++j) {
    BLOCK(c, j)->w128[0] = BLOCK(c, j)->w128[1] = mm128_zero;
  }

  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      const block_t* Ablock1 = &Ablock[((idx >> 0) & 0xff) * 5];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock1, 5);
      Ablock += moff2;

      const block_t* Ablock2 = &Ablock[((idx >> 8) & 0xff) * 5];
      oqs_sig_picnic_mzd_xor_s128_blocks(cblock, cblock, Ablock2, 5);
      Ablock += moff2;
    }
  }
}
#endif

#if defined(WITH_AVX2)
ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_vl_s256_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {mm256_zero, mm256_zero};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      cval[0] = mm256_xor(cval[0], Ablock[(idx >> 0) & 0xff].w256);
      Ablock += moff2;
      cval[1] = mm256_xor(cval[1], Ablock[(idx >> 8) & 0xff].w256);
      Ablock += moff2;
    }
  }
  cblock->w256 = _mm256_xor_si256(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_vl_s256_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {cblock->w256, mm256_zero};
  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      cval[0] = mm256_xor(cval[0], Ablock[(idx >> 0) & 0xff].w256);
      Ablock += moff2;
      cval[1] = mm256_xor(cval[1], Ablock[(idx >> 8) & 0xff].w256);
      Ablock += moff2;
    }
  }
  cblock->w256 = _mm256_xor_si256(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_vl_s256_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {mm256_zero, mm256_zero};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      cval[0] = mm256_xor(cval[0], Ablock[(idx >> 0) & 0xff].w256);
      Ablock += moff2;
      cval[1] = mm256_xor(cval[1], Ablock[(idx >> 8) & 0xff].w256);
      Ablock += moff2;
    }
  }
  cblock->w256 = _mm256_xor_si256(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_vl_s256_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {cblock->w256, mm256_zero};
  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      cval[0] = mm256_xor(cval[0], Ablock[(idx >> 0) & 0xff].w256);
      Ablock += moff2;
      cval[1] = mm256_xor(cval[1], Ablock[(idx >> 8) & 0xff].w256);
      Ablock += moff2;
    }
  }
  cblock->w256 = _mm256_xor_si256(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_vl_s256_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 128;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {mm256_zero, mm256_zero};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 4, idx >>= 32) {
      const word256 t1 =
          _mm256_set_m128i(Ablock[(idx >> 1) & 0x7f].w128[idx & 0x1],
                           Ablock[((idx >> 9) & 0x7f) + moff2].w128[(idx >> 8) & 0x1]);
      cval[0] = mm256_xor(cval[0], t1);
      Ablock += 2 * moff2;

      const word256 t2 =
          _mm256_set_m128i(Ablock[(idx >> 17) & 0x7f].w128[(idx >> 16) & 0x1],
                           Ablock[((idx >> 25) & 0x7f) + moff2].w128[(idx >> 24) & 0x1]);
      cval[1] = mm256_xor(cval[1], t2);
      Ablock += 2 * moff2;
    }
  }
  cval[0] = mm256_xor(cval[0], cval[1]);
  cblock->w128[0] =
      mm128_xor(_mm256_extractf128_si256(cval[0], 0), _mm256_extractf128_si256(cval[0], 1));
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_vl_s256_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 128;

  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  word256 cval[2] ATTR_ALIGNED(alignof(word256)) = {_mm256_castsi128_si256(cblock->w128[0]),
                                                    mm256_zero};
  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 4, idx >>= 32) {
      const word256 t1 =
          _mm256_set_m128i(Ablock[(idx >> 1) & 0x7f].w128[idx & 0x1],
                           Ablock[((idx >> 9) & 0x7f) + moff2].w128[(idx >> 8) & 0x1]);
      cval[0] = mm256_xor(cval[0], t1);
      Ablock += 2 * moff2;

      const word256 t2 =
          _mm256_set_m128i(Ablock[(idx >> 17) & 0x7f].w128[(idx >> 16) & 0x1],
                           Ablock[((idx >> 25) & 0x7f) + moff2].w128[(idx >> 24) & 0x1]);
      cval[1] = mm256_xor(cval[1], t2);
      Ablock += 2 * moff2;
    }
  }
  cval[0] = mm256_xor(cval[0], cval[1]);
  cblock->w128[0] =
      mm128_xor(_mm256_extractf128_si256(cval[0], 0), _mm256_extractf128_si256(cval[0], 1));
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_vl_s256_128_768(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256 * 3;

  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  for (unsigned int j = 0; j < 3; ++j) {
    BLOCK(c, j)->w256 = mm256_zero;
  }

  for (unsigned int w = 2; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(c, 0), CONST_BLOCK(c, 0), &Ablock[((idx >> 0) & 0xff) * 3], 3);
      Ablock += moff2;
      oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(c, 0), CONST_BLOCK(c, 0), &Ablock[((idx >> 8) & 0xff) * 3], 3);
      Ablock += moff2;
    }
  }
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_vl_s256_192_1024(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256 * 4;

  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  for (unsigned int j = 0; j < 4; ++j) {
    BLOCK(c, j)->w256 = mm256_zero;
  }

  for (unsigned int w = 3; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(c, 0), CONST_BLOCK(c, 0), &Ablock[((idx >> 0) & 0xff) * 4], 4);
      Ablock += moff2;
      oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(c, 0), CONST_BLOCK(c, 0), &Ablock[((idx >> 8) & 0xff) * 4], 4);
      Ablock += moff2;
    }
  }
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_mul_vl_s256_256_1280(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  static const unsigned int moff2 = 256 * 5;

  const block_t* Ablock = CONST_BLOCK(A, 0);
  word const* vptr      = CONST_BLOCK(v, 0)->w64;

  for (unsigned int j = 0; j < 5; ++j) {
    BLOCK(c, j)->w256 = mm256_zero;
  }

  for (unsigned int w = 4; w; --w, ++vptr) {
    word idx = *vptr;
    for (unsigned int s = sizeof(word); s; s -= 2, idx >>= 16) {
      oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(c, 0), CONST_BLOCK(c, 0), &Ablock[((idx >> 0) & 0xff) * 5], 5);
      Ablock += moff2;
      oqs_sig_picnic_mzd_xor_s256_blocks(BLOCK(c, 0), CONST_BLOCK(c, 0), &Ablock[((idx >> 8) & 0xff) * 5], 5);
      Ablock += moff2;
    }
  }
}
#endif
#endif

void oqs_sig_picnic_mzd_addmul_vl_uint64_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock  = BLOCK(c, 0);
  word const* vptr = CONST_BLOCK(v, 0)->w64;

  for (unsigned int w = 0; w < 2; ++w, ++vptr) {
    unsigned int add = 0;
    for (word idx = *vptr; idx; idx >>= 8, add += 128) {
      const word comb            = idx & 0xff;
      const unsigned int aoffset = (comb & 0x1) << 1;
      const block_t* Ablock      = CONST_BLOCK(A, w * sizeof(word) * 8 * 16 + add + (comb >> 1));

      cblock->w64[0] ^= Ablock->w64[aoffset];
      cblock->w64[1] ^= Ablock->w64[aoffset + 1];
    }
  }
}

void oqs_sig_picnic_mzd_mul_vl_uint64_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  clear_uint64_block(BLOCK(c, 0), 2);
  oqs_sig_picnic_mzd_addmul_vl_uint64_128(c, v, A);
}

static void oqs_sig_picnic_mzd_addmul_vl_uint64_256_len(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A,
                                         const unsigned int len) {
  block_t* cblock  = BLOCK(c, 0);
  word const* vptr = CONST_BLOCK(v, 0)->w64;

  for (unsigned int w = 0; w < len; ++w, ++vptr) {
    unsigned int add = 0;
    for (word idx = *vptr; idx; idx >>= 8, add += 256) {
      const word comb = idx & 0xff;

      oqs_sig_picnic_mzd_xor_uint64_block(cblock, cblock, CONST_BLOCK(A, w * sizeof(word) * 8 * 32 + add + comb),
                           len);
    }
  }
}

void oqs_sig_picnic_mzd_addmul_vl_uint64_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_vl_uint64_256_len(c, v, A, 3);
}

void oqs_sig_picnic_mzd_mul_vl_uint64_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  clear_uint64_block(BLOCK(c, 0), 3);
  oqs_sig_picnic_mzd_addmul_vl_uint64_192(c, v, A);
}

void oqs_sig_picnic_mzd_addmul_vl_uint64_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_vl_uint64_256_len(c, v, A, 4);
}

void oqs_sig_picnic_mzd_mul_vl_uint64_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  clear_uint64_block(BLOCK(c, 0), 4);
  oqs_sig_picnic_mzd_addmul_vl_uint64_256(c, v, A);
}

void oqs_sig_picnic_mzd_mul_vl_uint64_128_576(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock  = BLOCK(c, 0);
  word const* vptr = CONST_BLOCK(v, 0)->w64;

  clear_uint64_blocks(cblock, 2);
  clear_uint64_block(&cblock[2], 1);

  for (unsigned int w = 0; w < 2; ++w, ++vptr) {
    unsigned int add = 0;
    for (word idx = *vptr; idx; idx >>= 8, add += 256) {
      const word comb       = idx & 0xff;
      const block_t* Ablock = CONST_BLOCK(A, (w * sizeof(word) * 8 * 32 + add + comb) * 3);

      oqs_sig_picnic_mzd_xor_uint64_blocks(cblock, cblock, Ablock, 2);
      oqs_sig_picnic_mzd_xor_uint64_block(&cblock[2], &cblock[2], &Ablock[2], 1);
    }
  }
}

void oqs_sig_picnic_mzd_mul_vl_uint64_128_640(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock  = BLOCK(c, 0);
  word const* vptr = CONST_BLOCK(v, 0)->w64;

  clear_uint64_blocks(cblock, 2);
  clear_uint64_block(&cblock[2], 2);

  for (unsigned int w = 0; w < 2; ++w, ++vptr) {
    unsigned int add = 0;
    for (word idx = *vptr; idx; idx >>= 8, add += 256) {
      const word comb       = idx & 0xff;
      const block_t* Ablock = CONST_BLOCK(A, (w * sizeof(word) * 8 * 32 + add + comb) * 3);

      oqs_sig_picnic_mzd_xor_uint64_blocks(cblock, cblock, Ablock, 2);
      oqs_sig_picnic_mzd_xor_uint64_block(&cblock[2], &cblock[2], &Ablock[2], 2);
    }
  }
}

void oqs_sig_picnic_mzd_mul_vl_uint64_192_896(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock  = BLOCK(c, 0);
  word const* vptr = CONST_BLOCK(v, 0)->w64;

  clear_uint64_blocks(cblock, 3);
  clear_uint64_block(&cblock[3], 2);

  for (unsigned int w = 0; w < 3; ++w, ++vptr) {
    unsigned int add = 0;
    for (word idx = *vptr; idx; idx >>= 8, add += 256) {
      const word comb       = idx & 0xff;
      const block_t* Ablock = CONST_BLOCK(A, (w * sizeof(word) * 8 * 32 + add + comb) * 4);

      oqs_sig_picnic_mzd_xor_uint64_blocks(cblock, cblock, Ablock, 3);
      oqs_sig_picnic_mzd_xor_uint64_block(&cblock[3], &cblock[3], &Ablock[3], 2);
    }
  }
}

void oqs_sig_picnic_mzd_mul_vl_uint64_192_960(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock  = BLOCK(c, 0);
  word const* vptr = CONST_BLOCK(v, 0)->w64;

  clear_uint64_blocks(cblock, 3);
  clear_uint64_block(&cblock[3], 3);

  for (unsigned int w = 0; w < 3; ++w, ++vptr) {
    unsigned int add = 0;
    for (word idx = *vptr; idx; idx >>= 8, add += 256) {
      const word comb       = idx & 0xff;
      const block_t* Ablock = CONST_BLOCK(A, (w * sizeof(word) * 8 * 32 + add + comb) * 4);

      oqs_sig_picnic_mzd_xor_uint64_blocks(cblock, cblock, Ablock, 3);
      oqs_sig_picnic_mzd_xor_uint64_block(&cblock[3], &cblock[3], &Ablock[3], 3);
    }
  }
}

void oqs_sig_picnic_mzd_mul_vl_uint64_256_1152(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock  = BLOCK(c, 0);
  word const* vptr = CONST_BLOCK(v, 0)->w64;

  clear_uint64_blocks(cblock, 4);
  clear_uint64_block(&cblock[4], 2);

  for (unsigned int w = 0; w < 4; ++w, ++vptr) {
    unsigned int add = 0;
    for (word idx = *vptr; idx; idx >>= 8, add += 256) {
      const word comb       = idx & 0xff;
      const block_t* Ablock = CONST_BLOCK(A, (w * sizeof(word) * 8 * 32 + add + comb) * 5);

      oqs_sig_picnic_mzd_xor_uint64_blocks(cblock, cblock, Ablock, 4);
      oqs_sig_picnic_mzd_xor_uint64_block(&cblock[4], &cblock[4], &Ablock[4], 2);
    }
  }
}

void oqs_sig_picnic_mzd_mul_vl_uint64_256_1216(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock  = BLOCK(c, 0);
  word const* vptr = CONST_BLOCK(v, 0)->w64;

  clear_uint64_blocks(cblock, 4);
  clear_uint64_block(&cblock[4], 3);

  for (unsigned int w = 0; w < 4; ++w, ++vptr) {
    unsigned int add = 0;
    for (word idx = *vptr; idx; idx >>= 8, add += 256) {
      const word comb       = idx & 0xff;
      const block_t* Ablock = CONST_BLOCK(A, (w * sizeof(word) * 8 * 32 + add + comb) * 5);

      oqs_sig_picnic_mzd_xor_uint64_blocks(cblock, cblock, Ablock, 4);
      oqs_sig_picnic_mzd_xor_uint64_block(&cblock[4], &cblock[4], &Ablock[4], 3);
    }
  }
}
#endif

// specific instances
#if defined(OPTIMIZED_LINEAR_LAYER_EVALUATION)
// bit extract, non-constant time for mask, but mask is public in our calls
static word extract_bits(word in, word mask) {
  word res = 0;
  for (word bb = 1; mask != 0; bb <<= 1, mask &= (mask - 1)) {
    res |= bb & (-((word) !!(in & mask & -mask)));
  }
  return res;
}

static inline void oqs_sig_picnic_mzd_shuffle_30_idx(mzd_local_t* x, const word mask, unsigned int idx) {
  const word w          = CONST_BLOCK(x, 0)->w64[idx];
  const word a          = extract_bits(w, mask) << 34;
  BLOCK(x, 0)->w64[idx] = a | extract_bits(w, ~mask);
}

void oqs_sig_picnic_mzd_shuffle_128_30(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_30_idx(x, mask, 1);
}

void oqs_sig_picnic_mzd_shuffle_192_30(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_30_idx(x, mask, 2);
}

void oqs_sig_picnic_mzd_shuffle_256_30(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_30_idx(x, mask, 3);
}

static inline void oqs_sig_picnic_mzd_shuffle_3_idx(mzd_local_t* x, const word mask, unsigned int idx) {
  const word w          = CONST_BLOCK(x, 0)->w64[idx];
  const word a          = extract_bits(w, mask) << 61;
  BLOCK(x, 0)->w64[idx] = a | extract_bits(w, ~mask);
}

void oqs_sig_picnic_mzd_shuffle_128_3(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_3_idx(x, mask, 1);
}

void oqs_sig_picnic_mzd_shuffle_192_3(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_3_idx(x, mask, 2);
}

void oqs_sig_picnic_mzd_shuffle_256_3(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_3_idx(x, mask, 3);
}

// no SIMD
void oqs_sig_picnic_mzd_addmul_v_uint64_30_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word idx = CONST_BLOCK(v, 0)->w64[1] >> 34;
  for (unsigned int i = 15; i; --i, idx >>= 2, ++Ablock) {
    const uint64_t mask1 = -(idx & 1);
    const uint64_t mask2 = -((idx >> 1) & 1);
    for (unsigned int j = 0; j < 2; ++j) {
      cblock->w64[j] ^= (Ablock->w64[j] & mask1) ^ (Ablock->w64[j + 2] & mask2);
    }
  }
}

void oqs_sig_picnic_mzd_addmul_v_uint64_30_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word idx = CONST_BLOCK(v, 0)->w64[2] >> 34;
  for (unsigned int i = 30; i; --i, idx >>= 1, ++Ablock) {
    const uint64_t mask = -(idx & 1);
    oqs_sig_picnic_mzd_xor_mask_uint64_block(cblock, Ablock, mask, 3);
  }
}

void oqs_sig_picnic_mzd_addmul_v_uint64_30_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word idx = CONST_BLOCK(v, 0)->w64[3] >> 34;
  for (unsigned int i = 30; i; --i, idx >>= 1, ++Ablock) {
    const uint64_t mask = -(idx & 1);
    oqs_sig_picnic_mzd_xor_mask_uint64_block(cblock, Ablock, mask, 4);
  }
}

void oqs_sig_picnic_mzd_addmul_v_uint64_3_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock        = BLOCK(c, 0);
  const block_t* Ablock1 = CONST_BLOCK(A, 0);
  const block_t* Ablock2 = CONST_BLOCK(A, 1);

  const word idx       = CONST_BLOCK(v, 0)->w64[1] >> 61;
  const uint64_t mask1 = -(idx & 1);
  const uint64_t mask2 = -((idx >> 1) & 1);
  const uint64_t mask3 = -((idx >> 2) & 1);

  for (unsigned int j = 0; j < 2; ++j) {
    cblock->w64[j] ^=
        (Ablock1->w64[j] & mask1) ^ (Ablock1->w64[j + 2] & mask2) ^ (Ablock2->w64[j] & mask3);
  }
}

void oqs_sig_picnic_mzd_addmul_v_uint64_3_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word idx = CONST_BLOCK(v, 0)->w64[2] >> 61;
  for (unsigned int i = 3; i; --i, idx >>= 1, ++Ablock) {
    const uint64_t mask = -(idx & 1);
    oqs_sig_picnic_mzd_xor_mask_uint64_block(cblock, Ablock, mask, 3);
  }
}

void oqs_sig_picnic_mzd_addmul_v_uint64_3_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word idx = CONST_BLOCK(v, 0)->w64[3] >> 61;
  for (unsigned int i = 3; i; --i, idx >>= 1, ++Ablock) {
    const uint64_t mask = -(idx & 1);
    oqs_sig_picnic_mzd_xor_mask_uint64_block(cblock, Ablock, mask, 4);
  }
}

#if defined(WITH_SSE2) || defined(WITH_NEON)
ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_v_s128_30_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word idx              = CONST_BLOCK(v, 0)->w64[1] >> 34;

  word128 cval[2] ATTR_ALIGNED(alignof(word128));
  cval[0] = mm128_xor_mask(cblock->w128[0], Ablock[0].w128[0], mm128_compute_mask(idx, 0));
  cval[1] = mm128_and(Ablock[0].w128[1], mm128_compute_mask(idx, 1));
  idx >>= 2;
  Ablock += 1;

  for (unsigned int i = 28; i; i -= 4, idx >>= 4, Ablock += 2) {
    cval[0] = mm128_xor_mask(cval[0], Ablock[0].w128[0], mm128_compute_mask(idx, 0));
    cval[1] = mm128_xor_mask(cval[1], Ablock[0].w128[1], mm128_compute_mask(idx, 1));
    cval[0] = mm128_xor_mask(cval[0], Ablock[1].w128[0], mm128_compute_mask(idx, 2));
    cval[1] = mm128_xor_mask(cval[1], Ablock[1].w128[1], mm128_compute_mask(idx, 3));
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[1]);
}

ATTR_TARGET_S128
static void oqs_sig_picnic_mzd_addmul_v_s128_30_256_idx(mzd_local_t* c, mzd_local_t const* A, word idx) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {cblock->w128[0], cblock->w128[1], mm128_zero,
                                                    mm128_zero};
  for (unsigned int i = 30; i; i -= 2, idx >>= 2, Ablock += 2) {
    mm128_xor_mask_region(&cval[0], Ablock[0].w128, mm128_compute_mask(idx, 0), 2);
    mm128_xor_mask_region(&cval[2], Ablock[1].w128, mm128_compute_mask(idx, 1), 2);
  }
  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_v_s128_30_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_v_s128_30_256_idx(c, A, CONST_BLOCK(v, 0)->w64[2] >> 34);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_v_s128_30_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_v_s128_30_256_idx(c, A, CONST_BLOCK(v, 0)->w64[3] >> 34);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_v_s128_3_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  const word idx        = CONST_BLOCK(v, 0)->w64[1] >> 61;

  word128 cval[2] ATTR_ALIGNED(alignof(word128));
  cval[0]         = mm128_xor_mask(cblock->w128[0], Ablock[0].w128[0], mm128_compute_mask(idx, 0));
  cval[1]         = mm128_and(Ablock[0].w128[1], mm128_compute_mask(idx, 1));
  cval[0]         = mm128_xor_mask(cval[0], Ablock[1].w128[0], mm128_compute_mask(idx, 2));
  cblock->w128[0] = mm128_xor(cval[0], cval[1]);
}

ATTR_TARGET_S128
static void oqs_sig_picnic_mzd_addmul_v_s128_3_256_idx(mzd_local_t* c, mzd_local_t const* A, const word idx) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word128 cval[4] ATTR_ALIGNED(alignof(word128)) = {cblock->w128[0], cblock->w128[1], mm128_zero,
                                                    mm128_zero};
  mm128_xor_mask_region(&cval[0], Ablock[0].w128, mm128_compute_mask(idx, 0), 2);
  mm128_xor_mask_region(&cval[2], Ablock[1].w128, mm128_compute_mask(idx, 1), 2);
  mm128_xor_mask_region(&cval[0], Ablock[2].w128, mm128_compute_mask(idx, 2), 2);

  cblock->w128[0] = mm128_xor(cval[0], cval[2]);
  cblock->w128[1] = mm128_xor(cval[1], cval[3]);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_v_s128_3_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_v_s128_3_256_idx(c, A, CONST_BLOCK(v, 0)->w64[2] >> 61);
}

ATTR_TARGET_S128
void oqs_sig_picnic_mzd_addmul_v_s128_3_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_v_s128_3_256_idx(c, A, CONST_BLOCK(v, 0)->w64[3] >> 61);
}
#endif

#if defined(WITH_AVX2)
ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_v_s256_30_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* vblock = CONST_BLOCK(v, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  word idx              = vblock->w64[1] >> 34;

  word256 cval[2] ATTR_ALIGNED(alignof(word256));
  cval[0] = mm256_xor_mask(_mm256_castsi128_si256(cblock->w128[0]), Ablock[0].w256,
                           mm256_compute_mask_2(idx, 0));
  cval[1] = mm256_and(Ablock[1].w256, mm256_compute_mask_2(idx, 2));
  idx >>= 4;
  Ablock += 2;

  for (unsigned int i = 24; i; i -= 8, idx >>= 8, Ablock += 4) {
    cval[0] = mm256_xor_mask(cval[0], Ablock[0].w256, mm256_compute_mask_2(idx, 0));
    cval[1] = mm256_xor_mask(cval[1], Ablock[1].w256, mm256_compute_mask_2(idx, 2));
    cval[0] = mm256_xor_mask(cval[0], Ablock[2].w256, mm256_compute_mask_2(idx, 4));
    cval[1] = mm256_xor_mask(cval[1], Ablock[3].w256, mm256_compute_mask_2(idx, 6));
  }
  cval[0] = mm256_xor_mask(cval[0], Ablock[0].w256, mm256_compute_mask_2(idx, 0));

  cval[0] = _mm256_xor_si256(cval[0], cval[1]);
  cblock->w128[0] =
      _mm_xor_si128(_mm256_extractf128_si256(cval[0], 0), _mm256_extractf128_si256(cval[0], 1));
}

ATTR_TARGET_AVX2
static void oqs_sig_picnic_mzd_addmul_v_s256_30_256_idx(mzd_local_t* c, mzd_local_t const* A, word idx) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[2] ATTR_ALIGNED(alignof(word256));
  cval[0] = mm256_xor_mask(cblock->w256, Ablock[0].w256, mm256_compute_mask(idx, 0));
  cval[1] = mm256_and(Ablock[1].w256, mm256_compute_mask(idx, 1));
  idx >>= 2;
  Ablock += 2;

  for (unsigned int i = 28; i; i -= 4, idx >>= 4, Ablock += 4) {
    cval[0] = mm256_xor_mask(cval[0], Ablock[0].w256, mm256_compute_mask(idx, 0));
    cval[1] = mm256_xor_mask(cval[1], Ablock[1].w256, mm256_compute_mask(idx, 1));
    cval[0] = mm256_xor_mask(cval[0], Ablock[2].w256, mm256_compute_mask(idx, 2));
    cval[1] = mm256_xor_mask(cval[1], Ablock[3].w256, mm256_compute_mask(idx, 3));
  }
  cblock->w256 = mm256_xor(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_v_s256_30_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_v_s256_30_256_idx(c, A, CONST_BLOCK(v, 0)->w64[2] >> 34);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_v_s256_30_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_v_s256_30_256_idx(c, A, CONST_BLOCK(v, 0)->w64[3] >> 34);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_v_s256_3_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);
  const word idx        = CONST_BLOCK(v, 0)->w64[1] >> 61;

  word128 cval[2] ATTR_ALIGNED(alignof(word128));
  cval[0]         = mm128_xor_mask(cblock->w128[0], Ablock[0].w128[0], mm128_compute_mask(idx, 0));
  cval[1]         = mm128_and(Ablock[0].w128[1], mm128_compute_mask(idx, 1));
  cval[0]         = mm128_xor_mask(cval[0], Ablock[1].w128[0], mm128_compute_mask(idx, 2));
  cblock->w128[0] = mm128_xor(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
static void oqs_sig_picnic_mzd_addmul_v_s256_3_256_idx(mzd_local_t* c, mzd_local_t const* A, const word idx) {
  block_t* cblock       = BLOCK(c, 0);
  const block_t* Ablock = CONST_BLOCK(A, 0);

  word256 cval[2] ATTR_ALIGNED(alignof(word256));
  cval[0]      = mm256_xor_mask(cblock->w256, Ablock[0].w256, mm256_compute_mask(idx, 0));
  cval[1]      = mm256_and(Ablock[1].w256, mm256_compute_mask(idx, 1));
  cval[0]      = mm256_xor_mask(cval[0], Ablock[2].w256, mm256_compute_mask(idx, 2));
  cblock->w256 = mm256_xor(cval[0], cval[1]);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_v_s256_3_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_v_s256_3_256_idx(c, A, CONST_BLOCK(v, 0)->w64[2] >> 61);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_addmul_v_s256_3_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) {
  oqs_sig_picnic_mzd_addmul_v_s256_3_256_idx(c, A, CONST_BLOCK(v, 0)->w64[3] >> 61);
}

#if !defined(__x86_64__) && !defined(_M_X64)
ATTR_TARGET_AVX2 ATTR_CONST static uint8_t popcount_32(uint32_t value) {
  uint64_t result =
      ((value & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f;
  result +=
      (((value & 0xfff000) >> 12) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f;
  result += ((value >> 24) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f;
  return result;
}

ATTR_TARGET_AVX2 ATTR_CONST static uint64_t _pext_u64(uint64_t a, uint64_t mask) {
  const uint32_t low  = _pext_u32(a, mask);
  const uint32_t high = _pext_u32(a >> 32, mask >> 32);

  return (((uint64_t)high) << popcount_32(mask)) | low;
}
#endif

ATTR_TARGET_AVX2
static inline void oqs_sig_picnic_mzd_shuffle_pext_30_idx(mzd_local_t* x, const word mask, unsigned int idx) {
  const word w          = CONST_BLOCK(x, 0)->w64[idx];
  const word a          = _pext_u64(w, mask) << 34;
  BLOCK(x, 0)->w64[idx] = a | _pext_u64(w, ~mask);
}

void oqs_sig_picnic_mzd_shuffle_pext_128_30(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_pext_30_idx(x, mask, 1);
}

void oqs_sig_picnic_mzd_shuffle_pext_192_30(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_pext_30_idx(x, mask, 2);
}

void oqs_sig_picnic_mzd_shuffle_pext_256_30(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_pext_30_idx(x, mask, 3);
}

ATTR_TARGET_AVX2
static inline void oqs_sig_picnic_mzd_shuffle_pext_3_idx(mzd_local_t* x, const word mask, unsigned int idx) {
  const word w          = CONST_BLOCK(x, 0)->w64[idx];
  const word a          = _pext_u64(w, mask) << 61;
  BLOCK(x, 0)->w64[idx] = a | _pext_u64(w, ~mask);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_shuffle_pext_128_3(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_pext_3_idx(x, mask, 1);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_shuffle_pext_192_3(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_pext_3_idx(x, mask, 2);
}

ATTR_TARGET_AVX2
void oqs_sig_picnic_mzd_shuffle_pext_256_3(mzd_local_t* x, const word mask) {
  oqs_sig_picnic_mzd_shuffle_pext_3_idx(x, mask, 3);
}
#endif
#endif
