#include <string.h>
#include <immintrin.h>

#include "vole_private.h"

EXPORT void bitvec_xor_to_avx2(uint64_t bytelen, bitvec_t* res, const bitvec_t* b) {
  uint8_t* r = (uint8_t*)res;
  const uint8_t* bb = (uint8_t*)b;
  uint64_t i = 0;
  for (; i + 32 <= bytelen; i += 32) {
    __m256i rv = _mm256_loadu_si256((const __m256i*)(r + i));
    __m256i bv = _mm256_loadu_si256((const __m256i*)(bb + i));
    _mm256_storeu_si256((__m256i*)(r + i), _mm256_xor_si256(rv, bv));
  }
  for (; i < bytelen; i++) {
    r[i] ^= bb[i];
  }
}

EXPORT void bitvec_xor_avx2(uint64_t bytelen, bitvec_t* res, const bitvec_t* a, const bitvec_t* b) {
  uint8_t* r = (uint8_t*)res;
  const uint8_t* aa = (uint8_t*)a;
  const uint8_t* bb = (uint8_t*)b;
  uint64_t i = 0;
  for (; i + 32 <= bytelen; i += 32) {
    __m256i av = _mm256_loadu_si256((const __m256i*)(aa + i));
    __m256i bv = _mm256_loadu_si256((const __m256i*)(bb + i));
    _mm256_storeu_si256((__m256i*)(r + i), _mm256_xor_si256(av, bv));
  }
  for (; i < bytelen; i++) {
    r[i] = aa[i] ^ bb[i];
  }
}

// there is a missing builtin in gcc < 10.
// clang also defines __GNUC__ (as 4) but ships the real intrinsic, so exclude it —
// otherwise this redefines clang's builtin and the whole tree fails to compile under clang.
#if defined(__GNUC__) && !defined(__clang__)
#if __GNUC__ < 10
__m256i _mm256_zextsi128_si256 (__m128i x)
{
  return __extension__ (__m256i) { x[0], x[1], 0, 0 };
}
#endif
#endif


// Compute parity of a 64-bit value
static inline uint8_t parity64(uint64_t x) {
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    return (uint8_t)((0x6996 >> (x & 0xF)) & 1);
}

// Reduce a __m256i to a 64-bit XOR of all its bits
static inline uint64_t avx2_reduce_to_64(__m256i x) {
    __m128i x128 = _mm_xor_si128(_mm256_castsi256_si128(x),
                                 _mm256_extracti128_si256(x, 1));
    x128 = _mm_xor_si128(x128, _mm_srli_si128(x128, 8));
    uint64_t val64 = (uint64_t)_mm_cvtsi128_si64(x128);
    return val64;
}

// Reduce a __m128i to a 64-bit XOR of all its bits
static inline uint64_t sse2_reduce_to_64(__m128i x) {
    x = _mm_xor_si128(x, _mm_srli_si128(x, 8));
    uint64_t val64 = (uint64_t)_mm_cvtsi128_si64(x);
    return val64;
}


