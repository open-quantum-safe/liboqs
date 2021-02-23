/*
  This file is for functions related to 256-bit vectors
  including functions for bitsliced field operations
*/

#include "vec256.h"

extern void PQCLEAN_MCELIECE348864_AVX_vec256_mul_asm(vec256 *, vec256 *, const vec256 *);

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_set1_16b(uint16_t a) {
    return _mm256_set1_epi16(a);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_setzero(void) {
    return  _mm256_setzero_si256();
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_set4x(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) {
    return  _mm256_set_epi64x(a3, a2, a1, a0);
}

int PQCLEAN_MCELIECE348864_AVX_vec256_testz(vec256 a) {
    return _mm256_testz_si256(a, a);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_and(vec256 a, vec256 b) {
    return _mm256_and_si256(a, b);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_xor(vec256 a, vec256 b) {
    return _mm256_xor_si256(a, b);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_or(vec256 a, vec256 b) {
    return _mm256_or_si256(a, b);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_sll_4x(vec256 a, int s) {
    return _mm256_slli_epi64(a, s);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_srl_4x(vec256 a, int s) {
    return _mm256_srli_epi64(a, s);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_unpack_low(vec256 a, vec256 b) {
    return _mm256_permute2x128_si256 (a, b, 0x20);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_unpack_high(vec256 a, vec256 b) {
    return _mm256_permute2x128_si256 (a, b, 0x31);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_unpack_low_2x(vec256 a, vec256 b) {
    return _mm256_unpacklo_epi64 (a, b);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_unpack_high_2x(vec256 a, vec256 b) {
    return _mm256_unpackhi_epi64 (a, b);
}

vec256 PQCLEAN_MCELIECE348864_AVX_vec256_or_reduce(const vec256 *a) {
    int i;
    vec256 ret;

    ret = a[0];
    for (i = 1; i < GFBITS; i++) {
        ret = PQCLEAN_MCELIECE348864_AVX_vec256_or(ret, a[i]);
    }

    return ret;
}

void PQCLEAN_MCELIECE348864_AVX_vec256_copy(vec256 *dest, const vec256 *src) {
    int i;

    for (i = 0; i < GFBITS; i++) {
        dest[i] = src[i];
    }
}


void PQCLEAN_MCELIECE348864_AVX_vec256_mul(vec256 *h, vec256 *f, const vec256 *g) {
    PQCLEAN_MCELIECE348864_AVX_vec256_mul_asm(h, f, g);
}

/* bitsliced field squarings */
void PQCLEAN_MCELIECE348864_AVX_vec256_sq(vec256 *out, const vec256 *in) {
    int i;
    vec256 result[GFBITS];

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
void PQCLEAN_MCELIECE348864_AVX_vec256_inv(vec256 *out, const vec256 *in) {
    vec256 tmp_11[ GFBITS ];
    vec256 tmp_1111[ GFBITS ];

    PQCLEAN_MCELIECE348864_AVX_vec256_copy(out, in);

    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, out);
    PQCLEAN_MCELIECE348864_AVX_vec256_mul(tmp_11, out, in); // ^11

    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, tmp_11);
    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, out);
    PQCLEAN_MCELIECE348864_AVX_vec256_mul(tmp_1111, out, tmp_11); // ^1111

    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, tmp_1111);
    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, out);
    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, out);
    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, out);
    PQCLEAN_MCELIECE348864_AVX_vec256_mul(out, out, tmp_1111); // ^11111111

    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, out);
    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, out);
    PQCLEAN_MCELIECE348864_AVX_vec256_mul(out, out, tmp_11); // ^1111111111
    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, out);
    PQCLEAN_MCELIECE348864_AVX_vec256_mul(out, out, in); // ^11111111111

    PQCLEAN_MCELIECE348864_AVX_vec256_sq(out, out); // ^111111111110
}
