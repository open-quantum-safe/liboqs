#ifndef POLY_H
#define POLY_H

#include "params.h"
#include <stdint.h>

typedef	int64_t __attribute__ ((aligned(32))) poly[PARAM_N];
typedef	int64_t __attribute__ ((aligned(32))) poly_k[PARAM_N*PARAM_K];

int64_t reduce(int64_t a);
int64_t barr_reduce(int64_t a);
void ntt(poly a, const poly w);
void nttinv(poly a, const poly w);
void poly_ntt(poly x_ntt, const poly x);
void poly_mul(poly result, const poly x, const poly y);
void poly_add(poly result, const poly x, const poly y);
void poly_sub(poly result, const poly x, const poly y);
void poly_uniform(poly_k a, const unsigned char *seed);

#endif
