#include "fp2.h"
#include <encoded_sizes.h>
#include <assert.h>
#include <inttypes.h>

/* Arithmetic modulo X^2 + 1 */

void
fp2_encode(void *dst, const fp2_t *a)
{
    uint8_t *buf = dst;
    fp_encode(buf, &(a->re));
    fp_encode(buf + FP_ENCODED_BYTES, &(a->im));
}

uint32_t
fp2_decode(fp2_t *d, const void *src)
{
    const uint8_t *buf = src;
    uint32_t re, im;

    re = fp_decode(&(d->re), buf);
    im = fp_decode(&(d->im), buf + FP_ENCODED_BYTES);
    return re & im;
}

void
fp2_mul_by_i(fp2_t *x, const fp2_t *y, uint32_t ctl)
{
    fp_t t0, t1;

    fp_neg(&t0, &(y->re));
    fp_neg(&t1, &(y->im));
    fp_select(&(x->re), &(y->im), &t1, -ctl);
    fp_select(&(x->im), &t0, &(y->re), -ctl);
}

void
fp2_frob(fp2_t *out, const fp2_t *in)
{
    fp_copy(&(out->re), &(in->re));
    fp_neg(&(out->im), &(in->im));
}

void
fp2_inv(fp2_t *x)
{
    fp_t t0, t1;

    fp_sqr(&t0, &(x->re));
    fp_sqr(&t1, &(x->im));
    fp_add(&t0, &t0, &t1);
    fp_inv(&t0);
    fp_mul(&(x->re), &(x->re), &t0);
    fp_mul(&(x->im), &(x->im), &t0);
    fp_neg(&(x->im), &(x->im));
}

void
fp2_batched_inv(fp2_t *x, int len)
{
    assert(len >= 1 && len <= 12);
    fp2_t t1[12], t2[12];
    fp2_t inverse;

    // x = x0,...,xn
    // t1 = x0, x0*x1, ... ,x0 * x1 * ... * xn
    t1[0] = x[0];
    for (int i = 1; i < len; i++) {
        fp2_mul(&t1[i], &t1[i - 1], &x[i]);
    }

    // inverse = 1/ (x0 * x1 * ... * xn)
    inverse = t1[len - 1];
    fp2_inv(&inverse);
    t2[0] = inverse;

    // t2 = 1/ (x0 * x1 * ... * xn), 1/ (x0 * x1 * ... * x(n-1)) , ... , 1/xO
    for (int i = 1; i < len; i++) {
        fp2_mul(&t2[i], &t2[i - 1], &x[len - i]);
    }

    x[0] = t2[len - 1];
    for (int i = 1; i < len; i++) {
        fp2_mul(&x[i], &t1[i - 1], &t2[len - i - 1]);
    }
}

uint32_t
fp2_is_square(const fp2_t *x)
{
    fp_t t0, t1;

    fp_sqr(&t0, &(x->re));
    fp_sqr(&t1, &(x->im));
    fp_add(&t0, &t0, &t1);

    return fp_is_square(&t0);
}

void
fp2_sqrt(fp2_t *a)
{
    fp_t x0, x1, t0, t1;

    /* From "Optimized One-Dimensional SQIsign Verification on Intel and Cortex-M4" by Aardal et al:
     * https://eprint.iacr.org/2024/1563 */

    // x0 = \delta = sqrt(a0^2 + a1^2).
    fp_sqr(&x0, &(a->re));
    fp_sqr(&x1, &(a->im));
    fp_add(&x0, &x0, &x1);
    fp_sqrt(&x0);
    // If a1 = 0, there is a risk of \delta = -a0, which makes x0 = 0 below.
    // In that case, we restore the value \delta = a0.
    fp_select(&x0, &x0, &(a->re), fp_is_zero(&(a->im)));
    // x0 = \delta + a0, t0 = 2 * x0.
    fp_add(&x0, &x0, &(a->re));
    fp_add(&t0, &x0, &x0);
    // x1 = t0^(p-3)/4.
    fp_copy(&x1, &t0);
    fp_exp3div4(&x1);
    // x0 = x0 * x1, x1 = x1 * a1, t1 = (2x0)^2.
    fp_mul(&x0, &x0, &x1);
    fp_mul(&x1, &x1, &(a->im));
    fp_add(&t1, &x0, &x0);
    fp_sqr(&t1, &t1);
    // If t1 = t0, return x0 + x1*i, otherwise x1 - x0*i.
    fp_sub(&t0, &t0, &t1);
    uint32_t f = fp_is_zero(&t0);
    fp_neg(&t1, &x0);
    fp_copy(&t0, &x1);
    fp_select(&a->re, &t0, &x0, f);
    fp_select(&a->im, &t1, &x1, f);
}

uint32_t
fp2_sqrt_verify(fp2_t *a)
{
    fp2_t t0, t1;

    fp2_copy(&t0, a);
    fp2_sqrt(a);
    fp2_sqr(&t1, a);

    return (fp2_is_equal(&t0, &t1));
}

// exponentiation
void
fp2_pow_vartime(fp2_t *out, const fp2_t *x, const digit_t *exp, const int size)
{
    fp2_t acc;
    digit_t bit;

    fp2_copy(&acc, x);
    fp2_set_one(out);

    // Iterate over each word of exp
    for (int j = 0; j < size; j++) {
        // Iterate over each bit of the word
        for (int i = 0; i < RADIX; i++) {
            bit = (exp[j] >> i) & 1;
            if (bit == 1) {
                fp2_mul(out, out, &acc);
            }
            fp2_sqr(&acc, &acc);
        }
    }
}

void
fp2_print(const char *name, const fp2_t *a)
{
    printf("%s0x", name);

    uint8_t buf[FP_ENCODED_BYTES];
    fp_encode(&buf, &a->re); // Encoding ensures canonical rep
    for (int i = 0; i < FP_ENCODED_BYTES; i++) {
        printf("%02x", buf[FP_ENCODED_BYTES - i - 1]);
    }

    printf(" + i*0x");

    fp_encode(&buf, &a->im);
    for (int i = 0; i < FP_ENCODED_BYTES; i++) {
        printf("%02x", buf[FP_ENCODED_BYTES - i - 1]);
    }
    printf("\n");
}

uint32_t
fp2_less_than(const fp2_t *x1, const fp2_t *x2)
{
    uint8_t buf1[FP2_ENCODED_BYTES];
    uint8_t buf2[FP2_ENCODED_BYTES];

    fp2_encode(buf1, x1);
    fp2_encode(buf2, x2);

    uint32_t is_less = 0;

    for (int i = 0; i < FP2_ENCODED_BYTES; i++) {
        uint32_t a = buf1[i];
        uint32_t b = buf2[i];

        uint32_t lt = (a - b) >> 31;
        uint32_t gt = (b - a) >> 31;
        uint32_t eq = 1 ^ (lt | gt);

        is_less = (eq & is_less) | lt;
    }

    return (uint32_t)(0U - is_less);
}
