/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_FIELDS_H
#define FAEST_FIELDS_H

#include "macros.h"
#include "endian_compat.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

FAEST_BEGIN_C_DECL

typedef uint8_t bf8_t;
typedef uint64_t bf64_t;

#if defined(HAVE_ATTR_VECTOR_SIZE)
#define BF_VALUE(v, i) ((v)[i])

typedef uint64_t bf128_t ATTR_VECTOR_SIZE(16);
typedef uint64_t bf192_t ATTR_VECTOR_SIZE(32);
typedef uint64_t bf256_t ATTR_VECTOR_SIZE(32);
typedef struct {
  bf128_t inner[3];
} bf384_t;
typedef struct {
  bf192_t inner[3];
} bf576_t;
typedef struct {
  bf256_t inner[3];
} bf768_t;

#define BF128_ALIGN 16
#define BF192_ALIGN 32
#define BF256_ALIGN 32

#define BF128C(x0, x1) {x0, x1}
#define BF192C(x0, x1, x2) {x0, x1, x2, UINT64_C(0)}
#define BF256C(x0, x1, x2, x3) {x0, x1, x2, x3}
#define BF384C(x0, x1, x2, x3, x4, x5)                                                             \
  {                                                                                                \
    {                                                                                              \
      BF128C(x0, x1), BF128C(x2, x3), BF128C(x4, x5)                                               \
    }                                                                                              \
  }
#define BF576C(x0, x1, x2, x3, x4, x5, x6, x7, x8)                                                 \
  {                                                                                                \
    {                                                                                              \
      BF192C(x0, x1, x2), BF192C(x3, x4, x5), BF192C(x6, x7, x8)                                   \
    }                                                                                              \
  }
#define BF768C(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11)                                   \
  {                                                                                                \
    {                                                                                              \
      BF256C(x0, x1, x2, x3), BF256C(x4, x5, x6, x7), BF256C(x8, x9, x10, x11)                     \
    }                                                                                              \
  }
#else
#define BF_VALUE(v, i) ((v).values[i])

typedef struct {
  uint64_t values[2];
} bf128_t;

typedef struct {
  uint64_t values[3];
} bf192_t;

typedef struct {
  uint64_t values[4];
} bf256_t;

typedef struct {
  uint64_t values[6];
} bf384_t;

typedef struct {
  uint64_t values[9];
} bf576_t;

typedef struct {
  uint64_t values[12];
} bf768_t;

#define BF128C(x0, x1)                                                                             \
  {                                                                                                \
    {                                                                                              \
      x0, x1                                                                                       \
    }                                                                                              \
  }
#define BF192C(x0, x1, x2)                                                                         \
  {                                                                                                \
    {                                                                                              \
      x0, x1, x2                                                                                   \
    }                                                                                              \
  }
#define BF256C(x0, x1, x2, x3)                                                                     \
  {                                                                                                \
    {                                                                                              \
      x0, x1, x2, x3                                                                               \
    }                                                                                              \
  }
#define BF384C(x0, x1, x2, x3, x4, x5)                                                             \
  {                                                                                                \
    {                                                                                              \
      x0, x1, x2, x3, x4, x5                                                                       \
    }                                                                                              \
  }
#define BF576C(x0, x1, x2, x3, x4, x5, x6, x7, x8)                                                 \
  {                                                                                                \
    {                                                                                              \
      x0, x1, x2, x3, x4, x5, x6, x7, x8                                                           \
    }                                                                                              \
  }
#define BF768C(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11)                                   \
  {                                                                                                \
    {                                                                                              \
      x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11                                             \
    }                                                                                              \
  }

#define BF128_ALIGN 8
#define BF192_ALIGN 8
#define BF256_ALIGN 8
#endif

#define BF128_NUM_BYTES (128 / 8)
#define BF192_NUM_BYTES (192 / 8)
#define BF256_NUM_BYTES (256 / 8)
#define BF384_NUM_BYTES (384 / 8)
#define BF576_NUM_BYTES (576 / 8)
#define BF768_NUM_BYTES (768 / 8)

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

// GF(2^128) implementation

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf128_load(bf128_t* dst, const uint8_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF128_NUM_BYTES / sizeof(uint64_t); ++i, src += sizeof(uint64_t)) {
    memcpy(&BF_VALUE(*dst, i), src, sizeof(uint64_t));
    BF_VALUE(*dst, i) = le64toh(BF_VALUE(*dst, i));
  }
