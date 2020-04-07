#ifndef ROUNDING_H
#define ROUNDING_H

#include "params.h"
#include <stdint.h>

uint32_t PQCLEAN_DILITHIUM2_AVX2_power2round(uint32_t a, uint32_t *a0);
uint32_t PQCLEAN_DILITHIUM2_AVX2_decompose(uint32_t a, uint32_t *a0);
unsigned int PQCLEAN_DILITHIUM2_AVX2_make_hint(uint32_t a0, uint32_t a1);
uint32_t PQCLEAN_DILITHIUM2_AVX2_use_hint(uint32_t a, unsigned int hint);

#endif
