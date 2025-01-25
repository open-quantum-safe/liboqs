/**
 * @file gf.c
 * @brief Galois field implementation with multiplication using the pclmulqdq instruction
 */

#include "gf.h"
#include "parameters.h"
#include <stdint.h>

static uint16_t gf_reduce(uint64_t x, size_t deg_x);


/**
 * @brief Generates exp and log lookup tables of GF(2^m).
 *
 * The logarithm of 0 is defined as 2^PARAM_M by convention. <br>
 * The last two elements of the exp table are needed by the gf_mul function from gf_lutmul.c
 * (for example if both elements to multiply are zero).
 * @param[out] exp Array of size 2^PARAM_M + 2 receiving the powers of the primitive element
 * @param[out] log Array of size 2^PARAM_M receiving the logarithms of the elements of GF(2^m)
 * @param[in] m Parameter of Galois field GF(2^m)
 */
void gf_generate(uint16_t *exp, uint16_t *log, const int16_t m) {
    uint16_t elt = 1;
    uint16_t alpha = 2; // primitive element of GF(2^PARAM_M)
    uint16_t gf_poly = PARAM_GF_POLY;

    for (size_t i = 0 ; i < (1U << m) - 1 ; ++i){
        exp[i] = elt;
        log[elt] = i;

        elt *= alpha;
        if(elt >= 1 << m)
            elt ^= gf_poly;
    }

    exp[(1 << m) - 1] = 1;
    exp[1 << m] = 2;
    exp[(1 << m) + 1] = 4;
    log[0] = 0; // by convention
}



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
uint16_t gf_mul(uint16_t a, uint16_t b) {
    __m128i va = _mm_cvtsi32_si128(a);
    __m128i vb = _mm_cvtsi32_si128(b);
    __m128i vab = _mm_clmulepi64_si128(va, vb, 0);
    uint32_t ab = _mm_cvtsi128_si32(vab);

    return gf_reduce(ab, 2*(PARAM_M-1));
}



/**
 *  Compute 16 products in GF(2^GF_M).
 *  @returns the product (a0b0,a1b1,...,a15b15) , ai,bi in GF(2^GF_M)
 *  @param[in] a 256-bit register where a0,..,a15 are stored as 16 bit integers
 *  @param[in] b 256-bit register where b0,..,b15 are stored as 16 bit integer
 * 
 */
__m256i gf_mul_vect(__m256i a, __m256i b) {
    __m128i al = _mm256_extractf128_si256(a, 0);
    __m128i ah = _mm256_extractf128_si256(a, 1);
    __m128i bl = _mm256_extractf128_si256(b, 0);
    __m128i bh = _mm256_extractf128_si256(b, 1);

    __m128i abl0 = _mm_clmulepi64_si128(al & maskl, bl & maskl, 0x0);
    abl0 &= middlemaskl;
    abl0 ^= (_mm_clmulepi64_si128(al & maskh, bl & maskh, 0x0) & middlemaskh);

    __m128i abh0 = _mm_clmulepi64_si128(al & maskl, bl & maskl, 0x11);
    abh0 &= middlemaskl;
    abh0 ^= (_mm_clmulepi64_si128(al & maskh, bl & maskh, 0x11) & middlemaskh);

    abl0 = _mm_shuffle_epi8(abl0, indexl);
    abl0 ^= _mm_shuffle_epi8(abh0, indexh);

    __m128i abl1 = _mm_clmulepi64_si128(ah & maskl, bh & maskl, 0x0);
    abl1 &= middlemaskl;
    abl1 ^= (_mm_clmulepi64_si128(ah & maskh, bh & maskh, 0x0) & middlemaskh);

    __m128i abh1 = _mm_clmulepi64_si128(ah & maskl, bh & maskl, 0x11);
    abh1 &= middlemaskl;
    abh1 ^= (_mm_clmulepi64_si128(ah & maskh, bh & maskh, 0x11) & middlemaskh);

    abl1 = _mm_shuffle_epi8(abl1, indexl);
    abl1 ^= _mm_shuffle_epi8(abh1, indexh);

    __m256i ret = _mm256_set_m128i(abl1, abl0);

    __m256i aux = mr0;

    for (int32_t i = 0; i < 7; i++) {
        ret ^= red[i] & _mm256_cmpeq_epi16((ret & aux), aux);
        aux = aux << 1;
    }

    ret &= lastMask;
    return ret;
}



/**
 * Squares an element of GF(2^GF_M).
 * @returns a^2
 * @param[in] a Element of GF(2^GF_M)
 */
uint16_t gf_square(uint16_t a) {
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
uint16_t gf_inverse(uint16_t a) {
    uint16_t inv = a;
    uint16_t tmp1, tmp2;

    inv = gf_square(a); /* a^2 */
    tmp1 = gf_mul(inv, a); /* a^3 */
    inv = gf_square(inv); /* a^4 */
    tmp2 = gf_mul(inv, tmp1); /* a^7 */
    tmp1 = gf_mul(inv, tmp2); /* a^11 */
    inv = gf_mul(tmp1, inv); /* a^15 */
    inv = gf_square(inv); /* a^30 */
    inv = gf_square(inv); /* a^60 */
    inv = gf_square(inv); /* a^120 */
    inv = gf_mul(inv, tmp2); /* a^127 */
    inv = gf_square(inv); /* a^254 */
    return inv;
}



/**
 * Returns i modulo 2^GF_M-1.
 * i must be less than 2*(2^GF_M-1).
 * Therefore, the return value is either i or i-2^GF_M+1.
 * @returns i mod (2^GF_M-1)
 * @param[in] i The integer whose modulo is taken
 */
uint16_t gf_mod(uint16_t i) {
    uint16_t tmp = i - PARAM_GF_MUL_ORDER;

    // mask = 0xffff if (i < GF_MUL_ORDER)
    int16_t mask = -(tmp >> 15);

    return tmp + (mask & PARAM_GF_MUL_ORDER);
}
