#ifndef BCH_H
#define BCH_H


/**
 * @file bch.h
 * Header file of bch.c
 */

#include "parameters.h"
#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQC256_AVX2_bch_code_decode(uint64_t *message, uint64_t *vector);


#endif
