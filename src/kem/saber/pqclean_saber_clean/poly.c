#include "api.h"
#include "cbd.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "poly.h"
#include <stddef.h>

void PQCLEAN_SABER_CLEAN_MatrixVectorMul(poly c[SABER_L], const poly A[SABER_L][SABER_L], const poly s[SABER_L], int16_t transpose) {
    size_t i, j;

    if (transpose) {
        for (i = 0; i < SABER_L; i++) {
            PQCLEAN_SABER_CLEAN_poly_mul(&c[i], &A[0][i], &s[0], 0);
            for (j = 1; j < SABER_L; j++) {
                PQCLEAN_SABER_CLEAN_poly_mul(&c[i], &A[j][i], &s[j], 1);
            }
        }
    } else {
        for (i = 0; i < SABER_L; i++) {
            PQCLEAN_SABER_CLEAN_poly_mul(&c[i], &A[i][0], &s[0], 0);
            for (j = 1; j < SABER_L; j++) {
                PQCLEAN_SABER_CLEAN_poly_mul(&c[i], &A[i][j], &s[j], 1);
            }
        }
    }
}

void PQCLEAN_SABER_CLEAN_InnerProd(poly *c, const poly b[SABER_L], const poly s[SABER_L]) {
    size_t i;

    PQCLEAN_SABER_CLEAN_poly_mul(c, &b[0], &s[0], 0);
    for (i = 1; i < SABER_L; i++) {
        PQCLEAN_SABER_CLEAN_poly_mul(c, &b[i], &s[i], 1);
    }
}

void PQCLEAN_SABER_CLEAN_GenMatrix(poly A[SABER_L][SABER_L], const uint8_t seed[SABER_SEEDBYTES]) {
    size_t i;
    uint8_t buf[SABER_L * SABER_POLYVECBYTES];

    shake128(buf, sizeof(buf), seed, SABER_SEEDBYTES);

    for (i = 0; i < SABER_L; i++) {
        PQCLEAN_SABER_CLEAN_BS2POLVECq(A[i], buf + i * SABER_POLYVECBYTES);
    }
}

void PQCLEAN_SABER_CLEAN_GenSecret(poly s[SABER_L], const uint8_t seed[SABER_NOISESEEDBYTES]) {
    size_t i;
    uint8_t buf[SABER_L * SABER_POLYCOINBYTES];

    shake128(buf, sizeof(buf), seed, SABER_NOISESEEDBYTES);

    for (i = 0; i < SABER_L; i++) {
        PQCLEAN_SABER_CLEAN_cbd(s[i].coeffs, buf + i * SABER_POLYCOINBYTES);
    }
}
