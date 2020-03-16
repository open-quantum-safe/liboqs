#ifndef _BLAS_H_
#define _BLAS_H_
/// @file blas.h
/// @brief Functions for implementing basic linear algebra functions.
///

#include "rainbow_config.h"
#include <stddef.h>
#include <stdint.h>

void PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf256v_predicated_add(uint8_t *accu_b, uint8_t predicate, const uint8_t *a, size_t _num_byte);
void PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf256v_add(uint8_t *accu_b, const uint8_t *a, size_t _num_byte);


void PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf256v_mul_scalar(uint8_t *a, uint8_t b, size_t _num_byte);
void PQCLEAN_RAINBOWIIICCYCLICCOMPRESSED_CLEAN_gf256v_madd(uint8_t *accu_c, const uint8_t *a, uint8_t gf256_b, size_t _num_byte);


#endif // _BLAS_H_
