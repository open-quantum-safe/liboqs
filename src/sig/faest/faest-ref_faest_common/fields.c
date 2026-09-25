/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include "fields.h"
#include "utils.h"

// GF(2^8) with X^8 + X^4 + X^3 + X^1 + 1
#define bf8_modulus (UINT8_C((1 << 4) | (1 << 3) | (1 << 1) | 1))
// GF(2^64) with X^64 + X^4 + X^3 + X^1 + 1
#define bf64_modulus (UINT64_C((1 << 4) | (1 << 3) | (1 << 1) | 1))
// GF(2^128) with X^128 + X^7 + X^2 + X^1 + 1
#define bf128_modulus (UINT64_C((1 << 7) | (1 << 2) | (1 << 1) | 1))
// GF(2^192) with X^192 + X^7 + X^2 + X^1 + 1
#define bf192_modulus (UINT64_C((1 << 7) | (1 << 2) | (1 << 1) | 1))
// GF(2^256) with X^256 + X^10 + X^5 + X^2 + 1
#define bf256_modulus (UINT64_C((1 << 10) | (1 << 5) | (1 << 2) | 1))
// GF(2^384) with X^384 + X^12 + X^3 + X^2 + 1
#define bf384_modulus (UINT64_C((1 << 12) | (1 << 3) | (1 << 2) | 1))
// GF(2^576) with X^576 + X^13 + X^4 + X^3 + 1
#define bf576_modulus (UINT64_C((1 << 13) | (1 << 4) | (1 << 3) | 1))
// GF(2^768) with X^768 + X^19 + X^17 + X^4 + 1
#define bf768_modulus (UINT64_C((1 << 19) | (1 << 17) | (1 << 4) | 1))

#define U64C(x0, x1, x2, x3, x4, x5, x6, x7)                                                       \
  ((UINT64_C(x7) << 56) | (UINT64_C(x6) << 48) | (UINT64_C(x5) << 40) | (UINT64_C(x4) << 32) |     \
   (UINT64_C(x3) << 24) | (UINT64_C(x2) << 16) | (UINT64_C(x1) << 8) | UINT64_C(x0))

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

bf8_t bf8_square(bf8_t lhs) {
  bf8_t result = -(lhs & 1) & lhs;
  bf8_t rhs    = lhs;
  for (unsigned int idx = 1; idx < 8; ++idx) {
    const uint8_t mask = -((lhs >> 7) & 1);
    lhs                = (lhs << 1) ^ (mask & bf8_modulus);
    result ^= -((rhs >> idx) & 1) & lhs;
  }
  return result;
}

