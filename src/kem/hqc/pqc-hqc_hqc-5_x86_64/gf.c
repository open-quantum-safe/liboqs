/**
 * @file gf.c
 * @brief AVX2 GF(2^8) arithmetic for the field defined by @c PARAM_GF_POLY.
 */

#include "gf.h"
#include <stdint.h>
#include "parameters.h"

static uint16_t gf_reduce(uint16_t x);

/**
 * @brief Generates exp and log lookup tables for the field defined by @c PARAM_GF_POLY and ::gf_alpha.
 *
 * @note   this function is not used in the code; it was used to generate
 *         the lookup table for GF(2^8).
 *
 * The logarithm of 0 is stored as 0 by convention. <br>
 * The last two elements of the exp table are needed by the gf_mul function from gf_lutmul.c
 * (for example if both elements to multiply are zero).
 * @param[out] exp Array of size 2^PARAM_M + 2 receiving the powers of the primitive element
 * @param[out] log Array of size 2^PARAM_M receiving the logarithms of the elements of GF(2^m)
 * @param[in] m Parameter of Galois field GF(2^m)
 */
void gf_generate(uint16_t *exp, uint16_t *log, const int16_t m) {
    uint16_t elt = 1;
    const uint16_t alpha = gf_alpha;

    for (size_t i = 0; i < (1U << m) - 1; ++i) {
        exp[i] = elt;
        log[elt] = i;

        elt = gf_mul(elt, alpha);
    }

    exp[(1 << m) - 1] = 1;
    exp[1 << m] = alpha;
    exp[(1 << m) + 1] = gf_mul(alpha, alpha);
    log[0] = 0;  // by convention
}

/**
 * @brief Feedback bit positions used for modular reduction by @c PARAM_GF_POLY = 0x11B.
 *
 * These values are derived from the binary form of the polynomial:
 *     0x11B = 0b100011011 → bits set at positions: 8, 4, 3, 1, 0
 *
 * To reduce a polynomial modulo this irreducible polynomial:
 * - Bit 8 (the leading term) is handled via shifting: mod = x >> PARAM_M
 * - Bit 0 (constant term) is handled by the initial XOR
 *
 * The remaining set bits at positions 4, 3, and 1 define where the shifted
 * high bits (mod) must be XORed back into the result. These represent the
 * feedback positions used during reduction.
 */
static const uint8_t gf_reduction_taps[] = {4, 3, 1};

/**
 * @brief Reduce a polynomial modulo @c PARAM_GF_POLY in GF(2^8).
 *
 * This function performs modular reduction of a 16-bit polynomial `x`
 * by the irreducible polynomial @c PARAM_GF_POLY = 0x11B
 * (i.e., x⁸ + x⁴ + x³ + x + 1), used in GF(2^8).
 *
 * It assumes the input polynomial has degree ≤ 14 and uses a fixed
 * number of reduction steps and fixed feedback tap positions
 * ({4, 3, 1}) to produce a result of degree < 8.
 *
 * @param x 16-bit input polynomial to reduce (deg(x) ≤ 14)
 * @return Reduced 8-bit polynomial modulo @c PARAM_GF_POLY (deg(x) < 8)
 */
uint16_t gf_reduce(uint16_t x) {
    uint64_t mod;
    const int reduction_steps = 2;            // For deg(x) = 2 * (PARAM_M - 1) = 14, reduce twice to bring degree < 8
    const size_t gf_reduction_tap_count = 3;  // Number of feedback positions

    for (int i = 0; i < reduction_steps; ++i) {
        mod = x >> PARAM_M;       // Extract upper bits
        x &= (1 << PARAM_M) - 1;  // Keep lower bits
        x ^= mod;                 // Pre-XOR with no shift

        uint16_t z1 = 0;
        for (size_t j = gf_reduction_tap_count; j; --j) {
            uint16_t z2 = gf_reduction_taps[j - 1];
            uint16_t dist = z2 - z1;
            mod <<= dist;
            x ^= mod;
            z1 = z2;
        }
    }

    return x;
}

/**
 * Multiplies two elements of GF(2^8).
 * @returns the product a*b
 * @param[in] a Element of GF(2^8)
 * @param[in] b Element of GF(2^8)
 */
uint16_t gf_mul(uint16_t a, uint16_t b) {
    __m128i va = _mm_cvtsi32_si128(a);
    __m128i vb = _mm_cvtsi32_si128(b);
    __m128i vab = _mm_clmulepi64_si128(va, vb, 0);
    uint32_t ab = _mm_cvtsi128_si32(vab);

    return gf_reduce(ab);
}

/**
 *  Compute 16 products in GF(2^8).
 *  @returns the product (a0b0,a1b1,...,a15b15) , ai,bi in GF(2^8)
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
 * Squares an element of GF(2^8).
 * @returns a^2
 * @param[in] a Element of GF(2^8)
 */
uint16_t gf_square(uint16_t a) {
    uint32_t b = a;
    uint32_t s = b & 1;
    for (size_t i = 1; i < PARAM_M; ++i) {
        b <<= 1;
        s ^= b & (1 << 2 * i);
    }

    return gf_reduce(s);
}

/**
 * Computes the inverse of an element of GF(2^8),
 * using the addition chain 1 2 3 4 7 11 15 30 60 120 127 254
 * @returns the inverse of a
 * @param[in] a Element of GF(2^8)
 */
uint16_t gf_inverse(uint16_t a) {
    uint16_t inv = a;
    uint16_t tmp1, tmp2;

    inv = gf_square(a);       /* a^2 */
    tmp1 = gf_mul(inv, a);    /* a^3 */
    inv = gf_square(inv);     /* a^4 */
    tmp2 = gf_mul(inv, tmp1); /* a^7 */
    tmp1 = gf_mul(inv, tmp2); /* a^11 */
    inv = gf_mul(tmp1, inv);  /* a^15 */
    inv = gf_square(inv);     /* a^30 */
    inv = gf_square(inv);     /* a^60 */
    inv = gf_square(inv);     /* a^120 */
    inv = gf_mul(inv, tmp2);  /* a^127 */
    inv = gf_square(inv);     /* a^254 */
    return inv;
}
