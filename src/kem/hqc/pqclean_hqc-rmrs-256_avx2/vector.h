#ifndef VECTOR_H
#define VECTOR_H


/**
 * @file vector.h
 * @brief Header file for vector.c
 */
#include "nistseedexpander.h"
#include "randombytes.h"
#include <stdint.h>

void PQCLEAN_HQCRMRS256_AVX2_vect_set_random_fixed_weight(AES_XOF_struct *ctx, uint64_t *v, uint16_t weight);

void PQCLEAN_HQCRMRS256_AVX2_vect_set_random(AES_XOF_struct *ctx, uint64_t *v);

void PQCLEAN_HQCRMRS256_AVX2_vect_set_random_from_randombytes(uint64_t *v);


void PQCLEAN_HQCRMRS256_AVX2_vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, uint32_t size);

uint8_t PQCLEAN_HQCRMRS256_AVX2_vect_compare(const uint8_t *v1, const uint8_t *v2, uint32_t size);

void PQCLEAN_HQCRMRS256_AVX2_vect_resize(uint64_t *o, uint32_t size_o, const uint64_t *v, uint32_t size_v);


#endif
