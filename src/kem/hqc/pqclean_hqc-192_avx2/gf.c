#include "gf.h"
#include "parameters.h"
#include <emmintrin.h>
#include <immintrin.h>
#include <stdint.h>
/**
 * @file gf.c
 * Galois field implementation with multiplication using the pclmulqdq instruction
 */


static uint16_t gf_reduce(uint64_t x, size_t deg_x);
static uint16_t gf_quad(uint64_t a);



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
uint16_t PQCLEAN_HQC192_AVX2_gf_mul(uint16_t a, uint16_t b) {
    __m128i va = _mm_cvtsi32_si128(a);
    __m128i vb = _mm_cvtsi32_si128(b);
    __m128i vab = _mm_clmulepi64_si128(va, vb, 0);
    uint32_t ab = _mm_cvtsi128_si32(vab);

    return gf_reduce(ab, 2 * (PARAM_M - 1));
}



/**
 * Squares an element of GF(2^GF_M).
 * @returns a^2
 * @param[in] a Element of GF(2^GF_M)
 */
uint16_t PQCLEAN_HQC192_AVX2_gf_square(uint16_t a) {
    uint32_t b = a;
    uint32_t s = b & 1;
    for (size_t i = 1; i < PARAM_M; ++i) {
        b <<= 1;
        s ^= b & (1 << 2 * i);
    }

    return gf_reduce(s, 2 * (PARAM_M - 1));
}



/**
 * Computes the 4th power of an element of GF(2^GF_M).
 * @returns a^4
 * @param[in] a Element of GF(2^GF_M)
 */
static uint16_t gf_quad(uint64_t a) {
    uint64_t q = a & 1;
    for (size_t i = 1; i < PARAM_M; ++i) {
        a <<= 3;
        q ^= a & (1ull << 4 * i);
    }

    return gf_reduce(q, 4 * (PARAM_M - 1));
}



/**
 * Computes the inverse of an element of GF(2^10),
 * using a shorter chain of squares and multiplications than fast exponentiation.
 * @returns the inverse of a
 * @param[in] a Element of GF(2^10)
 */
uint16_t PQCLEAN_HQC192_AVX2_gf_inverse(uint16_t a) {
    uint16_t p;
    uint16_t a2;

    a2 = PQCLEAN_HQC192_AVX2_gf_square(a);  // a^2
    a = PQCLEAN_HQC192_AVX2_gf_mul(a2, a);  // a^2.a
    p = gf_quad(a);     // a^8.a^4
    a = PQCLEAN_HQC192_AVX2_gf_mul(p, a);   // a^8.a^4.a^2.a
    p = gf_quad(a);     // a^32.a^16.a^8.a^4
    p = gf_quad(p);     // a^128.a^64.a^32.a^16
    a = PQCLEAN_HQC192_AVX2_gf_mul(p, a);   // a^128.a^64.a^32.a^16.a^8.a^4.a^2.a
    p = gf_quad(a);     // a^512.a^256.a^128.a^64.a^32.a^16.a^8.a^4
    p = PQCLEAN_HQC192_AVX2_gf_mul(p, a2);  // a^-1

    return p;
}



/**
 * Returns i modulo 2^GF_M-1.
 * i must be less than 2*(2^GF_M-1).
 * Therefore, the return value is either i or i-2^GF_M+1.
 * @returns i mod (2^GF_M-1)
 * @param[in] i The integer whose modulo is taken
 */
uint16_t PQCLEAN_HQC192_AVX2_gf_mod(uint16_t i) {
    uint16_t tmp = (uint16_t) (i - PARAM_GF_MUL_ORDER);

    // mask = 0xffff if (i < GF_MUL_ORDER)
    uint16_t mask = -(tmp >> 15);

    return tmp + (mask & PARAM_GF_MUL_ORDER);
}