EXPORT void matrix_vector_product_f2_avx2(
    uint64_t nrows, uint64_t ncols,
    bitvec_t* res,
    const bitmat_t* a,
    const bitvec_t* b)
{
    memset(res, 0, (nrows+7) >> 3);

    uint64_t n_256 = ncols >> 8;
    uint64_t r = ncols & 0xFF;

    uint64_t n_128 = r >> 7;
    r &= 0x7F;

    uint64_t n_64 = r >> 6;
    r &= 0x3F;

    uint64_t n_32 = r >> 5;
    r &= 0x1F;

    uint64_t n_8 = r >> 3; // remainder in bytes

    for (uint64_t i = 0; i < nrows; i++) {
        uint64_t accum = 0ULL; // accumulate XOR of all bits

        uint64_t row_offset = i * ncols;

        // 1. Process 256-bit blocks with AVX2
        if (n_256 > 0) {
            __m256i avx_accum = _mm256_setzero_si256();
            for (uint64_t c = 0; c < n_256; c++) {
                uint64_t bitpos = row_offset + (c << 8);
                const uint8_t* A_ptr = a + (bitpos >> 3);
                const uint8_t* B_ptr = b + c*32;

                __m256i A_vec = _mm256_loadu_si256((const __m256i*)A_ptr);
                __m256i B_vec = _mm256_loadu_si256((const __m256i*)B_ptr);
                __m256i and_vec = _mm256_and_si256(A_vec, B_vec);
                avx_accum = _mm256_xor_si256(avx_accum, and_vec);
            }
            accum ^= avx2_reduce_to_64(avx_accum);
        }

        uint64_t used_bits = n_256 << 8;
        uint64_t offset_after_256 = row_offset + used_bits;

        // 2. Process 128-bit blocks with SSE2
        if (n_128 > 0) {
            __m128i sse_accum = _mm_setzero_si128();
            for (uint64_t c = 0; c < n_128; c++) {
                uint64_t bitpos = offset_after_256 + (c << 7);
                const uint8_t* A_ptr = a + (bitpos >> 3);
                const uint8_t* B_ptr = b + (used_bits >> 3) + (c << 4);

                __m128i A_vec = _mm_loadu_si128((const __m128i*)A_ptr);
                __m128i B_vec = _mm_loadu_si128((const __m128i*)B_ptr);
                __m128i and_vec = _mm_and_si128(A_vec, B_vec);
                sse_accum = _mm_xor_si128(sse_accum, and_vec);
            }
            accum ^= sse2_reduce_to_64(sse_accum);
        }

        used_bits += n_128 << 7;
        uint64_t offset_after_128 = row_offset + used_bits;

        // 3. Process 64-bit blocks
        for (uint64_t c = 0; c < n_64; c++) {
            uint64_t bitpos = offset_after_128 + (c << 6);
            const uint8_t* A_ptr = a + (bitpos >> 3);
            const uint8_t* B_ptr = b + (used_bits >> 3) + (c << 3);

            uint64_t A_val = *(const uint64_t*)A_ptr;
            uint64_t B_val = *(const uint64_t*)B_ptr;
            accum ^= (A_val & B_val);
        }

        used_bits += n_64 << 6;
        uint64_t offset_after_64 = row_offset + used_bits;

        // 4. Process 32-bit blocks
        for (uint64_t c = 0; c < n_32; c++) {
            uint64_t bitpos = offset_after_64 + (c << 5);
            const uint8_t* A_ptr = a + (bitpos >> 3);
            const uint8_t* B_ptr = b + (used_bits >> 3) + (c << 2);

            uint32_t A_val = *(const uint32_t*)A_ptr;
            uint32_t B_val = *(const uint32_t*)B_ptr;
            uint32_t and_val = A_val & B_val;
            accum ^= (uint64_t)and_val;
        }

        used_bits += n_32 << 5;
        uint64_t offset_after_32 = row_offset + used_bits;

        // 5. Process 8-bit blocks
        for (uint64_t c = 0; c < n_8; c++) {
            uint64_t bitpos = offset_after_32 + (c << 3);
            const uint8_t* A_ptr = a + (bitpos >> 3);
            const uint8_t* B_ptr = b + (used_bits >> 3) + c;

            uint8_t A_val = *A_ptr;
            uint8_t B_val = *B_ptr;
            accum ^= (A_val & B_val);
        }

        // Compute parity of accum
        uint8_t parity = parity64(accum);

        // Set bit i of res to parity  (branchless).
        ((uint8_t*)res)[i >> 3] ^= (uint8_t)(parity << (i & 7));
    }
}






// Number of A rows processed together by the lambda=128 kernel. Each B[j] block is
// loaded once per row-group instead of once per row, cutting redundant B reads ~R x.
// Must be even (rows are packed 2-per-__m256i). Tuned by benchmark.
#define MVP128_BLOCK_ROWS 4