#else
  memcpy(dst, src, BF128_NUM_BYTES);
#endif
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf128_store(uint8_t* dst,
                                                                  const bf128_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF128_NUM_BYTES / sizeof(uint64_t); ++i, dst += sizeof(uint64_t)) {
    uint64_t tmp = htole64(BF_VALUE(*src, i));
    memcpy(dst, &tmp, sizeof(tmp));
  }
#else
  memcpy(dst, src, BF128_NUM_BYTES);
#endif
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf128_t bf128_from_bit(uint8_t bit) {
  bf128_t ret      = BF128C(0, 0);
  BF_VALUE(ret, 0) = bit & 1u;
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf128_t bf128_zero(void) {
  const bf128_t ret = BF128C(0, 0);
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf128_t bf128_one(void) {
  const bf128_t ret = BF128C(1u, 0);
  return ret;
}

#if defined(FAEST_TESTS)
ATTR_PURE bf128_t bf128_get_alpha(unsigned int idx);
#endif
void bf128_byte_combine(bf128_t* dst, const bf128_t* x);
void bf128_byte_combine_bits(bf128_t* dst, uint8_t x);
void bf128_sq_bit(bf128_t* out_tag, const bf128_t* in_tag);
void bf128_sq_bit_inplace(bf128_t* tag);
void bf128_byte_combine_sq(bf128_t* dst, const bf128_t* x);

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf128_add(bf128_t* dst, const bf128_t* lhs,
                                                                const bf128_t* rhs) {
  *dst = *lhs ^ *rhs;
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf128_add_inplace(bf128_t* lhs,
                                                                        const bf128_t* rhs) {
  *lhs ^= *rhs;
}
#else
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf128_add(bf128_t* dst, const bf128_t* lhs,
                                                                const bf128_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] ^ rhs->values[i];
  }
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf128_add_inplace(bf128_t* lhs,
                                                                        const bf128_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    lhs->values[i] ^= rhs->values[i];
  }
}
#endif

void bf128_mul(bf128_t* dst, const bf128_t* lhs, const bf128_t* rhs);
void bf128_square(bf128_t* dst, const bf128_t* lhs);
void bf128_mul_inplace(bf128_t* lhs, const bf128_t* rhs);
void bf128_mul_64_inplace(bf128_t* lhs, bf64_t rhs);
#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void
bf128_mul_bit(bf128_t* dst, const bf128_t* lhs, uint8_t rhs) {
  *dst = *lhs & -((uint64_t)(rhs) & 1);
}
#else
void bf128_mul_bit(bf128_t* dst, const bf128_t* lhs, uint8_t rhs);
#endif
#if defined(FAEST_TESTS)
void bf128_sum_poly(bf128_t* dst, const bf128_t* xs);
#endif
void bf128_sum_poly_bits(bf128_t* dst, const uint8_t* xs);

// GF(2^192) implemenation

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf192_load(bf192_t* dst, const uint8_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF192_NUM_BYTES / sizeof(uint64_t); ++i, src += sizeof(uint64_t)) {
    memcpy(&BF_VALUE(*dst, i), src, sizeof(uint64_t));
    BF_VALUE(*dst, i) = le64toh(BF_VALUE(*dst, i));
  }
#else
  memcpy(dst, src, BF192_NUM_BYTES);
#endif
#if defined(HAVE_ATTR_VECTOR_SIZE)
  BF_VALUE(*dst, 3) = 0;
#endif
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf192_store(uint8_t* dst,
                                                                  const bf192_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF192_NUM_BYTES / sizeof(uint64_t); ++i, dst += sizeof(uint64_t)) {
    uint64_t tmp = htole64(BF_VALUE(*src, i));
    memcpy(dst, &tmp, sizeof(tmp));
  }
#else
  memcpy(dst, src, BF192_NUM_BYTES);
#endif
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf192_t bf192_from_bit(uint8_t bit) {
  bf192_t ret      = BF192C(0, 0, 0);
  BF_VALUE(ret, 0) = bit & 1u;
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf192_t bf192_zero(void) {
  const bf192_t ret = BF192C(0, 0, 0);
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf192_t bf192_one(void) {
  const bf192_t ret = BF192C(1, 0, 0);
  return ret;
}

#if defined(FAEST_TESTS)
ATTR_PURE bf192_t bf192_get_alpha(unsigned int idx);
#endif
void bf192_byte_combine(bf192_t* dst, const bf192_t* x);
void bf192_byte_combine_bits(bf192_t* dst, uint8_t x);
void bf192_sq_bit(bf192_t* out_tag, const bf192_t* in_tag);
void bf192_sq_bit_inplace(bf192_t* tag);
void bf192_byte_combine_sq(bf192_t* dst, const bf192_t* x);

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf192_add(bf192_t* dst, const bf192_t* lhs,
                                                                const bf192_t* rhs) {
  *dst = *lhs ^ *rhs;
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf192_add_inplace(bf192_t* lhs,
                                                                        const bf192_t* rhs) {
  *lhs ^= *rhs;
}
#else
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf192_add(bf192_t* dst, const bf192_t* lhs,
                                                                const bf192_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] ^ rhs->values[i];
  }
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf192_add_inplace(bf192_t* lhs,
                                                                        const bf192_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    lhs->values[i] ^= rhs->values[i];
  }
}
#endif

void bf192_mul(bf192_t* dst, const bf192_t* lhs, const bf192_t* rhs);
void bf192_square(bf192_t* dst, const bf192_t* lhs);
void bf192_mul_inplace(bf192_t* lhs, const bf192_t* rhs);
void bf192_mul_64_inplace(bf192_t* lhs, bf64_t rhs);
#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void
bf192_mul_bit(bf192_t* dst, const bf192_t* lhs, uint8_t rhs) {
  *dst = *lhs & -((uint64_t)(rhs) & 1);
}
#else
void bf192_mul_bit(bf192_t* dst, const bf192_t* lhs, uint8_t rhs);
#endif
#if defined(FAEST_TESTS)
void bf192_sum_poly(bf192_t* dst, const bf192_t* xs);
#endif
void bf192_sum_poly_bits(bf192_t* dst, const uint8_t* xs);

// GF(2^256) implementation

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf256_load(bf256_t* dst, const uint8_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF256_NUM_BYTES / sizeof(uint64_t); ++i, src += sizeof(uint64_t)) {
    memcpy(&BF_VALUE(*dst, i), src, sizeof(uint64_t));
    BF_VALUE(*dst, i) = le64toh(BF_VALUE(*dst, i));
  }
#else
  memcpy(dst, src, BF256_NUM_BYTES);
#endif
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf256_store(uint8_t* dst,
                                                                  const bf256_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF256_NUM_BYTES / sizeof(uint64_t); ++i, dst += sizeof(uint64_t)) {
    uint64_t tmp = htole64(BF_VALUE(*src, i));
    memcpy(dst, &tmp, sizeof(tmp));
  }
#else
  memcpy(dst, src, BF256_NUM_BYTES);
#endif
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf256_t bf256_from_bit(uint8_t bit) {
  bf256_t ret      = BF256C(0, 0, 0, 0);
  BF_VALUE(ret, 0) = bit & 1u;
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf256_t bf256_zero(void) {
  const bf256_t ret = BF256C(0, 0, 0, 0);
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf256_t bf256_one(void) {
  const bf256_t ret = BF256C(1, 0, 0, 0);
  return ret;
}

#if defined(FAEST_TESTS)
ATTR_PURE bf256_t bf256_get_alpha(unsigned int idx);
#endif
void bf256_byte_combine(bf256_t* dst, const bf256_t* x);
void bf256_byte_combine_bits(bf256_t* dst, uint8_t x);
void bf256_sq_bit(bf256_t* out_tag, const bf256_t* in_tag);
void bf256_sq_bit_inplace(bf256_t* tag);
void bf256_byte_combine_sq(bf256_t* dst, const bf256_t* x);

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf256_add(bf256_t* dst, const bf256_t* lhs,
                                                                const bf256_t* rhs) {
  *dst = *lhs ^ *rhs;
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf256_add_inplace(bf256_t* lhs,
                                                                        const bf256_t* rhs) {
  *lhs ^= *rhs;
}
#else
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf256_add(bf256_t* dst, const bf256_t* lhs,
                                                                const bf256_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] ^ rhs->values[i];
  }
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf256_add_inplace(bf256_t* lhs,
                                                                        const bf256_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    lhs->values[i] ^= rhs->values[i];
  }
}
#endif

void bf256_mul(bf256_t* dst, const bf256_t* lhs, const bf256_t* rhs);
void bf256_square(bf256_t* dst, const bf256_t* lhs);
void bf256_mul_inplace(bf256_t* lhs, const bf256_t* rhs);
void bf256_mul_64_inplace(bf256_t* lhs, bf64_t rhs);
#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void
bf256_mul_bit(bf256_t* dst, const bf256_t* lhs, uint8_t rhs) {
  *dst = *lhs & -((uint64_t)(rhs) & 1);
}
#else
void bf256_mul_bit(bf256_t* dst, const bf256_t* lhs, uint8_t rhs);
#endif
#if defined(FAEST_TESTS)
void bf256_sum_poly(bf256_t* dst, const bf256_t* xs);
#endif
void bf256_sum_poly_bits(bf256_t* dst, const uint8_t* xs);

// GF(2^384) implementation

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf384_load(bf384_t* dst, const uint8_t* src) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  bf128_load(&dst->inner[0], src);
  bf128_load(&dst->inner[1], src + BF128_NUM_BYTES);
  bf128_load(&dst->inner[2], src + 2 * BF128_NUM_BYTES);
#else
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF384_NUM_BYTES / sizeof(uint64_t); ++i, src += sizeof(uint64_t)) {
    memcpy(&BF_VALUE(*dst, i), src, sizeof(uint64_t));
    BF_VALUE(*dst, i) = le64toh(BF_VALUE(*dst, i));
  }
#else
  memcpy(dst, src, BF384_NUM_BYTES);
#endif
#endif
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf384_store(uint8_t* dst,
                                                                  const bf384_t* src) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  bf128_store(dst, &src->inner[0]);
  bf128_store(dst + BF128_NUM_BYTES, &src->inner[1]);
  bf128_store(dst + 2 * BF128_NUM_BYTES, &src->inner[2]);
#else
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF384_NUM_BYTES / sizeof(uint64_t); ++i, dst += sizeof(uint64_t)) {
    uint64_t tmp = htole64(BF_VALUE(*src, i));
    memcpy(dst, &tmp, sizeof(tmp));
  }
#else
  memcpy(dst, src, BF384_NUM_BYTES);
#endif
#endif
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf384_t bf384_zero(void) {
  const bf384_t ret = BF384C(0, 0, 0, 0, 0, 0);
  return ret;
}

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf384_add(bf384_t* dst, const bf384_t* lhs,
                                                                const bf384_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->inner); ++i) {
    dst->inner[i] = lhs->inner[i] ^ rhs->inner[i];
  }
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf384_add_inplace(bf384_t* lhs,
                                                                        const bf384_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->inner); ++i) {
    lhs->inner[i] ^= rhs->inner[i];
  }
}
#else
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf384_add(bf384_t* dst, const bf384_t* lhs,
                                                                const bf384_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] ^ rhs->values[i];
  }
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf384_add_inplace(bf384_t* lhs,
                                                                        const bf384_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    lhs->values[i] ^= rhs->values[i];
  }
}
#endif

