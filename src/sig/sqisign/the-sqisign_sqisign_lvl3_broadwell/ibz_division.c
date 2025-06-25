#include "hnf_internal.h"
#if defined(MINI_GMP)
#include <mini-gmp.h>
#else
#include <gmp.h>
#endif

void
ibz_xgcd(ibz_t *gcd, ibz_t *u, ibz_t *v, const ibz_t *a, const ibz_t *b)
{
    mpz_gcdext(*gcd, *u, *v, *a, *b);
}