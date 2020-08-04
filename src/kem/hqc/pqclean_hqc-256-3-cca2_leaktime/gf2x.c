/**
 * \file gf2x.c
 * \brief Implementation of multiplication of two polynomials
 */

#include "gf2x.h"
#include "parameters.h"
#include "util.h"

#include <stdint.h>
#include <string.h>

#define WORD_TYPE                 uint64_t
#define WORD_TYPE_BITS            (sizeof(WORD_TYPE) * 8)
#define UTILS_VECTOR_ARRAY_SIZE   CEIL_DIVIDE(PARAM_N, WORD_TYPE_BITS)

static int vect_mul_precompute_rows(WORD_TYPE *o, const WORD_TYPE *v);


/**
 * @brief A subroutine used in the function sparse_dense_mul()
 *
 * @param[out] o Pointer to an array
 * @param[in] v Pointer to an array
 * @return 0 if precomputation is successful, -1 otherwise
 */
static int vect_mul_precompute_rows(WORD_TYPE *o, const WORD_TYPE *v) {
    int8_t var;
    for (size_t i = 0; i < PARAM_N; ++i) {
        var = 0;

        // All the bits that we need are in the same block
        if (((i % WORD_TYPE_BITS) == 0) && (i != PARAM_N - (PARAM_N % WORD_TYPE_BITS))) {
            var = 1;
        }

        // Cases where the bits are in before the last block, the last block and the first block
        if (i > PARAM_N - WORD_TYPE_BITS) {
            if (i >= PARAM_N - (PARAM_N % WORD_TYPE_BITS)) {
                var = 2;
            } else {
                var = 3;
            }
        }

        switch (var) {
        case 0:
            // Take bits in the last block and the first one
            o[i] = 0;
            o[i] += v[i / WORD_TYPE_BITS] >> (i % WORD_TYPE_BITS);
            o[i] += v[(i / WORD_TYPE_BITS) + 1] << (WORD_TYPE_BITS - (i % WORD_TYPE_BITS));
            break;

        case 1:
            o[i] = v[i / WORD_TYPE_BITS];
            break;

        case 2:
            o[i] = 0;
            o[i] += v[i / WORD_TYPE_BITS] >> (i % WORD_TYPE_BITS);
            o[i] += v[0] << ((PARAM_N - i) % WORD_TYPE_BITS);
            break;

        case 3:
            o[i] = 0;
            o[i] += v[i / WORD_TYPE_BITS] >> (i % WORD_TYPE_BITS);
            o[i] += v[(i / WORD_TYPE_BITS) + 1] << (WORD_TYPE_BITS - (i % WORD_TYPE_BITS));
            o[i] += v[0] << ((WORD_TYPE_BITS - i + (PARAM_N % WORD_TYPE_BITS)) % WORD_TYPE_BITS);
            break;

        default:
            return -1;
        }
    }

    return 0;
}



/**
 * @brief Multiplies two vectors
 *
 * This function multiplies two vectors: a sparse vector of Hamming weight equal to <b>weight</b> and a dense (random) vector.
 * The vector <b>a1</b> is the sparse vector and <b>a2</b> is the dense vector.
 * We notice that the idea is explained using vector of 32 bits elements instead of 64 (the algorithm works in booth cases).
 *
 * @param[out] o Pointer to a vector that is the result of the multiplication
 * @param[in] a1 Pointer to the sparse vector stored by position
 * @param[in] a2 Pointer to the dense vector
 * @param[in] weight Integer that is the weight of the sparse vector
 */
void PQCLEAN_HQC2563CCA2_LEAKTIME_vect_mul(uint8_t *o, const uint32_t *a1, const uint8_t *a2, uint16_t weight) {
    WORD_TYPE v1[UTILS_VECTOR_ARRAY_SIZE] = {0};
    WORD_TYPE res[UTILS_VECTOR_ARRAY_SIZE] = {0};
    WORD_TYPE precomputation_array [PARAM_N] = {0};
    WORD_TYPE row [UTILS_VECTOR_ARRAY_SIZE] = {0};
    uint32_t index;

    PQCLEAN_HQC2563CCA2_LEAKTIME_load8_arr(v1, UTILS_VECTOR_ARRAY_SIZE, a2, VEC_N_SIZE_BYTES);
    vect_mul_precompute_rows(precomputation_array, v1);

    for (size_t i = 0; i < weight; ++i) {
        int32_t k = UTILS_VECTOR_ARRAY_SIZE;

        for (size_t j = 0; j < UTILS_VECTOR_ARRAY_SIZE - 1; ++j) {
            index = WORD_TYPE_BITS * (uint32_t)j - a1[i];
            if (index > PARAM_N) {
                index += PARAM_N;
            }
            row[j] = precomputation_array[index];
        }

        index = WORD_TYPE_BITS * (UTILS_VECTOR_ARRAY_SIZE - 1) - a1[i];
        row[UTILS_VECTOR_ARRAY_SIZE - 1] = precomputation_array[(index < PARAM_N ? index : index + PARAM_N)] & BITMASK(PARAM_N, WORD_TYPE_BITS);

        while (k--) {
            res[k] ^= row[k];
        }
    }

    PQCLEAN_HQC2563CCA2_LEAKTIME_store8_arr(o, VEC_N_SIZE_BYTES, res, UTILS_VECTOR_ARRAY_SIZE);
}
