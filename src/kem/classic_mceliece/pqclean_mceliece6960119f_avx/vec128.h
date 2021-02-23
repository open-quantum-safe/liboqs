#ifndef PQCLEAN_MCELIECE6960119F_AVX_VEC128_H
#define PQCLEAN_MCELIECE6960119F_AVX_VEC128_H
/*
  This file is for functions related to 128-bit vectors
  including functions for bitsliced field operations
*/


#include "params.h"

#include <immintrin.h>
#include <stdint.h>

typedef __m128i vec128;

// this needs to be a macro, because
// _mm_extract_epi64 requires a literal int argument.
#define PQCLEAN_MCELIECE6960119F_AVX_vec128_extract(a, i) ((uint64_t) _mm_extract_epi64((vec128) (a), (i)))

int PQCLEAN_MCELIECE6960119F_AVX_vec128_testz(vec128 a);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_set1_16b(uint16_t a);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_setzero(void);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_and(vec128 a, vec128 b);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_xor(vec128 a, vec128 b);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_or(vec128 a, vec128 b);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_sll_2x(vec128 a, int s);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_srl_2x(vec128 a, int s);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_set2x(uint64_t a0, uint64_t a1);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_unpack_low(vec128 a, vec128 b);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_unpack_high(vec128 a, vec128 b);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_setbits(uint64_t a);
void PQCLEAN_MCELIECE6960119F_AVX_vec128_copy(vec128 *dest, const vec128 *src);
void PQCLEAN_MCELIECE6960119F_AVX_vec128_add(vec128 *c, const vec128 *a, const vec128 *b);
vec128 PQCLEAN_MCELIECE6960119F_AVX_vec128_or_reduce(const vec128 *a);

extern void PQCLEAN_MCELIECE6960119F_AVX_vec128_mul_asm(vec128 *, vec128 *, const vec128 *, int);

/* bitsliced field multiplications */
void PQCLEAN_MCELIECE6960119F_AVX_vec128_mul(vec128 *h, vec128 *f, const vec128 *g);

#endif
