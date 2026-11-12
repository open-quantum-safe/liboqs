/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include "fields.h"

#include "compat.h"
#include "utils.h"

// GF(2^8) with X^8 + X^4 + X^3 + X^1 + 1
#define bf8_modulus (UINT8_C((1 << 4) | (1 << 3) | (1 << 1) | 1))
// GF(2^64) with X^64 + X^4 + X^3 + X^1 + 1
#define bf64_modulus (UINT64_C((1 << 4) | (1 << 3) | (1 << 1) | 1))

// GF(2^8) implementation

bf8_t bf8_mul(bf8_t lhs, bf8_t rhs) {
  bf8_t result = -(rhs & 1) & lhs;
  for (unsigned int idx = 1; idx < 8; ++idx) {
    const uint8_t mask = -((lhs >> 7) & 1);
    lhs                = (lhs << 1) ^ (mask & bf8_modulus);
    result ^= -((rhs >> idx) & 1) & lhs;
  }
  return result;
}

// Multiply by X modulo X^8 + X^4 + X^3 + X + 1.
bf8_t bf8_mulX(bf8_t value) {
  const uint8_t mask = -(value >> 7);
  return (bf8_t)((value << 1) ^ (mask & bf8_modulus));
}

bf8_t bf8_square(bf8_t lhs) {
  // In characteristic two, squaring inserts a zero between consecutive polynomial bits.
  uint16_t square = lhs;
  square          = (square | (square << 4)) & UINT16_C(0x0f0f);
  square          = (square | (square << 2)) & UINT16_C(0x3333);
  square          = (square | (square << 1)) & UINT16_C(0x5555);

  // Reduce using X^8 = X^4 + X^3 + X + 1. The second fold fits in one byte.
  const uint16_t high  = square >> 8;
  square               = (square & UINT16_C(0xff)) ^ high ^ (high << 1) ^ (high << 3) ^ (high << 4);
  const uint16_t carry = square >> 8;
  return (bf8_t)(square ^ carry ^ (carry << 1) ^ (carry << 3) ^ (carry << 4));
}

bf8_t bf8_inv(bf8_t in) {
  // Compute in^254 with four multiplications and seven squarings; zero maps to zero.
  const bf8_t t2   = bf8_square(in);
  const bf8_t t3   = bf8_mul(in, t2);
  const bf8_t t6   = bf8_square(t3);
  const bf8_t t12  = bf8_square(t6);
  const bf8_t t15  = bf8_mul(t12, t3);
  const bf8_t t30  = bf8_square(t15);
  const bf8_t t60  = bf8_square(t30);
  const bf8_t t120 = bf8_square(t60);
  const bf8_t t240 = bf8_square(t120);
  const bf8_t t252 = bf8_mul(t240, t12);
  return bf8_mul(t252, t2);
}

// GF(2^64) implementation

#if defined(FAEST_TESTS)
bf64_t bf64_mul(bf64_t lhs, bf64_t rhs) {
  bf64_t result = (-(rhs & 1)) & lhs;
  for (unsigned int idx = 1; idx != 64; ++idx) {
    const uint64_t mask = -((lhs >> 63) & 1);
    lhs                 = (lhs << 1) ^ (mask & bf64_modulus);
    result ^= (-((rhs >> idx) & 1)) & lhs;
  }
  return result;
}
#endif

// GF(2^128) implementation

static ATTR_ALIGNED(BF128_ALIGN) const uint8_t bf128_alpha[7][BF128_NUM_BYTES] = {
    {0x0d, 0xce, 0x60, 0x55, 0xac, 0xe8, 0x3f, 0xa1, 0x1c, 0x9a, 0x97, 0xa9, 0x55, 0x85, 0x3d,
     0x05},
    {0xe1, 0xae, 0x88, 0x34, 0xca, 0x59, 0x77, 0xec, 0x84, 0xbb, 0xbf, 0x9c, 0x43, 0xb7, 0xf4,
     0x4c},
    {0xa8, 0x46, 0x39, 0x36, 0xae, 0x02, 0xcf, 0xbf, 0xc6, 0xd2, 0x51, 0x7d, 0x4f, 0x60, 0xad,
     0x35},
    {0x49, 0x98, 0x2e, 0x3c, 0x48, 0x30, 0x83, 0x6b, 0xfe, 0x22, 0xa2, 0x40, 0x46, 0x36, 0xcb,
     0x0d},
    {0xb4, 0x82, 0x1b, 0x7b, 0x27, 0x49, 0x2b, 0x25, 0xa5, 0xde, 0x88, 0x1a, 0xe1, 0x10, 0x98,
     0x54},
    {0x22, 0xff, 0x21, 0x25, 0xef, 0xf2, 0x2b, 0xc7, 0x75, 0x1f, 0x0c, 0x6c, 0x68, 0xa5, 0x81,
     0xd6},
    {0xbc, 0xf9, 0x36, 0xe1, 0x94, 0x8e, 0x7a, 0x7a, 0xe0, 0x8f, 0xb7, 0x4f, 0x1a, 0x31, 0x50,
     0x09},
};