bf8_t bf8_inv(bf8_t in) {
  const bf8_t t2   = bf8_square(in);
  const bf8_t t3   = bf8_mul(in, t2);
  const bf8_t t5   = bf8_mul(t3, t2);
  const bf8_t t7   = bf8_mul(t5, t2);
  const bf8_t t14  = bf8_square(t7);
  const bf8_t t28  = bf8_square(t14);
  const bf8_t t56  = bf8_square(t28);
  const bf8_t t63  = bf8_mul(t56, t7);
  const bf8_t t126 = bf8_square(t63);
  const bf8_t t252 = bf8_square(t126);
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

#define bf64_bit_to_mask(value, bit) -((((uint64_t)(value)) >> (bit)) & 1)

// Helpers for bfXXX_square implementations

// Square a 32-bit polynomial over GF(2) by inserting a zero between each pair of input bits.
static inline uint64_t bf_square_expand(uint32_t value) {
  uint64_t expanded = value;
  expanded          = (expanded | (expanded << 16)) & UINT64_C(0x0000ffff0000ffff);
  expanded          = (expanded | (expanded << 8)) & UINT64_C(0x00ff00ff00ff00ff);
  expanded          = (expanded | (expanded << 4)) & UINT64_C(0x0f0f0f0f0f0f0f0f);
  expanded          = (expanded | (expanded << 2)) & UINT64_C(0x3333333333333333);
  return (expanded | (expanded << 1)) & UINT64_C(0x5555555555555555);
}

// Reduce the upper half of a squared field element modulo X^(64*limbs) + X^7 + X^2 + X + 1.
static void bf_square_reduce_7_2_1(uint64_t* square, unsigned int limbs) {
  for (unsigned int i = 2 * limbs; i-- > limbs;) {
    const uint64_t high    = square[i];
    const unsigned int low = i - limbs;
    square[i]              = 0;
    square[low] ^= high ^ (high << 1) ^ (high << 2) ^ (high << 7);
    square[low + 1] ^= (high >> 63) ^ (high >> 62) ^ (high >> 57);
  }
}

// Reduce the upper half of a squared field element modulo X^(64*limbs) + X^10 + X^5 + X^2 + 1.
static void bf_square_reduce_10_5_2(uint64_t* square, unsigned int limbs) {
  for (unsigned int i = 2 * limbs; i-- > limbs;) {
    const uint64_t high    = square[i];
    const unsigned int low = i - limbs;
    square[i]              = 0;
    square[low] ^= high ^ (high << 2) ^ (high << 5) ^ (high << 10);
    square[low + 1] ^= (high >> 62) ^ (high >> 59) ^ (high >> 54);
  }
}

// GF(2^128) implementation

static const bf128_t bf128_alpha[7] = {
    BF128C(U64C(0x0d, 0xce, 0x60, 0x55, 0xac, 0xe8, 0x3f, 0xa1),
           U64C(0x1c, 0x9a, 0x97, 0xa9, 0x55, 0x85, 0x3d, 0x05)),
    BF128C(U64C(0xe1, 0xae, 0x88, 0x34, 0xca, 0x59, 0x77, 0xec),
           U64C(0x84, 0xbb, 0xbf, 0x9c, 0x43, 0xb7, 0xf4, 0x4c)),
    BF128C(U64C(0xa8, 0x46, 0x39, 0x36, 0xae, 0x02, 0xcf, 0xbf),
           U64C(0xc6, 0xd2, 0x51, 0x7d, 0x4f, 0x60, 0xad, 0x35)),
    BF128C(U64C(0x49, 0x98, 0x2e, 0x3c, 0x48, 0x30, 0x83, 0x6b),
           U64C(0xfe, 0x22, 0xa2, 0x40, 0x46, 0x36, 0xcb, 0x0d)),
    BF128C(U64C(0xb4, 0x82, 0x1b, 0x7b, 0x27, 0x49, 0x2b, 0x25),
           U64C(0xa5, 0xde, 0x88, 0x1a, 0xe1, 0x10, 0x98, 0x54)),
    BF128C(U64C(0x22, 0xff, 0x21, 0x25, 0xef, 0xf2, 0x2b, 0xc7),
           U64C(0x75, 0x1f, 0x0c, 0x6c, 0x68, 0xa5, 0x81, 0xd6)),
    BF128C(U64C(0xbc, 0xf9, 0x36, 0xe1, 0x94, 0x8e, 0x7a, 0x7a),
           U64C(0xe0, 0x8f, 0xb7, 0x4f, 0x1a, 0x31, 0x50, 0x09)),
};

#if defined(FAEST_TESTS)
bf128_t bf128_get_alpha(unsigned int idx) {
  return bf128_alpha[idx];
}
#endif

void bf128_byte_combine(bf128_t* dst, const bf128_t* x) {
  *dst = x[0];
  for (unsigned int i = 1; i < 8; ++i) {
    bf128_t tmp;
    bf128_mul(&tmp, &x[i], &bf128_alpha[i - 1]);
    bf128_add_inplace(dst, &tmp);
  }
}

void bf128_sq_bit(bf128_t* out_tag, const bf128_t* in_tag) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  out_tag[7] = in_tag[6] ^ in_tag[7];
  out_tag[0] = in_tag[0] ^ in_tag[4] ^ in_tag[6];
  out_tag[1] = in_tag[4] ^ out_tag[7];
  out_tag[2] = in_tag[1] ^ in_tag[5];
  out_tag[3] = out_tag[1] ^ in_tag[5];
  out_tag[4] = in_tag[2] ^ in_tag[4] ^ in_tag[7];
  out_tag[5] = in_tag[5] ^ in_tag[6];
  out_tag[6] = in_tag[3] ^ in_tag[5];
#else
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
#endif
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
    bf128_mul_bit(&tmp, &bf128_alpha[i - 1], get_bit(x, i));
    bf128_add_inplace(dst, &tmp);
  }
}

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf128_and_64(bf128_t* dst, const bf128_t* lhs,
                                                                   bf64_t rhs) {
  *dst = *lhs & rhs;
}
#else
static inline void bf128_and_64(bf128_t* dst, const bf128_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}
#endif

#if defined(HAVE_ATTR_VECTOR_SIZE)
#if __has_builtin(__builtin_shufflevector)
#define bf128_shift_right_64(v1) __builtin_shufflevector((v1), bf128_zero(), 2, 0)
#else
ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf128_t
bf128_shift_right_64(bf128_t v1) {
  bf128_t ret;
  BF_VALUE(ret, 0) = 0;
  BF_VALUE(ret, 1) = BF_VALUE(v1, 0);
  return ret;
}
#endif

#define bf128_shift_left_1(value) ((value << 1) | bf128_shift_right_64(value >> 63))
#else
ATTR_CONST
static inline bf128_t bf128_shift_left_1(bf128_t value) {
  value.values[1] = (value.values[1] << 1) | (value.values[0] >> 63);
  value.values[0] = value.values[0] << 1;
  return value;
}
#endif

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
    BF_VALUE(tmp, 0) ^= (mask & bf128_modulus);

    bf128_t tmp1;
    bf128_and_64(&tmp1, &tmp, bf128_bit_to_uint64_mask(rhs, idx));
    bf128_add_inplace(dst, &tmp1);
  }
}

void bf128_square(bf128_t* dst, const bf128_t* lhs) {
  uint64_t square[4];
  for (unsigned int i = 0; i != 2; ++i) {
    square[2 * i]     = bf_square_expand(BF_VALUE(*lhs, i));
    square[2 * i + 1] = bf_square_expand(BF_VALUE(*lhs, i) >> 32);
  }
  bf_square_reduce_7_2_1(square, 2);

  BF_VALUE(*dst, 0) = square[0];
  BF_VALUE(*dst, 1) = square[1];
}

void bf128_mul_inplace(bf128_t* lhs, const bf128_t* rhs) {
  bf128_t tmp = *lhs;
  bf128_and_64(lhs, lhs, bf128_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 128; ++idx) {
    const uint64_t mask = bf128_bit_to_uint64_mask(&tmp, 128 - 1);
    tmp                 = bf128_shift_left_1(tmp);
    BF_VALUE(tmp, 0) ^= (mask & bf128_modulus);

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
    BF_VALUE(tmp, 0) ^= (mask & bf128_modulus);

    bf128_t tmp1;
    bf128_and_64(&tmp1, &tmp, bf64_bit_to_mask(rhs, idx));
    bf128_add_inplace(lhs, &tmp1);
  }
}

