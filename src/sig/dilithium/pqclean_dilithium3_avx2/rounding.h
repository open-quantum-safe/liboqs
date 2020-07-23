#ifndef ROUNDING_H
#define ROUNDING_H

#include <stdint.h>
#include "params.h"

#define power2round_avx DILITHIUM_NAMESPACE(_power2round_avx)
void power2round_avx(uint32_t a1[N], uint32_t a0[N], const uint32_t a[N]);
#define decompose_avx DILITHIUM_NAMESPACE(_decompose_avx)
void decompose_avx(uint32_t a1[N], uint32_t a0[N], const uint32_t a[N]);
#define make_hint_avx DILITHIUM_NAMESPACE(_make_hint_avx)
unsigned int make_hint_avx(uint32_t h[N], const uint32_t a0[N], const uint32_t a1[N]);
#define use_hint_avx DILITHIUM_NAMESPACE(_use_hint_avx)
void use_hint_avx(uint32_t b[N], const uint32_t a[N], const uint32_t hint[N]);

#endif
