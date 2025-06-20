#ifndef MINI_GMP_EXTRA_H
#define MINI_GMP_EXTRA_H

#if defined MINI_GMP
#include "mini-gmp.h"

typedef long mp_exp_t;

int mpz_legendre(const mpz_t a, const mpz_t p);
double mpz_get_d_2exp(signed long int *exp, const mpz_t op);
#else
// This configuration is used only for testing
#include <gmp.h>
#endif

int mini_mpz_legendre(const mpz_t a, const mpz_t p);
double mini_mpz_get_d_2exp(signed long int *exp, const mpz_t op);

#endif
