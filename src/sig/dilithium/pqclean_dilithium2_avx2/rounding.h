#ifndef PQCLEAN_DILITHIUM2_AVX2_ROUNDING_H
#define PQCLEAN_DILITHIUM2_AVX2_ROUNDING_H
#include "params.h"
#include <immintrin.h>
#include <stdint.h>

void PQCLEAN_DILITHIUM2_AVX2_power2round_avx(__m256i *a1, __m256i *a0, const __m256i *a);
void PQCLEAN_DILITHIUM2_AVX2_decompose_avx(__m256i *a1, __m256i *a0, const __m256i *a);
unsigned int PQCLEAN_DILITHIUM2_AVX2_make_hint_avx(uint8_t hint[N], const __m256i *a0, const __m256i *a1);
void PQCLEAN_DILITHIUM2_AVX2_use_hint_avx(__m256i *b, const __m256i *a, const __m256i *hint);

#endif
