#ifndef PQCLEAN_KYBER51290S_AVX2_NTT_H
#define PQCLEAN_KYBER51290S_AVX2_NTT_H

#include <immintrin.h>
#include <stdint.h>

void PQCLEAN_KYBER51290S_AVX2_ntt_avx(__m256i *r, const __m256i *PQCLEAN_KYBER51290S_AVX2_qdata);
void PQCLEAN_KYBER51290S_AVX2_invntt_avx(__m256i *r, const __m256i *PQCLEAN_KYBER51290S_AVX2_qdata);

void PQCLEAN_KYBER51290S_AVX2_nttpack_avx(__m256i *r, const __m256i *PQCLEAN_KYBER51290S_AVX2_qdata);
void PQCLEAN_KYBER51290S_AVX2_nttunpack_avx(__m256i *r, const __m256i *PQCLEAN_KYBER51290S_AVX2_qdata);

void PQCLEAN_KYBER51290S_AVX2_basemul_avx(__m256i *r,
        const __m256i *a,
        const __m256i *b,
        const __m256i *PQCLEAN_KYBER51290S_AVX2_qdata);

void PQCLEAN_KYBER51290S_AVX2_ntttobytes_avx(uint8_t *r, const __m256i *a, const __m256i *PQCLEAN_KYBER51290S_AVX2_qdata);
void PQCLEAN_KYBER51290S_AVX2_nttfrombytes_avx(__m256i *r, const uint8_t *a, const __m256i *PQCLEAN_KYBER51290S_AVX2_qdata);

#endif
