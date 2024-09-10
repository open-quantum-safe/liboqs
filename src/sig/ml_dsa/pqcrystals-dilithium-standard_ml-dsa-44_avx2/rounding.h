#ifndef ROUNDING_H
#define ROUNDING_H

#include <stdint.h>
#include <immintrin.h>
#include "params.h"

#define power2round_avx DILITHIUM_NAMESPACE(power2round_avx)
void power2round_avx(__m256i *a1, __m256i *a0, const __m256i *a);
#define decompose_avx DILITHIUM_NAMESPACE(decompose_avx)
void decompose_avx(__m256i *a1, __m256i *a0, const __m256i *a);
#define make_hint_avx DILITHIUM_NAMESPACE(make_hint_avx)
unsigned int make_hint_avx(uint8_t hint[N], const __m256i *a0, const __m256i *a1);
#define use_hint_avx DILITHIUM_NAMESPACE(use_hint_avx)
void use_hint_avx(__m256i *b, const __m256i *a, const __m256i *hint);

#endif
