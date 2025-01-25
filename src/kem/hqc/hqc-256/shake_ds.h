#ifndef HQC256_SHAKE_DS_H
#define HQC256_SHAKE_DS_H

/**
 * @file shake_ds.h
 * @brief Header file of shake_ds.c
 */

#include <stdint.h>
#include "fips202.h"
#include "domains.h"

void HQC256_shake256_512_ds(shake256incctx *state, uint8_t *output, const uint8_t *input, size_t inlen, uint8_t domain);

#endif
