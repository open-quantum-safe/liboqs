/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include "utils.h"

#if defined(HAVE_AVX2) || defined(HAVE_SSE2)
#include "cpu.h"
#endif
#if defined(HAVE_AVX2)
#include <immintrin.h>
#elif defined(HAVE_SSE2)
#include <emmintrin.h>
#endif

#include <assert.h>
#include <string.h>

uint16_t extract_bit_slice(const uint8_t* src, unsigned int offset, unsigned int bits) {
  const unsigned int bit_offset = offset & 7;
  const unsigned int byte_idx   = offset >> 3;
  assert(bits > 0 && bits <= 16);

  uint32_t value = src[byte_idx] >> bit_offset;
  if (bit_offset + bits > 8) {
    value |= (uint32_t)src[byte_idx + 1] << (8 - bit_offset);
  }
  if (bit_offset + bits > 16) {
    value |= (uint32_t)src[byte_idx + 2] << (16 - bit_offset);
  }
  return (uint16_t)(value & ((UINT32_C(1) << bits) - 1));
}

// DecodeChall_3
static bool decode_chall_3(uint16_t* decoded_chall, const uint8_t* chall, unsigned int i,
                           const faest_paramset_t* params) {
  if (i >= params->tau) {
    return false;
  }

  const unsigned int t1 = params->tau1;
  const unsigned int k  = params->k;

  unsigned int lo;
  unsigned int hi;
  if (i < t1) {
    lo = i * k;
    hi = (i + 1) * k;
  } else {
    const unsigned int t = i - t1;

    lo = (t1 * k) + (t * (k - 1));
    hi = (t1 * k) + ((t + 1) * (k - 1));
  }

  const unsigned int bits = hi - lo;
  *decoded_chall          = extract_bit_slice(chall, lo, bits);
  return true;
}

bool decode_all_chall_3(uint16_t* decoded_chall, const uint8_t* chall,
                        const faest_paramset_t* params) {
  for (unsigned int i = 0; i != params->tau; ++i) {
    if (!decode_chall_3(&decoded_chall[i], chall, i, params)) {
      return false;
    }
  }
  return true;
}

#if defined(HAVE_AVX2)
ATTR_TARGET_AVX2 static inline void xor_u8_array_256(uint8_t* out, const uint8_t* a,
                                                     const uint8_t* b, size_t len) {
  const size_t blocks = len / sizeof(__m256i);
  for (size_t idx = 0; idx != blocks;
       ++idx, a += sizeof(__m256i), b += sizeof(__m256i), out += sizeof(__m256i)) {
    const __m256i_u* pa = (const __m256i_u*)a;
    const __m256i_u* pb = (const __m256i_u*)b;
    __m256i_u* pout     = (__m256i_u*)out;

    _mm256_storeu_si256(pout, _mm256_xor_si256(_mm256_loadu_si256(pa), _mm256_loadu_si256(pb)));
  }

  xor_small_u8_array(out, a, b, len - blocks * sizeof(__m256i));
}

ATTR_TARGET_AVX2 static inline void xor_u8_array_inplace_256(uint8_t* out, const uint8_t* a,
                                                             size_t len) {
  const size_t blocks = len / sizeof(__m256i);
  for (size_t idx = 0; idx != blocks; ++idx, a += sizeof(__m256i), out += sizeof(__m256i)) {
    const __m256i_u* pa = (const __m256i_u*)a;
    __m256i_u* pout     = (__m256i_u*)out;

    _mm256_storeu_si256(pout, _mm256_xor_si256(_mm256_loadu_si256(pout), _mm256_loadu_si256(pa)));
  }

  xor_small_u8_array_inplace(out, a, len - blocks * sizeof(__m256i));
}
#endif

#if defined(HAVE_SSE2)
ATTR_TARGET_SSE2 static inline void xor_u8_array_128(uint8_t* out, const uint8_t* a,
                                                     const uint8_t* b, size_t len) {
  const size_t blocks = len / sizeof(__m128i);
  for (size_t idx = 0; idx != blocks;
       ++idx, a += sizeof(__m128i), b += sizeof(__m128i), out += sizeof(__m128i)) {
    const __m128i_u* pa = (const __m128i_u*)a;
    const __m128i_u* pb = (const __m128i_u*)b;
    __m128i_u* pout     = (__m128i_u*)out;

    _mm_storeu_si128(pout, _mm_xor_si128(_mm_loadu_si128(pa), _mm_loadu_si128(pb)));
  }

  xor_small_u8_array(out, a, b, len - blocks * sizeof(__m128i));
}

