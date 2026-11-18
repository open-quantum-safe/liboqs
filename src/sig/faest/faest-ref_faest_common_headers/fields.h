/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_FIELDS_H
#define FAEST_FIELDS_H

#include "macros.h"
#include "compat.h"
#include "endian_compat.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

FAEST_BEGIN_C_DECL

typedef uint8_t bf8_t;
typedef uint64_t bf64_t;

#define BF128_NUM_BYTES (128 / 8)
#define BF192_NUM_BYTES (192 / 8)
#define BF256_NUM_BYTES (256 / 8)
#define BF384_NUM_BYTES (384 / 8)
#define BF576_NUM_BYTES (576 / 8)
#define BF768_NUM_BYTES (768 / 8)

// GF(2^128) with X^128 + X^7 + X^2 + X^1 + 1
#define BF128_MODULUS (UINT64_C((1 << 7) | (1 << 2) | (1 << 1) | 1))
// GF(2^192) with X^192 + X^7 + X^2 + X^1 + 1
#define BF192_MODULUS (UINT64_C((1 << 7) | (1 << 2) | (1 << 1) | 1))
// GF(2^256) with X^256 + X^10 + X^5 + X^2 + 1
#define BF256_MODULUS (UINT64_C((1 << 10) | (1 << 5) | (1 << 2) | 1))
// GF(2^384) with X^384 + X^12 + X^3 + X^2 + 1
#define BF384_MODULUS (UINT64_C((1 << 12) | (1 << 3) | (1 << 2) | 1))
// GF(2^576) with X^576 + X^13 + X^4 + X^3 + 1
#define BF576_MODULUS (UINT64_C((1 << 13) | (1 << 4) | (1 << 3) | 1))
// GF(2^768) with X^768 + X^19 + X^17 + X^4 + 1
#define BF768_MODULUS (UINT64_C((1 << 19) | (1 << 17) | (1 << 4) | 1))

ATTR_CONST uint8_t bits_sq(uint8_t x);