#if !defined(HAVE_ATTR_VECTOR_SIZE)
void bf128_mul_bit(bf128_t* dst, const bf128_t* lhs, uint8_t rhs) {
  bf128_and_64(dst, lhs, -((uint64_t)rhs & 1));
}
#endif

#if defined(FAEST_TESTS)
static inline void bf128_dbl_inplace(bf128_t* lhs) {
  uint64_t mask = bf128_bit_to_uint64_mask(lhs, 128 - 1);
  *lhs          = bf128_shift_left_1(*lhs);
  BF_VALUE(*lhs, 0) ^= (mask & bf128_modulus);
}

void bf128_sum_poly(bf128_t* dst, const bf128_t* xs) {
  *dst = xs[128 - 1];
  for (size_t i = 1; i < 128; ++i) {
    bf128_dbl_inplace(dst);
    bf128_add_inplace(dst, &xs[128 - 1 - i]);
  }
}
#endif

void bf128_sum_poly_bits(bf128_t* dst, const uint8_t* xs) {
  bf128_load(dst, xs);
}

// GF(2^192) implementation

static const bf192_t bf192_alpha[7] = {
    BF192C(U64C(0x63, 0x97, 0x38, 0x6f, 0xd5, 0xa3, 0xc8, 0xcc),
           U64C(0xea, 0xbd, 0x6e, 0x96, 0x6c, 0xd7, 0x65, 0xe6),
           U64C(0x62, 0x36, 0x6b, 0x0e, 0x14, 0xc8, 0x0b, 0x31)),
    BF192C(U64C(0xbb, 0x50, 0xf4, 0x7c, 0x9e, 0x61, 0x33, 0xb2),
           U64C(0x26, 0x3f, 0x63, 0xd5, 0x19, 0x1f, 0xf6, 0x7b),
           U64C(0x34, 0xdb, 0x91, 0xd4, 0x26, 0x37, 0x93, 0xda)),
    BF192C(U64C(0x0d, 0x8a, 0x39, 0xf5, 0x13, 0x2c, 0x6d, 0x9c),
           U64C(0x19, 0x8d, 0x32, 0x06, 0x77, 0xe3, 0x32, 0x82),
           U64C(0xf6, 0x4e, 0x75, 0x3c, 0x70, 0x0d, 0x3b, 0x0c)),
    BF192C(U64C(0x5d, 0xf7, 0x2b, 0xbd, 0x7c, 0x74, 0x20, 0xdd),
           U64C(0x2e, 0xd2, 0x58, 0x00, 0xab, 0x42, 0x55, 0x7a),
           U64C(0x51, 0x12, 0xbc, 0x94, 0x9c, 0x51, 0xec, 0x45)),
    BF192C(U64C(0xf8, 0x2b, 0xce, 0x8a, 0xe2, 0x0c, 0xd5, 0xd8),
           U64C(0x84, 0xbe, 0xde, 0x67, 0xb7, 0x8c, 0x16, 0x08),
           U64C(0x45, 0x70, 0xa6, 0x4b, 0x6a, 0x14, 0x7d, 0xd6)),
    BF192C(U64C(0xba, 0xe1, 0xd5, 0xee, 0x76, 0x9c, 0x0f, 0x97),
           U64C(0x48, 0x20, 0xd7, 0x5f, 0xae, 0xf7, 0xea, 0xf3),
           U64C(0x43, 0xea, 0x6c, 0x69, 0x5f, 0xbd, 0xa6, 0x29)),
    BF192C(U64C(0x71, 0x85, 0x06, 0x65, 0xc2, 0x5d, 0x94, 0xf5),
           U64C(0xd3, 0xe9, 0x06, 0x39, 0x62, 0xfd, 0x19, 0x60),
           U64C(0xb0, 0xc4, 0x87, 0x0f, 0x54, 0x56, 0x7c, 0xc7)),
};

#if defined(FAEST_TESTS)
bf192_t bf192_get_alpha(unsigned int idx) {
  return bf192_alpha[idx];
}
#endif

void bf192_byte_combine(bf192_t* dst, const bf192_t* x) {
  *dst = x[0];
  for (unsigned int i = 1; i < 8; ++i) {
    bf192_t tmp;
    bf192_mul(&tmp, &x[i], &bf192_alpha[i - 1]);
    bf192_add_inplace(dst, &tmp);
  }
}

void bf192_sq_bit(bf192_t* out_tag, const bf192_t* in_tag) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  out_tag[7] = in_tag[6] ^ in_tag[7];
  out_tag[0] = in_tag[0] ^ in_tag[4] ^ in_tag[6];
  out_tag[1] = in_tag[4] ^ out_tag[7];
  out_tag[2] = in_tag[1] ^ in_tag[5];
  out_tag[3] = out_tag[1] ^ in_tag[5];
  out_tag[4] = in_tag[2] ^ in_tag[4] ^ in_tag[7];
  out_tag[5] = in_tag[5] ^ in_tag[6];
  out_tag[6] = in_tag[3] ^ in_tag[5];
#else
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
#endif
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
    bf192_mul_bit(&tmp, &bf192_alpha[i - 1], get_bit(x, i));
    bf192_add_inplace(dst, &tmp);
  }
}

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf192_and_64(bf192_t* dst, const bf192_t* lhs,
                                                                   bf64_t rhs) {
  *dst = *lhs & rhs;
}
#else
static inline void bf192_and_64(bf192_t* dst, const bf192_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}
#endif

