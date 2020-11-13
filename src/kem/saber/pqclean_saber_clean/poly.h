#ifndef POLY_H
#define POLY_H
#include "SABER_params.h"
#include <stdint.h>

typedef union {
    uint16_t coeffs[SABER_N];
} poly;


void PQCLEAN_SABER_CLEAN_MatrixVectorMul(poly c[SABER_L], const poly A[SABER_L][SABER_L], const poly s[SABER_L], int16_t transpose);

void PQCLEAN_SABER_CLEAN_InnerProd(poly *c, const poly b[SABER_L], const poly s[SABER_L]);

void PQCLEAN_SABER_CLEAN_GenMatrix(poly A[SABER_L][SABER_L], const uint8_t seed[SABER_SEEDBYTES]);

void PQCLEAN_SABER_CLEAN_GenSecret(poly s[SABER_L], const uint8_t seed[SABER_NOISESEEDBYTES]);


void PQCLEAN_SABER_CLEAN_poly_mul(poly *c, const poly *a, const poly *b, int accumulate);


#endif
