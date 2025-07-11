#ifndef FP2_H
#define FP2_H

#include <sqisign_namespace.h>
#include "fp.h"
#include <stdio.h>

// Structure for representing elements in GF(p^2)
typedef struct fp2_t
{
    fp_t re, im;
} fp2_t;

void fp2_set_small(fp2_t *x, const digit_t val);
void fp2_mul_small(fp2_t *x, const fp2_t *y, uint32_t n);
void fp2_set_one(fp2_t *x);
void fp2_set_zero(fp2_t *x);
uint32_t fp2_is_zero(const fp2_t *a);
uint32_t fp2_is_equal(const fp2_t *a, const fp2_t *b);
uint32_t fp2_is_one(const fp2_t *a);
void fp2_copy(fp2_t *x, const fp2_t *y);
void fp2_add(fp2_t *x, const fp2_t *y, const fp2_t *z);
void fp2_add_one(fp2_t *x, const fp2_t *y);
void fp2_sub(fp2_t *x, const fp2_t *y, const fp2_t *z);
void fp2_neg(fp2_t *x, const fp2_t *y);
void fp2_mul(fp2_t *x, const fp2_t *y, const fp2_t *z);
void fp2_sqr(fp2_t *x, const fp2_t *y);
void fp2_inv(fp2_t *x);
uint32_t fp2_is_square(const fp2_t *x);
void fp2_sqrt(fp2_t *x);
uint32_t fp2_sqrt_verify(fp2_t *a);
void fp2_half(fp2_t *x, const fp2_t *y);
void fp2_batched_inv(fp2_t *x, int len);
void fp2_pow_vartime(fp2_t *out, const fp2_t *x, const digit_t *exp, const int size);
void fp2_print(const char *name, const fp2_t *a);
void fp2_encode(void *dst, const fp2_t *a);
uint32_t fp2_decode(fp2_t *d, const void *src);
void fp2_select(fp2_t *d, const fp2_t *a0, const fp2_t *a1, uint32_t ctl);
void fp2_cswap(fp2_t *a, fp2_t *b, uint32_t ctl);

#endif
