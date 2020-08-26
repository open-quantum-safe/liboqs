#ifndef POLY_R2_INV_H
#define POLY_R2_INV_H

#include "poly.h"

void PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a);
void PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in);

extern void PQCLEAN_NTRUHPS2048509_AVX2_square_1_509(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048509_AVX2_square_3_509(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048509_AVX2_square_6_509(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048509_AVX2_square_15_509(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048509_AVX2_square_30_509(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048509_AVX2_square_63_509(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048509_AVX2_square_126_509(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS2048509_AVX2_square_252_509(unsigned char *out, const unsigned char *a);

extern void PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(unsigned char *out, const unsigned char *a,
        const unsigned char *b);
#endif