ATTR_TARGET_SSE2 static inline void xor_u8_array_inplace_128(uint8_t* out, const uint8_t* a,
                                                             size_t len) {
  const size_t blocks = len / sizeof(__m128i);
  for (size_t idx = 0; idx != blocks; ++idx, a += sizeof(__m128i), out += sizeof(__m128i)) {
    const __m128i_u* pa = (const __m128i_u*)a;
    __m128i_u* pout     = (__m128i_u*)out;

    _mm_storeu_si128(pout, _mm_xor_si128(_mm_loadu_si128(pout), _mm_loadu_si128(pa)));
  }

  xor_small_u8_array_inplace(out, a, len - blocks * sizeof(__m128i));
}
#endif

static inline void xor_u8_array_64(uint8_t* out, const uint8_t* a, const uint8_t* b, size_t len) {
  const size_t blocks = len / sizeof(uint64_t);
  for (size_t idx = 0; idx != blocks;
       ++idx, a += sizeof(uint64_t), b += sizeof(uint64_t), out += sizeof(uint64_t)) {
    uint64_t lhs;
    uint64_t rhs;
    memcpy(&lhs, a, sizeof(lhs));
    memcpy(&rhs, b, sizeof(rhs));
    lhs ^= rhs;
    memcpy(out, &lhs, sizeof(lhs));
  }

  xor_small_u8_array(out, a, b, len - blocks * sizeof(uint64_t));
}

static inline void xor_u8_array_inplace_64(uint8_t* out, const uint8_t* a, size_t len) {
  const size_t blocks = len / sizeof(uint64_t);
  for (size_t idx = 0; idx != blocks; ++idx, a += sizeof(uint64_t), out += sizeof(uint64_t)) {
    uint64_t lhs;
    uint64_t rhs;
    memcpy(&lhs, out, sizeof(lhs));
    memcpy(&rhs, a, sizeof(rhs));
    lhs ^= rhs;
    memcpy(out, &lhs, sizeof(lhs));
  }

  xor_small_u8_array_inplace(out, a, len - blocks * sizeof(uint64_t));
}

#if defined(HAVE_AVX2)
ATTR_TARGET_AVX2 static inline void masked_xor_u8_array_256(uint8_t* out, const uint8_t* a,
                                                            const uint8_t* b, uint8_t mask_bit,
                                                            size_t len) {
  const size_t blocks = len / sizeof(__m256i);
  const __m256i mask  = _mm256_set1_epi64x(-(uint64_t)mask_bit);
  for (size_t idx = 0; idx != blocks;
       ++idx, a += sizeof(__m256i), b += sizeof(__m256i), out += sizeof(__m256i)) {
    const __m256i_u* pa = (const __m256i_u*)a;
    const __m256i_u* pb = (const __m256i_u*)b;
    __m256i_u* pout     = (__m256i_u*)out;
    _mm256_storeu_si256(pout, _mm256_xor_si256(_mm256_loadu_si256(pa),
                                               _mm256_and_si256(_mm256_loadu_si256(pb), mask)));
  }

  masked_xor_small_u8_array(out, a, b, mask_bit, len - blocks * sizeof(__m256i));
}
#endif

#if defined(HAVE_SSE2)
ATTR_TARGET_SSE2 static inline void masked_xor_u8_array_128(uint8_t* out, const uint8_t* a,
                                                            const uint8_t* b, uint8_t mask_bit,
                                                            size_t len) {
  const size_t blocks = len / sizeof(__m128i);
  const __m128i mask  = _mm_set1_epi64x(-(uint64_t)mask_bit);
  for (size_t idx = 0; idx != blocks;
       ++idx, a += sizeof(__m128i), b += sizeof(__m128i), out += sizeof(__m128i)) {
    const __m128i_u* pa = (const __m128i_u*)a;
    const __m128i_u* pb = (const __m128i_u*)b;
    __m128i_u* pout     = (__m128i_u*)out;

    _mm_storeu_si128(pout,
                     _mm_xor_si128(_mm_loadu_si128(pa), _mm_and_si128(_mm_loadu_si128(pb), mask)));
  }

  masked_xor_small_u8_array(out, a, b, mask_bit, len - blocks * sizeof(__m128i));
}
#endif

