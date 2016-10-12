#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"
#include <oqs/rand.h>

typedef struct {
	uint16_t coeffs[PARAM_N];
} poly __attribute__ ((aligned (32)));

void poly_uniform(poly *a, const unsigned char *seed);
void poly_getnoise(poly *r, OQS_RAND *rand);
void poly_add(poly *r, const poly *a, const poly *b);

void poly_ntt(poly *r);
void poly_invntt(poly *r);
void poly_pointwise(poly *r, const poly *a, const poly *b);

void poly_frombytes(poly *r, const unsigned char *a);
void poly_tobytes(unsigned char *r, const poly *p);

#endif