void bf384_mul_128_inplace(bf384_t* lhs, const bf128_t* rhs);

// GF(2^576) implementation

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf576_load(bf576_t* dst, const uint8_t* src) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  bf192_load(&dst->inner[0], src);
  bf192_load(&dst->inner[1], src + BF192_NUM_BYTES);
  bf192_load(&dst->inner[2], src + 2 * BF192_NUM_BYTES);
#else
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF576_NUM_BYTES / sizeof(uint64_t); ++i, src += sizeof(uint64_t)) {
    memcpy(&BF_VALUE(*dst, i), src, sizeof(uint64_t));
    BF_VALUE(*dst, i) = le64toh(BF_VALUE(*dst, i));
  }
#else
  memcpy(dst, src, BF576_NUM_BYTES);
#endif
#endif
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf576_store(uint8_t* dst,
                                                                  const bf576_t* src) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  bf192_store(dst, &src->inner[0]);
  bf192_store(dst + BF192_NUM_BYTES, &src->inner[1]);
  bf192_store(dst + 2 * BF192_NUM_BYTES, &src->inner[2]);
#else
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF576_NUM_BYTES / sizeof(uint64_t); ++i, dst += sizeof(uint64_t)) {
    uint64_t tmp = htole64(BF_VALUE(*src, i));
    memcpy(dst, &tmp, sizeof(tmp));
  }
