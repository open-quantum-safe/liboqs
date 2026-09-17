#include <quaternion.h>
#include <quaternion_constants.h>
#include "internal.h"
#include <lll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void
ibz_sum_two_squares(ibz_t *sum, const ibz_t *a, const ibz_t *b)
{
    ibz_t prod;
    ibz_init(&prod);
    ibz_mul(&prod, b, b);
    ibz_mul(sum, a, a);
    ibz_add(sum, sum, &prod);
}

void
ibz_rounded_div(ibz_t *q, const ibz_t *a, const ibz_t *b)
{
    ibz_t r, sign_q, abs_b;
    ibz_init(&r);
    ibz_init(&sign_q);
    ibz_init(&abs_b);

    // ibz_div is assumed to round a/b towards 0 for positive b
    ibz_abs(&abs_b, b);
    // q is of same sign as a*b (and 0 if a is 0)
    ibz_set(&sign_q, (2 * (ibz_cmp(a, &ibz_const_zero) >= 0) - 1) * (2 * (ibz_cmp(b, &ibz_const_zero) >= 0) - 1), 2);
    ibz_div(q, &r, a, b);
    ibz_abs(&r, &r);
    ibz_add(&r, &r, &r);
    ibz_set(&sign_q, (1 - 2 * (ibz_cmp(&sign_q, &ibz_const_zero) < 0)) * (ibz_cmp(&r, &abs_b) > 0), 2);
    ibz_add(q, q, &sign_q);
}

// Random prime generation for tests
int
ibz_generate_random_prime(ibz_t *p, int is3mod4, int bitsize)
{
    assert(bitsize != 0);
    int found = 0;
    ibz_t two_pow, two_powp;

    ibz_init(&two_pow);
    ibz_init(&two_powp);
    ibz_mul_2exp(&two_pow, &ibz_const_one, (bitsize - 1) - (0 != is3mod4));
    ibz_mul_2exp(&two_powp, &ibz_const_one, bitsize - (0 != is3mod4));

    int cnt = 0;
    while (!found) {
        cnt++;
        if (cnt % 100000 == 0) {
            printf("Random prime generation is still running after %d attempts, this is not "
                   "normal! The expected number of attempts is %d \n",
                   cnt,
                   bitsize);
        }
        if (!ibz_rand_interval(p, &two_pow, &two_powp))
            break;
        ibz_mul_2exp(p, p, 1);
        if (is3mod4) {
            ibz_mul_2exp(p, p, 1);
            ibz_add(p, &ibz_const_two, p);
        }
        ibz_add(p, &ibz_const_one, p);

        found = ibz_probab_prime(p, QUAT_primality_num_iter);
    }

    if (!found) {
        ibz_set(p, 0, 0);
    }

    return (found);
}

// solves x^2 +  y^2 == p for positive integers x, y
// assumes that p is prime and -1 mod p is a square
int
ibz_cornacchia_prime(ibz_t *x, ibz_t *y, const ibz_t *p)
{
    ibz_t x2, y2, root;
    ibz_init(&root);
    ibz_init(&x2);
    ibz_init(&y2);

    // manage case p = 2 separately
    if (!ibz_cmp(p, &ibz_const_two)) {
        ibz_set(x, 1, 2);
        ibz_set(y, 1, 2);
        return (1);
    }

    // manage case p = 1 separately
    if (!ibz_cmp(p, &ibz_const_one)) {
        ibz_set(x, 0, 0);
        ibz_set(y, 1, 2);
        return (1);
    }

    // For an odd prime, a sum of two squares requires p = 1 mod 4.
    if ((ibz_get(p) & 3) != 1)
        return (0);

    if (!ibz_sqrt_m1_mod_verified(&root, p))
        return (0);

    quat_lll_dim2_sumofsquares(x, y, p, &root);
    ibz_mul(&x2, x, x);
    ibz_mul(&y2, y, y);
    ibz_add(&x2, &x2, &y2);
    if (ibz_cmp(x, y) > 0)
        ibz_swap(x, y);

    return (ibz_cmp(&x2, p) == 0);
}