#if defined(FAEST_TESTS)
bf128_t bf128_get_alpha(unsigned int idx) {
  bf128_t ret;
  bf128_load(&ret, bf128_alpha[idx]);
  return ret;
}
#endif

void bf128_byte_combine(bf128_t* dst, const bf128_t* x) {
  *dst = x[0];
  for (unsigned int i = 1; i < 8; ++i) {
    bf128_t tmp;
    bf128_load(&tmp, bf128_alpha[i - 1]);
    bf128_mul_inplace(&tmp, &x[i]);
    bf128_add_inplace(dst, &tmp);
  }
}

void bf128_sq_bit_inplace(bf128_t* tag) {
  bf128_add_inplace(&tag[0], &tag[4]);
  bf128_add_inplace(&tag[0], &tag[6]);
  const bf128_t i1 = tag[1];
  bf128_add(&tag[1], &tag[4], &tag[6]);
  bf128_add_inplace(&tag[1], &tag[7]);
  const bf128_t i2 = tag[2];
  bf128_add(&tag[2], &i1, &tag[5]);
  const bf128_t i3 = tag[3];
  bf128_add(&tag[3], &tag[4], &tag[5]);
  bf128_add_inplace(&tag[3], &tag[6]);
  bf128_add_inplace(&tag[3], &tag[7]);
  bf128_add_inplace(&tag[4], &tag[7]);
  bf128_add_inplace(&tag[4], &i2);
  const bf128_t i5 = tag[5];
  bf128_add_inplace(&tag[5], &tag[6]);
  const bf128_t i6 = tag[6];
  bf128_add(&tag[6], &i3, &i5);
  bf128_add_inplace(&tag[7], &i6);
}

void bf128_byte_combine_sq(bf128_t* dst, const bf128_t* x) {
  bf128_t bf_tmp[8];
  bf128_sq_bit(bf_tmp, x);
  bf128_byte_combine(dst, bf_tmp);
}

void bf128_byte_combine_bits(bf128_t* dst, uint8_t x) {
  *dst = bf128_from_bit(get_bit(x, 0));
  for (unsigned int i = 1; i < 8; ++i) {
    bf128_t tmp;
    bf128_t alpha;
    bf128_load(&alpha, bf128_alpha[i - 1]);
    bf128_mul_bit(&tmp, &alpha, get_bit(x, i));
    bf128_add_inplace(dst, &tmp);
  }
}

void bf128_sum_poly_bits(bf128_t* dst, const uint8_t* xs) {
  bf128_load(dst, xs);
}

// GF(2^192) implementation

static ATTR_ALIGNED(BF192_ALIGN) const uint8_t bf192_alpha[7][BF192_NUM_BYTES] = {
    {0x63, 0x97, 0x38, 0x6f, 0xd5, 0xa3, 0xc8, 0xcc, 0xea, 0xbd, 0x6e, 0x96,
     0x6c, 0xd7, 0x65, 0xe6, 0x62, 0x36, 0x6b, 0x0e, 0x14, 0xc8, 0x0b, 0x31},
    {0xbb, 0x50, 0xf4, 0x7c, 0x9e, 0x61, 0x33, 0xb2, 0x26, 0x3f, 0x63, 0xd5,
     0x19, 0x1f, 0xf6, 0x7b, 0x34, 0xdb, 0x91, 0xd4, 0x26, 0x37, 0x93, 0xda},
    {0x0d, 0x8a, 0x39, 0xf5, 0x13, 0x2c, 0x6d, 0x9c, 0x19, 0x8d, 0x32, 0x06,
     0x77, 0xe3, 0x32, 0x82, 0xf6, 0x4e, 0x75, 0x3c, 0x70, 0x0d, 0x3b, 0x0c},
    {0x5d, 0xf7, 0x2b, 0xbd, 0x7c, 0x74, 0x20, 0xdd, 0x2e, 0xd2, 0x58, 0x00,
     0xab, 0x42, 0x55, 0x7a, 0x51, 0x12, 0xbc, 0x94, 0x9c, 0x51, 0xec, 0x45},
    {0xf8, 0x2b, 0xce, 0x8a, 0xe2, 0x0c, 0xd5, 0xd8, 0x84, 0xbe, 0xde, 0x67,
     0xb7, 0x8c, 0x16, 0x08, 0x45, 0x70, 0xa6, 0x4b, 0x6a, 0x14, 0x7d, 0xd6},
    {0xba, 0xe1, 0xd5, 0xee, 0x76, 0x9c, 0x0f, 0x97, 0x48, 0x20, 0xd7, 0x5f,
     0xae, 0xf7, 0xea, 0xf3, 0x43, 0xea, 0x6c, 0x69, 0x5f, 0xbd, 0xa6, 0x29},
    {0x71, 0x85, 0x06, 0x65, 0xc2, 0x5d, 0x94, 0xf5, 0xd3, 0xe9, 0x06, 0x39,
     0x62, 0xfd, 0x19, 0x60, 0xb0, 0xc4, 0x87, 0x0f, 0x54, 0x56, 0x7c, 0xc7},
};

