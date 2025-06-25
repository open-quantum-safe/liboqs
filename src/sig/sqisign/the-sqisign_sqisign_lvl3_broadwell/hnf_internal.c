#include "hnf_internal.h"
#include "internal.h"

// Small helper for integers
void
ibz_mod_not_zero(ibz_t *res, const ibz_t *x, const ibz_t *mod)
{
    ibz_t m, t;
    ibz_init(&m);
    ibz_init(&t);
    ibz_mod(&m, x, mod);
    ibz_set(&t, ibz_is_zero(&m));
    ibz_mul(&t, &t, mod);
    ibz_add(res, &m, &t);
    ibz_finalize(&m);
    ibz_finalize(&t);
}

// centered and rather positive then negative
void
ibz_centered_mod(ibz_t *remainder, const ibz_t *a, const ibz_t *mod)
{
    assert(ibz_cmp(mod, &ibz_const_zero) > 0);
    ibz_t tmp, d, t;
    ibz_init(&tmp);
    ibz_init(&d);
    ibz_init(&t);
    ibz_div_floor(&d, &tmp, mod, &ibz_const_two);
    ibz_mod_not_zero(&tmp, a, mod);
    ibz_set(&t, ibz_cmp(&tmp, &d) > 0);
    ibz_mul(&t, &t, mod);
    ibz_sub(remainder, &tmp, &t);
    ibz_finalize(&tmp);
    ibz_finalize(&d);
    ibz_finalize(&t);
}

// if c, res = x, else res = y
void
ibz_conditional_assign(ibz_t *res, const ibz_t *x, const ibz_t *y, int c)
{
    ibz_t s, t, r;
    ibz_init(&r);
    ibz_init(&s);
    ibz_init(&t);
    ibz_set(&s, c != 0);
    ibz_sub(&t, &ibz_const_one, &s);
    ibz_mul(&r, &s, x);
    ibz_mul(res, &t, y);
    ibz_add(res, &r, res);
    ibz_finalize(&r);
    ibz_finalize(&s);
    ibz_finalize(&t);
}

// mpz_gcdext specification specifies unique outputs used here
void
ibz_xgcd_with_u_not_0(ibz_t *d, ibz_t *u, ibz_t *v, const ibz_t *x, const ibz_t *y)
{
    if (ibz_is_zero(x) & ibz_is_zero(y)) {
        ibz_set(d, 1);
        ibz_set(u, 1);
        ibz_set(v, 0);
        return;
    }
    ibz_t q, r, x1, y1;
    ibz_init(&q);
    ibz_init(&r);
    ibz_init(&x1);
    ibz_init(&y1);
    ibz_copy(&x1, x);
    ibz_copy(&y1, y);

    // xgcd
    ibz_xgcd(d, u, v, &x1, &y1);

    // make sure u!=0 (v can be 0 if needed)
    // following GMP specification, u == 0 implies y|x
    if (ibz_is_zero(u)) {
        if (!ibz_is_zero(&x1)) {
            if (ibz_is_zero(&y1)) {
                ibz_set(&y1, 1);
            }
            ibz_div(&q, &r, &x1, &y1);
            assert(ibz_is_zero(&r));
            ibz_sub(v, v, &q);
        }
        ibz_set(u, 1);
    }
    if (!ibz_is_zero(&x1)) {
        // Make sure ux > 0 (and as small as possible)
        assert(ibz_cmp(d, &ibz_const_zero) > 0);
        ibz_mul(&r, &x1, &y1);
        int neg = ibz_cmp(&r, &ibz_const_zero) < 0;
        ibz_mul(&q, &x1, u);
        while (ibz_cmp(&q, &ibz_const_zero) <= 0) {
            ibz_div(&q, &r, &y1, d);
            assert(ibz_is_zero(&r));
            if (neg) {
                ibz_neg(&q, &q);
            }
            ibz_add(u, u, &q);
            ibz_div(&q, &r, &x1, d);
            assert(ibz_is_zero(&r));
            if (neg) {
                ibz_neg(&q, &q);
            }
            ibz_sub(v, v, &q);

            ibz_mul(&q, &x1, u);
        }
    }

#ifndef NDEBUG
    int res = 0;
    ibz_t sum, prod, test, cmp;
    ibz_init(&sum);
    ibz_init(&prod);
    ibz_init(&cmp);
    ibz_init(&test);
    // sign correct
    res = res | !(ibz_cmp(d, &ibz_const_zero) >= 0);
    if (ibz_is_zero(&x1) && ibz_is_zero(&y1)) {
        res = res | !(ibz_is_zero(v) && ibz_is_one(u) && ibz_is_one(d));
    } else {
        if (!ibz_is_zero(&x1) && !ibz_is_zero(&y1)) {
            // GCD divides x
            ibz_div(&sum, &prod, &x1, d);
            res = res | !ibz_is_zero(&prod);
            // Small enough
            ibz_mul(&prod, &x1, u);
            res = res | !(ibz_cmp(&prod, &ibz_const_zero) > 0);
            ibz_mul(&sum, &sum, &y1);
            ibz_abs(&sum, &sum);
            res = res | !(ibz_cmp(&prod, &sum) <= 0);

            // GCD divides y
            ibz_div(&sum, &prod, &y1, d);
            res = res | !ibz_is_zero(&prod);
            // Small enough
            ibz_mul(&prod, &y1, v);
            res = res | !(ibz_cmp(&prod, &ibz_const_zero) <= 0);
            ibz_mul(&sum, &sum, &x1);
            ibz_abs(&sum, &sum);
            res = res | !(ibz_cmp(&prod, &sum) < 0);
        } else {
            // GCD divides x
            ibz_div(&sum, &prod, &x1, d);
            res = res | !ibz_is_zero(&prod);
            // GCD divides y
            ibz_div(&sum, &prod, &y1, d);
            res = res | !ibz_is_zero(&prod);
            if (ibz_is_zero(&x1) && !ibz_is_zero(&y1)) {
                ibz_abs(&prod, v);
                res = res | !(ibz_is_one(&prod));
                res = res | !(ibz_is_one(u));
            } else {
                ibz_abs(&prod, u);
                res = res | !(ibz_is_one(&prod));
                res = res | !(ibz_is_zero(v));
            }
        }

        // Bezout coeffs
        ibz_mul(&sum, &x1, u);
        ibz_mul(&prod, &y1, v);
        ibz_add(&sum, &sum, &prod);
        res = res | !(ibz_cmp(&sum, d) == 0);
    }
    assert(!res);
    ibz_finalize(&sum);
    ibz_finalize(&prod);
    ibz_finalize(&cmp);
    ibz_finalize(&test);

#endif

    ibz_finalize(&x1);
    ibz_finalize(&y1);
    ibz_finalize(&q);
    ibz_finalize(&r);
}
