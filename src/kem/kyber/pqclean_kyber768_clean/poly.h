#ifndef POLY_H
#define POLY_H

#include "params.h"
#include <stdint.h>

/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef struct {
    uint16_t coeffs[KYBER_N];
} poly;

void PQCLEAN_KYBER768_CLEAN_poly_compress(unsigned char *r, const poly *a);
void PQCLEAN_KYBER768_CLEAN_poly_decompress(poly *r, const unsigned char *a);

void PQCLEAN_KYBER768_CLEAN_poly_tobytes(unsigned char *r, const poly *a);
void PQCLEAN_KYBER768_CLEAN_poly_frombytes(poly *r, const unsigned char *a);

void PQCLEAN_KYBER768_CLEAN_poly_frommsg(poly *r, const unsigned char msg[KYBER_SYMBYTES]);
void PQCLEAN_KYBER768_CLEAN_poly_tomsg(unsigned char msg[KYBER_SYMBYTES], const poly *a);

void PQCLEAN_KYBER768_CLEAN_poly_getnoise(poly *r, const unsigned char *seed, unsigned char nonce);

void PQCLEAN_KYBER768_CLEAN_poly_ntt(poly *r);
void PQCLEAN_KYBER768_CLEAN_poly_invntt(poly *r);

void PQCLEAN_KYBER768_CLEAN_poly_add(poly *r, const poly *a, const poly *b);
void PQCLEAN_KYBER768_CLEAN_poly_sub(poly *r, const poly *a, const poly *b);

#endif
