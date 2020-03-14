#ifndef PQCLEAN_MCELIECE6688128_VEC_SK_GEN_H
#define PQCLEAN_MCELIECE6688128_VEC_SK_GEN_H
/*
  This file is for secret-key generation
*/


#include "gf.h"

#include <stdint.h>

int PQCLEAN_MCELIECE6688128_VEC_genpoly_gen(gf * /*out*/, gf * /*f*/);
int PQCLEAN_MCELIECE6688128_VEC_perm_check(const uint32_t * /*p*/);

#endif