#if defined(HAVE_ATTR_VECTOR_SIZE)
#if __has_builtin(__builtin_shufflevector)
#define bf192_shift_right_64(v1) __builtin_shufflevector((v1), bf192_zero(), 4, 0, 1, 5)
#else
ATTR_CONST ATTR_ALWAYS_INLINE static inline bf192_t bf192_shift_right_64(bf192_t v1) {
  bf192_t ret;
  BF_VALUE(ret, 0) = 0;
  BF_VALUE(ret, 1) = BF_VALUE(v1, 0);
  BF_VALUE(ret, 2) = BF_VALUE(v1, 1);
  BF_VALUE(ret, 3) = 0;
  return ret;
}
#endif
#endif

ATTR_CONST
static inline bf192_t bf192_shift_left_1(bf192_t value) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  const bf192_t mask = BF192C(0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff);
  return ((value << 1) | bf192_shift_right_64(value >> 63)) & mask;
#else
  value.values[2] = (value.values[2] << 1) | (value.values[1] >> 63);
  value.values[1] = (value.values[1] << 1) | (value.values[0] >> 63);
  value.values[0] = value.values[0] << 1;
#endif
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
    BF_VALUE(tmp, 0) ^= (mask & bf192_modulus);

    bf192_t tmp1;
    bf192_and_64(&tmp1, &tmp, bf192_bit_to_uint64_mask(rhs, idx));
    bf192_add_inplace(dst, &tmp1);
  }
}

void bf192_square(bf192_t* dst, const bf192_t* lhs) {
  uint64_t square[6];
  for (unsigned int i = 0; i != 3; ++i) {
    square[2 * i]     = bf_square_expand(BF_VALUE(*lhs, i));
    square[2 * i + 1] = bf_square_expand(BF_VALUE(*lhs, i) >> 32);
  }
  bf_square_reduce_7_2_1(square, 3);

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
    BF_VALUE(tmp, 0) ^= (mask & bf192_modulus);

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
    BF_VALUE(tmp, 0) ^= (mask & bf192_modulus);

    bf192_t tmp1;
    bf192_and_64(&tmp1, &tmp, bf64_bit_to_mask(rhs, idx));
    bf192_add_inplace(lhs, &tmp1);
  }
}

#if !defined(HAVE_ATTR_VECTOR_SIZE)
void bf192_mul_bit(bf192_t* dst, const bf192_t* lhs, uint8_t rhs) {
  bf192_and_64(dst, lhs, -((uint64_t)rhs & 1));
}
#endif

#if defined(FAEST_TESTS)
static inline void bf192_dbl_inplace(bf192_t* lhs) {
  uint64_t mask = bf192_bit_to_uint64_mask(lhs, 192 - 1);
  *lhs          = bf192_shift_left_1(*lhs);
  BF_VALUE(*lhs, 0) ^= (mask & bf192_modulus);
}

void bf192_sum_poly(bf192_t* dst, const bf192_t* xs) {
  *dst = xs[192 - 1];
  for (size_t i = 1; i < 192; ++i) {
    bf192_dbl_inplace(dst);
    bf192_add_inplace(dst, &xs[192 - 1 - i]);
  }
}
#endif

void bf192_sum_poly_bits(bf192_t* dst, const uint8_t* xs) {
  bf192_load(dst, xs);
}

// GF(2^256) implementation

static const bf256_t bf256_alpha[7] = {
    BF256C(U64C(0xe7, 0xfe, 0xde, 0x0b, 0x42, 0x88, 0x97, 0x96),
           U64C(0x67, 0x4e, 0x47, 0xa0, 0x38, 0x8d, 0xd6, 0xbe),
           U64C(0x6a, 0xe1, 0xf1, 0xf8, 0x45, 0x98, 0x22, 0xdf),
           U64C(0x33, 0x58, 0xc9, 0x20, 0xcf, 0xa8, 0xc9, 0x04)),
    BF256C(U64C(0xc1, 0x89, 0x22, 0xd5, 0x2a, 0xf5, 0x5a, 0xa9),
           U64C(0x2f, 0x07, 0x42, 0x2c, 0x8d, 0xc4, 0xa5, 0x2b),
           U64C(0xea, 0xb0, 0x00, 0x6c, 0x37, 0x0d, 0x4a, 0xd1),
           U64C(0xf1, 0x4a, 0x5b, 0x9c, 0x69, 0x4d, 0x4e, 0x06)),
    BF256C(U64C(0x1d, 0x9d, 0x80, 0x3f, 0x83, 0xb3, 0xda, 0x55),
           U64C(0x57, 0x0f, 0x3b, 0x53, 0x1e, 0x83, 0x71, 0x17),
           U64C(0x10, 0xac, 0x3f, 0xad, 0x3f, 0x57, 0x96, 0xfb),
           U64C(0x8d, 0xf6, 0x11, 0x70, 0xdb, 0xe3, 0x95, 0x61)),
    BF256C(U64C(0xd5, 0xcd, 0x1b, 0xb0, 0x19, 0x05, 0x01, 0xde),
           U64C(0xf6, 0xe3, 0x30, 0x1a, 0x91, 0x58, 0x27, 0x75),
           U64C(0x3f, 0xa0, 0x9e, 0x48, 0xb6, 0x78, 0x07, 0x2a),
           U64C(0x38, 0x88, 0x76, 0x4f, 0xd6, 0x4f, 0xc2, 0x56)),
    BF256C(U64C(0xb6, 0x30, 0x8a, 0xe9, 0x29, 0xf5, 0xc2, 0x98),
           U64C(0x82, 0x84, 0xf1, 0x40, 0xd4, 0xdb, 0xc4, 0x1b),
           U64C(0x81, 0xa9, 0x49, 0x7d, 0x94, 0x09, 0xbe, 0x2f),
           U64C(0xfc, 0x4f, 0x57, 0x71, 0x6d, 0x0b, 0x27, 0x22)),
    BF256C(U64C(0x0b, 0x67, 0x44, 0xde, 0xb9, 0xaf, 0x75, 0x9e),
           U64C(0xbc, 0xaf, 0xf1, 0x66, 0xc6, 0x66, 0xed, 0xac),
           U64C(0x7e, 0x1f, 0x99, 0xf2, 0x3f, 0x25, 0x01, 0xf0),
           U64C(0xf3, 0x29, 0xfa, 0xd1, 0x2f, 0x37, 0x3d, 0xc0)),
    BF256C(U64C(0x8b, 0xe8, 0x32, 0xb3, 0x98, 0xb6, 0x43, 0xba),
           U64C(0x0d, 0x6f, 0xb8, 0x25, 0xd6, 0xc4, 0x37, 0x52),
           U64C(0x45, 0x15, 0xe8, 0xf4, 0x2a, 0x2b, 0x65, 0x2f),
           U64C(0xb8, 0x7b, 0x6b, 0xd2, 0x09, 0xea, 0x3e, 0x13)),
};

