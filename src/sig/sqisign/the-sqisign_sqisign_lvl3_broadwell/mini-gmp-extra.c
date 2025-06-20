#include <float.h>
#include <math.h>
#if defined(MINI_GMP)
#include "mini-gmp.h"
#else
// This configuration is used only for testing
#include <gmp.h>
#endif
#include <tutil.h>

// Exported for testing
int
mini_mpz_legendre(const mpz_t a, const mpz_t p)
{
    int res = 0;
    mpz_t e;
    mpz_init_set(e, p);
    mpz_sub_ui(e, e, 1);
    mpz_fdiv_q_2exp(e, e, 1);
    mpz_powm(e, a, e, p);

    if (mpz_cmp_ui(e, 1) <= 0) {
        res = mpz_get_si(e);
    } else {
        res = -1;
    }
    mpz_clear(e);
    return res;
}

#if defined(MINI_GMP)
int
mpz_legendre(const mpz_t a, const mpz_t p)
{
    return mini_mpz_legendre(a, p);
}
#endif

// Exported for testing
double
mini_mpz_get_d_2exp(signed long int *exp, const mpz_t op)
{
    double ret;
    int tmp_exp;
    mpz_t tmp;

    // Handle the case where op is 0
    if (mpz_cmp_ui(op, 0) == 0) {
        *exp = 0;
        return 0.0;
    }

    *exp = mpz_sizeinbase(op, 2);

    mpz_init_set(tmp, op);

    if (*exp > DBL_MAX_EXP) {
        mpz_fdiv_q_2exp(tmp, tmp, *exp - DBL_MAX_EXP);
    }

    ret = frexp(mpz_get_d(tmp), &tmp_exp);
    mpz_clear(tmp);

    return ret;
}

#if defined(MINI_GMP)
double
mpz_get_d_2exp(signed long int *exp, const mpz_t op)
{
    return mini_mpz_get_d_2exp(exp, op);
}
#endif