#if defined(FAEST_TESTS)
bf192_t bf192_get_alpha(unsigned int idx) {
  bf192_t ret;
  bf192_load(&ret, bf192_alpha[idx]);
  return ret;
}
#endif

void bf192_byte_combine(bf192_t* dst, const bf192_t* x) {
  *dst = x[0];
  for (unsigned int i = 1; i < 8; ++i) {
    bf192_t tmp;
    bf192_load(&tmp, bf192_alpha[i - 1]);
    bf192_mul_inplace(&tmp, &x[i]);
    bf192_add_inplace(dst, &tmp);
  }
}

void bf192_sq_bit_inplace(bf192_t* tag) {
  bf192_add_inplace(&tag[0], &tag[4]);
  bf192_add_inplace(&tag[0], &tag[6]);
  const bf192_t i1 = tag[1];
  bf192_add(&tag[1], &tag[4], &tag[6]);
  bf192_add_inplace(&tag[1], &tag[7]);
  const bf192_t i2 = tag[2];
  bf192_add(&tag[2], &i1, &tag[5]);
  const bf192_t i3 = tag[3];
  bf192_add(&tag[3], &tag[4], &tag[5]);
  bf192_add_inplace(&tag[3], &tag[6]);
  bf192_add_inplace(&tag[3], &tag[7]);
  bf192_add_inplace(&tag[4], &tag[7]);
  bf192_add_inplace(&tag[4], &i2);
  const bf192_t i5 = tag[5];
  bf192_add_inplace(&tag[5], &tag[6]);
  const bf192_t i6 = tag[6];
  bf192_add(&tag[6], &i3, &i5);
  bf192_add_inplace(&tag[7], &i6);
}

void bf192_byte_combine_sq(bf192_t* dst, const bf192_t* x) {
  bf192_t bf_tmp[8];
  bf192_sq_bit(bf_tmp, x);
  bf192_byte_combine(dst, bf_tmp);
}

void bf192_byte_combine_bits(bf192_t* dst, uint8_t x) {
  *dst = bf192_from_bit(get_bit(x, 0));
  for (unsigned int i = 1; i < 8; ++i) {
    bf192_t tmp;
    bf192_t alpha;
    bf192_load(&alpha, bf192_alpha[i - 1]);
    bf192_mul_bit(&tmp, &alpha, get_bit(x, i));
    bf192_add_inplace(dst, &tmp);
  }
}

void bf192_sum_poly_bits(bf192_t* dst, const uint8_t* xs) {
  bf192_load(dst, xs);
}

// GF(2^256) implementation