#if defined(FAEST_TESTS)
bf256_t bf256_get_alpha(unsigned int idx) {
  return bf256_alpha[idx];
}
#endif

void bf256_byte_combine(bf256_t* dst, const bf256_t* x) {
  *dst = x[0];
  for (unsigned int i = 1; i < 8; ++i) {
    bf256_t tmp;
    bf256_mul(&tmp, &x[i], &bf256_alpha[i - 1]);
    bf256_add_inplace(dst, &tmp);
  }
}

void bf256_sq_bit(bf256_t* out_tag, const bf256_t* in_tag) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  out_tag[7] = in_tag[6] ^ in_tag[7];
  out_tag[0] = in_tag[0] ^ in_tag[4] ^ in_tag[6];
  out_tag[1] = in_tag[4] ^ out_tag[7];
  out_tag[2] = in_tag[1] ^ in_tag[5];
  out_tag[3] = out_tag[1] ^ in_tag[5];
  out_tag[4] = in_tag[2] ^ in_tag[4] ^ in_tag[7];
  out_tag[5] = in_tag[5] ^ in_tag[6];
  out_tag[6] = in_tag[3] ^ in_tag[5];
#else
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
#endif
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
    bf256_mul_bit(&tmp, &bf256_alpha[i - 1], get_bit(x, i));
    bf256_add_inplace(dst, &tmp);
  }
}

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf256_and_64(bf256_t* dst, const bf256_t* lhs,
                                                                   bf64_t rhs) {
  *dst = *lhs & rhs;
}
#else
ATTR_ALWAYS_INLINE static inline void bf256_and_64(bf256_t* dst, const bf256_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}
#endif

#if defined(HAVE_ATTR_VECTOR_SIZE)
#if __has_builtin(__builtin_shufflevector)
#define bf256_shift_right_64(v1) __builtin_shufflevector((v1), bf256_zero(), 4, 0, 1, 2)
#else
ATTR_CONST ATTR_ALWAYS_INLINE static inline bf256_t bf256_shift_right_64(bf256_t v1) {
  bf256_t ret;
  BF_VALUE(ret, 0) = 0;
  BF_VALUE(ret, 1) = BF_VALUE(v1, 0);
  BF_VALUE(ret, 2) = BF_VALUE(v1, 1);
  BF_VALUE(ret, 3) = BF_VALUE(v1, 2);
  return ret;
}
#endif

#define bf256_shift_left_1(value) ((value << 1) | bf256_shift_right_64(value >> 63))
#else
ATTR_CONST
static inline bf256_t bf256_shift_left_1(bf256_t value) {
  value.values[3] = (value.values[3] << 1) | (value.values[2] >> 63);
  value.values[2] = (value.values[2] << 1) | (value.values[1] >> 63);
  value.values[1] = (value.values[1] << 1) | (value.values[0] >> 63);
  value.values[0] = value.values[0] << 1;
  return value;
}
#endif

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline uint64_t
bf256_bit_to_uint64_mask(const bf256_t* value, unsigned int bit) {
  const unsigned int byte_idx = bit / 64;
  const unsigned int bit_idx  = bit % 64;

  return -((BF_VALUE(*value, byte_idx) >> bit_idx) & 1);
}

void bf256_mul(bf256_t* dst, const bf256_t* lhs, const bf256_t* rhs) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  const bf256_t mod = BF256C(bf256_modulus, 0, 0, 0);
#endif
  bf256_t tmp = *lhs;
  bf256_and_64(dst, lhs, bf256_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 256; ++idx) {
    const uint64_t mask = bf256_bit_to_uint64_mask(&tmp, 256 - 1);
    tmp                 = bf256_shift_left_1(tmp);
#if defined(HAVE_ATTR_VECTOR_SIZE)
    tmp ^= mod & mask;
#else
    BF_VALUE(tmp, 0) ^= (mask & bf256_modulus);
#endif

    bf256_t tmp1;
    bf256_and_64(&tmp1, &tmp, bf256_bit_to_uint64_mask(rhs, idx));
    bf256_add_inplace(dst, &tmp1);
  }
}

void bf256_square(bf256_t* dst, const bf256_t* lhs) {
  uint64_t square[8];
  for (unsigned int i = 0; i != 4; ++i) {
    square[2 * i]     = bf_square_expand(BF_VALUE(*lhs, i));
    square[2 * i + 1] = bf_square_expand(BF_VALUE(*lhs, i) >> 32);
  }
  bf_square_reduce_10_5_2(square, 4);

  BF_VALUE(*dst, 0) = square[0];
  BF_VALUE(*dst, 1) = square[1];
  BF_VALUE(*dst, 2) = square[2];
  BF_VALUE(*dst, 3) = square[3];
}

