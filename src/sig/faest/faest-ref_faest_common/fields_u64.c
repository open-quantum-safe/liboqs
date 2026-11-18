/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include "fields.h"

#if FAEST_FIELDS == FAEST_FIELDS_USE_U64
#define bf64_bit_to_mask(value, bit) -((((uint64_t)(value)) >> (bit)) & 1)

// Square a 64-bit polynomial over GF(2) by inserting a zero between each pair of input bits.
static inline void bf_square_expand(uint64_t* dst, uint64_t value) {
  uint64_t low  = value & UINT64_C(0xffffffff);
  uint64_t high = value >> 32;

  low        = (low | (low << 16)) & UINT64_C(0x0000ffff0000ffff);
  high       = (high | (high << 16)) & UINT64_C(0x0000ffff0000ffff);
  low        = (low | (low << 8)) & UINT64_C(0x00ff00ff00ff00ff);
  high       = (high | (high << 8)) & UINT64_C(0x00ff00ff00ff00ff);
  low        = (low | (low << 4)) & UINT64_C(0x0f0f0f0f0f0f0f0f);
  high       = (high | (high << 4)) & UINT64_C(0x0f0f0f0f0f0f0f0f);
  low        = (low | (low << 2)) & UINT64_C(0x3333333333333333);
  high       = (high | (high << 2)) & UINT64_C(0x3333333333333333);
  *dst       = (low | (low << 1)) & UINT64_C(0x5555555555555555);
  *(dst + 1) = (high | (high << 1)) & UINT64_C(0x5555555555555555);
}

// Reduce the upper half of a squared field element modulo X^(64*2) + X^7 + X^2 + X + 1.
static void bf_square_reduce_7_2_1_2(uint64_t* square) {
  for (unsigned int i = 2 * 2; i-- > 2;) {
    const uint64_t high    = square[i];
    const unsigned int low = i - 2;

    square[i] = 0;
    square[low] ^= high ^ (high << 1) ^ (high << 2) ^ (high << 7);
    square[low + 1] ^= (high >> 63) ^ (high >> 62) ^ (high >> 57);
  }
}

// Reduce the upper half of a squared field element modulo X^(64*3) + X^7 + X^2 + X + 1.
static void bf_square_reduce_7_2_1_3(uint64_t* square) {
  for (unsigned int i = 2 * 3; i-- > 3;) {
    const uint64_t high    = square[i];
    const unsigned int low = i - 3;

    square[i] = 0;
    square[low] ^= high ^ (high << 1) ^ (high << 2) ^ (high << 7);
    square[low + 1] ^= (high >> 63) ^ (high >> 62) ^ (high >> 57);
  }
}

// Reduce the upper half of a squared field element modulo X^(64*4) + X^10 + X^5 + X^2 + 1.
static void bf_square_reduce_10_5_2(uint64_t* square) {
  for (unsigned int i = 2 * 4; i-- > 4;) {
    const uint64_t high    = square[i];
    const unsigned int low = i - 4;

    square[i] = 0;
    square[low] ^= high ^ (high << 2) ^ (high << 5) ^ (high << 10);
    square[low + 1] ^= (high >> 62) ^ (high >> 59) ^ (high >> 54);
  }
}

static inline void bf128_and_64(bf128_t* dst, const bf128_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}

ATTR_CONST
static inline bf128_t bf128_shift_left_1(bf128_t value) {
  value.values[1] = (value.values[1] << 1) | (value.values[0] >> 63);
  value.values[0] = value.values[0] << 1;
  return value;
}

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline uint64_t
bf128_bit_to_uint64_mask(const bf128_t* value, unsigned int bit) {
  const unsigned int byte_idx = bit / 64;
  const unsigned int bit_idx  = bit % 64;

  return -((BF_VALUE(*value, byte_idx) >> bit_idx) & 1);
}

void bf128_mul(bf128_t* dst, const bf128_t* lhs, const bf128_t* rhs) {
  bf128_t tmp = *lhs;
  bf128_and_64(dst, lhs, bf128_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 128; ++idx) {
    const uint64_t mask = bf128_bit_to_uint64_mask(&tmp, 128 - 1);
    tmp                 = bf128_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & BF128_MODULUS);

    bf128_t tmp1;
    bf128_and_64(&tmp1, &tmp, bf128_bit_to_uint64_mask(rhs, idx));
    bf128_add_inplace(dst, &tmp1);
  }
}

