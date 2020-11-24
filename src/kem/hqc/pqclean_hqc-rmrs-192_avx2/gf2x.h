#ifndef GF2X_H
#define GF2X_H


/**
 * @file gf2x.h
 * @brief Header file for gf2x.c
 */
#include "parameters.h"
#include <immintrin.h>
#include <stdint.h>

typedef union {
    uint64_t arr64[VEC_N_256_SIZE_64];
    __m256i dummy;
} aligned_vec_t;

void PQCLEAN_HQCRMRS192_AVX2_vect_mul(uint64_t *o, const aligned_vec_t *a1, const aligned_vec_t *a2);


#endif