void bf256_mul_inplace(bf256_t* lhs, const bf256_t* rhs) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  const bf256_t mod = BF256C(bf256_modulus, 0, 0, 0);
#endif
  bf256_t tmp = *lhs;

  bf256_and_64(lhs, lhs, bf256_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 256; ++idx) {
    const uint64_t mask = bf256_bit_to_uint64_mask(&tmp, 256 - 1);
    tmp                 = bf256_shift_left_1(tmp);
#if defined(HAVE_ATTR_VECTOR_SIZE)
    tmp ^= mod & mask;
#else
    BF_VALUE(tmp, 0) ^= (mask & bf256_modulus);
#endif

    bf256_t tmp1;
    bf256_and_64(&tmp1, &tmp, bf256_bit_to_uint64_mask(rhs, idx));
    bf256_add_inplace(lhs, &tmp1);
  }
}

void bf256_mul_64_inplace(bf256_t* lhs, bf64_t rhs) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  const bf256_t mod = BF256C(bf256_modulus, 0, 0, 0);
#endif
  bf256_t tmp = *lhs;
  bf256_and_64(lhs, lhs, bf64_bit_to_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 64; ++idx) {
    const uint64_t mask = bf256_bit_to_uint64_mask(&tmp, 256 - 1);
    tmp                 = bf256_shift_left_1(tmp);
#if defined(HAVE_ATTR_VECTOR_SIZE)
    tmp ^= mod & mask;
#else
    BF_VALUE(tmp, 0) ^= (mask & bf256_modulus);
#endif

    bf256_t tmp1;
    bf256_and_64(&tmp1, &tmp, bf64_bit_to_mask(rhs, idx));
    bf256_add_inplace(lhs, &tmp1);
  }
}

#if !defined(HAVE_ATTR_VECTOR_SIZE)
void bf256_mul_bit(bf256_t* dst, const bf256_t* lhs, uint8_t rhs) {
  bf256_and_64(dst, lhs, -((uint64_t)rhs & 1));
}
#endif

#if defined(FAEST_TESTS)
static inline void bf256_dbl_inplace(bf256_t* lhs) {
  uint64_t mask = bf256_bit_to_uint64_mask(lhs, 256 - 1);
  *lhs          = bf256_shift_left_1(*lhs);
#if defined(HAVE_ATTR_VECTOR_SIZE)
  const bf256_t mod = BF256C(bf256_modulus, 0, 0, 0);
  *lhs ^= mod & mask;
#else
  BF_VALUE(*lhs, 0) ^= mask & bf256_modulus;
#endif
}

void bf256_sum_poly(bf256_t* dst, const bf256_t* xs) {
  *dst = xs[256 - 1];
  for (size_t i = 1; i < 256; ++i) {
    bf256_dbl_inplace(dst);
    bf256_add_inplace(dst, &xs[256 - 1 - i]);
  }
}
#endif

void bf256_sum_poly_bits(bf256_t* dst, const uint8_t* xs) {
  bf256_load(dst, xs);
}

// GF(2^384)

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf384_and_64(bf384_t* dst, const bf384_t* lhs,
                                                                   bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->inner); ++i) {
    dst->inner[i] = lhs->inner[i] & rhs;
  }
}
#else
static inline void bf384_and_64(bf384_t* dst, const bf384_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}
#endif

#if defined(HAVE_ATTR_VECTOR_SIZE)
#if __has_builtin(__builtin_shufflevector)
ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf384_t
bf384_shift_right_64(bf384_t v1) {
  bf384_t ret;
  ret.inner[0] = __builtin_shufflevector(v1.inner[0], bf128_zero(), 2, 0);
  ret.inner[1] = __builtin_shufflevector(v1.inner[1], bf128_zero(), 2, 0) |
                 __builtin_shufflevector(v1.inner[0], bf128_zero(), 1, 3);
  ret.inner[2] = __builtin_shufflevector(v1.inner[2], bf128_zero(), 2, 0) |
                 __builtin_shufflevector(v1.inner[1], bf128_zero(), 1, 3);
  return ret;
}
#else
ATTR_CONST ATTR_ALWAYS_INLINE static inline bf384_t bf384_shift_right_64(bf384_t v1) {
  bf384_t ret;
  BF_VALUE(ret.inner[0], 0) = 0;
  BF_VALUE(ret.inner[0], 1) = BF_VALUE(v1.inner[0], 0);
  BF_VALUE(ret.inner[1], 0) = BF_VALUE(v1.inner[0], 1);
  BF_VALUE(ret.inner[1], 1) = BF_VALUE(v1.inner[1], 0);
  BF_VALUE(ret.inner[2], 0) = BF_VALUE(v1.inner[1], 1);
  BF_VALUE(ret.inner[2], 1) = BF_VALUE(v1.inner[2], 0);
  return ret;
}
#endif

ATTR_CONST
static inline bf384_t bf384_shift_left_1(bf384_t value) {
  const bf384_t rhs = bf384_shift_right_64(value);
  for (unsigned int i = 0; i != ARRAY_SIZE(value.inner); ++i) {
    value.inner[i] = (value.inner[i] << 1) | (rhs.inner[i] >> 63);
  }
  return value;
}

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline uint64_t
bf384_bit_to_uint64_mask(const bf384_t* value, unsigned int bit) {
  const unsigned int inner_idx = bit / 128;
  const unsigned int inner_bit = bit % 128;
  const unsigned int byte_idx  = inner_bit / 64;
  const unsigned int bit_idx   = inner_bit % 64;

  return -((BF_VALUE(value->inner[inner_idx], byte_idx) >> bit_idx) & 1);
}
#else
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
#endif

