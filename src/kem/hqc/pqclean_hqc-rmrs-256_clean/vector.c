#include "nistseedexpander.h"
#include "parameters.h"
#include "parsing.h"
#include "randombytes.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>
/**
 * @file vector.c
 * @brief Implementation of vectors sampling and some utilities for the HQC scheme
 */


/**
 * @brief Generates a vector of a given Hamming weight
 *
 * This function generates uniformly at random a binary vector of a Hamming weight equal to the parameter <b>weight</b>. The vector
 * is stored by position.
 * To generate the vector we have to sample uniformly at random values in the interval [0, PARAM_N -1]. Suppose the PARAM_N is equal to \f$ 70853 \f$, to select a position \f$ r\f$ the function works as follow:
 *  1. It makes a call to the seedexpander function to obtain a random number \f$ x\f$ in \f$ [0, 2^{24}[ \f$.
 *  2. Let \f$ t = \lfloor {2^{24} \over 70853} \rfloor \times  70853\f$
 *  3. If \f$ x \geq t\f$, go to 1
 *  4. It return \f$ r = x \mod 70853\f$
 *
 * The parameter \f$ t \f$ is precomputed and it's denoted by UTILS_REJECTION_THRESHOLD (see the file parameters.h).
 *
 * @param[in] v Pointer to an array
 * @param[in] weight Integer that is the Hamming weight
 * @param[in] ctx Pointer to the context of the seed expander
 */
void PQCLEAN_HQCRMRS256_CLEAN_vect_set_random_fixed_weight_by_coordinates(AES_XOF_struct *ctx, uint32_t *v, uint16_t weight) {
    size_t random_bytes_size = 3 * weight;
    uint8_t rand_bytes[3 * PARAM_OMEGA_R] = {0}; // weight is expected to be <= PARAM_OMEGA_R
    uint8_t inc;
    size_t i, j;

    i = 0;
    j = random_bytes_size;
    while (i < weight) {
        do {
            if (j == random_bytes_size) {
                seedexpander(ctx, rand_bytes, random_bytes_size);
                j = 0;
            }

            v[i]  = ((uint32_t) rand_bytes[j++]) << 16;
            v[i] |= ((uint32_t) rand_bytes[j++]) << 8;
            v[i] |= rand_bytes[j++];

        } while (v[i] >= UTILS_REJECTION_THRESHOLD);

        v[i] = v[i] % PARAM_N;

        inc = 1;
        for (size_t k = 0; k < i; k++) {
            if (v[k] == v[i]) {
                inc = 0;
            }
        }
        i += inc;
    }
}



/**
 * @brief Generates a vector of a given Hamming weight
 *
 * This function generates uniformly at random a binary vector of a Hamming weight equal to the parameter <b>weight</b>.
 * To generate the vector we have to sample uniformly at random values in the interval [0, PARAM_N -1]. Suppose the PARAM_N is equal to \f$ 70853 \f$, to select a position \f$ r\f$ the function works as follow:
 *  1. It makes a call to the seedexpander function to obtain a random number \f$ x\f$ in \f$ [0, 2^{24}[ \f$.
 *  2. Let \f$ t = \lfloor {2^{24} \over 70853} \rfloor \times  70853\f$
 *  3. If \f$ x \geq t\f$, go to 1
 *  4. It return \f$ r = x \mod 70853\f$
 *
 * The parameter \f$ t \f$ is precomputed and it's denoted by UTILS_REJECTION_THRESHOLD (see the file parameters.h).
 *
 * @param[in] v Pointer to an array
 * @param[in] weight Integer that is the Hamming weight
 * @param[in] ctx Pointer to the context of the seed expander
 */
void PQCLEAN_HQCRMRS256_CLEAN_vect_set_random_fixed_weight(AES_XOF_struct *ctx, uint64_t *v, uint16_t weight) {
    uint32_t tmp[PARAM_OMEGA_R] = {0};

    PQCLEAN_HQCRMRS256_CLEAN_vect_set_random_fixed_weight_by_coordinates(ctx, tmp, weight);

    for (size_t i = 0; i < weight; ++i) {
        int32_t index = tmp[i] / 64;
        int32_t pos = tmp[i] % 64;
        v[index] |= ((uint64_t) 1) << pos;
    }
}



/**
 * @brief Generates a random vector of dimension <b>PARAM_N</b>
 *
 * This function generates a random binary vector of dimension <b>PARAM_N</b>. It generates a random
 * array of bytes using the seedexpander function, and drop the extra bits using a mask.
 *
 * @param[in] v Pointer to an array
 * @param[in] ctx Pointer to the context of the seed expander
 */
void PQCLEAN_HQCRMRS256_CLEAN_vect_set_random(AES_XOF_struct *ctx, uint64_t *v) {
    uint8_t rand_bytes[VEC_N_SIZE_BYTES] = {0};

    seedexpander(ctx, rand_bytes, VEC_N_SIZE_BYTES);

    PQCLEAN_HQCRMRS256_CLEAN_load8_arr(v, VEC_N_SIZE_64, rand_bytes, VEC_N_SIZE_BYTES);
    v[VEC_N_SIZE_64 - 1] &= RED_MASK;
}



/**
 * @brief Adds two vectors
 *
 * @param[out] o Pointer to an array that is the result
 * @param[in] v1 Pointer to an array that is the first vector
 * @param[in] v2 Pointer to an array that is the second vector
 * @param[in] size Integer that is the size of the vectors
 */
void PQCLEAN_HQCRMRS256_CLEAN_vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        o[i] = v1[i] ^ v2[i];
    }
}



/**
 * @brief Compares two vectors
 *
 * @param[in] v1 Pointer to an array that is first vector
 * @param[in] v2 Pointer to an array that is second vector
 * @param[in] size Integer that is the size of the vectors
 * @returns 0 if the vectors are equals and a negative/psotive value otherwise
 */
uint8_t PQCLEAN_HQCRMRS256_CLEAN_vect_compare(const uint8_t *v1, const uint8_t *v2, uint32_t size) {
    uint64_t r = 0;
    for (size_t i = 0; i < size; i++) {
        r |= v1[i] ^ v2[i];
    }
    r = (~r + 1) >> 63;
    return (uint8_t) r;
}



/**
 * @brief Resize a vector so that it contains <b>size_o</b> bits
 *
 * @param[out] o Pointer to the output vector
 * @param[in] size_o Integer that is the size of the output vector in bits
 * @param[in] v Pointer to the input vector
 * @param[in] size_v Integer that is the size of the input vector in bits
 */
void PQCLEAN_HQCRMRS256_CLEAN_vect_resize(uint64_t *o, uint32_t size_o, const uint64_t *v, uint32_t size_v) {
    if (size_o < size_v) {
        uint64_t mask = 0x7FFFFFFFFFFFFFFF;
        int8_t val = 0;

        if (size_o % 64) {
            val = 64 - (size_o % 64);
        }

        memcpy(o, v, 8 * VEC_N1N2_SIZE_64);

        for (int8_t i = 0; i < val; ++i) {
            o[VEC_N1N2_SIZE_64 - 1] &= (mask >> i);
        }
    } else {
        memcpy(o, v, 8 * CEIL_DIVIDE(size_v, 64));
    }
}
