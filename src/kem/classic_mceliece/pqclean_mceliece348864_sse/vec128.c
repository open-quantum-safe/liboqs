/*
  This file is for functions related to 128-bit vectors
  including functions for bitsliced field operations
*/

#include "vec128.h"

#include "params.h"

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_set1_16b(uint16_t a) {
    return _mm_set1_epi16(a);
}

int PQCLEAN_MCELIECE348864_SSE_vec128_testz(vec128 a) {
    return _mm_testz_si128(a, a);
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_setzero(void) {
    return _mm_setzero_si128();
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_and(vec128 a, vec128 b) {
    return _mm_and_si128(a, b);
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_xor(vec128 a, vec128 b) {
    return _mm_xor_si128(a, b);
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_or(vec128 a, vec128 b) {
    return _mm_or_si128(a, b);
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_sll_2x(vec128 a, int s) {
    return _mm_slli_epi64(a, s);
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_srl_2x(vec128 a, int s) {
    return _mm_srli_epi64(a, s);
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_set2x(uint64_t a0, uint64_t a1) {
    return _mm_set_epi64x(a1, a0);
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_unpack_low(vec128 a, vec128 b) {
    return _mm_unpacklo_epi64(a, b);
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_unpack_high(vec128 a, vec128 b) {
    return _mm_unpackhi_epi64(a, b);
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_setbits(uint64_t a) {
    return _mm_set1_epi64x(-a);
}

void PQCLEAN_MCELIECE348864_SSE_vec128_copy(vec128 *dest, const vec128 *src) {
    int i;

    for (i = 0; i < GFBITS; i++) {
        dest[i] = src[i];
    }
}

void PQCLEAN_MCELIECE348864_SSE_vec128_add(vec128 *c, const vec128 *a, const vec128 *b) {
    int i;

    for (i = 0; i < GFBITS; i++) {
        c[i] = PQCLEAN_MCELIECE348864_SSE_vec128_xor(a[i], b[i]);
    }
}

vec128 PQCLEAN_MCELIECE348864_SSE_vec128_or_reduce(const vec128 *a) {
    int i;
    vec128 ret;

    ret = a[0];
    for (i = 1; i < GFBITS; i++) {
        ret = PQCLEAN_MCELIECE348864_SSE_vec128_or(ret, a[i]);
    }

    return ret;
}

/* bitsliced field multiplications */
void PQCLEAN_MCELIECE348864_SSE_vec128_mul(vec128 *h, vec128 *f, const vec128 *g) {
    PQCLEAN_MCELIECE348864_SSE_vec128_mul_asm(h, f, g, 16);
}

/* bitsliced field squarings */
void PQCLEAN_MCELIECE348864_SSE_vec128_sq(vec128 *out, const vec128 *in) {
    int i;
    vec128 result[GFBITS];

    result[0] = in[0] ^ in[6];
    result[1] = in[11];
    result[2] = in[1] ^ in[7];
    result[3] = in[6];
    result[4] = in[2] ^ in[11] ^ in[8];
    result[5] = in[7];
    result[6] = in[3] ^ in[9];
    result[7] = in[8];
    result[8] = in[4] ^ in[10];
    result[9] = in[9];
    result[10] = in[5] ^ in[11];
    result[11] = in[10];

    for (i = 0; i < GFBITS; i++) {
        out[i] = result[i];
    }
}

/* bitsliced field inverses */
void PQCLEAN_MCELIECE348864_SSE_vec128_inv(vec128 *out, const vec128 *in) {
    vec128 tmp_11[ GFBITS ];
    vec128 tmp_1111[ GFBITS ];

    PQCLEAN_MCELIECE348864_SSE_vec128_copy(out, in);

    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, out);
    PQCLEAN_MCELIECE348864_SSE_vec128_mul(tmp_11, out, in); // 11

    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, tmp_11);
    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, out);
    PQCLEAN_MCELIECE348864_SSE_vec128_mul(tmp_1111, out, tmp_11); // 1111

    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, tmp_1111);
    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, out);
    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, out);
    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, out);
    PQCLEAN_MCELIECE348864_SSE_vec128_mul(out, out, tmp_1111); // 11111111

    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, out);
    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, out);
    PQCLEAN_MCELIECE348864_SSE_vec128_mul(out, out, tmp_11); // 1111111111

    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, out);
    PQCLEAN_MCELIECE348864_SSE_vec128_mul(out, out, in); // 11111111111

    PQCLEAN_MCELIECE348864_SSE_vec128_sq(out, out); // 111111111110
}

