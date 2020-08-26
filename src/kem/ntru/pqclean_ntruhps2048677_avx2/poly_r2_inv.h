#ifndef POLY_R2_INV_H
#define POLY_R2_INV_H

#include "poly.h"

void PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in);

extern void PQCLEAN_NTRUHPS2048677_AVX2_square_1_677(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048677_AVX2_square_2_677(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048677_AVX2_square_3_677(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048677_AVX2_square_5_677(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048677_AVX2_square_10_677(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048677_AVX2_square_21_677(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048677_AVX2_square_42_677(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048677_AVX2_square_84_677(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048677_AVX2_square_168_677(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048677_AVX2_square_336_677(unsigned char *out, const unsigned char *a);

extern void PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(unsigned char *out, const unsigned char *a,
        const unsigned char *b);

#endif
