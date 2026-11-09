/**
 * @file gf2x.h
 * @brief Header file for gf2x.c
 */

#ifndef HQC_GF2X_H
#define HQC_GF2X_H

#include <immintrin.h>
#include <stdint.h>

void vect_mul(__m256i *o, const __m256i *v1, const __m256i *v2);

#endif  // HQC_GF2X_H
