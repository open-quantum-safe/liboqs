#ifndef HQC192_SHAKE_PRNG_H
#define HQC192_SHAKE_PRNG_H

/**
 * @file shake_prng.h
 * @brief Header file of shake_prng.c
 */

#include <stdint.h>
#include "fips202.h"
#include "domains.h"

typedef shake256incctx seedexpander_state;

void HQC192_shake_prng_init(uint8_t *entropy_input, uint8_t *personalization_string, uint32_t enlen, uint32_t perlen);
void HQC192_shake_prng(uint8_t *output, uint32_t outlen);
void HQC192_seedexpander_init(seedexpander_state *state, const uint8_t *seed, uint32_t seedlen);
void HQC192_seedexpander(seedexpander_state *state, uint8_t *output, uint32_t outlen);

#endif
