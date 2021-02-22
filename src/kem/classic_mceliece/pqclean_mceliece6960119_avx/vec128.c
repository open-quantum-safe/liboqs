#include "vec128.h"

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_set1_16b(uint16_t a) {
    return _mm_set1_epi16(a);
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_setzero(void) {
    return _mm_setzero_si128();
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_and(vec128 a, vec128 b) {
    return _mm_and_si128(a, b);
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_xor(vec128 a, vec128 b) {
    return _mm_xor_si128(a, b);
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_or(vec128 a, vec128 b) {
    return _mm_or_si128(a, b);
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_sll_2x(vec128 a, int s) {
    return _mm_slli_epi64(a, s);
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_srl_2x(vec128 a, int s) {
    return _mm_srli_epi64(a, s);
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_set2x(uint64_t a0, uint64_t a1) {
    return _mm_set_epi64x(a1, a0);
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_unpack_low(vec128 a, vec128 b) {
    return _mm_unpacklo_epi64(a, b);
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_unpack_high(vec128 a, vec128 b) {
    return _mm_unpackhi_epi64(a, b);
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_setbits(uint64_t a) {
    return _mm_set1_epi64x(-a);
}

void PQCLEAN_MCELIECE6960119_AVX_vec128_copy(vec128 *dest, const vec128 *src) {
    int i;

    for (i = 0; i < GFBITS; i++) {
        dest[i] = src[i];
    }
}

void PQCLEAN_MCELIECE6960119_AVX_vec128_add(vec128 *c, const vec128 *a, const vec128 *b) {
    int i;

    for (i = 0; i < GFBITS; i++) {
        c[i] = PQCLEAN_MCELIECE6960119_AVX_vec128_xor(a[i], b[i]);
    }
}

vec128 PQCLEAN_MCELIECE6960119_AVX_vec128_or_reduce(const vec128 *a) {
    int i;
    vec128 ret;

    ret = a[0];
    for (i = 1; i < GFBITS; i++) {
        ret = PQCLEAN_MCELIECE6960119_AVX_vec128_or(ret, a[i]);
    }

    return ret;
}

/* bitsliced field multiplications */
void PQCLEAN_MCELIECE6960119_AVX_vec128_mul(vec128 *h, vec128 *f, const vec128 *g) {
    PQCLEAN_MCELIECE6960119_AVX_vec128_mul_asm(h, f, g, 16);
}