static inline void masked_xor_u8_array_64(uint8_t* out, const uint8_t* a, const uint8_t* b,
                                          uint8_t mask_bit, size_t len) {
  const size_t blocks = len / sizeof(uint64_t);
  const uint64_t mask = -(uint64_t)mask_bit;
  for (size_t idx = 0; idx != blocks;
       ++idx, a += sizeof(uint64_t), b += sizeof(uint64_t), out += sizeof(uint64_t)) {
    uint64_t lhs;
    uint64_t rhs;
    memcpy(&lhs, a, sizeof(lhs));
    memcpy(&rhs, b, sizeof(rhs));
    lhs ^= rhs & mask;
    memcpy(out, &lhs, sizeof(lhs));
  }

  masked_xor_small_u8_array(out, a, b, mask_bit, len - blocks * sizeof(uint64_t));
}

void xor_u8_array(uint8_t* out, const uint8_t* a, const uint8_t* b, size_t len) {
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AVX2) {
    xor_u8_array_256(out, a, b, len);
    return;
  }
#endif

#if defined(HAVE_SSE2)
  if (CPU_SUPPORTS_SSE2) {
    xor_u8_array_128(out, a, b, len);
    return;
  }
#endif

  if (len >= sizeof(uint64_t)) {
    xor_u8_array_64(out, a, b, len);
    return;
  }

  for (size_t i = 0; i < len; i++) {
    out[i] = a[i] ^ b[i];
  }
}

void xor_u8_array_inplace(uint8_t* out, const uint8_t* a, size_t len) {
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AVX2) {
    xor_u8_array_inplace_256(out, a, len);
    return;
  }
#endif

#if defined(HAVE_SSE2)
  if (CPU_SUPPORTS_SSE2) {
    xor_u8_array_inplace_128(out, a, len);
    return;
  }
#endif

  if (len >= sizeof(uint64_t)) {
    xor_u8_array_inplace_64(out, a, len);
    return;
  }

  for (size_t i = 0; i < len; i++) {
    out[i] ^= a[i];
  }
}

void masked_xor_u8_array(uint8_t* out, const uint8_t* a, const uint8_t* b, uint8_t mask_bit,
                         size_t len) {
  mask_bit &= 1u;
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AVX2) {
    masked_xor_u8_array_256(out, a, b, mask_bit, len);
    return;
  }
#endif

#if defined(HAVE_SSE2)
  if (CPU_SUPPORTS_SSE2) {
    masked_xor_u8_array_128(out, a, b, mask_bit, len);
    return;
  }
#endif

  if (len >= sizeof(uint64_t)) {
    masked_xor_u8_array_64(out, a, b, mask_bit, len);
    return;
  }

  const uint8_t mask = -mask_bit;
  for (size_t i = 0; i < len; i++) {
    out[i] = a[i] ^ (b[i] & mask);
  }
}

#if 0
void print_u8_array(const char* label, const uint8_t* arr, size_t m) {
  printf("%s: ", label);
  for (size_t i = 0; i < m; i++) {
    printf("%02x ", arr[i]);
  }
  printf("\n");
}

void print_u8_array_bits(const char* label, const uint8_t* arr, size_t m) {
  printf("%s: ", label);
  for (size_t i = 0; i < m; i++) {
    for (int b = 0; b < 8; b++) {
      printf("%u", (arr[i] >> b) & 1u);
    }
    printf(" ");
  }
  printf("\n");
}
#endif

// Based on Henry S. Warren, Jr., Hacker's Delight, "Transposing a Bit Matrix":
// https://github.com/hcs0/Hackers-Delight/blob/master/transpose8.c.txt
static inline uint64_t transpose_8x8(uint64_t value) {
  uint64_t tmp = (value ^ (value >> 7)) & UINT64_C(0x00aa00aa00aa00aa);
  value ^= tmp ^ (tmp << 7);
  tmp = (value ^ (value >> 14)) & UINT64_C(0x0000cccc0000cccc);
  value ^= tmp ^ (tmp << 14);
  tmp = (value ^ (value >> 28)) & UINT64_C(0x00000000f0f0f0f0);
  return value ^ tmp ^ (tmp << 28);
}