// Specialized function for lambda=128 bits (16 bytes).
//
// Computes res = A * B over F2: res[i] (128 bits) = XOR of B[j] (128 bits) over all
// columns j where A[i][j] == 1.
//
// Strategy: process MVP128_BLOCK_ROWS rows of A at a time, keeping their result
// accumulators live in registers while sweeping the columns once. Each B[j] is loaded
// a single time for the whole row-group and XORed (branchlessly, via an all-ones /
// all-zeros mask built from the A bit) into every accumulator whose row selects it.
// Two rows share one __m256i accumulator (one 128-bit lane each), so B[j] is broadcast
// to both lanes and full AVX2 width is used.
//
// Assumes ncols is a multiple of 8 (guaranteed by the consistency-check callers), so A
// rows are byte aligned and there are no leftover bits.
static void matrix_vector_product_f2_block_128(
    uint64_t nrows,
    uint64_t ncols,
    uint8_t* res,
    const uint8_t* A,
    const uint8_t* B
) {
    const uint64_t lambda = 16;
    memset(res, 0, nrows * lambda);

    const uint64_t full_bytes = ncols / 8;  // ncols is a multiple of 8
    const uint64_t row_bytes = full_bytes;
    const int R = MVP128_BLOCK_ROWS;

    uint64_t i = 0;
    for (; i + (uint64_t)R <= nrows; i += R) {
        __m256i acc[MVP128_BLOCK_ROWS / 2];
        for (int p = 0; p < R / 2; p++) acc[p] = _mm256_setzero_si256();

        const uint8_t* A_rows[MVP128_BLOCK_ROWS];
        for (int r = 0; r < R; r++) A_rows[r] = A + (i + r) * row_bytes;

        for (uint64_t byte_i = 0; byte_i < full_bytes; byte_i++) {
            uint8_t a_byte[MVP128_BLOCK_ROWS];
            for (int r = 0; r < R; r++) a_byte[r] = A_rows[r][byte_i];

            for (int bit = 0; bit < 8; bit++) {
                const uint8_t* b_j = B + (byte_i * 8 + bit) * lambda;
                __m128i bj = _mm_loadu_si128((const __m128i*)b_j);
                __m256i bcast = _mm256_broadcastsi128_si256(bj);  // [B[j], B[j]]

                for (int p = 0; p < R / 2; p++) {
                    // Lane 0 -> row (i+2p), lane 1 -> row (i+2p+1); mask is all-ones
                    // when the corresponding A bit is set, all-zeros otherwise.
                    int64_t m0 = -(int64_t)((a_byte[2 * p] >> bit) & 1);
                    int64_t m1 = -(int64_t)((a_byte[2 * p + 1] >> bit) & 1);
                    __m256i mask = _mm256_set_epi64x(m1, m1, m0, m0);
                    acc[p] = _mm256_xor_si256(acc[p], _mm256_and_si256(bcast, mask));
                }
            }
        }

        for (int p = 0; p < R / 2; p++) {
            __m128i lo = _mm256_castsi256_si128(acc[p]);
            __m128i hi = _mm256_extracti128_si256(acc[p], 1);
            _mm_storeu_si128((__m128i*)(res + (i + 2 * p) * lambda), lo);
            _mm_storeu_si128((__m128i*)(res + (i + 2 * p + 1) * lambda), hi);
        }
    }

    // Tail: fewer than R rows left. Handle one row at a time.
    for (; i < nrows; i++) {
        __m256i acc = _mm256_setzero_si256();
        const uint8_t* A_row = A + i * row_bytes;
        for (uint64_t byte_i = 0; byte_i < full_bytes; byte_i++) {
            uint8_t a = A_row[byte_i];
            while (a) {
                int bit = __builtin_ctz(a);
                a &= (uint8_t)(a - 1);
                const uint8_t* b_j = B + (byte_i * 8 + bit) * lambda;
                __m128i bj = _mm_loadu_si128((const __m128i*)b_j);
                acc = _mm256_xor_si256(acc, _mm256_zextsi128_si256(bj));
            }
        }
        _mm_storeu_si128((__m128i*)(res + i * lambda), _mm256_castsi256_si128(acc));
    }
}


// Number of A rows processed together by the lambda=192 kernel (see block_128 for the
// rationale). A 192-bit row occupies 24 of the 32 bytes of a __m256i, so each row gets
// its own full-width accumulator (no 2-rows-per-lane packing).
#define MVP192_BLOCK_ROWS 8

