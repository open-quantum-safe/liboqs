/**
 * @file tensor.c
 * @brief Implementation of tensor code
 */

#include "bch.h"
#include "parameters.h"
#include "repetition.h"
#include "tensor.h"
#include <stdint.h>


/**
 * @brief Encoding the message m to a code word em using the tensor code
 *
 * First we encode the message using the BCH code, then with the repetition code to obtain
 * a tensor code word.
 *
 * @param[out] em Pointer to an array that is the tensor code word
 * @param[in] m Pointer to an array that is the message
 */
void PQCLEAN_HQC1281CCA2_LEAKTIME_tensor_code_encode(uint8_t *em, const uint8_t *m) {
    uint8_t tmp[VEC_N1_SIZE_BYTES] = {0};

    PQCLEAN_HQC1281CCA2_LEAKTIME_bch_code_encode(tmp, m);
    PQCLEAN_HQC1281CCA2_LEAKTIME_repetition_code_encode(em, tmp);
}



/**
 * @brief Decoding the code word em to a message m using the tensor code
 *
 * @param[out] m Pointer to an array that is the message
 * @param[in] em Pointer to an array that is the code word
 */
void PQCLEAN_HQC1281CCA2_LEAKTIME_tensor_code_decode(uint8_t *m, const uint8_t *em) {
    uint8_t tmp[VEC_N1_SIZE_BYTES] = {0};

    PQCLEAN_HQC1281CCA2_LEAKTIME_repetition_code_decode(tmp, em);
    PQCLEAN_HQC1281CCA2_LEAKTIME_bch_code_decode(m, tmp);
}
