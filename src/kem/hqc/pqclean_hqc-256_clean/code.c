#include "bch.h"
#include "code.h"
#include "parameters.h"
#include "repetition.h"
#include <stdint.h>
#include <string.h>
/**
 * @file code.c
 * @brief Implementation of tensor code
 */



/**
 *
 * @brief Encoding the message m to a code word em using the tensor code
 *
 * First we encode the message using the BCH code, then with the repetition code to obtain
 * a tensor code word.
 *
 * @param[out] em Pointer to an array that is the tensor code word
 * @param[in] m Pointer to an array that is the message
 */
void PQCLEAN_HQC256_CLEAN_code_encode(uint64_t *em, const uint64_t *m) {

    uint64_t tmp[VEC_N1_SIZE_64] = {0};

    PQCLEAN_HQC256_CLEAN_bch_code_encode(tmp, m);
    PQCLEAN_HQC256_CLEAN_repetition_code_encode(em, tmp);

}



/**
 * @brief Decoding the code word em to a message m using the tensor code
 *
 * @param[out] m Pointer to an array that is the message
 * @param[in] em Pointer to an array that is the code word
 */
void PQCLEAN_HQC256_CLEAN_code_decode(uint64_t *m, const uint64_t *em) {

    uint64_t tmp[VEC_N1_SIZE_64] = {0};

    PQCLEAN_HQC256_CLEAN_repetition_code_decode(tmp, em);
    PQCLEAN_HQC256_CLEAN_bch_code_decode(m, tmp);


}
