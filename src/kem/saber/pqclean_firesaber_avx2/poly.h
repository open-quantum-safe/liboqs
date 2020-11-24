#ifndef POLY_H
#define POLY_H
#include "SABER_params.h"
#include <immintrin.h>
#include <stdint.h>

typedef union {
    uint16_t coeffs[SABER_N];
    __m256i dummy;
} poly;

typedef union {
    uint16_t coeffs[4 * SABER_N];
    __m256i dummy;
} toom4_points;

typedef union {
    uint16_t coeffs[8 * SABER_N];
    __m256i dummy;
} toom4_points_product;

void PQCLEAN_FIRESABER_AVX2_MatrixVectorMul(poly c[SABER_L], const poly A[SABER_L][SABER_L], const toom4_points s_eval[SABER_L], int transpose);

void PQCLEAN_FIRESABER_AVX2_InnerProd(poly *c, const poly b[SABER_L], const toom4_points s_eval[SABER_L]);

void PQCLEAN_FIRESABER_AVX2_GenMatrix(poly a[SABER_L][SABER_L], const uint8_t seed[SABER_SEEDBYTES]);

void PQCLEAN_FIRESABER_AVX2_GenSecret(poly s[SABER_L], const uint8_t seed[SABER_NOISESEEDBYTES]);


void PQCLEAN_FIRESABER_AVX2_toom4_interp(poly *res_avx, const toom4_points_product *c_eval);

void PQCLEAN_FIRESABER_AVX2_toom4_eval(toom4_points *b_eval, const poly *b);

void PQCLEAN_FIRESABER_AVX2_toom4_mul_A_by_B_eval(toom4_points_product *c_eval, const poly *a_avx, const toom4_points *b_eval, int accumulate);


#endif
