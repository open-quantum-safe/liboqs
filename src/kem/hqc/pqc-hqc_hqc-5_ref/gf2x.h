/**
 * @file gf2x.h
 * @brief Header file for gf2x.c
 */

#ifndef HQC_GF2X_H
#define HQC_GF2X_H

#include <stdint.h>

void vect_mul(uint64_t *o, const uint64_t *v1, const uint64_t *v2);

#endif  // HQC_GF2X_H
