#ifndef ROUNDING_H
#define ROUNDING_H

#include <stdint.h>
#include "params.h"

#define power2round DILITHIUM_NAMESPACE(_power2round)
uint32_t power2round(uint32_t a, uint32_t *a0);

#define decompose DILITHIUM_NAMESPACE(_decompose)
uint32_t decompose(uint32_t a, uint32_t *a0);

#define make_hint DILITHIUM_NAMESPACE(_make_hint)
unsigned int make_hint(uint32_t a0, uint32_t a1);

#define use_hint DILITHIUM_NAMESPACE(_use_hint)
uint32_t use_hint(uint32_t a, unsigned int hint);

#endif
