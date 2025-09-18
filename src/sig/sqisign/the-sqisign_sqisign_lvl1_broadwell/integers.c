#include <quaternion.h>
#include "internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Random prime generation for tests
int
ibz_generate_random_prime(ibz_t *p, int is3mod4, int bitsize, int probability_test_iterations)
{
    assert(bitsize != 0);
    int found = 0;
    ibz_t two_pow, two_powp;

    ibz_init(&two_pow);
    ibz_init(&two_powp);
    ibz_pow(&two_pow, &ibz_const_two, (bitsize - 1) - (0 != is3mod4));
    ibz_pow(&two_powp, &ibz_const_two, bitsize - (0 != is3mod4));

    int cnt = 0;
    while (!found) {
        cnt++;
        if (cnt % 100000 == 0) {
            printf("Random prime generation is still running after %d attempts, this is not "
                   "normal! The expected number of attempts is %d \n",
                   cnt,
                   bitsize);
        }
        ibz_rand_interval(p, &two_pow, &two_powp);
        ibz_add(p, p, p);
        if (is3mod4) {
            ibz_add(p, p, p);
            ibz_add(p, &ibz_const_two, p);
        }
        ibz_add(p, &ibz_const_one, p);

        found = ibz_probab_prime(p, probability_test_iterations);
    }
    ibz_finalize(&two_pow);
    ibz_finalize(&two_powp);
    return found;
}

// solves x^2 + n y^2 == p for positive integers x, y
// assumes that p is prime and -n mod p is a square
int
ibz_cornacchia_prime(ibz_t *x, ibz_t *y, const ibz_t *n, const ibz_t *p)
{
    ibz_t r0, r1, r2, a, prod;
    ibz_init(&r0);
    ibz_init(&r1);
    ibz_init(&r2);
    ibz_init(&a);
    ibz_init(&prod);

    int res = 0;

    // manage case p = 2 separately
    if (!ibz_cmp(p, &ibz_const_two)) {
        if (ibz_is_one(n)) {
            ibz_set(x, 1);
            ibz_set(y, 1);
            res = 1;
        }
        goto done;
    }
    // manage case p = n separately
    if (!ibz_cmp(p, n)) {
        ibz_set(x, 0);
        ibz_set(y, 1);
        res = 1;
        goto done;
    }

    // test coprimality (should always be ok in our cases)
    ibz_gcd(&r2, p, n);
    if (!ibz_is_one(&r2))
        goto done;

    // get sqrt of -n mod p
    ibz_neg(&r2, n);
    if (!ibz_sqrt_mod_p(&r2, &r2, p))
        goto done;

    // run loop
    ibz_copy(&prod, p);
    ibz_copy(&r1, p);
    ibz_copy(&r0, p);
    while (ibz_cmp(&prod, p) >= 0) {
        ibz_div(&a, &r0, &r2, &r1);
        ibz_mul(&prod, &r0, &r0);
        ibz_copy(&r2, &r1);
        ibz_copy(&r1, &r0);
    }
    // test if result is solution
    ibz_sub(&a, p, &prod);
    ibz_div(&a, &r2, &a, n);
    if (!ibz_is_zero(&r2))
        goto done;
    if (!ibz_sqrt(y, &a))
        goto done;

    ibz_copy(x, &r0);
    ibz_mul(&a, y, y);
    ibz_mul(&a, &a, n);
    ibz_add(&prod, &prod, &a);
    res = !ibz_cmp(&prod, p);

done:
    ibz_finalize(&r0);
    ibz_finalize(&r1);
    ibz_finalize(&r2);
    ibz_finalize(&a);
    ibz_finalize(&prod);
    return res;
}
