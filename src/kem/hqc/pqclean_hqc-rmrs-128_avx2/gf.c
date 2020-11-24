#include "gf.h"
#include "parameters.h"
#include <stdint.h>
/**
 * @file gf.c
 * Galois field implementation with multiplication using the pclmulqdq instruction
 */


static uint16_t gf_reduce(uint64_t x, size_t deg_x);



/**
 * Reduces polynomial x modulo primitive polynomial GF_POLY.
 * @returns x mod GF_POLY
 * @param[in] x Polynomial of degree less than 64
 * @param[in] deg_x The degree of polynomial x
 */
static uint16_t gf_reduce(uint64_t x, size_t deg_x) {
    uint16_t z1, z2, rmdr, dist;
    uint64_t mod;
    size_t steps, i, j;

    // Deduce the number of steps of reduction
    steps = CEIL_DIVIDE(deg_x - (PARAM_M - 1), PARAM_GF_POLY_M2);

    // Reduce
    for (i = 0; i < steps; ++i) {
        mod = x >> PARAM_M;
        x &= (1 << PARAM_M) - 1;
        x ^= mod;

        z1 = 0;
        rmdr = PARAM_GF_POLY ^ 1;
        for (j = PARAM_GF_POLY_WT - 2; j; --j) {
            z2 = __tzcnt_u16(rmdr);
            dist = (uint16_t) (z2 - z1);
            mod <<= dist;
            x ^= mod;
            rmdr ^= 1 << z2;
            z1 = z2;
        }
    }

    return x;
}



/**
 * Multiplies two elements of GF(2^GF_M).
 * @returns the product a*b
 * @param[in] a Element of GF(2^GF_M)
 * @param[in] b Element of GF(2^GF_M)
 */
uint16_t PQCLEAN_HQCRMRS128_AVX2_gf_mul(uint16_t a, uint16_t b) {
    __m128i va = _mm_cvtsi32_si128(a);
    __m128i vb = _mm_cvtsi32_si128(b);
    __m128i vab = _mm_clmulepi64_si128(va, vb, 0);
    uint32_t ab = _mm_cvtsi128_si32(vab);

    return gf_reduce(ab, 2 * (PARAM_M - 1));
}



/**
 *  Compute 16 products in GF(2^GF_M).
 *  @returns the product (a0b0,a1b1,...,a15b15) , ai,bi in GF(2^GF_M)
 *  @param[in] a 256-bit register where a0,..,a15 are stored as 16 bit integers
 *  @param[in] b 256-bit register where b0,..,b15 are stored as 16 bit integer
 *
 */
__m256i PQCLEAN_HQCRMRS128_AVX2_gf_mul_vect(__m256i a, __m256i b) {
    __m128i al = _mm256_extractf128_si256(a, 0);
    __m128i ah = _mm256_extractf128_si256(a, 1);
    __m128i bl = _mm256_extractf128_si256(b, 0);
    __m128i bh = _mm256_extractf128_si256(b, 1);

    __m128i abl0 = _mm_clmulepi64_si128(al & CONST128_MASKL, bl & CONST128_MASKL, 0x0);
    abl0 &= CONST128_MIDDLEMASKL;
    abl0 ^= (_mm_clmulepi64_si128(al & CONST128_MASKH, bl & CONST128_MASKH, 0x0) & CONST128_MIDDLEMASKH);

    __m128i abh0 = _mm_clmulepi64_si128(al & CONST128_MASKL, bl & CONST128_MASKL, 0x11);
    abh0 &= CONST128_MIDDLEMASKL;
    abh0 ^= (_mm_clmulepi64_si128(al & CONST128_MASKH, bl & CONST128_MASKH, 0x11) & CONST128_MIDDLEMASKH);

    abl0 = _mm_shuffle_epi8(abl0, CONST128_INDEXL);
    abl0 ^= _mm_shuffle_epi8(abh0, CONST128_INDEXH);

    __m128i abl1 = _mm_clmulepi64_si128(ah & CONST128_MASKL, bh & CONST128_MASKL, 0x0);
    abl1 &= CONST128_MIDDLEMASKL;
    abl1 ^= (_mm_clmulepi64_si128(ah & CONST128_MASKH, bh & CONST128_MASKH, 0x0) & CONST128_MIDDLEMASKH);

    __m128i abh1 = _mm_clmulepi64_si128(ah & CONST128_MASKL, bh & CONST128_MASKL, 0x11);
    abh1 &= CONST128_MIDDLEMASKL;
    abh1 ^= (_mm_clmulepi64_si128(ah & CONST128_MASKH, bh & CONST128_MASKH, 0x11) & CONST128_MIDDLEMASKH);

    abl1 = _mm_shuffle_epi8(abl1, CONST128_INDEXL);
    abl1 ^= _mm_shuffle_epi8(abh1, CONST128_INDEXH);

    __m256i ret = _mm256_set_m128i(abl1, abl0);

    __m256i aux = CONST256_MR0;

    for (int32_t i = 0; i < 7; i++) {
        ret ^= red[i] & _mm256_cmpeq_epi16((ret & aux), aux);
        aux = aux << 1;
    }

    ret &= CONST256_LASTMASK;
    return ret;
}



