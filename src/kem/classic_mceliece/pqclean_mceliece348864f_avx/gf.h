#ifndef PQCLEAN_MCELIECE348864F_AVX_GF_H
#define PQCLEAN_MCELIECE348864F_AVX_GF_H
/*
  This file is for functions for field arithmetic
*/


#include "params.h"

#include <stdint.h>

typedef uint16_t gf;

gf PQCLEAN_MCELIECE348864F_AVX_gf_iszero(gf /*a*/);
gf PQCLEAN_MCELIECE348864F_AVX_gf_add(gf /*in0*/, gf /*in1*/);
gf PQCLEAN_MCELIECE348864F_AVX_gf_mul(gf /*in0*/, gf /*in1*/);
gf PQCLEAN_MCELIECE348864F_AVX_gf_frac(gf /*den*/, gf /*num*/);
gf PQCLEAN_MCELIECE348864F_AVX_gf_inv(gf /*in*/);

void PQCLEAN_MCELIECE348864F_AVX_GF_mul(gf * /*out*/, const gf * /*in0*/, const gf * /*in1*/);

/* 2 field multiplications */
uint64_t PQCLEAN_MCELIECE348864F_AVX_gf_mul2(gf a, gf b0, gf b1);

#endif

