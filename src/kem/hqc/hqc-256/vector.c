/**
 * @file vector.c
 * @brief Implementation of vectors sampling and some utilities for the HQC scheme
 */

#include "shake_prng.h"
#include "parameters.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>
#include <immintrin.h>
#include <stdio.h>

#define SHBIT32 47

static uint64_t v_val[PARAM_OMEGA_R] = {2441790661, 2441833027, 2441875394, 2441917763, 2441960133, 2442002504, 2442044877, 2442087252, 2442129628, 2442172005, 2442214384, 2442256765, 2442299147, 2442341530, 2442383915, 2442426301, 2442468689, 2442511078, 2442553469, 2442595861, 2442638255, 2442680650, 2442723047, 2442765445, 2442807844, 2442850245, 2442892648, 2442935052, 2442977457, 2443019864, 2443062272, 2443104682, 2443147094, 2443189506, 2443231921, 2443274336, 2443316754, 2443359172, 2443401593, 2443444014, 2443486437, 2443528862, 2443571288, 2443613715, 2443656144, 2443698575, 2443741007, 2443783440, 2443825875, 2443868312, 2443910749, 2443953189, 2443995630, 2444038072, 2444080516, 2444122961, 2444165407, 2444207856, 2444250305, 2444292756, 2444335209, 2444377663, 2444420119, 2444462576, 2444505034, 2444547494, 2444589955, 2444632418, 2444674883, 2444717349, 2444759816, 2444802285, 2444844755, 2444887227, 2444929700, 2444972175, 2445014651, 2445057129, 2445099608, 2445142088, 2445184571, 2445227054, 2445269539, 2445312026, 2445354514, 2445397003, 2445439494, 2445481987, 2445524480, 2445566976, 2445609473, 2445651971, 2445694471, 2445736972, 2445779475, 2445821979, 2445864485, 2445906992, 2445949501, 2445992011, 2446034523, 2446077036, 2446119550, 2446162066, 2446204584, 2446247103, 2446289623, 2446332145, 2446374669, 2446417194, 2446459720, 2446502248, 2446544778, 2446587308, 2446629841, 2446672375, 2446714910, 2446757447, 2446799985, 2446842525, 2446885066, 2446927608, 2446970153, 2447012698, 2447055245, 2447097794, 2447140344, 2447182896, 2447225449, 2447268003, 2447310559, 2447353117, 2447395676, 2447438236, 2447480798, 2447523361, 2447565926, 2447608493, 2447651060, 2447693630, 2447736201, 2447778773, 2447821347, 2447863922, 2447906499, 2447949077, 2447991657, 2448034238, 2448076820};

static inline uint16_t barrett_reduce(uint32_t a, uint16_t i);

static inline uint32_t compare_u32(const uint32_t v1, const uint32_t v2);

/**
 * @brief Constant-time Barret reduction
 *
 * @param[in] a An integer to be reduced 
 * @param[in] i An array index
 * @return an integer equal to a % (PARAM_N - i)
 */
static inline uint16_t barrett_reduce(uint32_t a, uint16_t i) {
    uint32_t t;

    t = ((v_val[i] * a + v_val[i]) >> SHBIT32);
    t *= (PARAM_N - i);
    return (uint16_t)(a - t);
}



/**
 * @brief Constant-time comparison of two integers v1 and v2
 *
 * Returns 1 if v1 is equal to v2 and 0 otherwise
 * https://gist.github.com/sneves/10845247
 *
 * @param[in] v1 
 * @param[in] v2 
 */
static inline uint32_t compare_u32(const uint32_t v1, const uint32_t v2) {
    return 1 ^ (((v1 - v2)|(v2 - v1)) >> 31);
}


/**
 * @brief Generates a vector of a given Hamming weight
 *
 * Implementation of Algorithm 5 in https://eprint.iacr.org/2021/1631.pdf
 *
 * @param[in] ctx Pointer to the context of the seed expander
 * @param[in] v Pointer to an array
 * @param[in] weight Integer that is the Hamming weight
 */