/**
 * Squares an element of GF(2^GF_M).
 * @returns a^2
 * @param[in] a Element of GF(2^GF_M)
 */
uint16_t PQCLEAN_HQCRMRS128_AVX2_gf_square(uint16_t a) {
    uint32_t b = a;
    uint32_t s = b & 1;
    for (size_t i = 1; i < PARAM_M; ++i) {
        b <<= 1;
        s ^= b & (1 << 2 * i);
    }

    return gf_reduce(s, 2 * (PARAM_M - 1));
}



/**
 * Computes the inverse of an element of GF(2^8),
 * using the addition chain 1 2 3 4 7 11 15 30 60 120 127 254
 * @returns the inverse of a
 * @param[in] a Element of GF(2^GF_M)
 */
uint16_t PQCLEAN_HQCRMRS128_AVX2_gf_inverse(uint16_t a) {
    uint16_t inv = a;
    uint16_t tmp1, tmp2;

    inv = PQCLEAN_HQCRMRS128_AVX2_gf_square(a); /* a^2 */
    tmp1 = PQCLEAN_HQCRMRS128_AVX2_gf_mul(inv, a); /* a^3 */
    inv = PQCLEAN_HQCRMRS128_AVX2_gf_square(inv); /* a^4 */
    tmp2 = PQCLEAN_HQCRMRS128_AVX2_gf_mul(inv, tmp1); /* a^7 */
    tmp1 = PQCLEAN_HQCRMRS128_AVX2_gf_mul(inv, tmp2); /* a^11 */
    inv = PQCLEAN_HQCRMRS128_AVX2_gf_mul(tmp1, inv); /* a^15 */
    inv = PQCLEAN_HQCRMRS128_AVX2_gf_square(inv); /* a^30 */
    inv = PQCLEAN_HQCRMRS128_AVX2_gf_square(inv); /* a^60 */
    inv = PQCLEAN_HQCRMRS128_AVX2_gf_square(inv); /* a^120 */
    inv = PQCLEAN_HQCRMRS128_AVX2_gf_mul(inv, tmp2); /* a^127 */
    inv = PQCLEAN_HQCRMRS128_AVX2_gf_square(inv); /* a^254 */
    return inv;
}



/**
 * Returns i modulo 2^GF_M-1.
 * i must be less than 2*(2^GF_M-1).
 * Therefore, the return value is either i or i-2^GF_M+1.
 * @returns i mod (2^GF_M-1)
 * @param[in] i The integer whose modulo is taken
 */
uint16_t PQCLEAN_HQCRMRS128_AVX2_gf_mod(uint16_t i) {
    uint16_t tmp = (uint16_t) (i - PARAM_GF_MUL_ORDER);

    // mask = 0xffff if (i < GF_MUL_ORDER)
    uint16_t mask = -(tmp >> 15);

    return tmp + (mask & PARAM_GF_MUL_ORDER);
}
