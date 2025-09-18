#include <assert.h>
#include "fp.h"

const digit_t p[NWORDS_FIELD] = { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                  0xffffffffffffffff, 0xffffffffffffffff, 0x40ffffffffffffff };
const digit_t p2[NWORDS_FIELD] = { 0xfffffffffffffffe, 0xffffffffffffffff, 0xffffffffffffffff,
                                   0xffffffffffffffff, 0xffffffffffffffff, 0x81ffffffffffffff };

void
fp_sqrt(fp_t *x)
{
    (void)gf65376_sqrt(x, x);
}

uint32_t
fp_is_square(const fp_t *a)
{
    // ls is (0, 1, -1) and we want fp_is_square
    // to return 0xFF..FF when ls is 1 or 0 and 0x00..00 otherwise
    int32_t ls = gf65376_legendre(a);
    return ~(uint32_t)(ls >> 1);
}

void
fp_inv(fp_t *x)
{
    (void)gf65376_invert(x, x);
}

void
fp_exp3div4(fp_t *a)
{
    //
    // We optimise this by using the shape of the prime
    // to avoid almost all multiplications:
    //
    // We write:
    //     (p - 3) / 4 = (65*2^376 - 4) / 4
    //                 = 65*2^374 - 1
    //                 = 65*(2^374 - 1) + 64
    // Then we first compute:
    //     a374 = a**(2^374 - 1)
    // Then from this we get the desired result as:
    //     a**((p-3)/4) = a374**65 * a**64
    // We can compute this with 13 multiplications and 383 squares.
    fp_t z3, z64, t11, tmp;
    // Compute a**3, a**4 and a**64
    fp_sqr(&z64, a);
    fp_mul(&z3, a, &z64);
    fp_sqr(&z64, &z64);
    // Compute t11 = a^3 * a^4 = a**(2^3 - 1) = a**7
    fp_mul(&t11, &z3, &z64);
    fp_sqr(&z64, &z64);
    fp_sqr(&z64, &z64);
    fp_sqr(&z64, &z64);
    fp_sqr(&z64, &z64);
    // Compute a**(2^4 - 1) = a**15
    fp_sqr(&tmp, &t11);
    fp_mul(a, a, &tmp);
    // Compute a**(2^8 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 4; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^11 - 1)
    for (int i = 0; i < 3; i++)
        fp_sqr(a, a);
    fp_mul(&t11, &t11, a);
    // Compute a**(2^22 - 1)
    fp_sqr(&tmp, &t11);
    for (int i = 1; i < 11; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, &t11, &tmp);
    // Compute a**(2^44 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 22; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^88 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 44; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^176 - 1)'
    fp_sqr(&tmp, a);
    for (int i = 1; i < 88; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^187 - 1)
    for (int i = 0; i < 11; i++)
        fp_sqr(a, a);
    fp_mul(a, a, &t11);
    // Compute a**(2^374 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 187; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(65*(2^374 - 1))
    fp_sqr(&tmp, a);
    fp_sqr(&tmp, &tmp);
    fp_sqr(&tmp, &tmp);
    fp_sqr(&tmp, &tmp);
    fp_sqr(&tmp, &tmp);
    fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(65*(2^374 - 1) + 64)
    fp_mul(a, a, &z64);
}