void bf128_square(bf128_t* dst, const bf128_t* lhs) {
  ATTR_ALIGNED(16) uint64_t square[4];
  for (unsigned int i = 0; i != 2; ++i) {
    bf_square_expand(square + 2 * i, BF_VALUE(*lhs, i));
  }
  bf_square_reduce_7_2_1_2(square);

  BF_VALUE(*dst, 0) = square[0];
  BF_VALUE(*dst, 1) = square[1];
}

void bf128_mul_inplace(bf128_t* lhs, const bf128_t* rhs) {
  bf128_t tmp = *lhs;
  bf128_and_64(lhs, lhs, bf128_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 128; ++idx) {
    const uint64_t mask = bf128_bit_to_uint64_mask(&tmp, 128 - 1);
    tmp                 = bf128_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & BF128_MODULUS);

    bf128_t tmp1;
    bf128_and_64(&tmp1, &tmp, bf128_bit_to_uint64_mask(rhs, idx));
    bf128_add_inplace(lhs, &tmp1);
  }
}

void bf128_mul_64_inplace(bf128_t* lhs, bf64_t rhs) {
  bf128_t tmp = *lhs;
  bf128_and_64(lhs, lhs, bf64_bit_to_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 64; ++idx) {
    const uint64_t mask = bf128_bit_to_uint64_mask(&tmp, 128 - 1);
    tmp                 = bf128_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & BF128_MODULUS);

    bf128_t tmp1;
    bf128_and_64(&tmp1, &tmp, bf64_bit_to_mask(rhs, idx));
    bf128_add_inplace(lhs, &tmp1);
  }
}

void bf128_mul_bit(bf128_t* dst, const bf128_t* lhs, uint8_t rhs) {
  bf128_and_64(dst, lhs, -((uint64_t)rhs & 1));
}

void bf128_mul_bit_8(bf128_t* dst, const bf128_t* lhs, uint8_t rhs) {
  const bf128_t value = *lhs;
  for (unsigned int i = 0; i != 8; ++i) {
    bf128_and_64(&dst[i], &value, -((uint64_t)(rhs >> i) & 1));
  }
}

#if defined(FAEST_TESTS)
static inline void bf128_dbl_inplace(bf128_t* lhs) {
  uint64_t mask = bf128_bit_to_uint64_mask(lhs, 128 - 1);
  *lhs          = bf128_shift_left_1(*lhs);
  BF_VALUE(*lhs, 0) ^= (mask & BF128_MODULUS);
}

void bf128_sum_poly(bf128_t* dst, const bf128_t* xs) {
  *dst = xs[128 - 1];
  for (size_t i = 1; i < 128; ++i) {
    bf128_dbl_inplace(dst);
    bf128_add_inplace(dst, &xs[128 - 1 - i]);
  }
}
#endif
static inline void bf192_and_64(bf192_t* dst, const bf192_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}

ATTR_CONST
static inline bf192_t bf192_shift_left_1(bf192_t value) {
  value.values[2] = (value.values[2] << 1) | (value.values[1] >> 63);
  value.values[1] = (value.values[1] << 1) | (value.values[0] >> 63);
  value.values[0] = value.values[0] << 1;
  return value;
}

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline uint64_t
bf192_bit_to_uint64_mask(const bf192_t* value, unsigned int bit) {
  const unsigned int byte_idx = bit / 64;
  const unsigned int bit_idx  = bit % 64;

  return -((BF_VALUE(*value, byte_idx) >> bit_idx) & 1);
}

void bf192_mul(bf192_t* dst, const bf192_t* lhs, const bf192_t* rhs) {
  bf192_t tmp = *lhs;
  bf192_and_64(dst, lhs, bf192_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 192; ++idx) {
    const uint64_t mask = bf192_bit_to_uint64_mask(&tmp, 192 - 1);
    tmp                 = bf192_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & BF192_MODULUS);

    bf192_t tmp1;
    bf192_and_64(&tmp1, &tmp, bf192_bit_to_uint64_mask(rhs, idx));
    bf192_add_inplace(dst, &tmp1);
  }
}

void bf192_square(bf192_t* dst, const bf192_t* lhs) {
  ATTR_ALIGNED(16) uint64_t square[6];
  for (unsigned int i = 0; i != 3; ++i) {
    bf_square_expand(square + 2 * i, BF_VALUE(*lhs, i));
  }
  bf_square_reduce_7_2_1_3(square);

  BF_VALUE(*dst, 0) = square[0];
  BF_VALUE(*dst, 1) = square[1];
  BF_VALUE(*dst, 2) = square[2];
}

