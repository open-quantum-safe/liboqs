#ifndef FP2_H
#define FP2_H

#define NO_FP2X_MUL
#define NO_FP2X_SQR

#include <fp2x.h>

extern void fp2_sq_c0(fp2_t *out, const fp2_t *in);
extern void fp2_sq_c1(fp_t *out, const fp2_t *in);

extern void fp2_mul_c0(fp_t *out, const fp2_t *in0, const fp2_t *in1);
extern void fp2_mul_c1(fp_t *out, const fp2_t *in0, const fp2_t *in1);

static inline void
fp2_mul(fp2_t *x, const fp2_t *y, const fp2_t *z)
{
    fp_t t;

    fp2_mul_c0(&t, y, z);     // c0 = a0*b0 - a1*b1
    fp2_mul_c1(&x->im, y, z); // c1 = a0*b1 + a1*b0
    x->re.arr[0] = t.arr[0];
    x->re.arr[1] = t.arr[1];
    x->re.arr[2] = t.arr[2];
    x->re.arr[3] = t.arr[3];
}

static inline void
fp2_sqr(fp2_t *x, const fp2_t *y)
{
    fp2_t t;

    fp2_sq_c0(&t, y);     // c0 = (a0+a1)(a0-a1)
    fp2_sq_c1(&x->im, y); // c1 = 2a0*a1
    x->re.arr[0] = t.re.arr[0];
    x->re.arr[1] = t.re.arr[1];
    x->re.arr[2] = t.re.arr[2];
    x->re.arr[3] = t.re.arr[3];
}

#endif