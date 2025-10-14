#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"

/*
 * Elements of R_q = Z_q[X]/(X^n - X^n/2 + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef struct{
	int16_t coeffs[NTRUPLUS_N];
} poly;

void poly_tobytes(uint8_t r[NTRUPLUS_POLYBYTES], const poly *a);
void poly_frombytes(poly *r, const uint8_t a[NTRUPLUS_POLYBYTES]);

void poly_cbd1(poly *r, const uint8_t buf[NTRUPLUS_N/4]);
void poly_sotp(poly *r, const unsigned char *msg, const unsigned char *buf);
int  poly_sotp_inv(unsigned char *msg, const poly *e, const unsigned char *buf);

void poly_ntt(poly *r, const poly *a);
void poly_invntt(poly *r, const poly *a);
int  poly_baseinv(poly *r, const poly *a);
void poly_basemul(poly *r, const poly *a, const poly *b);
void poly_basemul_add(poly *r, const poly *a, const poly *b, const poly *c);
void poly_sub(poly *r, const poly *a, const poly *b);
void poly_triple(poly *r, const poly *a);
void poly_crepmod3(poly *r, const poly *a);

#endif
