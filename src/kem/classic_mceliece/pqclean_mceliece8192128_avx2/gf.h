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
#define gf_mul2 CRYPTO_NAMESPACE(gf_mul2)
#define gf_mul CRYPTO_NAMESPACE(gf_mul)
#define GF_mul CRYPTO_NAMESPACE(GF_mul)

#include "params.h"

#include <stdint.h>

typedef uint16_t gf;

gf gf_iszero(gf a);
gf gf_mul(gf in0, gf in1);
gf gf_frac(gf den, gf num);
gf gf_inv(gf den);

void GF_mul(gf *out, gf *in0, gf *in1);

/* 2 field multiplications */
static inline uint64_t gf_mul2(gf a, gf b0, gf b1) {
    int i;

    uint64_t tmp = 0;
    uint64_t t0;
    uint64_t t1;
    uint64_t t;
    uint64_t mask = 0x0000000100000001;

    t0 = a;
    t1 = b1;
    t1 = (t1 << 32) | b0;

    for (i = 0; i < GFBITS; i++) {
        tmp ^= t0 * (t1 & mask);
        mask += mask;
    }

    //

    t = tmp & 0x01FF000001FF0000;
    tmp ^= (t >> 9) ^ (t >> 10) ^ (t >> 12) ^ (t >> 13);

    t = tmp & 0x0000E0000000E000;
    tmp ^= (t >> 9) ^ (t >> 10) ^ (t >> 12) ^ (t >> 13);

    return tmp & 0x00001FFF00001FFF;
}

#endif
