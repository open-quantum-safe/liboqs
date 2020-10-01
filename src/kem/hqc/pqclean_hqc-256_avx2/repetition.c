#include "parameters.h"
#include "repetition.h"
#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
/**
 * @file repetition.c
 * @brief Implementation of repetition codes
 */



/**
 * @brief Decoding the code words to a message using the repetition code
 *
 * We use a majority decoding. In fact we have that PARAM_N2 = 2 * PARAM_T + 1, thus,
 * if the Hamming weight of the vector is greater than PARAM_T, the code word is decoded
 * to 1 and 0 otherwise.
 *
 * @param[out] m Pointer to an array that is the message
 * @param[in] em Pointer to an array that is the code word
 */
void PQCLEAN_HQC256_AVX2_repetition_code_decode(uint64_t *m, const uint64_t *em) {
    uint32_t t = 0;
    uint32_t b, bn, bi, c, cn, ci;
    uint64_t cx, ones;
    uint64_t cy;
    uint64_t mask;

    for (b = 0; b < PARAM_N1N2 - PARAM_N2 + 1; b += PARAM_N2) {
        bn = b >> 6;
        bi = b & 63;
        c = b + PARAM_N2 - 1;
        cn = c >> 6;
        ci = c & 63;
        cx = em[cn] << (63 - ci);
        mask = (uint64_t) (-((int64_t) (cn ^ (bn + 1))) >> 63); // cn != bn+1
        cy = em[bn + 1];
        ones = _mm_popcnt_u64((em[bn] >> bi) | (cx & mask)) + _mm_popcnt_u64((mask & cy) + (~mask & cx));
        m[t >> 6] |= (uint64_t) (((((int64_t)PARAM_T - ones) >> 63) & 1) << (t & 63)); // 1 << (t&63) if ones > PARAM_T else 0
        t++;
    }
}
