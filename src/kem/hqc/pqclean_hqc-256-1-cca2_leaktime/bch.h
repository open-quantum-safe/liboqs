#ifndef PQCLEAN_HQC2561CCA2_LEAKTIME_BCH_H
#define PQCLEAN_HQC2561CCA2_LEAKTIME_BCH_H

/**
 * @file bch.h
 * Header file of bch.c
 */

#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQC2561CCA2_LEAKTIME_bch_code_encode(uint8_t *codeword, const uint8_t *message);
void PQCLEAN_HQC2561CCA2_LEAKTIME_bch_code_decode(uint8_t *message, uint8_t *vector);

#endif
