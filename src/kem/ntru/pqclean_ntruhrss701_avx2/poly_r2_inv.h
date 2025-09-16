#ifndef POLY_R2_INV_H
#define POLY_R2_INV_H

#include "poly.h"

void PQCLEAN_NTRUHRSS701_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a);
void PQCLEAN_NTRUHRSS701_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in);

extern void PQCLEAN_NTRUHRSS701_AVX2_square_1_701(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHRSS701_AVX2_square_3_701(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHRSS701_AVX2_square_6_701(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHRSS701_AVX2_square_12_701(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHRSS701_AVX2_square_15_701(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHRSS701_AVX2_square_27_701(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHRSS701_AVX2_square_42_701(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHRSS701_AVX2_square_84_701(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHRSS701_AVX2_square_168_701(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHRSS701_AVX2_square_336_701(unsigned char *out, const unsigned char *a);

extern void PQCLEAN_NTRUHRSS701_AVX2_poly_R2_mul(unsigned char *out, const unsigned char *a,
        const unsigned char *b);
#endif