#else
  memcpy(dst, src, BF576_NUM_BYTES);
#endif
#endif
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf576_t bf576_zero(void) {
  const bf576_t ret = BF576C(0, 0, 0, 0, 0, 0, 0, 0, 0);
  return ret;
}

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf576_add(bf576_t* dst, const bf576_t* lhs,
                                                                const bf576_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->inner); ++i) {
    dst->inner[i] = lhs->inner[i] ^ rhs->inner[i];
  }
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf576_add_inplace(bf576_t* lhs,
                                                                        const bf576_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->inner); ++i) {
    lhs->inner[i] ^= rhs->inner[i];
  }
}
#else
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf576_add(bf576_t* dst, const bf576_t* lhs,
                                                                const bf576_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] ^ rhs->values[i];
  }
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf576_add_inplace(bf576_t* lhs,
                                                                        const bf576_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    lhs->values[i] ^= rhs->values[i];
  }
}
#endif

void bf576_mul_192_inplace(bf576_t* lhs, const bf192_t* rhs);

// GF(2^768) implementation

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf768_load(bf768_t* dst, const uint8_t* src) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  bf256_load(&dst->inner[0], src);
  bf256_load(&dst->inner[1], src + BF256_NUM_BYTES);
  bf256_load(&dst->inner[2], src + 2 * BF256_NUM_BYTES);