void bf384_mul_128_inplace(bf384_t* lhs, const bf128_t* rhs) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  const bf128_t mod = BF128C(bf384_modulus, 0);
#endif
  bf384_t tmp = *lhs;
  bf384_and_64(lhs, lhs, bf128_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 128; ++idx) {
    const uint64_t mask = bf384_bit_to_uint64_mask(&tmp, 384 - 1);
    tmp                 = bf384_shift_left_1(tmp);
#if defined(HAVE_ATTR_VECTOR_SIZE)
    tmp.inner[0] ^= mod & mask;
#else
    BF_VALUE(tmp, 0) ^= mask & bf384_modulus;
#endif

    bf384_t tmp1;
    bf384_and_64(&tmp1, &tmp, bf128_bit_to_uint64_mask(rhs, idx));
    bf384_add_inplace(lhs, &tmp1);
  }
}

// GF(2^576)

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf576_and_64(bf576_t* dst, const bf576_t* lhs,
                                                                   bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->inner); ++i) {
    dst->inner[i] = lhs->inner[i] & rhs;
  }
}
#else
static inline void bf576_and_64(bf576_t* dst, const bf576_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}
#endif

#if defined(HAVE_ATTR_VECTOR_SIZE)
#if __has_builtin(__builtin_shufflevector)
ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf576_t
bf576_shift_right_64(bf576_t v1) {
  bf576_t ret;
  ret.inner[0] = __builtin_shufflevector(v1.inner[0], bf256_zero(), 4, 0, 1, 7);
  ret.inner[1] = __builtin_shufflevector(v1.inner[1], bf256_zero(), 4, 0, 1, 7) |
                 __builtin_shufflevector(v1.inner[0], bf256_zero(), 2, 5, 6, 7);
  ret.inner[2] = __builtin_shufflevector(v1.inner[2], bf256_zero(), 4, 0, 1, 7) |
                 __builtin_shufflevector(v1.inner[1], bf256_zero(), 2, 5, 6, 7);
  return ret;
}
#else
ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf576_t
bf576_shift_right_64(bf576_t v1) {
  bf576_t ret;
  BF_VALUE(ret.inner[0], 0) = 0;
  BF_VALUE(ret.inner[0], 1) = BF_VALUE(v1.inner[0], 0);
  BF_VALUE(ret.inner[0], 2) = BF_VALUE(v1.inner[0], 1);
  BF_VALUE(ret.inner[1], 0) = BF_VALUE(v1.inner[0], 2);
  BF_VALUE(ret.inner[1], 1) = BF_VALUE(v1.inner[1], 0);
  BF_VALUE(ret.inner[1], 2) = BF_VALUE(v1.inner[1], 1);
  BF_VALUE(ret.inner[2], 0) = BF_VALUE(v1.inner[1], 2);
  BF_VALUE(ret.inner[2], 1) = BF_VALUE(v1.inner[2], 0);
  BF_VALUE(ret.inner[2], 2) = BF_VALUE(v1.inner[2], 1);
  return ret;
}
#endif

ATTR_CONST
static inline bf576_t bf576_shift_left_1(bf576_t value) {
  const bf576_t rhs = bf576_shift_right_64(value);
  for (unsigned int i = 0; i != ARRAY_SIZE(value.inner); ++i) {
    value.inner[i] = (value.inner[i] << 1) | (rhs.inner[i] >> 63);
  }
  return value;
}

ATTR_PURE ATTR_ALWAYS_INLINE static inline uint64_t bf576_bit_to_uint64_mask(const bf576_t* value,
                                                                             unsigned int bit) {
  const unsigned int inner_idx = bit / 192;
  const unsigned int inner_bit = bit % 192;
  const unsigned int byte_idx  = inner_bit / 64;
  const unsigned int bit_idx   = inner_bit % 64;

  return -((BF_VALUE(value->inner[inner_idx], byte_idx) >> bit_idx) & 1);
}
#else
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
#endif

void bf576_mul_192_inplace(bf576_t* lhs, const bf192_t* rhs) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  const bf192_t mod = BF192C(bf576_modulus, 0, 0);
#endif
  bf576_t tmp = *lhs;
  bf576_and_64(lhs, lhs, bf192_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 192; ++idx) {
    const uint64_t mask = bf576_bit_to_uint64_mask(&tmp, 576 - 1);
    tmp                 = bf576_shift_left_1(tmp);
#if defined(HAVE_ATTR_VECTOR_SIZE)
    tmp.inner[0] ^= mod & mask;
#else
    BF_VALUE(tmp, 0) ^= mask & bf576_modulus;
#endif

    bf576_t tmp1;
    bf576_and_64(&tmp1, &tmp, bf192_bit_to_uint64_mask(rhs, idx));
    bf576_add_inplace(lhs, &tmp1);
  }
}

// GF(2^768)

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE static inline void bf768_and_64(bf768_t* dst, const bf768_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->inner); ++i) {
    dst->inner[i] = lhs->inner[i] & rhs;
  }
}
#else
static inline void bf768_and_64(bf768_t* dst, const bf768_t* lhs, bf64_t rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] & rhs;
  }
}
#endif