void bf192_mul_inplace(bf192_t* lhs, const bf192_t* rhs) {
  bf192_t tmp = *lhs;
  bf192_and_64(lhs, lhs, bf192_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 192; ++idx) {
    const uint64_t mask = bf192_bit_to_uint64_mask(&tmp, 192 - 1);
    tmp                 = bf192_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & BF192_MODULUS);

    bf192_t tmp1;
    bf192_and_64(&tmp1, &tmp, bf192_bit_to_uint64_mask(rhs, idx));
    bf192_add_inplace(lhs, &tmp1);
  }
}

void bf192_mul_64_inplace(bf192_t* lhs, bf64_t rhs) {
  bf192_t tmp = *lhs;
  bf192_and_64(lhs, lhs, bf64_bit_to_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 64; ++idx) {
    const uint64_t mask = bf192_bit_to_uint64_mask(&tmp, 192 - 1);
    tmp                 = bf192_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & BF192_MODULUS);

    bf192_t tmp1;
    bf192_and_64(&tmp1, &tmp, bf64_bit_to_mask(rhs, idx));
    bf192_add_inplace(lhs, &tmp1);
  }
}

void bf192_mul_bit(bf192_t* dst, const bf192_t* lhs, uint8_t rhs) {
  bf192_and_64(dst, lhs, -((uint64_t)rhs & 1));
}

void bf192_mul_bit_8(bf192_t* dst, const bf192_t* lhs, uint8_t rhs) {
  const bf192_t value = *lhs;
  for (unsigned int i = 0; i != 8; ++i) {
    bf192_and_64(&dst[i], &value, -((uint64_t)(rhs >> i) & 1));
  }
}

#if defined(FAEST_TESTS)
static inline void bf192_dbl_inplace(bf192_t* lhs) {
  uint64_t mask = bf192_bit_to_uint64_mask(lhs, 192 - 1);
  *lhs          = bf192_shift_left_1(*lhs);
  BF_VALUE(*lhs, 0) ^= (mask & BF192_MODULUS);
}

void bf192_sum_poly(bf192_t* dst, const bf192_t* xs) {
  *dst = xs[192 - 1];
  for (size_t i = 1; i < 192; ++i) {
    bf192_dbl_inplace(dst);
    bf192_add_inplace(dst, &xs[192 - 1 - i]);
  }
}
#endif
ATTR_ALWAYS_INLINE static inline void bf256_and_64(bf256_t* dst, const bf256_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}

ATTR_CONST
static inline bf256_t bf256_shift_left_1(bf256_t value) {
  value.values[3] = (value.values[3] << 1) | (value.values[2] >> 63);
  value.values[2] = (value.values[2] << 1) | (value.values[1] >> 63);
  value.values[1] = (value.values[1] << 1) | (value.values[0] >> 63);
  value.values[0] = value.values[0] << 1;
  return value;
}

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline uint64_t
bf256_bit_to_uint64_mask(const bf256_t* value, unsigned int bit) {
  const unsigned int byte_idx = bit / 64;
  const unsigned int bit_idx  = bit % 64;

  return -((BF_VALUE(*value, byte_idx) >> bit_idx) & 1);
}

void bf256_mul(bf256_t* dst, const bf256_t* lhs, const bf256_t* rhs) {
  bf256_t tmp = *lhs;
  bf256_and_64(dst, lhs, bf256_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 256; ++idx) {
    const uint64_t mask = bf256_bit_to_uint64_mask(&tmp, 256 - 1);
    tmp                 = bf256_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & BF256_MODULUS);

    bf256_t tmp1;
    bf256_and_64(&tmp1, &tmp, bf256_bit_to_uint64_mask(rhs, idx));
    bf256_add_inplace(dst, &tmp1);
  }
}

void bf256_square(bf256_t* dst, const bf256_t* lhs) {
  ATTR_ALIGNED(16) uint64_t square[8];
  for (unsigned int i = 0; i != 4; ++i) {
    bf_square_expand(square + 2 * i, BF_VALUE(*lhs, i));
  }
  bf_square_reduce_10_5_2(square);

  BF_VALUE(*dst, 0) = square[0];
  BF_VALUE(*dst, 1) = square[1];
  BF_VALUE(*dst, 2) = square[2];
  BF_VALUE(*dst, 3) = square[3];
}

