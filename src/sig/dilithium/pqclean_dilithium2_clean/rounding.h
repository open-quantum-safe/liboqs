#ifndef ROUNDING_H
#define ROUNDING_H

#include <stdint.h>

uint32_t PQCLEAN_DILITHIUM2_CLEAN_power2round(uint32_t a, uint32_t *a0);
uint32_t PQCLEAN_DILITHIUM2_CLEAN_decompose(uint32_t a, uint32_t *a0);
unsigned int PQCLEAN_DILITHIUM2_CLEAN_make_hint(uint32_t a0, uint32_t a1);
uint32_t PQCLEAN_DILITHIUM2_CLEAN_use_hint(uint32_t a, unsigned int hint);

#endif