// Specialized function for lambda=192 bits (24 bytes). Same row-blocked, branchless
// strategy as matrix_vector_product_f2_block_128: process MVP192_BLOCK_ROWS rows at a
// time so each B[j] block is loaded once per row-group and XORed (via an all-ones /
// all-zeros mask) into the accumulator of every selecting row.
//
// Each 24-byte B[j] is assembled from a 16-byte and an 8-byte load (never a 32-byte
// load) so the final block does not read past the end of B; the high 8 bytes of the
// __m256i stay zero, so the unused top of every accumulator remains zero and only 24
// bytes are stored per row.
//
// Assumes ncols is a multiple of 8 (guaranteed by the consistency-check callers).
static void matrix_vector_product_f2_block_192(
    uint64_t nrows,
    uint64_t ncols,
    uint8_t* res,
    const uint8_t* A,
    const uint8_t* B
) {
    const uint64_t lambda = 24;
    memset(res, 0, nrows * lambda);

    const uint64_t full_bytes = ncols / 8;  // ncols is a multiple of 8
    const uint64_t row_bytes = full_bytes;
    const int R = MVP192_BLOCK_ROWS;

    uint64_t i = 0;
    for (; i + (uint64_t)R <= nrows; i += R) {
        __m256i acc[MVP192_BLOCK_ROWS];
        for (int r = 0; r < R; r++) acc[r] = _mm256_setzero_si256();

        const uint8_t* A_rows[MVP192_BLOCK_ROWS];
        for (int r = 0; r < R; r++) A_rows[r] = A + (i + r) * row_bytes;

        for (uint64_t byte_i = 0; byte_i < full_bytes; byte_i++) {
            uint8_t a_byte[MVP192_BLOCK_ROWS];
            for (int r = 0; r < R; r++) a_byte[r] = A_rows[r][byte_i];

            for (int bit = 0; bit < 8; bit++) {
                const uint8_t* b_j = B + (byte_i * 8 + bit) * lambda;
                __m128i lo = _mm_loadu_si128((const __m128i*)b_j);      // bytes 0..15
                __m128i hi = _mm_loadl_epi64((const __m128i*)(b_j + 16));  // bytes 16..23
                __m256i bj = _mm256_inserti128_si256(_mm256_castsi128_si256(lo), hi, 1);

                for (int r = 0; r < R; r++) {
                    __m256i mask = _mm256_set1_epi32(-(int32_t)((a_byte[r] >> bit) & 1));
                    acc[r] = _mm256_xor_si256(acc[r], _mm256_and_si256(bj, mask));
                }
            }
        }

        for (int r = 0; r < R; r++) {
            uint8_t* r_i = res + (i + r) * lambda;
            _mm_storeu_si128((__m128i*)r_i, _mm256_castsi256_si128(acc[r]));
            _mm_storel_epi64((__m128i*)(r_i + 16), _mm256_extracti128_si256(acc[r], 1));
        }
    }

    // Tail: fewer than R rows left. Handle one row at a time.
    for (; i < nrows; i++) {
        __m256i acc = _mm256_setzero_si256();
        const uint8_t* A_row = A + i * row_bytes;
        for (uint64_t byte_i = 0; byte_i < full_bytes; byte_i++) {
            uint8_t a = A_row[byte_i];
            while (a) {
                int bit = __builtin_ctz(a);
                a &= (uint8_t)(a - 1);
                const uint8_t* b_j = B + (byte_i * 8 + bit) * lambda;
                __m128i lo = _mm_loadu_si128((const __m128i*)b_j);
                __m128i hi = _mm_loadl_epi64((const __m128i*)(b_j + 16));
                __m256i bj = _mm256_inserti128_si256(_mm256_castsi128_si256(lo), hi, 1);
                acc = _mm256_xor_si256(acc, bj);
            }
        }
        uint8_t* r_i = res + i * lambda;
        _mm_storeu_si128((__m128i*)r_i, _mm256_castsi256_si128(acc));
        _mm_storel_epi64((__m128i*)(r_i + 16), _mm256_extracti128_si256(acc, 1));
    }
}


// Number of A rows processed together by the lambda=256 kernel (see block_128 for the
// rationale). A 256-bit row is exactly one __m256i, so each row gets its own accumulator.
#define MVP256_BLOCK_ROWS 12