void bf256_mul_inplace(bf256_t* lhs, const bf256_t* rhs) {
  bf256_t tmp = *lhs;

  bf256_and_64(lhs, lhs, bf256_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 256; ++idx) {
    const uint64_t mask = bf256_bit_to_uint64_mask(&tmp, 256 - 1);
    tmp                 = bf256_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & BF256_MODULUS);

    bf256_t tmp1;
    bf256_and_64(&tmp1, &tmp, bf256_bit_to_uint64_mask(rhs, idx));
    bf256_add_inplace(lhs, &tmp1);
  }
}

void bf256_mul_64_inplace(bf256_t* lhs, bf64_t rhs) {
  bf256_t tmp = *lhs;
  bf256_and_64(lhs, lhs, bf64_bit_to_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 64; ++idx) {
    const uint64_t mask = bf256_bit_to_uint64_mask(&tmp, 256 - 1);
    tmp                 = bf256_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & BF256_MODULUS);

    bf256_t tmp1;
    bf256_and_64(&tmp1, &tmp, bf64_bit_to_mask(rhs, idx));
    bf256_add_inplace(lhs, &tmp1);
  }
}

void bf256_mul_bit(bf256_t* dst, const bf256_t* lhs, uint8_t rhs) {
  bf256_and_64(dst, lhs, -((uint64_t)rhs & 1));
}

void bf256_mul_bit_8(bf256_t* dst, const bf256_t* lhs, uint8_t rhs) {
  const bf256_t value = *lhs;
  for (unsigned int i = 0; i != 8; ++i) {
    bf256_and_64(&dst[i], &value, -((uint64_t)(rhs >> i) & 1));
  }
}

#if defined(FAEST_TESTS)
static inline void bf256_dbl_inplace(bf256_t* lhs) {
  uint64_t mask = bf256_bit_to_uint64_mask(lhs, 256 - 1);
  *lhs          = bf256_shift_left_1(*lhs);
  BF_VALUE(*lhs, 0) ^= mask & BF256_MODULUS;
}

void bf256_sum_poly(bf256_t* dst, const bf256_t* xs) {
  *dst = xs[256 - 1];
  for (size_t i = 1; i < 256; ++i) {
    bf256_dbl_inplace(dst);
    bf256_add_inplace(dst, &xs[256 - 1 - i]);
  }
}
#endif
static inline void bf384_and_64(bf384_t* dst, const bf384_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}

ATTR_CONST
static inline bf384_t bf384_shift_left_1(bf384_t value) {
  for (unsigned int i = ARRAY_SIZE(value.values) - 1; i; --i) {
    value.values[i] = (value.values[i] << 1) | (value.values[i - 1] >> 63);
  }
  value.values[0] = value.values[0] << 1;
  return value;
}

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline uint64_t
bf384_bit_to_uint64_mask(const bf384_t* value, unsigned int bit) {
  const unsigned int byte_idx = bit / 64;
  const unsigned int bit_idx  = bit % 64;

  return -((BF_VALUE(*value, byte_idx) >> bit_idx) & 1);
}

void bf384_mul_128_inplace(bf384_t* lhs, const bf128_t* rhs) {
  bf384_t tmp = *lhs;
  bf384_and_64(lhs, lhs, bf128_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 128; ++idx) {
    const uint64_t mask = bf384_bit_to_uint64_mask(&tmp, 384 - 1);
    tmp                 = bf384_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= mask & BF384_MODULUS;

    bf384_t tmp1;
    bf384_and_64(&tmp1, &tmp, bf128_bit_to_uint64_mask(rhs, idx));
    bf384_add_inplace(lhs, &tmp1);
  }
}

// GF(2^576)

static inline void bf576_and_64(bf576_t* dst, const bf576_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}

ATTR_CONST
static inline bf576_t bf576_shift_left_1(bf576_t value) {
  for (unsigned int i = ARRAY_SIZE(value.values) - 1; i; --i) {
    value.values[i] = (value.values[i] << 1) | (value.values[i - 1] >> 63);
  }
  value.values[0] = value.values[0] << 1;
  return value;
}

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline uint64_t
bf576_bit_to_uint64_mask(const bf576_t* value, unsigned int bit) {
  const unsigned int byte_idx = bit / 64;
  const unsigned int bit_idx  = bit % 64;

  return -((BF_VALUE(*value, byte_idx) >> bit_idx) & 1);
}