static ATTR_ALIGNED(BF256_ALIGN) const uint8_t bf256_alpha[7][BF256_NUM_BYTES] = {
    {0xe7, 0xfe, 0xde, 0x0b, 0x42, 0x88, 0x97, 0x96, 0x67, 0x4e, 0x47,
     0xa0, 0x38, 0x8d, 0xd6, 0xbe, 0x6a, 0xe1, 0xf1, 0xf8, 0x45, 0x98,
     0x22, 0xdf, 0x33, 0x58, 0xc9, 0x20, 0xcf, 0xa8, 0xc9, 0x04},
    {0xc1, 0x89, 0x22, 0xd5, 0x2a, 0xf5, 0x5a, 0xa9, 0x2f, 0x07, 0x42,
     0x2c, 0x8d, 0xc4, 0xa5, 0x2b, 0xea, 0xb0, 0x00, 0x6c, 0x37, 0x0d,
     0x4a, 0xd1, 0xf1, 0x4a, 0x5b, 0x9c, 0x69, 0x4d, 0x4e, 0x06},
    {0x1d, 0x9d, 0x80, 0x3f, 0x83, 0xb3, 0xda, 0x55, 0x57, 0x0f, 0x3b,
     0x53, 0x1e, 0x83, 0x71, 0x17, 0x10, 0xac, 0x3f, 0xad, 0x3f, 0x57,
     0x96, 0xfb, 0x8d, 0xf6, 0x11, 0x70, 0xdb, 0xe3, 0x95, 0x61},
    {0xd5, 0xcd, 0x1b, 0xb0, 0x19, 0x05, 0x01, 0xde, 0xf6, 0xe3, 0x30,
     0x1a, 0x91, 0x58, 0x27, 0x75, 0x3f, 0xa0, 0x9e, 0x48, 0xb6, 0x78,
     0x07, 0x2a, 0x38, 0x88, 0x76, 0x4f, 0xd6, 0x4f, 0xc2, 0x56},
    {0xb6, 0x30, 0x8a, 0xe9, 0x29, 0xf5, 0xc2, 0x98, 0x82, 0x84, 0xf1,
     0x40, 0xd4, 0xdb, 0xc4, 0x1b, 0x81, 0xa9, 0x49, 0x7d, 0x94, 0x09,
     0xbe, 0x2f, 0xfc, 0x4f, 0x57, 0x71, 0x6d, 0x0b, 0x27, 0x22},
    {0x0b, 0x67, 0x44, 0xde, 0xb9, 0xaf, 0x75, 0x9e, 0xbc, 0xaf, 0xf1,
     0x66, 0xc6, 0x66, 0xed, 0xac, 0x7e, 0x1f, 0x99, 0xf2, 0x3f, 0x25,
     0x01, 0xf0, 0xf3, 0x29, 0xfa, 0xd1, 0x2f, 0x37, 0x3d, 0xc0},
    {0x8b, 0xe8, 0x32, 0xb3, 0x98, 0xb6, 0x43, 0xba, 0x0d, 0x6f, 0xb8,
     0x25, 0xd6, 0xc4, 0x37, 0x52, 0x45, 0x15, 0xe8, 0xf4, 0x2a, 0x2b,
     0x65, 0x2f, 0xb8, 0x7b, 0x6b, 0xd2, 0x09, 0xea, 0x3e, 0x13},
};

#if defined(FAEST_TESTS)
bf256_t bf256_get_alpha(unsigned int idx) {
  bf256_t ret;
  bf256_load(&ret, bf256_alpha[idx]);
  return ret;
}
#endif

void bf256_byte_combine(bf256_t* dst, const bf256_t* x) {
  *dst = x[0];
  for (unsigned int i = 1; i < 8; ++i) {
    bf256_t tmp;
    bf256_load(&tmp, bf256_alpha[i - 1]);
    bf256_mul_inplace(&tmp, &x[i]);
    bf256_add_inplace(dst, &tmp);
  }
}

void bf256_sq_bit_inplace(bf256_t* tag) {
  bf256_add_inplace(&tag[0], &tag[4]);
  bf256_add_inplace(&tag[0], &tag[6]);
  const bf256_t i1 = tag[1];
  bf256_add(&tag[1], &tag[4], &tag[6]);
  bf256_add_inplace(&tag[1], &tag[7]);
  const bf256_t i2 = tag[2];
  bf256_add(&tag[2], &i1, &tag[5]);
  const bf256_t i3 = tag[3];
  bf256_add(&tag[3], &tag[4], &tag[5]);
  bf256_add_inplace(&tag[3], &tag[6]);
  bf256_add_inplace(&tag[3], &tag[7]);
  bf256_add_inplace(&tag[4], &tag[7]);
  bf256_add_inplace(&tag[4], &i2);
  const bf256_t i5 = tag[5];
  bf256_add_inplace(&tag[5], &tag[6]);
  const bf256_t i6 = tag[6];
  bf256_add(&tag[6], &i3, &i5);
  bf256_add_inplace(&tag[7], &i6);
}

void bf256_byte_combine_sq(bf256_t* dst, const bf256_t* x) {
  bf256_t bf_tmp[8];
  bf256_sq_bit(bf_tmp, x);
  bf256_byte_combine(dst, bf_tmp);
}

