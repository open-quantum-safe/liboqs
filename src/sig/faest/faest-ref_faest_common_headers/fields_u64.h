/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_FIELDS_U64_H
#define FAEST_FIELDS_U64_H

// Included by fields.h after the common field types and sizes.

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

#define BF128_ALIGN 8
#define BF192_ALIGN 8
#define BF256_ALIGN 8

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
  const uint8_t bytes[BF128_NUM_BYTES] = {(uint8_t)(bit & 1u)};
  bf128_t ret;
  bf128_load(&ret, bytes);
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf128_t bf128_zero(void) {
  static const uint8_t bytes[BF128_NUM_BYTES] = {0};
  bf128_t ret;
  bf128_load(&ret, bytes);
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf128_t bf128_one(void) {
  static const uint8_t bytes[BF128_NUM_BYTES] = {1};
  bf128_t ret;
  bf128_load(&ret, bytes);
  return ret;
}

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

void bf128_mul_bit(bf128_t* dst, const bf128_t* lhs, uint8_t rhs);
// perform bf128_mul_bit for each bit in rhs and store in dst[0], .., dst[7] for a single input
void bf128_mul_bit_8(bf128_t* dst, const bf128_t* lhs, uint8_t rhs);
void bf128_mul(bf128_t* dst, const bf128_t* lhs, const bf128_t* rhs);
void bf128_square(bf128_t* dst, const bf128_t* lhs);
void bf128_mul_inplace(bf128_t* lhs, const bf128_t* rhs);
void bf128_mul_64_inplace(bf128_t* lhs, bf64_t rhs);

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
  const uint8_t bytes[BF192_NUM_BYTES] = {(uint8_t)(bit & 1u)};
  bf192_t ret;
  bf192_load(&ret, bytes);
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf192_t bf192_zero(void) {
  static const uint8_t bytes[BF192_NUM_BYTES] = {0};
  bf192_t ret;
  bf192_load(&ret, bytes);
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf192_t bf192_one(void) {
  static const uint8_t bytes[BF192_NUM_BYTES] = {1};
  bf192_t ret;
  bf192_load(&ret, bytes);
  return ret;
}

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

void bf192_mul_bit(bf192_t* dst, const bf192_t* lhs, uint8_t rhs);
// perform bf192_mul_bit for each bit in rhs and store in dst[0], .., dst[7] for a single input
void bf192_mul_bit_8(bf192_t* dst, const bf192_t* lhs, uint8_t rhs);
void bf192_mul(bf192_t* dst, const bf192_t* lhs, const bf192_t* rhs);
void bf192_square(bf192_t* dst, const bf192_t* lhs);
void bf192_mul_inplace(bf192_t* lhs, const bf192_t* rhs);
void bf192_mul_64_inplace(bf192_t* lhs, bf64_t rhs);

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
  const uint8_t bytes[BF256_NUM_BYTES] = {(uint8_t)(bit & 1u)};
  bf256_t ret;
  bf256_load(&ret, bytes);
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf256_t bf256_zero(void) {
  static const uint8_t bytes[BF256_NUM_BYTES] = {0};
  bf256_t ret;
  bf256_load(&ret, bytes);
  return ret;
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf256_t bf256_one(void) {
  static const uint8_t bytes[BF256_NUM_BYTES] = {1};
  bf256_t ret;
  bf256_load(&ret, bytes);
  return ret;
}

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

void bf256_mul_bit(bf256_t* dst, const bf256_t* lhs, uint8_t rhs);
// perform bf256_mul_bit for each bit in rhs and store in dst[0], .., dst[7] for a single input
void bf256_mul_bit_8(bf256_t* dst, const bf256_t* lhs, uint8_t rhs);
void bf256_mul(bf256_t* dst, const bf256_t* lhs, const bf256_t* rhs);
void bf256_square(bf256_t* dst, const bf256_t* lhs);
void bf256_mul_inplace(bf256_t* lhs, const bf256_t* rhs);
void bf256_mul_64_inplace(bf256_t* lhs, bf64_t rhs);

// GF(2^384) implementation

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf384_load(bf384_t* dst, const uint8_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF384_NUM_BYTES / sizeof(uint64_t); ++i, src += sizeof(uint64_t)) {
    memcpy(&BF_VALUE(*dst, i), src, sizeof(uint64_t));
    BF_VALUE(*dst, i) = le64toh(BF_VALUE(*dst, i));
  }
#else
  memcpy(dst, src, BF384_NUM_BYTES);
#endif
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf384_store(uint8_t* dst,
                                                                  const bf384_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF384_NUM_BYTES / sizeof(uint64_t); ++i, dst += sizeof(uint64_t)) {
    uint64_t tmp = htole64(BF_VALUE(*src, i));
    memcpy(dst, &tmp, sizeof(tmp));
  }
#else
  memcpy(dst, src, BF384_NUM_BYTES);
#endif
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf384_t bf384_zero(void) {
  static const uint8_t bytes[BF384_NUM_BYTES] = {0};
  bf384_t ret;
  bf384_load(&ret, bytes);
  return ret;
}

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

// GF(2^576) implementation

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf576_load(bf576_t* dst, const uint8_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF576_NUM_BYTES / sizeof(uint64_t); ++i, src += sizeof(uint64_t)) {
    memcpy(&BF_VALUE(*dst, i), src, sizeof(uint64_t));
    BF_VALUE(*dst, i) = le64toh(BF_VALUE(*dst, i));
  }
#else
  memcpy(dst, src, BF576_NUM_BYTES);
#endif
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf576_store(uint8_t* dst,
                                                                  const bf576_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF576_NUM_BYTES / sizeof(uint64_t); ++i, dst += sizeof(uint64_t)) {
    uint64_t tmp = htole64(BF_VALUE(*src, i));
    memcpy(dst, &tmp, sizeof(tmp));
  }
#else
  memcpy(dst, src, BF576_NUM_BYTES);
#endif
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf576_t bf576_zero(void) {
  static const uint8_t bytes[BF576_NUM_BYTES] = {0};
  bf576_t ret;
  bf576_load(&ret, bytes);
  return ret;
}

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

// GF(2^768) implementation

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf768_load(bf768_t* dst, const uint8_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF768_NUM_BYTES / sizeof(uint64_t); ++i, src += sizeof(uint64_t)) {
    memcpy(&BF_VALUE(*dst, i), src, sizeof(uint64_t));
    BF_VALUE(*dst, i) = le64toh(BF_VALUE(*dst, i));
  }
#else
  memcpy(dst, src, BF768_NUM_BYTES);
#endif
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void bf768_store(uint8_t* dst,
                                                                  const bf768_t* src) {
#if defined(FAEST_IS_BIG_ENDIAN)
  for (unsigned int i = 0; i != BF768_NUM_BYTES / sizeof(uint64_t); ++i, dst += sizeof(uint64_t)) {
    uint64_t tmp = htole64(BF_VALUE(*src, i));
    memcpy(dst, &tmp, sizeof(tmp));
  }
#else
  memcpy(dst, src, BF768_NUM_BYTES);
#endif
}

ATTR_CONST ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline bf768_t bf768_zero(void) {
  static const uint8_t bytes[BF768_NUM_BYTES] = {0};
  bf768_t ret;
  bf768_load(&ret, bytes);
  return ret;
}

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