void bf576_mul_192_inplace(bf576_t* lhs, const bf192_t* rhs) {
  bf576_t tmp = *lhs;
  bf576_and_64(lhs, lhs, bf192_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 192; ++idx) {
    const uint64_t mask = bf576_bit_to_uint64_mask(&tmp, 576 - 1);
    tmp                 = bf576_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= mask & BF576_MODULUS;

    bf576_t tmp1;
    bf576_and_64(&tmp1, &tmp, bf192_bit_to_uint64_mask(rhs, idx));
    bf576_add_inplace(lhs, &tmp1);
  }
}

// GF(2^768)

static inline void bf768_and_64(bf768_t* dst, const bf768_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}

ATTR_CONST
static inline bf768_t bf768_shift_left_1(bf768_t value) {
  for (unsigned int i = ARRAY_SIZE(value.values) - 1; i; --i) {
    value.values[i] = (value.values[i] << 1) | (value.values[i - 1] >> 63);
  }
  value.values[0] = value.values[0] << 1;
  return value;
}

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline uint64_t
bf768_bit_to_uint64_mask(const bf768_t* value, unsigned int bit) {
  const unsigned int byte_idx = bit / 64;
  const unsigned int bit_idx  = bit % 64;

  return -((BF_VALUE(*value, byte_idx) >> bit_idx) & 1);
}

void bf768_mul_256_inplace(bf768_t* lhs, const bf256_t* rhs) {
  bf768_t tmp = *lhs;
  bf768_and_64(lhs, lhs, bf256_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 256; ++idx) {
    const uint64_t mask = bf768_bit_to_uint64_mask(&tmp, 768 - 1);
    tmp                 = bf768_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= mask & BF768_MODULUS;

    bf768_t tmp1;
    bf768_and_64(&tmp1, &tmp, bf256_bit_to_uint64_mask(rhs, idx));
    bf768_add_inplace(lhs, &tmp1);
  }
}

void bf128_sq_bit(bf128_t* out_tag, const bf128_t* in_tag) {
  bf128_add(&out_tag[7], &in_tag[6], &in_tag[7]);
  bf128_add(&out_tag[0], &in_tag[0], &in_tag[4]);
  bf128_add_inplace(&out_tag[0], &in_tag[6]);
  bf128_add(&out_tag[1], &in_tag[4], &out_tag[7]);
  bf128_add(&out_tag[2], &in_tag[1], &in_tag[5]);
  bf128_add(&out_tag[3], &out_tag[1], &in_tag[5]);
  bf128_add(&out_tag[4], &in_tag[2], &in_tag[4]);
  bf128_add_inplace(&out_tag[4], &in_tag[7]);
  bf128_add(&out_tag[5], &in_tag[5], &in_tag[6]);
  bf128_add(&out_tag[6], &in_tag[3], &in_tag[5]);
}

void bf192_sq_bit(bf192_t* out_tag, const bf192_t* in_tag) {
  bf192_add(&out_tag[7], &in_tag[6], &in_tag[7]);
  bf192_add(&out_tag[0], &in_tag[0], &in_tag[4]);
  bf192_add_inplace(&out_tag[0], &in_tag[6]);
  bf192_add(&out_tag[1], &in_tag[4], &out_tag[7]);
  bf192_add(&out_tag[2], &in_tag[1], &in_tag[5]);
  bf192_add(&out_tag[3], &out_tag[1], &in_tag[5]);
  bf192_add(&out_tag[4], &in_tag[2], &in_tag[4]);
  bf192_add_inplace(&out_tag[4], &in_tag[7]);
  bf192_add(&out_tag[5], &in_tag[5], &in_tag[6]);
  bf192_add(&out_tag[6], &in_tag[3], &in_tag[5]);
}

void bf256_sq_bit(bf256_t* out_tag, const bf256_t* in_tag) {
  bf256_add(&out_tag[7], &in_tag[6], &in_tag[7]);
  bf256_add(&out_tag[0], &in_tag[0], &in_tag[4]);
  bf256_add_inplace(&out_tag[0], &in_tag[6]);
  bf256_add(&out_tag[1], &in_tag[4], &out_tag[7]);
  bf256_add(&out_tag[2], &in_tag[1], &in_tag[5]);
  bf256_add(&out_tag[3], &out_tag[1], &in_tag[5]);
  bf256_add(&out_tag[4], &in_tag[2], &in_tag[4]);
  bf256_add_inplace(&out_tag[4], &in_tag[7]);
  bf256_add(&out_tag[5], &in_tag[5], &in_tag[6]);
  bf256_add(&out_tag[6], &in_tag[3], &in_tag[5]);
}

#endif
