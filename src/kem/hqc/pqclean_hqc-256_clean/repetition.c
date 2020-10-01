#include "parameters.h"
#include "repetition.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
/**
 * @file repetition.c
 * @brief Implementation of repetition codes
 */

static inline int32_t popcount(uint64_t n);

/**
 * @brief Encoding each bit in the message m using the repetition code
 *
 *
 * @param[out] em Pointer to an array that is the code word
 * @param[in] m Pointer to an array that is the message
 */
void PQCLEAN_HQC256_CLEAN_repetition_code_encode(uint64_t *em, const uint64_t *m) {
    uint64_t bit, idx_r, idx2;
    size_t pos_r;

    pos_r = 0;
    for (size_t i = 0; i < VEC_N1_SIZE_64; i++) {
        for (size_t j = 0; j < 64 && pos_r < PARAM_N1N2; j++) {
            bit = (uint64_t) (-(int64_t) ((m[i] >> j) & 1));
            idx_r = (pos_r & 0x3f);
            idx2 = 41 - idx_r;
            idx2 &= (uint64_t) (-((int64_t)idx2) >> 63);
            em[(pos_r >> 6) + 0] ^= (bit & 0xFFFFFFFFFFFFFFFFUL) << idx_r;
            em[(pos_r >> 6) + 1] ^= (bit & 0xFFFFFFFFFFFFFFFFUL) >> idx2;
            em[(pos_r >> 6) + 2] ^= (bit & 0x3FFFFFUL) >> ((63 - idx_r));
            pos_r += PARAM_N2;
        }
    }
}



/**
 * @brief  Compute the Hamming weight of the 64-bit integer n
 *
 * The Hamming weight is computed using a trick described in
 * Henry S. Warren  : "Hacker's Delight", chap 5., p. 66
 * @param[out] the Hamming weight of n
 * @param[in] a 64-bit integer n
 */
static inline int32_t popcount(uint64_t n) {
    n -= (n >> 1) & 0x5555555555555555UL;
    n = (n & 0x3333333333333333UL) + ((n >> 2) & 0x3333333333333333UL);
    n = (n + (n >> 4)) & 0x0f0f0f0f0f0f0f0fUL;
    return (n * 0x0101010101010101UL) >> 56;
}



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
void PQCLEAN_HQC256_CLEAN_repetition_code_decode(uint64_t *m, const uint64_t *em) {
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
        ones = popcount((em[bn] >> bi) | (cx & mask)) + popcount((mask & cy) + (~mask & cx));
        m[t >> 6] |= (uint64_t) (((((int64_t)PARAM_T - ones) >> 63) & 1) << (t & 63)); // 1 << (t&63) if ones > PARAM_T else 0
        t++;
    }
}
