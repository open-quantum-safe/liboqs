#ifndef _BLAS_U32_H_
#define _BLAS_U32_H_
/// @file blas_u32.h
/// @brief Inlined functions for implementing basic linear algebra functions for uint32 arch.
///

#include "rainbow_config.h"
#include <stdint.h>

void PQCLEAN_RAINBOWIACYCLICCOMPRESSED_CLEAN_gf256v_predicated_add_u32(uint8_t *accu_b, uint8_t predicate, const uint8_t *a, unsigned int _num_byte);
void PQCLEAN_RAINBOWIACYCLICCOMPRESSED_CLEAN_gf256v_add_u32(uint8_t *accu_b, const uint8_t *a, unsigned int _num_byte);


void PQCLEAN_RAINBOWIACYCLICCOMPRESSED_CLEAN_gf16v_madd_u32(uint8_t *accu_c, const uint8_t *a, uint8_t gf16_b, unsigned int _num_byte);
void PQCLEAN_RAINBOWIACYCLICCOMPRESSED_CLEAN_gf16v_mul_scalar_u32(uint8_t *a, uint8_t gf16_b, unsigned int _num_byte);
uint8_t PQCLEAN_RAINBOWIACYCLICCOMPRESSED_CLEAN_gf16v_dot_u32(const uint8_t *a, const uint8_t *b, unsigned int _num_byte);


#endif // _BLAS_U32_H_
