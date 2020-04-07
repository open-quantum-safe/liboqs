#ifndef NTT_H
#define NTT_H

#include "consts.h"

#include <stdint.h>

void PQCLEAN_KYBER76890S_AVX2_ntt_level0_avx(int16_t *r, const uint16_t *zetas);
void PQCLEAN_KYBER76890S_AVX2_ntt_levels1t6_avx(int16_t *r, const uint16_t *zetas);
void PQCLEAN_KYBER76890S_AVX2_invntt_levels0t5_avx(int16_t *r, const uint16_t *zetas);
void PQCLEAN_KYBER76890S_AVX2_invntt_level6_avx(int16_t *r, const uint16_t *zetas);
void PQCLEAN_KYBER76890S_AVX2_nttpack_avx(int16_t *r);
void PQCLEAN_KYBER76890S_AVX2_nttunpack_avx(int16_t *r);
void PQCLEAN_KYBER76890S_AVX2_basemul_avx(int16_t *r, const int16_t *a, const int16_t *b, const uint16_t *zeta);
void PQCLEAN_KYBER76890S_AVX2_basemul_acc_avx(int16_t *r, const int16_t *a, const int16_t *b, const uint16_t *zeta);

void PQCLEAN_KYBER76890S_AVX2_ntttobytes_avx(uint8_t *r, const int16_t *a);
void PQCLEAN_KYBER76890S_AVX2_nttfrombytes_avx(int16_t *r, const uint8_t *a);

#endif
