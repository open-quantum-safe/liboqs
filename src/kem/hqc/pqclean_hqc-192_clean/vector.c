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


static uint32_t m_val[114] = { 119800, 119803, 119807, 119810, 119813, 119817, 119820, 119823, 119827, 119830, 119833, 119837, 119840, 119843, 119847, 119850, 119853, 119857, 119860, 119864, 119867, 119870, 119874, 119877, 119880, 119884, 119887, 119890, 119894, 119897, 119900, 119904, 119907, 119910, 119914, 119917, 119920, 119924, 119927, 119930, 119934, 119937, 119941, 119944, 119947, 119951, 119954, 119957, 119961, 119964, 119967, 119971, 119974, 119977, 119981, 119984, 119987, 119991, 119994, 119997, 120001, 120004, 120008, 120011, 120014, 120018, 120021, 120024, 120028, 120031, 120034, 120038, 120041, 120044, 120048, 120051, 120054, 120058, 120061, 120065, 120068, 120071, 120075, 120078, 120081, 120085, 120088, 120091, 120095, 120098, 120101, 120105, 120108, 120112, 120115, 120118, 120122, 120125, 120128, 120132, 120135, 120138, 120142, 120145, 120149, 120152, 120155, 120159, 120162, 120165, 120169, 120172, 120175, 120179 };

/**
 * @brief Constant-time comparison of two integers v1 and v2
 *
 * Returns 1 if v1 is equal to v2 and 0 otherwise
 * https://gist.github.com/sneves/10845247
 *
 * @param[in] v1
 * @param[in] v2
 */
static inline uint32_t compare_u32(uint32_t v1, uint32_t v2) {
    return 1 ^ ((uint32_t)((v1 - v2) | (v2 - v1)) >> 31);
}

static uint64_t single_bit_mask(uint32_t pos) {
    uint64_t ret = 0;
    uint64_t mask = 1;
    uint64_t tmp;

    for (size_t i = 0; i < 64; ++i) {
        tmp = pos - i;
        tmp = 0 - (1 - ((uint64_t)(tmp | (0 - tmp)) >> 63));
        ret |= mask & tmp;
        mask <<= 1;
    }

    return ret;
}

static inline uint32_t cond_sub(uint32_t r, uint32_t n) {
    uint32_t mask;
    r -= n;
    mask = 0 - (r >> 31);
    return r + (n & mask);
}

static inline uint32_t reduce(uint32_t a, size_t i) {
    uint32_t q, n, r;
    q = ((uint64_t) a * m_val[i]) >> 32;
    n = (uint32_t)(PARAM_N - i);
    r = a - q * n;
    return cond_sub(r, n);
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
void PQCLEAN_HQC192_CLEAN_vect_set_random_fixed_weight(seedexpander_state *ctx, uint64_t *v, uint16_t weight) {
    uint8_t rand_bytes[4 * PARAM_OMEGA_R] = {0}; // to be interpreted as PARAM_OMEGA_R 32-bit unsigned ints
    uint32_t support[PARAM_OMEGA_R] = {0};
    uint32_t index_tab [PARAM_OMEGA_R] = {0};
    uint64_t bit_tab [PARAM_OMEGA_R] = {0};
    uint32_t pos, found, mask32, tmp;
    uint64_t mask64, val;

    PQCLEAN_HQC192_CLEAN_seedexpander(ctx, rand_bytes, 4 * weight);

    for (size_t i = 0; i < weight; ++i) {
        support[i] = rand_bytes[4 * i];
        support[i] |= rand_bytes[4 * i + 1] << 8;
        support[i] |= (uint32_t)rand_bytes[4 * i + 2] << 16;
        support[i] |= (uint32_t)rand_bytes[4 * i + 3] << 24;
        support[i] = (uint32_t)(i + reduce(support[i], i)); // use constant-tme reduction
    }

    for (size_t i = (weight - 1); i-- > 0;) {
        found = 0;

        for (size_t j = i + 1; j < weight; ++j) {
            found |= compare_u32(support[j], support[i]);
        }

        mask32 = 0 - found;
        support[i] = (mask32 & i) ^ (~mask32 & support[i]);
    }

    for (size_t i = 0; i < weight; ++i) {
        index_tab[i] = support[i] >> 6;
        pos = support[i] & 0x3f;
        bit_tab[i] = single_bit_mask(pos); // avoid secret shift
    }

    for (size_t i = 0; i < VEC_N_SIZE_64; ++i) {
        val = 0;
        for (size_t j = 0; j < weight; ++j) {
            tmp = (uint32_t)(i - index_tab[j]);
            tmp = 1 ^ ((uint32_t)(tmp | (0 - tmp)) >> 31);
            mask64 = 0 - (uint64_t)tmp;
            val |= (bit_tab[j] & mask64);
        }
        v[i] |= val;
    }
}



/**
 * @brief Generates a random vector of dimension <b>PARAM_N</b>
 *
 * This function generates a random binary vector of dimension <b>PARAM_N</b>. It generates a random
 * array of bytes using the PQCLEAN_HQC192_CLEAN_seedexpander function, and drop the extra bits using a mask.
 *
 * @param[in] v Pointer to an array
 * @param[in] ctx Pointer to the context of the seed expander
 */
void PQCLEAN_HQC192_CLEAN_vect_set_random(seedexpander_state *ctx, uint64_t *v) {
    uint8_t rand_bytes[VEC_N_SIZE_BYTES] = {0};

    PQCLEAN_HQC192_CLEAN_seedexpander(ctx, rand_bytes, VEC_N_SIZE_BYTES);

    PQCLEAN_HQC192_CLEAN_load8_arr(v, VEC_N_SIZE_64, rand_bytes, VEC_N_SIZE_BYTES);
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
void PQCLEAN_HQC192_CLEAN_vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        o[i] = v1[i] ^ v2[i];
    }
}



/**
 * @brief Compares two vectors
 *
 * @param[in] v1 Pointer to an array that is first vector
 * @param[in] v2 Pointer to an array that is second vector
 * @param[in] size Integer that is the size of the vectors
 * @returns 0 if the vectors are equal and 1 otherwise
 */
uint8_t PQCLEAN_HQC192_CLEAN_vect_compare(const uint8_t *v1, const uint8_t *v2, size_t size) {
    uint16_t r = 0x0100;

    for (size_t i = 0; i < size; i++) {
        r |= v1[i] ^ v2[i];
    }

    return (r - 1) >> 8;
}



/**
 * @brief Resize a vector so that it contains <b>size_o</b> bits
 *
 * @param[out] o Pointer to the output vector
 * @param[in] size_o Integer that is the size of the output vector in bits
 * @param[in] v Pointer to the input vector
 * @param[in] size_v Integer that is the size of the input vector in bits
 */
void PQCLEAN_HQC192_CLEAN_vect_resize(uint64_t *o, uint32_t size_o, const uint64_t *v, uint32_t size_v) {
    uint64_t mask = 0x7FFFFFFFFFFFFFFF;
    size_t val = 0;
    if (size_o < size_v) {

        if (size_o % 64) {
            val = 64 - (size_o % 64);
        }

        memcpy(o, v, VEC_N1N2_SIZE_BYTES);

        for (size_t i = 0; i < val; ++i) {
            o[VEC_N1N2_SIZE_64 - 1] &= (mask >> i);
        }
    } else {
        memcpy(o, v, 8 * CEIL_DIVIDE(size_v, 64));
    }
}