// 64-bit generalization of the recursive 32x32 mask-and-swap transpose
// https://github.com/hcs0/Hackers-Delight/blob/master/transpose32.c.txt
static void transpose_64x64(uint64_t* value) {
  uint64_t mask = UINT64_C(0x00000000ffffffff);
  for (unsigned int shift = 32; shift != 0;) {
    for (unsigned int idx = 0; idx < 64; idx = (idx + shift + 1) & ~shift) {
      const uint64_t tmp = ((value[idx] >> shift) ^ value[idx + shift]) & mask;
      value[idx] ^= tmp << shift;
      value[idx + shift] ^= tmp;
    }
    shift >>= 1;
    mask ^= mask << shift;
  }
}

#if defined(HAVE_SSE2)
// Compose 16x8 SSE2 transposes into a 128x128 transpose.
// https://mischasan.wordpress.com/2011/10/03/the-full-sse2-bit-matrix-transpose-routine/
ATTR_TARGET_SSE2 static void transpose_128x128(uint8_t** v, uint8_t** v_row_maj, unsigned int row,
                                               unsigned int col) {
  for (unsigned int row_offset = 0; row_offset < 128; row_offset += 16) {
    const unsigned int dst_byte = (row + row_offset) / 8;
    for (unsigned int col_offset = 0; col_offset < 128; col_offset += 8) {
      const unsigned int src_byte = (col + col_offset) / 8;

      ATTR_ALIGNED(16) uint8_t input[16];
      for (unsigned int idx = 0; idx < 16; ++idx) {
        input[idx] = v[row + row_offset + idx][src_byte];
      }

      __m128i block = _mm_load_si128((const __m128i*)input);
      for (unsigned int bit = 0; bit < 8; ++bit) {
        const uint16_t output = _mm_movemask_epi8(block);
        memcpy(v_row_maj[col + col_offset + 7 - bit] + dst_byte, &output, sizeof(output));
        block = _mm_slli_epi64(block, 1);
      }
    }
  }
}
#endif

#if defined(HAVE_AVX2)
// Apply one stage of a 32x32 mask-and-swap transpose in all eight 32-bit lanes.
#define TRANSPOSE_AVX2_32X32_STAGE(value, shift, mask_value)                                       \
  do {                                                                                             \
    const __m256i mask = _mm256_set1_epi32(mask_value);                                            \
    for (unsigned int idx = 0; idx < 32; idx = (idx + (shift) + 1) & ~(shift)) {                   \
      const __m256i tmp = _mm256_and_si256(                                                        \
          _mm256_xor_si256(_mm256_srli_epi32((value)[idx], shift), (value)[idx + (shift)]), mask); \
      (value)[idx]           = _mm256_xor_si256((value)[idx], _mm256_slli_epi32(tmp, shift));      \
      (value)[idx + (shift)] = _mm256_xor_si256((value)[idx + (shift)], tmp);                      \
    }                                                                                              \
  } while (0)

// Vectorized generalization of the recursive 32x32 transpose
ATTR_TARGET_AVX2 static void transpose_256x256(uint8_t** v, uint8_t** v_row_maj, unsigned int row,
                                               unsigned int col) {
  for (unsigned int row_offset = 0; row_offset < 256; row_offset += 32) {
    const unsigned int dst_byte = (row + row_offset) / 8;
    const unsigned int src_byte = col / 8;
    __m256i block[32];

    for (unsigned int idx = 0; idx < 32; ++idx) {
      block[idx] = _mm256_loadu_si256((const __m256i*)(v[row + row_offset + idx] + src_byte));
    }

    TRANSPOSE_AVX2_32X32_STAGE(block, 16, 0x0000ffff);
    TRANSPOSE_AVX2_32X32_STAGE(block, 8, 0x00ff00ff);
    TRANSPOSE_AVX2_32X32_STAGE(block, 4, 0x0f0f0f0f);
    TRANSPOSE_AVX2_32X32_STAGE(block, 2, 0x33333333);
    TRANSPOSE_AVX2_32X32_STAGE(block, 1, 0x55555555);

    for (unsigned int idx = 0; idx < 32; ++idx) {
      uint32_t output[8];
      _mm256_storeu_si256((__m256i*)output, block[idx]);
      for (unsigned int lane = 0; lane < 8; ++lane) {
        memcpy(v_row_maj[col + 32 * lane + idx] + dst_byte, &output[lane], sizeof(output[lane]));
      }
    }
  }
}
#endif