#if defined(HAVE_ATTR_VECTOR_SIZE)
#if __has_builtin(__builtin_shufflevector)
ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf768_t
bf768_shift_right_64(bf768_t v1) {
  bf768_t ret;
  ret.inner[0] = __builtin_shufflevector(v1.inner[0], bf256_zero(), 4, 0, 1, 2);
  ret.inner[1] = __builtin_shufflevector(v1.inner[1], bf256_zero(), 4, 0, 1, 2) |
                 __builtin_shufflevector(v1.inner[0], bf256_zero(), 3, 5, 6, 7);
  ret.inner[2] = __builtin_shufflevector(v1.inner[2], bf256_zero(), 4, 0, 1, 2) |
                 __builtin_shufflevector(v1.inner[1], bf256_zero(), 3, 5, 6, 7);
  return ret;
}
#else
ATTR_CONST ATTR_ALWAYS_INLINE static inline bf768_t bf768_shift_right_64(bf768_t v1) {
  bf768_t ret;
  BF_VALUE(ret.inner[0], 0) = 0;
  BF_VALUE(ret.inner[0], 1) = BF_VALUE(v1.inner[0], 0);
  BF_VALUE(ret.inner[0], 2) = BF_VALUE(v1.inner[0], 1);
  BF_VALUE(ret.inner[0], 3) = BF_VALUE(v1.inner[0], 2);
  BF_VALUE(ret.inner[1], 0) = BF_VALUE(v1.inner[0], 3);
  BF_VALUE(ret.inner[1], 1) = BF_VALUE(v1.inner[1], 0);
  BF_VALUE(ret.inner[1], 2) = BF_VALUE(v1.inner[1], 1);
  BF_VALUE(ret.inner[1], 3) = BF_VALUE(v1.inner[1], 2);
  BF_VALUE(ret.inner[2], 0) = BF_VALUE(v1.inner[1], 3);
  BF_VALUE(ret.inner[2], 1) = BF_VALUE(v1.inner[2], 0);
  BF_VALUE(ret.inner[2], 2) = BF_VALUE(v1.inner[2], 1);
  BF_VALUE(ret.inner[2], 3) = BF_VALUE(v1.inner[2], 2);
  return ret;
}
#endif

ATTR_CONST
static inline bf768_t bf768_shift_left_1(bf768_t value) {
  bf768_t rhs = bf768_shift_right_64(value);
  for (unsigned int i = 0; i != ARRAY_SIZE(value.inner); ++i) {
    value.inner[i] = (value.inner[i] << 1) | (rhs.inner[i] >> 63);
  }
  return value;
}

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline uint64_t
bf768_bit_to_uint64_mask(const bf768_t* value, unsigned int bit) {
  const unsigned int inner_idx = bit / 256;
  const unsigned int inner_bit = bit % 256;
  const unsigned int byte_idx  = inner_bit / 64;
  const unsigned int bit_idx   = inner_bit % 64;

  return -((BF_VALUE(value->inner[inner_idx], byte_idx) >> bit_idx) & 1);
}
#else
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
#endif

void bf768_mul_256_inplace(bf768_t* lhs, const bf256_t* rhs) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  const bf256_t mod = BF256C(bf768_modulus, 0, 0, 0);
#endif
  bf768_t tmp = *lhs;
  bf768_and_64(lhs, lhs, bf256_bit_to_uint64_mask(rhs, 0));
  for (unsigned int idx = 1; idx != 256; ++idx) {
    const uint64_t mask = bf768_bit_to_uint64_mask(&tmp, 768 - 1);
    tmp                 = bf768_shift_left_1(tmp);
#if defined(HAVE_ATTR_VECTOR_SIZE)
    tmp.inner[0] ^= mod & mask;
#else
    BF_VALUE(tmp, 0) ^= mask & bf768_modulus;
#endif

    bf768_t tmp1;
    bf768_and_64(&tmp1, &tmp, bf256_bit_to_uint64_mask(rhs, idx));
    bf768_add_inplace(lhs, &tmp1);
  }
}

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
  const size_t word_bit_offset = word_idx * 64;
  if (word_bit_offset >= dst_bits) {
    return;
  }

  const size_t remaining_bits = dst_bits - word_bit_offset;
  const size_t word_bits      = remaining_bits < 64 ? remaining_bits : 64;
  const size_t word_bytes     = (word_bits + 7) / 8;
  uint8_t* dst_word           = dst + word_idx * sizeof(uint64_t);
  uint64_t ret                = 0;

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

static inline void xor_shifted_table_u16(uint8_t* dst, size_t dst_bits, uint64_t module,
                                         size_t shift, uint64_t mask) {
  const size_t dst_word_idx   = shift / 64;
  const size_t dst_bit_offset = shift % 64;

  if (dst_bit_offset == 0) {
    xor_dst_word(dst, dst_word_idx, dst_bits, module & mask);
  } else {
    const uint64_t shifted = module << dst_bit_offset;
    const uint64_t carry   = module >> (64 - dst_bit_offset);
    xor_dst_word(dst, dst_word_idx, dst_bits, shifted & mask);
    xor_dst_word(dst, dst_word_idx + 1, dst_bits, carry & mask);
  }
}

void bf2_poly_reduce(uint8_t* dst, const uint8_t* src, size_t src_bits, uint16_t module,
                     size_t deg) {
  size_t src_bytes = (src_bits + 7) / 8;
  memcpy(dst, src, src_bytes);

  for (size_t i = src_bits; i >= deg; --i) {
    const uint64_t mask = -((uint64_t)ptr_get_bit(dst, i - 1));
    const size_t shift  = i - deg;
    xor_shifted_table_u16(dst, src_bits, module, shift, mask);
  }
}