// GF(2^8) implementation

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf8_t bf8_load(const uint8_t* src) {
  return *src;
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf8_store(uint8_t* dst, bf8_t src) {
  *dst = src;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf8_t bf8_zero(void) {
  return 0;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf8_t bf8_one(void) {
  return 1;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL inline bf8_t bf8_add(bf8_t lhs, bf8_t rhs) {
  return lhs ^ rhs;
}

ATTR_CONST bf8_t bf8_mul(bf8_t lhs, bf8_t rhs);
ATTR_CONST bf8_t bf8_mulX(bf8_t v);
ATTR_CONST bf8_t bf8_square(bf8_t lhs);
ATTR_CONST bf8_t bf8_inv(bf8_t lhs);

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf8_t bf8_from_bit(uint8_t bit) {
  return bit & 1;
}

// GF(2^64) implementation

ATTR_PURE ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf64_t bf64_load(const uint8_t* src) {
  bf64_t ret;
  memcpy(&ret, src, sizeof(ret));
#if defined(FAEST_IS_BIG_ENDIAN)
  ret = le64toh(ret);
#endif
  return ret;
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf64_store(uint8_t* dst, bf64_t src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  src = htole64(src);
#endif
  memcpy(dst, &src, sizeof(src));
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf64_t bf64_zero(void) {
  return 0;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf64_t bf64_one(void) {
  return 1;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf64_t bf64_add(bf64_t lhs,
                                                                            bf64_t rhs) {
  return lhs ^ rhs;
}

#if defined(FAEST_TESTS)
ATTR_CONST bf64_t bf64_mul(bf64_t lhs, bf64_t rhs);
#endif

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf64_t bf64_from_bit(uint8_t bit) {
  return bit & 1;
}

#define FAEST_FIELDS_USE_NEON 4
#define FAEST_FIELDS_USE_AVX2 3
#define FAEST_FIELDS_USE_SSE2 2
#define FAEST_FIELDS_USE_VECTOR 1
#define FAEST_FIELDS_USE_U64 0

// If no field implementation has been selected, select the best candidate. Since the current code
// base only supports one field implementation, this is a compile decision and there is no runtime
// dispatch based on CPU features. Hence, we select the best option that is supported by
// compile-time settings.
#if !defined(FAEST_FIELDS)
#if defined(HAVE_AVX2) && defined(HAVE_PCLMUL) && __AVX2__ && __PCLMUL__
// If AVX2/PCLMUL is supported, and we are building with both enabled, use the AVX2 implementation.
#define FAEST_FIELDS FAEST_FIELDS_USE_AVX2
#elif defined(HAVE_SSE2) && __SSE2__
// If SSE2 is supported and we are building with SSE2 enabled, use the SSE2 implementation.
#define FAEST_FIELDS FAEST_FIELDS_USE_SSE2
#elif defined(HAVE_NEON) && defined(FAEST_IS_LITTLE_ENDIAN) &&                                     \
    (defined(__ARM_NEON) || defined(__ARM_NEON__) || defined(_M_ARM64) || defined(_M_ARM64EC))
// If NEON is supported, and we are building with it enabled, use the NENO implementation.
#define FAEST_FIELDS FAEST_FIELDS_USE_NEON
#elif defined(HAVE_ATTR_VECTOR_SIZE)
// If GCC vector extensions are available, use them.
#define FAEST_FIELDS FAEST_FIELDS_USE_VECTOR
#else
// Fallback to the uint64_t implementation
#define FAEST_FIELDS FAEST_FIELDS_USE_U64
#endif
#endif // !FAEST_FIELDS

// Use the selected implementation.
#if FAEST_FIELDS == FAEST_FIELDS_USE_AVX2
#include "fields_avx2.h"
#elif FAEST_FIELDS == FAEST_FIELDS_USE_SSE2
#include "fields_sse2.h"
#elif FAEST_FIELDS == FAEST_FIELDS_USE_NEON
#include "fields_neon.h"
#elif FAEST_FIELDS == FAEST_FIELDS_USE_VECTOR
#include "fields_vector.h"
#else
#include "fields_u64.h"
#endif

// GF(2^128) implementation

#if defined(FAEST_TESTS)
ATTR_PURE bf128_t bf128_get_alpha(unsigned int idx);
#endif
void bf128_byte_combine(bf128_t* dst, const bf128_t* x);
void bf128_byte_combine_bits(bf128_t* dst, uint8_t x);
void bf128_sq_bit(bf128_t* out_tag, const bf128_t* in_tag);
void bf128_sq_bit_inplace(bf128_t* tag);
void bf128_byte_combine_sq(bf128_t* dst, const bf128_t* x);

#if defined(FAEST_TESTS)
void bf128_sum_poly(bf128_t* dst, const bf128_t* xs);
#endif
void bf128_sum_poly_bits(bf128_t* dst, const uint8_t* xs);

// GF(2^192) implemenation

#if defined(FAEST_TESTS)
ATTR_PURE bf192_t bf192_get_alpha(unsigned int idx);
#endif
void bf192_byte_combine(bf192_t* dst, const bf192_t* x);
void bf192_byte_combine_bits(bf192_t* dst, uint8_t x);
void bf192_sq_bit(bf192_t* out_tag, const bf192_t* in_tag);
void bf192_sq_bit_inplace(bf192_t* tag);
void bf192_byte_combine_sq(bf192_t* dst, const bf192_t* x);

#if defined(FAEST_TESTS)
void bf192_sum_poly(bf192_t* dst, const bf192_t* xs);
#endif
void bf192_sum_poly_bits(bf192_t* dst, const uint8_t* xs);

// GF(2^256) implementation

#if defined(FAEST_TESTS)
ATTR_PURE bf256_t bf256_get_alpha(unsigned int idx);
#endif
void bf256_byte_combine(bf256_t* dst, const bf256_t* x);
void bf256_byte_combine_bits(bf256_t* dst, uint8_t x);
void bf256_sq_bit(bf256_t* out_tag, const bf256_t* in_tag);
void bf256_sq_bit_inplace(bf256_t* tag);
void bf256_byte_combine_sq(bf256_t* dst, const bf256_t* x);

#if defined(FAEST_TESTS)
void bf256_sum_poly(bf256_t* dst, const bf256_t* xs);
#endif
void bf256_sum_poly_bits(bf256_t* dst, const uint8_t* xs);

// GF(2^384) implementation

void bf384_mul_128_inplace(bf384_t* lhs, const bf128_t* rhs);

// GF(2^576) implementation

void bf576_mul_192_inplace(bf576_t* lhs, const bf192_t* rhs);

// GF(2^768) implementation

void bf768_mul_256_inplace(bf768_t* lhs, const bf256_t* rhs);

void bf2_matrix_mul_tbl(uint8_t* dst, const uint8_t* src, const uint64_t* table, size_t rows,
                        size_t columns, size_t table_words);

void bf2_poly_mul(uint8_t* dst, const uint8_t* src, size_t src_bits, const uint64_t* table,
                  size_t table_bits);

FAEST_END_C_DECL

#endif