void bf256_byte_combine_bits(bf256_t* dst, uint8_t x) {
  *dst = bf256_from_bit(get_bit(x, 0));
  for (unsigned int i = 1; i < 8; ++i) {
    bf256_t tmp;
    bf256_t alpha;
    bf256_load(&alpha, bf256_alpha[i - 1]);
    bf256_mul_bit(&tmp, &alpha, get_bit(x, i));
    bf256_add_inplace(dst, &tmp);
  }
}

void bf256_sum_poly_bits(bf256_t* dst, const uint8_t* xs) {
  bf256_load(dst, xs);
}

// GF2 polynomials

static inline uint64_t load_src_word(const uint8_t* src, size_t word_idx, size_t rows) {
  const size_t word_bit_offset = word_idx * 64;
  const size_t remaining_bits  = rows - word_bit_offset;
  const uint8_t* src_word      = src + word_idx * sizeof(uint64_t);

  uint64_t ret = 0;
  if (remaining_bits >= 64) {
    memcpy(&ret, src_word, sizeof(ret));
    return le64toh(ret);
  }

  const size_t remaining_bytes = (remaining_bits + 7) / 8;
  memcpy(&ret, src_word, remaining_bytes);
  return le64toh(ret) & bit_word_mask(remaining_bits);
}

static inline void xor_dst_word(uint8_t* dst, size_t word_idx, size_t dst_bits, uint64_t value) {
  if (word_idx >= (dst_bits + 63) / 64) {
    return;
  }

  const size_t remaining_bits = dst_bits - word_idx * 64;
  const size_t word_bits      = MIN(remaining_bits, 64);
  const size_t word_bytes     = (word_bits + 7) / 8;
  uint8_t* dst_word           = dst + word_idx * sizeof(uint64_t);

  uint64_t ret = 0;
  memcpy(&ret, dst_word, word_bytes);
#if defined(FAEST_IS_BIG_ENDIAN)
  ret = le64toh(ret);
#endif
  ret ^= value & bit_word_mask(word_bits);
#if defined(FAEST_IS_BIG_ENDIAN)
  ret = htole64(ret);
#endif
  memcpy(dst_word, &ret, word_bytes);
}

void bf2_matrix_mul_tbl(uint8_t* dst, const uint8_t* src, const uint64_t* table, size_t rows,
                        size_t columns, size_t table_words) {
  const size_t row_words = (rows + 63) / 64;

  for (size_t col = 0; col < columns; ++col) {
    uint64_t acc = 0;
    for (size_t word = 0; word < row_words; ++word) {
      acc ^= load_src_word(src, word, rows) & table[col * table_words + word];
    }
    ptr_set_bit(dst, col, parity64(acc));
  }
}

static inline void xor_shifted_table(uint8_t* dst, size_t dst_bits, const uint64_t* table,
                                     size_t table_bits, size_t shift, uint64_t mask) {
  const size_t table_words    = (table_bits + 63) / 64;
  const size_t dst_word_idx   = shift / 64;
  const size_t dst_bit_offset = shift % 64;

  uint64_t carry = 0;
  for (size_t table_word_idx = 0; table_word_idx < table_words; ++table_word_idx) {
    const uint64_t table_word = table[table_word_idx];
    uint64_t shifted          = table_word;
    if (dst_bit_offset != 0) {
      shifted = (table_word << dst_bit_offset) | carry;
      carry   = table_word >> (64 - dst_bit_offset);
    }
    xor_dst_word(dst, dst_word_idx + table_word_idx, dst_bits, shifted & mask);
  }

  if (dst_bit_offset != 0) {
    xor_dst_word(dst, dst_word_idx + table_words, dst_bits, carry & mask);
  }
}

void bf2_poly_mul(uint8_t* dst, const uint8_t* src, size_t src_bits, const uint64_t* table,
                  size_t table_bits) {
  const size_t src_words = (src_bits + 63) / 64;
  const size_t dst_bits  = src_bits + table_bits - 1;

  for (size_t src_word_idx = 0; src_word_idx < src_words; ++src_word_idx) {
    const uint64_t src_word      = load_src_word(src, src_word_idx, src_bits);
    const size_t remaining_bits  = src_bits - src_word_idx * 64;
    const size_t src_word_bits   = MIN(remaining_bits, 64);
    const size_t dst_word_offset = src_word_idx;

    for (size_t src_bit_idx = 0; src_bit_idx < src_word_bits; ++src_bit_idx) {
      const uint64_t src_mask = -((src_word >> src_bit_idx) & 1);
      xor_shifted_table(dst, dst_bits, table, table_bits, dst_word_offset * 64 + src_bit_idx,
                        src_mask);
    }
  }
}
