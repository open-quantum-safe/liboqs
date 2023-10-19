#include "gf.h"
#include "parameters.h"
#include <stddef.h>
#include <stdint.h>
/**
 * @file gf.c
 * @brief Galois field implementation
 */

/**
 * @brief Computes the number of trailing zero bits.
 *
 * @returns The number of trailing zero bits in a.
 * @param[in] a An operand
 */
static uint16_t trailing_zero_bits_count(uint16_t a) {
    uint16_t tmp = 0;
    uint16_t mask = 0xFFFF;
    for (size_t i = 0; i < 14; ++i) {
        tmp += ((1 - ((a >> i) & 0x0001)) & mask);
        mask &= - (1 - ((a >> i) & 0x0001));
    }
    return tmp;
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

    // Deduce the number of steps of reduction
    size_t steps = CEIL_DIVIDE(deg_x - (PARAM_M - 1), PARAM_GF_POLY_M2);

    // Reduce
    for (size_t i = 0; i < steps; ++i) {
        mod = x >> PARAM_M;
        x &= (1 << PARAM_M) - 1;
        x ^= mod;

        z1 = 0;
        rmdr = PARAM_GF_POLY ^ 1;
        for (size_t j = PARAM_GF_POLY_WT - 2; j; --j) {
            z2 = trailing_zero_bits_count(rmdr);
            dist = z2 - z1;
            mod <<= dist;
            x ^= mod;
            rmdr ^= 1 << z2;
            z1 = z2;
        }
    }

    return (uint16_t)x;
}

/**
 * Carryless multiplication of two polynomials a and b.
 *
 * Implementation of the algorithm mul1 in https://hal.inria.fr/inria-00188261v4/document
 * with s = 2 and w = 8
 *
 * @param[out] The polynomial c = a * b
 * @param[in] a The first polynomial
 * @param[in] b The second polynomial
 */
static void gf_carryless_mul(uint8_t c[2], uint8_t a, uint8_t b) {
    uint16_t h = 0, l = 0, g = 0, u[4];
    uint32_t tmp1, tmp2;
    uint16_t mask;
    u[0] = 0;
    u[1] = b & 0x7F;
    u[2] = u[1] << 1;
    u[3] = u[2] ^ u[1];
    tmp1 = a & 3;

    for (size_t i = 0; i < 4; i++) {
        tmp2 = (uint32_t)(tmp1 - i);
        g ^= (u[i] & (uint32_t)(0 - (1 - ((uint32_t)(tmp2 | (0 - tmp2)) >> 31))));
    }

    l = g;
    h = 0;

    for (size_t i = 2; i < 8; i += 2) {
        g = 0;
        tmp1 = (a >> i) & 3;
        for (size_t j = 0; j < 4; ++j) {
            tmp2 = (uint32_t)(tmp1 - j);
            g ^= (u[j] & (uint32_t)(0 - (1 - ((uint32_t)(tmp2 | (0 - tmp2)) >> 31))));
        }

        l ^= g << i;
        h ^= g >> (8 - i);
    }

    mask = (-((b >> 7) & 1));
    l ^= ((a << 7) & mask);
    h ^= ((a >> 1) & mask);

    c[0] = (uint8_t)l;
    c[1] = (uint8_t)h;
}

/**
 * Multiplies two elements of GF(2^GF_M).
 * @returns the product a*b
 * @param[in] a Element of GF(2^GF_M)
 * @param[in] b Element of GF(2^GF_M)
 */
uint16_t PQCLEAN_HQC128_CLEAN_gf_mul(uint16_t a, uint16_t b) {
    uint8_t c[2] = {0};
    gf_carryless_mul(c, (uint8_t) a, (uint8_t) b);
    uint16_t tmp = c[0] ^ (c[1] << 8);
    return gf_reduce(tmp, 2 * (PARAM_M - 1));
}

/**
 * @brief Squares an element of GF(2^PARAM_M).
 * @returns a^2
 * @param[in] a Element of GF(2^PARAM_M)
 */
uint16_t PQCLEAN_HQC128_CLEAN_gf_square(uint16_t a) {
    uint32_t b = a;
    uint32_t s = b & 1;
    for (size_t i = 1; i < PARAM_M; ++i) {
        b <<= 1;
        s ^= b & (1 << 2 * i);
    }

    return gf_reduce(s, 2 * (PARAM_M - 1));
}

/**
 * @brief Computes the inverse of an element of GF(2^PARAM_M),
 * using the addition chain 1 2 3 4 7 11 15 30 60 120 127 254
 * @returns the inverse of a if a != 0 or 0 if a = 0
 * @param[in] a Element of GF(2^PARAM_M)
 */
uint16_t PQCLEAN_HQC128_CLEAN_gf_inverse(uint16_t a) {
    uint16_t inv = a;
    uint16_t tmp1, tmp2;

    inv = PQCLEAN_HQC128_CLEAN_gf_square(a); /* a^2 */
    tmp1 = PQCLEAN_HQC128_CLEAN_gf_mul(inv, a); /* a^3 */
    inv = PQCLEAN_HQC128_CLEAN_gf_square(inv); /* a^4 */
    tmp2 = PQCLEAN_HQC128_CLEAN_gf_mul(inv, tmp1); /* a^7 */
    tmp1 = PQCLEAN_HQC128_CLEAN_gf_mul(inv, tmp2); /* a^11 */
    inv = PQCLEAN_HQC128_CLEAN_gf_mul(tmp1, inv); /* a^15 */
    inv = PQCLEAN_HQC128_CLEAN_gf_square(inv); /* a^30 */
    inv = PQCLEAN_HQC128_CLEAN_gf_square(inv); /* a^60 */
    inv = PQCLEAN_HQC128_CLEAN_gf_square(inv); /* a^120 */
    inv = PQCLEAN_HQC128_CLEAN_gf_mul(inv, tmp2); /* a^127 */
    inv = PQCLEAN_HQC128_CLEAN_gf_square(inv); /* a^254 */
    return inv;
}
