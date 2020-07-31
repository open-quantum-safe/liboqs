/**
 * @file repetition.c
 * @brief Implementation of repetition codes
 */

#include "parameters.h"
#include "repetition.h"
#include <stddef.h>
#include <stdint.h>

static void array_to_rep_codeword(uint8_t *o, const uint8_t *v);


/**
 * @brief Encoding each bit in the message m using the repetition code
 *
 * For reasons of clarity and comprehensibility, we do the encoding by storing the encoded bits in a String (each bit in an a uint8_t),
 * then we parse the obtained string to an compact array using the function array_to_rep_codeword().
 *
 * @param[out] em Pointer to an array that is the code word
 * @param[in] m Pointer to an array that is the message
 */
void PQCLEAN_HQC2561CCA2_LEAKTIME_repetition_code_encode(uint8_t *em, const uint8_t *m) {
    uint8_t tmp[PARAM_N1N2] = {0};
    uint8_t bit = 0;
    uint32_t index;

    for (size_t i = 0; i < (VEC_N1_SIZE_BYTES - 1); ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            bit = (m[i] >> j) & 0x01;
            index = (8 * (uint32_t)i + j) * PARAM_N2;
            for (uint8_t k = 0; k < PARAM_N2; ++k) {
                tmp[index + k] = bit;
            }
        }
    }

    for (uint8_t j = 0; j < (PARAM_N1 % 8); ++j) {
        bit = (m[VEC_N1_SIZE_BYTES - 1] >> j) & 0x01;
        index = (8 * (VEC_N1_SIZE_BYTES - 1) + j) * PARAM_N2;
        for (uint8_t k = 0; k < PARAM_N2; ++k) {
            tmp[index + k] = bit;
        }
    }

    array_to_rep_codeword(em, tmp);
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
void PQCLEAN_HQC2561CCA2_LEAKTIME_repetition_code_decode(uint8_t *m, const uint8_t *em) {
    size_t t = 0; // m index
    uint8_t k = PARAM_N2; // block counter
    uint8_t ones = 0; // number of 1 in the current block

    for (size_t i = 0; i < VEC_N1N2_SIZE_BYTES; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            ones += (em[i] >> j) & 0x01;

            if (--k) {
                continue;
            }

            m[t / 8] |= (ones > PARAM_T) << t % 8;
            ++t;
            k = PARAM_N2;
            ones = 0;
        }
    }
}



/**
 * @brief Parse an array to an compact array
 *
 * @param[out] o Pointer to an array
 * @param[in] v Pointer to an array
 */
static void array_to_rep_codeword(uint8_t *o, const uint8_t *v) {
    for (size_t i = 0; i < (VEC_N1N2_SIZE_BYTES - 1); ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            o[i] |= v[j + 8 * i] << j;
        }
    }

    for (uint8_t j = 0; j < PARAM_N1N2 % 8; ++j) {
        o[VEC_N1N2_SIZE_BYTES - 1] |= (v[j + 8 * (VEC_N1N2_SIZE_BYTES - 1)]) << j;
    }
}
