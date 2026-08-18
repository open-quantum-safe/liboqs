#ifndef GF_H
#define GF_H
/*
  This file is for functions for field arithmetic
*/

#include "namespace.h"

#define gf_add CRYPTO_NAMESPACE(gf_add)
#define gf_frac CRYPTO_NAMESPACE(gf_frac)
#define gf_inv CRYPTO_NAMESPACE(gf_inv)
#define gf_iszero CRYPTO_NAMESPACE(gf_iszero)
#define gf_mul CRYPTO_NAMESPACE(gf_mul)
#define GF_mul CRYPTO_NAMESPACE(GF_mul)

#include <stdint.h>

typedef uint16_t gf;

gf gf_iszero(gf a);
gf gf_add(gf in0, gf in1);
gf gf_mul(gf in0, gf in1);
gf gf_frac(gf den, gf num);
gf gf_inv(gf in);

void GF_mul(gf *out, gf *in0, gf *in1);

#endif
