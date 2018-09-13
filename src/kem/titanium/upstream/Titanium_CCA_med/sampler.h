/* ****************************** *
 * Titanium_CCA_med               *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Samplers                       *
 * ****************************** */
 
#ifndef SAMPLER_H
#define SAMPLER_H

#include "param.h"
#include <stdint.h>

void sampler_zb(uint32_t sample[T][N + K + 1]);
void sampler_zq(uint32_t *sample, uint32_t slen, uint32_t bytpc);
void sampler_binomial(uint32_t sample[T][D + K]);

#endif
