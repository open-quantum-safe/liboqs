#ifndef PQCLEAN_DILITHIUM5_AVX2_NTT_H
#define PQCLEAN_DILITHIUM5_AVX2_NTT_H

#include <immintrin.h>

void PQCLEAN_DILITHIUM5_AVX2_ntt_avx(__m256i *a, const __m256i *PQCLEAN_DILITHIUM5_AVX2_qdata);
void PQCLEAN_DILITHIUM5_AVX2_invntt_avx(__m256i *a, const __m256i *PQCLEAN_DILITHIUM5_AVX2_qdata);

void PQCLEAN_DILITHIUM5_AVX2_nttunpack_avx(__m256i *a);

void PQCLEAN_DILITHIUM5_AVX2_pointwise_avx(__m256i *c, const __m256i *a, const __m256i *b, const __m256i *PQCLEAN_DILITHIUM5_AVX2_qdata);
void PQCLEAN_DILITHIUM5_AVX2_pointwise_acc_avx(__m256i *c, const __m256i *a, const __m256i *b, const __m256i *PQCLEAN_DILITHIUM5_AVX2_qdata);

#endif