// Specialized function for lambda=256 bits (32 bytes). Same row-blocked, branchless
// strategy as matrix_vector_product_f2_block_128: process MVP256_BLOCK_ROWS rows at a
// time so each B[j] block is loaded once per row-group and XORed (via an all-ones /
// all-zeros mask) into the accumulator of every selecting row.
//
// Assumes ncols is a multiple of 8 (guaranteed by the consistency-check callers).
static void matrix_vector_product_f2_block_256(
    uint64_t nrows,
    uint64_t ncols,
    uint8_t* res,
    const uint8_t* A,
    const uint8_t* B
) {
    const uint64_t lambda = 32;
    memset(res, 0, nrows * lambda);

    const uint64_t full_bytes = ncols / 8;  // ncols is a multiple of 8
    const uint64_t row_bytes = full_bytes;
    const int R = MVP256_BLOCK_ROWS;

    uint64_t i = 0;
    for (; i + (uint64_t)R <= nrows; i += R) {
        __m256i acc[MVP256_BLOCK_ROWS];
        for (int r = 0; r < R; r++) acc[r] = _mm256_setzero_si256();

        const uint8_t* A_rows[MVP256_BLOCK_ROWS];
        for (int r = 0; r < R; r++) A_rows[r] = A + (i + r) * row_bytes;

        for (uint64_t byte_i = 0; byte_i < full_bytes; byte_i++) {
            uint8_t a_byte[MVP256_BLOCK_ROWS];
            for (int r = 0; r < R; r++) a_byte[r] = A_rows[r][byte_i];

            for (int bit = 0; bit < 8; bit++) {
                const uint8_t* b_j = B + (byte_i * 8 + bit) * lambda;
                __m256i bj = _mm256_loadu_si256((const __m256i*)b_j);

                for (int r = 0; r < R; r++) {
                    __m256i mask = _mm256_set1_epi32(-(int32_t)((a_byte[r] >> bit) & 1));
                    acc[r] = _mm256_xor_si256(acc[r], _mm256_and_si256(bj, mask));
                }
            }
        }

        for (int r = 0; r < R; r++) {
            _mm256_storeu_si256((__m256i*)(res + (i + r) * lambda), acc[r]);
        }
    }

    // Tail: fewer than R rows left. Handle one row at a time.
    for (; i < nrows; i++) {
        __m256i acc = _mm256_setzero_si256();
        const uint8_t* A_row = A + i * row_bytes;
        for (uint64_t byte_i = 0; byte_i < full_bytes; byte_i++) {
            uint8_t a = A_row[byte_i];
            while (a) {
                int bit = __builtin_ctz(a);
                a &= (uint8_t)(a - 1);
                const uint8_t* b_j = B + (byte_i * 8 + bit) * lambda;
                acc = _mm256_xor_si256(acc, _mm256_loadu_si256((const __m256i*)b_j));
            }
        }
        _mm256_storeu_si256((__m256i*)(res + i * lambda), acc);
    }
}

// Dispatcher function
void matrix_f2_times_vector_flambda_avx2(
    uint64_t lambda_bits, uint64_t nrows, uint64_t ncols,  //
    flambda_t* res,                                   // vector of size nrows. res = a*b
    const bitmat_t* a,                                // nrows x ncols matrix
    const flambda_t* b
) {
    uint64_t lambda = (uint64_t)lambda_bits >> 3; // bytes
    switch (lambda) {
        case 16:
            matrix_vector_product_f2_block_128(nrows, ncols, res, a, b);
            break;
        case 24:
            matrix_vector_product_f2_block_192(nrows, ncols, res, a, b);
            break;
        case 32:
            matrix_vector_product_f2_block_256(nrows, ncols, res, a, b);
            break;
        default:
            fprintf(stderr, "Unsupported lambda size\n");
            break;
    }
}

EXPORT void bitvec_cascade_xor_to_avx2(uint64_t bytelen, void* c, void* b, const void* a) {
  const uint8_t* ap = (const uint8_t*)a;
  uint8_t* bp = (uint8_t*)b;
  uint8_t* cp = (uint8_t*)c;
  uint64_t i = 0;
  for (; i + 32 <= bytelen; i += 32) {
    __m256i va = _mm256_loadu_si256((const __m256i*)(ap + i));
    __m256i vb = _mm256_loadu_si256((const __m256i*)(bp + i));
    __m256i vc = _mm256_loadu_si256((const __m256i*)(cp + i));
    __m256i vnewb = _mm256_xor_si256(va, vb);
    __m256i vnewc = _mm256_xor_si256(vc, vnewb);
    _mm256_storeu_si256((__m256i*)(bp + i), vnewb);
    _mm256_storeu_si256((__m256i*)(cp + i), vnewc);
  }
  if (i < bytelen) {
    // trailing loop: deal with the last <= 31 bytes
    for (; i + 8 <= bytelen; i += 8) {
      // movq loads/stores: _mm_loadl_epi64 / _mm_storel_epi64 are the SSE2 spellings,
      // unaligned like _mm_loadu_si64 but available on compilers older than gcc 9.
      __m128i va = _mm_loadl_epi64((const __m128i*)(ap + i));
      __m128i vb = _mm_loadl_epi64((const __m128i*)(bp + i));
      __m128i vc = _mm_loadl_epi64((const __m128i*)(cp + i));
      vb = _mm_xor_si128(vb, va);
      vc = _mm_xor_si128(vc, vb);
      _mm_storel_epi64((__m128i*)(bp + i), vb);
      _mm_storel_epi64((__m128i*)(cp + i), vc);
    }
    for (; i < bytelen; ++i) {
      bp[i] ^= ap[i];
      cp[i] ^= bp[i];
    }
  }
}
