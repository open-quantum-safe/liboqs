#ifndef POLY_R2_INV_H
#define POLY_R2_INV_H

#include "poly.h"

void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a);
void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in);

extern void PQCLEAN_NTRUHPS4096821_AVX2_square_1_821(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS4096821_AVX2_square_3_821(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS4096821_AVX2_square_6_821(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS4096821_AVX2_square_12_821(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS4096821_AVX2_square_24_821(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS4096821_AVX2_square_51_821(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS4096821_AVX2_square_102_821(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS4096821_AVX2_square_204_821(unsigned char *out, const unsigned char *a);
extern void PQCLEAN_NTRUHPS4096821_AVX2_square_408_821(unsigned char *out, const unsigned char *a);

extern void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_mul(unsigned char *out, const unsigned char *a,
        const unsigned char *b);
#endif
