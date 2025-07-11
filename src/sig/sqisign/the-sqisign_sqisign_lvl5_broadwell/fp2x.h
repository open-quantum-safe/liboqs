#ifndef FP2X_H
#define FP2X_H

#include <sqisign_namespace.h>
#include "fp.h"
#include <stdio.h>

// Structure for representing elements in GF(p^2)
typedef struct fp2_t
{
    fp_t re, im;
} fp2_t;

static inline void
fp2_set_small(fp2_t *x, const uint32_t val)
{
    fp_set_small(&(x->re), val);
    fp_set_zero(&(x->im));
}

static inline void
fp2_mul_small(fp2_t *x, const fp2_t *y, uint32_t n)
{
    fp_mul_small(&x->re, &y->re, n);
    fp_mul_small(&x->im, &y->im, n);
}

static inline void
fp2_set_zero(fp2_t *x)
{
    fp_set_zero(&(x->re));
    fp_set_zero(&(x->im));
}

static inline void
fp2_set_one(fp2_t *x)
{
    fp_set_one(&(x->re));
    fp_set_zero(&(x->im));
}

static inline uint32_t
fp2_is_equal(const fp2_t *a, const fp2_t *b)
{ // Compare two GF(p^2) elements in constant time
  // Returns 1 (true) if a=b, 0 (false) otherwise

    return fp_is_equal(&(a->re), &(b->re)) & fp_is_equal(&(a->im), &(b->im));
}

static inline uint32_t
fp2_is_zero(const fp2_t *a)
{ // Is a GF(p^2) element zero?
  // Returns 1 (true) if a=0, 0 (false) otherwise

    return fp_is_zero(&(a->re)) & fp_is_zero(&(a->im));
}

static inline uint32_t
fp2_is_one(const fp2_t *a)
{ // Is a GF(p^2) element one?
  // Returns 1 (true) if a=0, 0 (false) otherwise
    return fp_is_equal(&(a->re), &ONE) & fp_is_zero(&(a->im));
}

static inline void
fp2_half(fp2_t *x, const fp2_t *y)
{
    fp_half(&(x->re), &(y->re));
    fp_half(&(x->im), &(y->im));
}

static inline void
fp2_add(fp2_t *x, const fp2_t *y, const fp2_t *z)
{
    fp_add(&(x->re), &(y->re), &(z->re));
    fp_add(&(x->im), &(y->im), &(z->im));
}

static inline void
fp2_add_one(fp2_t *x, const fp2_t *y)
{
    fp_add(&x->re, &y->re, &ONE);
    fp_copy(&x->im, &y->im);
}

static inline void
fp2_sub(fp2_t *x, const fp2_t *y, const fp2_t *z)
{
    fp_sub(&(x->re), &(y->re), &(z->re));
    fp_sub(&(x->im), &(y->im), &(z->im));
}

static inline void
fp2_neg(fp2_t *x, const fp2_t *y)
{
    fp_neg(&(x->re), &(y->re));
    fp_neg(&(x->im), &(y->im));
}

#ifndef NO_FP2X_MUL
static inline void
fp2_mul(fp2_t *x, const fp2_t *y, const fp2_t *z)
{
    fp_t t0, t1;

    fp_add(&t0, &(y->re), &(y->im));
    fp_add(&t1, &(z->re), &(z->im));
    fp_mul(&t0, &t0, &t1);
    fp_mul(&t1, &(y->im), &(z->im));
    fp_mul(&(x->re), &(y->re), &(z->re));
    fp_sub(&(x->im), &t0, &t1);
    fp_sub(&(x->im), &(x->im), &(x->re));
    fp_sub(&(x->re), &(x->re), &t1);
}
#endif

#ifndef NO_FP2X_SQR
static inline void
fp2_sqr(fp2_t *x, const fp2_t *y)
{
    fp_t sum, diff;

    fp_add(&sum, &(y->re), &(y->im));
    fp_sub(&diff, &(y->re), &(y->im));
    fp_mul(&(x->im), &(y->re), &(y->im));
    fp_add(&(x->im), &(x->im), &(x->im));
    fp_mul(&(x->re), &sum, &diff);
}
#endif

static inline void
fp2_select(fp2_t *d, const fp2_t *a0, const fp2_t *a1, uint32_t ctl)
{
    fp_select(&(d->re), &(a0->re), &(a1->re), ctl);
    fp_select(&(d->im), &(a0->im), &(a1->im), ctl);
}

static inline void
fp2_cswap(fp2_t *a, fp2_t *b, uint32_t ctl)
{
    fp_cswap(&(a->re), &(b->re), ctl);
    fp_cswap(&(a->im), &(b->im), ctl);
}

static inline void
fp2_copy(fp2_t *x, const fp2_t *y)
{
    *x = *y;
}

// New functions
void fp2_encode(void *dst, const fp2_t *a);
uint32_t fp2_decode(fp2_t *d, const void *src);
void fp2_inv(fp2_t *x);
uint32_t fp2_is_square(const fp2_t *x);
void fp2_sqrt(fp2_t *x);
uint32_t fp2_sqrt_verify(fp2_t *a);
void fp2_batched_inv(fp2_t *x, int len);
void fp2_pow_vartime(fp2_t *out, const fp2_t *x, const uint64_t *exp, const int size);
void fp2_print(const char *name, const fp2_t *a);

#endif
