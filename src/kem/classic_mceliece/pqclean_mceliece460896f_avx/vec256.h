#ifndef PQCLEAN_MCELIECE460896F_AVX_VEC256_H
#define PQCLEAN_MCELIECE460896F_AVX_VEC256_H
/*
  This file is for functions related to 256-bit vectors
  including functions for bitsliced field operations
*/


#include "vec128.h"

#include <immintrin.h>

typedef __m256i vec256;

vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_set1_16b(uint16_t a);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_setzero(void);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_set4x(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3);

// Extract requires a literal argument so need to be macros
#define PQCLEAN_MCELIECE460896F_AVX_vec256_extract2x(a,i) ((vec128) _mm256_extractf128_si256((vec256) (a),(i)))
#define PQCLEAN_MCELIECE460896F_AVX_vec256_extract(a,i) ((uint64_t) _mm256_extract_epi64((vec256) (a),(i)))

int PQCLEAN_MCELIECE460896F_AVX_vec256_testz(vec256 a);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_and(vec256 a, vec256 b);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_xor(vec256 a, vec256 b);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_or(vec256 a, vec256 b);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_sll_4x(vec256 a, int s);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_srl_4x(vec256 a, int s);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_unpack_low(vec256 a, vec256 b);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_unpack_high(vec256 a, vec256 b);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_unpack_low_2x(vec256 a, vec256 b);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_unpack_high_2x(vec256 a, vec256 b);
vec256 PQCLEAN_MCELIECE460896F_AVX_vec256_or_reduce(const vec256 *a);
void PQCLEAN_MCELIECE460896F_AVX_vec256_copy(vec256 *dest, const vec256 *src);

/* bitsliced field multiplications */
void PQCLEAN_MCELIECE460896F_AVX_vec256_mul(vec256 *h, vec256 *f, const vec256 *g);
void PQCLEAN_MCELIECE460896F_AVX_vec256_sq(vec256 * /*out*/, const vec256 * /*in*/);
void PQCLEAN_MCELIECE460896F_AVX_vec256_inv(vec256 * /*out*/, const vec256 * /*in*/);

extern void PQCLEAN_MCELIECE460896F_AVX_vec256_maa_asm(vec256 *, vec256 *, const vec256 *);
extern void PQCLEAN_MCELIECE460896F_AVX_vec256_ama_asm(vec256 *, vec256 *, const vec256 *);

#endif