void vect_set_random_fixed_weight(seedexpander_state *ctx, __m256i *v256, uint16_t weight) {
    uint32_t rand_u32[PARAM_OMEGA_R] = {0};
    uint32_t tmp[PARAM_OMEGA_R] = {0};
    __m256i bit256[PARAM_OMEGA_R];
    __m256i bloc256[PARAM_OMEGA_R];
    static __m256i posCmp256 = (__m256i){0UL,1UL,2UL,3UL};

    seedexpander(ctx, (uint8_t *)&rand_u32, 4 * weight);

    for (size_t i = 0; i < weight; ++i) {
        tmp[i] = i + barrett_reduce(rand_u32[i], i);
    }

    for (int32_t i = (weight - 1); i -- > 0;) {
        uint32_t found = 0;

        for (size_t j = i + 1; j < weight; ++j) {
            found |= compare_u32(tmp[j], tmp[i]);
        }

        uint32_t mask = -found;
        tmp[i] = (mask & i) ^ (~mask & tmp[i]);
    }

    for (uint32_t i = 0 ; i < weight ; i++) {
        // we store the bloc number and bit position of each vb[i]
        uint64_t bloc = tmp[i] >> 6;
        bloc256[i] = _mm256_set1_epi64x(bloc >> 2);
        uint64_t pos = (bloc & 0x3UL);
        __m256i pos256 = _mm256_set1_epi64x(pos);
        __m256i mask256 = _mm256_cmpeq_epi64(pos256,posCmp256);
        uint64_t bit64 = 1ULL << (tmp[i] & 0x3f);
        __m256i bl256 = _mm256_set1_epi64x(bit64);
        bit256[i] = bl256&mask256;
    }

    for (uint32_t i = 0 ; i < LOOP_SIZE ; i++) {
        __m256i aux = _mm256_setzero_si256();
        __m256i i256 = _mm256_set1_epi64x(i);

        for (uint32_t j = 0 ; j < weight ; j++) {
            __m256i mask256 = _mm256_cmpeq_epi64(bloc256[j],i256);
            aux ^= bit256[j] & mask256;
        }

        _mm256_storeu_si256(&v256[i], _mm256_xor_si256(v256[i],aux));
    }

    #undef LOOP_SIZE
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
void vect_set_random(seedexpander_state *ctx, uint64_t *v) {
    uint8_t rand_bytes[VEC_N_SIZE_BYTES] = {0};

    seedexpander(ctx, rand_bytes, VEC_N_SIZE_BYTES);

    memcpy(v, rand_bytes, VEC_N_SIZE_BYTES);
    v[VEC_N_SIZE_64 - 1] &= BITMASK(PARAM_N, 64);
}



/**
 * @brief Generates a random vector
 *
 * This function generates a random binary vector. It uses the the prng function.
 *
 * @param[in] v Pointer to an array
 * @param[in] size_v Size of v
 */
void vect_set_random_from_prng(uint64_t *v, uint32_t size_v) {
    uint8_t rand_bytes [32] = {0}; // set to the maximum possible size - 256 bits

    shake_prng(rand_bytes, size_v << 3);
    memcpy(v, rand_bytes, size_v << 3);
}



/**
 * @brief Adds two vectors
 *
 * @param[out] o Pointer to an array that is the result
 * @param[in] v1 Pointer to an array that is the first vector
 * @param[in] v2 Pointer to an array that is the second vector
 * @param[in] size Integer that is the size of the vectors
 */
void vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, uint32_t size) {
    for (uint32_t i = 0 ; i < size ; ++i) {
        o[i] = v1[i] ^ v2[i];
    }
}



/**
 * @brief Compares two vectors
 *
 * @param[in] v1 Pointer to an array that is first vector
 * @param[in] v2 Pointer to an array that is second vector
 * @param[in] size Integer that is the size of the vectors
 * @returns 0 if the vectors are equals and a negative/positive value otherwise
 */
uint8_t vect_compare(const uint8_t *v1, const uint8_t *v2, uint32_t size) {
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
void vect_resize(uint64_t *o, uint32_t size_o, const uint64_t *v, uint32_t size_v) {
    uint64_t mask = 0x7FFFFFFFFFFFFFFF;
    int8_t val = 0;
    if (size_o < size_v) {

        if (size_o % 64) {
            val = 64 - (size_o % 64);
        }

        memcpy(o, v, VEC_N1N2_SIZE_BYTES);

        for (int8_t i = 0 ; i < val ; ++i) {
            o[VEC_N1N2_SIZE_64 - 1] &= (mask >> i);
        }
    } else {
        memcpy(o, v, CEIL_DIVIDE(size_v, 8));
    }
}



/**
 * @brief Prints a given number of bytes
 *
 * @param[in] v Pointer to an array of bytes
 * @param[in] size Integer that is number of bytes to be displayed
 */
void vect_print(const uint64_t *v, const uint32_t size) {
    if(size == VEC_K_SIZE_BYTES) {
        uint8_t tmp [VEC_K_SIZE_BYTES] = {0};
        memcpy(tmp, v, VEC_K_SIZE_BYTES);
        for (uint32_t i = 0; i < VEC_K_SIZE_BYTES; ++i) {
            printf("%02x", tmp[i]);
        }
    } else if (size == VEC_N_SIZE_BYTES) {
        uint8_t tmp [VEC_N_SIZE_BYTES] = {0};
        memcpy(tmp, v, VEC_N_SIZE_BYTES);
        for (uint32_t i = 0; i < VEC_N_SIZE_BYTES; ++i) {
            printf("%02x", tmp[i]);
        }
    } else if (size == VEC_N1N2_SIZE_BYTES) {
        uint8_t tmp [VEC_N1N2_SIZE_BYTES] = {0};
        memcpy(tmp, v, VEC_N1N2_SIZE_BYTES);
        for (uint32_t i = 0; i < VEC_N1N2_SIZE_BYTES; ++i) {
            printf("%02x", tmp[i]);
        }
    }  else if (size == VEC_N1_SIZE_BYTES) {
        uint8_t tmp [VEC_N1_SIZE_BYTES] = {0};
        memcpy(tmp, v, VEC_N1_SIZE_BYTES);
        for (uint32_t i = 0; i < VEC_N1_SIZE_BYTES; ++i) {
            printf("%02x", tmp[i]);
        }
    }
}



/**
 * @brief Prints a vector stored by positions
 *
 * @param[in] v Pointer to an array of integers
 * @param[in] weight Integer that is number positions to be displayed
 */
void vect_print_sparse(const uint32_t *v, const uint16_t weight) {
    for (uint16_t i = 0; i < weight-1; ++i) {
        printf("%d ,", v[i]);
    }
    printf("%d", v[weight - 1]);
}