void transpose_matrix(uint8_t** v, uint8_t** v_row_maj, unsigned int v_row_bits_len,
                      unsigned int v_col_bits_len) {
  unsigned int full_rows_128 = 0;
  unsigned int full_cols_128 = 0;

#if defined(HAVE_SSE2)
  unsigned int full_rows_256 = 0;
  unsigned int full_cols_256 = 0;
#if defined(HAVE_AVX2)
  // transpose 256x256 blocks first
  if (CPU_SUPPORTS_AVX2) {
    full_rows_256 = v_row_bits_len & ~255;
    full_cols_256 = v_col_bits_len & ~255;

    for (unsigned int row = 0; row < full_rows_256; row += 256) {
      for (unsigned int col = 0; col < full_cols_256; col += 256) {
        transpose_256x256(v, v_row_maj, row, col);
      }
    }
  }
#endif

  // transpose 128x128 blocks next
  if (CPU_SUPPORTS_SSE2) {
    full_rows_128 = v_row_bits_len & ~127;
    full_cols_128 = v_col_bits_len & ~127;

    for (unsigned int row = 0; row < full_rows_128; row += 128) {
      const unsigned int first_col = row < full_rows_256 ? full_cols_256 : 0;
      for (unsigned int col = first_col; col < full_cols_128; col += 128) {
        transpose_128x128(v, v_row_maj, row, col);
      }
    }
  }
#endif

  // transpose 64x64 blocks next
  const unsigned int full_rows_64 = v_row_bits_len & ~63;
  const unsigned int full_cols_64 = v_col_bits_len & ~63;

  for (unsigned int row = 0; row < full_rows_64; row += 64) {
    const unsigned int dst_byte  = row / 8;
    const unsigned int first_col = row < full_rows_128 ? full_cols_128 : 0;
    for (unsigned int col = first_col; col < full_cols_64; col += 64) {
      const unsigned int src_byte = col / 8;

      uint64_t block[64];
      for (unsigned int idx = 0; idx < 64; ++idx) {
        memcpy(&block[idx], v[row + idx] + src_byte, sizeof(block[idx]));
        block[idx] = le64toh(block[idx]);
      }
      transpose_64x64(block);
      for (unsigned int idx = 0; idx < 64; ++idx) {
        block[idx] = htole64(block[idx]);
        memcpy(v_row_maj[col + idx] + dst_byte, &block[idx], sizeof(block[idx]));
      }
    }
  }

  // then do 8x8 blocks
  const unsigned int full_rows_8 = v_row_bits_len & ~7;
  for (unsigned int row = 0; row < full_rows_8; row += 8) {
    const unsigned int dst_byte  = row / 8;
    const unsigned int first_col = row < full_rows_64 ? full_cols_64 : 0;
    for (unsigned int col = first_col; col < v_col_bits_len; col += 8) {
      const unsigned int src_byte = col / 8;
      const unsigned int columns  = MIN(v_col_bits_len - col, 8);
      uint64_t block              = 0;

      for (unsigned int bit = 0; bit < 8; ++bit) {
        block |= (uint64_t)v[row + bit][src_byte] << (bit * 8);
      }
      block = transpose_8x8(block);

      for (unsigned int bit = 0; bit < columns; ++bit) {
        v_row_maj[col + bit][dst_byte] = (uint8_t)(block >> (bit * 8));
      }
    }
  }

  // for the rest, perform bit operations
  const unsigned int remaining_rows = v_row_bits_len - full_rows_8;
  if (remaining_rows != 0) {
    const unsigned int dst_byte = full_rows_8 / 8;
    const uint8_t mask          = bit_word_mask(remaining_rows);

    for (unsigned int col = 0; col < v_col_bits_len; ++col) {
      uint8_t value = 0;
      for (unsigned int bit = 0; bit < remaining_rows; ++bit) {
        value |= ptr_get_bit(v[full_rows_8 + bit], col) << bit;
      }
      v_row_maj[col][dst_byte] = (v_row_maj[col][dst_byte] & ~mask) | value;
    }
  }
}

uint8_t** alloc_pointer_array(size_t rows, size_t columns) {
  uint8_t** array = malloc(rows * sizeof(uint8_t*)); // IGNORE memory-check
  assert(array);
  array[0] = calloc(rows, columns); // IGNORE memory-check
  assert(array[0]);
  for (unsigned int i = 1; i < rows; ++i) {
    array[i] = array[0] + i * columns;
  }

  return array;
}

void free_pointer_array(uint8_t*** ptr) {
  free((*ptr)[0]); // IGNORE memory-check
  free(*ptr); // IGNORE memory-check
  *ptr = NULL;
}