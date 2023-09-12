#ifndef VEC256_H
#define VEC256_H
/*
  This file is for functions related to 256-bit vectors
  including functions for bitsliced field operations
*/

#include "namespace.h"

#define vec256_ama_asm CRYPTO_NAMESPACE(vec256_ama_asm)
#define vec256_inv CRYPTO_NAMESPACE(vec256_inv)
#define vec256_maa_asm CRYPTO_NAMESPACE(vec256_maa_asm)
#define vec256_mul_asm CRYPTO_NAMESPACE(vec256_mul_asm)
#define vec256_sq CRYPTO_NAMESPACE(vec256_sq)

#include "vec128.h"

#include <immintrin.h>

typedef __m256i vec256;

static inline vec256 vec256_set1_16b(uint16_t a) {
    return _mm256_set1_epi16(a);
}

static inline vec256 vec256_set1_32b(uint64_t a) {
    return _mm256_set1_epi32(a);
}

static inline vec256 vec256_setzero(void) {
    return  _mm256_setzero_si256();
}

static inline vec256 vec256_set4x(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) {
    return  _mm256_set_epi64x(a3, a2, a1, a0);
}

#define vec256_extract2x(a,i) ((vec128) _mm256_extractf128_si256((vec256) (a),(i)))
#define vec256_extract(a,i) ((uint64_t) _mm256_extract_epi64((vec256) (a),(i)))

static inline int vec256_testz(vec256 a) {
    return _mm256_testz_si256(a, a);
}

static inline vec256 vec256_and(vec256 a, vec256 b) {
    return _mm256_and_si256(a, b);
}

static inline vec256 vec256_xor(vec256 a, vec256 b) {
    return _mm256_xor_si256(a, b);
}

static inline vec256 vec256_or(vec256 a, vec256 b) {
    return _mm256_or_si256(a, b);
}

#define vec256_sll_4x(a, s) ((vec256) _mm256_slli_epi64((vec256) (a), (s)))
#define vec256_srl_4x(a, s) ((vec256) _mm256_srli_epi64((vec256) (a), (s)))

static inline vec256 vec256_unpack_low(vec256 a, vec256 b) {
    return _mm256_permute2x128_si256 (a, b, 0x20);
}

static inline vec256 vec256_unpack_high(vec256 a, vec256 b) {
    return _mm256_permute2x128_si256 (a, b, 0x31);
}

static inline vec256 vec256_unpack_low_2x(vec256 a, vec256 b) {
    return _mm256_unpacklo_epi64 (a, b);
}

static inline vec256 vec256_unpack_high_2x(vec256 a, vec256 b) {
    return _mm256_unpackhi_epi64 (a, b);
}

static inline vec256 vec256_or_reduce(vec256 *a) {
    int i;
    vec256 ret;

    ret = a[0];
    for (i = 1; i < GFBITS; i++) {
        ret = vec256_or(ret, a[i]);
    }

    return ret;
}

static inline void vec256_copy(vec256 *dest, vec256 *src) {
    int i;

    for (i = 0; i < GFBITS; i++) {
        dest[i] = src[i];
    }
}

extern void vec256_mul_asm(vec256 *, vec256 *, const vec256 *);

/* bitsliced field multiplications */
static inline void vec256_mul(vec256 *h, vec256 *f, const vec256 *g) {
    vec256_mul_asm(h, f, g);
}

void vec256_sq(vec256 *out, vec256 *in);
void vec256_inv(vec256 *out, vec256 *in);

extern void vec256_maa_asm(vec256 *, vec256 *, const vec256 *);
extern void vec256_ama_asm(vec256 *, vec256 *, const vec256 *);

#endif
