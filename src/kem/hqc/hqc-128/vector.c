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

#define SHBIT32 46

static uint64_t v_val[PARAM_OMEGA_R] = {3982610457, 3982835871, 3983061310, 3983286775, 3983512265, 3983737781, 3983963323, 3984188890, 3984414482, 3984640100, 3984865744, 3985091413, 3985317108, 3985542828, 3985768574, 3985994345, 3986220142, 3986445965, 3986671813, 3986897687, 3987123586, 3987349511, 3987575461, 3987801438, 3988027439, 3988253467, 3988479520, 3988705599, 3988931703, 3989157833, 3989383988, 3989610169, 3989836376, 3990062609, 3990288867, 3990515151, 3990741460, 3990967795, 3991194156, 3991420543, 3991646955, 3991873393, 3992099856, 3992326346, 3992552861, 3992779401, 3993005968, 3993232560, 3993459178, 3993685821, 3993912490, 3994139185, 3994365906, 3994592653, 3994819425, 3995046223, 3995273047, 3995499896, 3995726771, 3995953672, 3996180599, 3996407552, 3996634530, 3996861534, 3997088564, 3997315620, 3997542701, 3997769808, 3997996942, 3998224101, 3998451285, 3998678496, 3998905732, 3999132994, 3999360282};

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
