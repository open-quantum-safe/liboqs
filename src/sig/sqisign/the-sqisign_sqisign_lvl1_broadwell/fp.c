#include <assert.h>
#include "fp.h"

const digit_t p[NWORDS_FIELD] = { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x04ffffffffffffff };
const digit_t p2[NWORDS_FIELD] = { 0xfffffffffffffffe, 0xffffffffffffffff, 0xffffffffffffffff, 0x09ffffffffffffff };

void
fp_sqrt(fp_t *x)
{
    (void)gf5248_sqrt(x, x);
}

uint32_t
fp_is_square(const fp_t *a)
{
    // ls is (0, 1, -1) and we want fp_is_square
    // to return 0xFF..FF when ls is 1 or 0 and 0x00..00 otherwise
    int32_t ls = gf5248_legendre(a);
    return ~(uint32_t)(ls >> 1);
}

void
fp_inv(fp_t *x)
{
    (void)gf5248_invert(x, x);
}

void
fp_exp3div4(fp_t *a)
{
    //
    // We optimise this by using the shape of the prime
    // to avoid almost all multiplications:
    //
    // We write:
    //     (p - 3) / 4 = (5*2^248 - 4) / 4
    //                 = 5*2^246 - 1
    //                 = 5*(2^246 - 1) + 4
    // Then we first compute:
    //     a246 = a**(2^246 - 1)
    // Then from this we get the desired result as:
    //     a**((p-3)/4) = a246**5 * a**4
    // We can compute this with 12 multiplications and 247 squares.
    fp_t z4, t3, t6, tmp;
    // Compute a**3 and a**4
    fp_sqr(&z4, a);
    fp_mul(&tmp, a, &z4);
    fp_sqr(&z4, &z4);
    // Compute a**(2^3 - 1) = a**7
    fp_mul(&t3, &tmp, &z4);
    // Compute a**(2^6 - 1)
    fp_sqr(&t6, &t3);
    for (int i = 1; i < 3; i++)
        fp_sqr(&t6, &t6);
    fp_mul(&t6, &t6, &t3);
    // Compute a**(2^12 - 1)
    fp_sqr(a, &t6);
    for (int i = 1; i < 6; i++)
        fp_sqr(a, a);
    fp_mul(a, a, &t6);
    // Compute a**(2^15 - 1)
    for (int i = 0; i < 3; i++)
        fp_sqr(a, a);
    fp_mul(a, a, &t3);
    // Compute a**(2^30 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 15; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^60 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 30; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^120 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 60; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^123 - 1)
    for (int i = 0; i < 3; i++)
        fp_sqr(a, a);
    fp_mul(a, a, &t3);
    // Compute a**(2^246 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 123; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(5*(2^246 - 1))
    fp_sqr(&tmp, a);
    fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(5*(2^246 - 1) + 4)
    fp_mul(a, a, &z4);
}
