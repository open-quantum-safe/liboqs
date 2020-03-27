#ifndef _GF16_H_
#define _GF16_H_

#include "rainbow_config.h"
#include <stdint.h>

/// @file gf16.h
/// @brief Library for arithmetics in GF(16) and GF(256)
///

uint8_t PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf16_mul(uint8_t a, uint8_t b);


uint8_t PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256_is_nonzero(uint8_t a);
uint8_t PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256_inv(uint8_t a);
uint8_t PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_gf256_mul(uint8_t a, uint8_t b);


#endif // _GF16_H_
