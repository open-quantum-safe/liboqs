#ifndef PQCLEAN_HQC1281CCA2_LEAKTIME_GF2X_H
#define PQCLEAN_HQC1281CCA2_LEAKTIME_GF2X_H

/**
 * @file gf2x.h
 * @brief Header file for gf2x.c
 */

#include <stdint.h>

void PQCLEAN_HQC1281CCA2_LEAKTIME_vect_mul(uint8_t *o, const uint32_t *a1, const uint8_t *a2, uint16_t weight);

#endif
