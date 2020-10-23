#ifndef GF2X_H
#define GF2X_H


/**
 * @file gf2x.h
 * @brief Header file for gf2x.c
 */

#include <stdint.h>

void PQCLEAN_HQCRMRS192_AVX2_vect_mul(uint64_t *o, const uint64_t *a1, const uint64_t *a2);


#endif
