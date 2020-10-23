#ifndef GF2X_H
#define GF2X_H


/**
 * @file gf2x.h
 * @brief Header file for gf2x.c
 */
#include "nistseedexpander.h"
#include "randombytes.h"
#include <stdint.h>

void PQCLEAN_HQCRMRS256_CLEAN_vect_mul(uint64_t *o, const uint32_t *a1, const uint64_t *a2, uint16_t weight, AES_XOF_struct *ctx);


#endif