#else
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF768_NUM_BYTES / sizeof(uint64_t); ++i, src += sizeof(uint64_t)) {
    memcpy(&BF_VALUE(*dst, i), src, sizeof(uint64_t));
    BF_VALUE(*dst, i) = le64toh(BF_VALUE(*dst, i));
  }
#else
  memcpy(dst, src, BF768_NUM_BYTES);
#endif
#endif
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf768_store(uint8_t* dst,
                                                                  const bf768_t* src) {
#if defined(HAVE_ATTR_VECTOR_SIZE)
  bf256_store(dst, &src->inner[0]);
  bf256_store(dst + BF256_NUM_BYTES, &src->inner[1]);
  bf256_store(dst + 2 * BF256_NUM_BYTES, &src->inner[2]);
#else
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF768_NUM_BYTES / sizeof(uint64_t); ++i, dst += sizeof(uint64_t)) {
    uint64_t tmp = htole64(BF_VALUE(*src, i));
    memcpy(dst, &tmp, sizeof(tmp));
  }
#else
  memcpy(dst, src, BF768_NUM_BYTES);
#endif
#endif
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf768_t bf768_zero(void) {
  const bf768_t ret = BF768C(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  return ret;
}

#if defined(HAVE_ATTR_VECTOR_SIZE)
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf768_add(bf768_t* dst, const bf768_t* lhs,
                                                                const bf768_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->inner); ++i) {
    dst->inner[i] = lhs->inner[i] ^ rhs->inner[i];
  }
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf768_add_inplace(bf768_t* lhs,
                                                                        const bf768_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->inner); ++i) {
    lhs->inner[i] ^= rhs->inner[i];
  }
}
#else
ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf768_add(bf768_t* dst, const bf768_t* lhs,
                                                                const bf768_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    dst->values[i] = lhs->values[i] ^ rhs->values[i];
  }
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf768_add_inplace(bf768_t* lhs,
                                                                        const bf768_t* rhs) {
  for (unsigned int i = 0; i != ARRAY_SIZE(lhs->values); ++i) {
    lhs->values[i] ^= rhs->values[i];
  }
}
#endif

void bf768_mul_256_inplace(bf768_t* lhs, const bf256_t* rhs);

void bf2_matrix_mul_tbl(uint8_t* dst, const uint8_t* src, const uint64_t* table, size_t rows,
                        size_t columns, size_t table_words);

void bf2_poly_mul(uint8_t* dst, const uint8_t* src, size_t src_bits, const uint64_t* table,
                  size_t table_bits);
void bf2_poly_reduce(uint8_t* dst, const uint8_t* src, size_t src_bits, uint16_t module,
                     size_t deg);

FAEST_END_C_DECL

#endif
