#include "gf.h"
#include "parameters.h"
#include <stdint.h>
/**
 * @file gf.c
 * Galois field implementation with multiplication using lookup tables
 */


/**
 * @brief Multiplies nonzero element a by element b
 * @returns the product a*b
 * @param[in] a First element of GF(2^PARAM_M) to multiply (cannot be zero)
 * @param[in] b Second element of GF(2^PARAM_M) to multiply (cannot be zero)
 */
uint16_t PQCLEAN_HQCRMRS128_CLEAN_gf_mul(uint16_t a, uint16_t b) {
    uint16_t mask;
    mask = (uint16_t) (-((int32_t) a) >> 31); // a != 0
    mask &= (uint16_t) (-((int32_t) b) >> 31); // b != 0
    return mask & gf_exp[PQCLEAN_HQCRMRS128_CLEAN_gf_mod(gf_log[a] + gf_log[b])];
}



/**
 * @brief Squares an element of GF(2^PARAM_M)
 * @returns a^2
 * @param[in] a Element of GF(2^PARAM_M)
 */
uint16_t PQCLEAN_HQCRMRS128_CLEAN_gf_square(uint16_t a) {
    int16_t mask = (uint16_t) (-((int32_t) a) >> 31); // a != 0
    return mask & gf_exp[PQCLEAN_HQCRMRS128_CLEAN_gf_mod(2 * gf_log[a])];
}



/**
 * @brief Computes the inverse of an element of GF(2^PARAM_M)
 * @returns the inverse of a
 * @param[in] a Element of GF(2^PARAM_M)
 */
uint16_t PQCLEAN_HQCRMRS128_CLEAN_gf_inverse(uint16_t a) {
    int16_t mask = (uint16_t) (-((int32_t) a) >> 31); // a != 0
    return mask & gf_exp[PARAM_GF_MUL_ORDER - gf_log[a]];
}



/**
 * @brief Returns i modulo 2^PARAM_M-1
 * i must be less than 2*(2^PARAM_M-1).
 * Therefore, the return value is either i or i-2^PARAM_M+1.
 * @returns i mod (2^PARAM_M-1)
 * @param[in] i The integer whose modulo is taken
 */
uint16_t PQCLEAN_HQCRMRS128_CLEAN_gf_mod(uint16_t i) {
    uint16_t tmp = (uint16_t) (i - PARAM_GF_MUL_ORDER);

    // mask = 0xffff if(i < PARAM_GF_MUL_ORDER)
    uint16_t mask = -(tmp >> 15);

    return tmp + (mask & PARAM_GF_MUL_ORDER);
}
