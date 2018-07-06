#ifndef POLY_H
#define POLY_H

#include "params.h"
#include <stdint.h>

typedef	int64_t __attribute__ ((aligned(32))) poly[PARAM_N];

int64_t reduce(int64_t a);
int32_t barr_reduce(int64_t a);
void ntt(poly a, const poly w);
void nttinv(poly a, const poly w);
void poly_mul(poly result, const poly x, const poly y);
void poly_add(poly result, const poly x, const poly y);
void poly_sub(poly result, const poly x, const poly y);
void poly_uniform(poly a, const unsigned char *seed);

#endif
