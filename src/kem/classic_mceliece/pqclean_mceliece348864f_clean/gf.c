/*
  This file is for functions for field arithmetic
*/

#include "gf.h"

#include "params.h"

gf gf_iszero(gf a) {
    uint32_t t = a;

    t -= 1;
    t >>= 19;

    return (gf) t;
}

gf gf_add(gf in0, gf in1) {
    return in0 ^ in1;
}

gf gf_mul(gf in0, gf in1) {
    int i;

    uint32_t tmp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t;

    t0 = in0;
    t1 = in1;

    tmp = t0 * (t1 & 1);

    for (i = 1; i < GFBITS; i++) {
        tmp ^= (t0 * (t1 & (1 << i)));
    }

    t = tmp & 0x7FC000;
    tmp ^= t >> 9;
    tmp ^= t >> 12;

    t = tmp & 0x3000;
    tmp ^= t >> 9;
    tmp ^= t >> 12;

    return tmp & ((1 << GFBITS) - 1);
}

/* input: field element in */
/* return: in^2 */
static inline gf gf_sq(gf in) {
    const uint32_t B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};

    uint32_t x = in;
    uint32_t t;

    x = (x | (x << 8)) & B[3];
    x = (x | (x << 4)) & B[2];
    x = (x | (x << 2)) & B[1];
    x = (x | (x << 1)) & B[0];

    t = x & 0x7FC000;
    x ^= t >> 9;
    x ^= t >> 12;

    t = x & 0x3000;
    x ^= t >> 9;
    x ^= t >> 12;

    return x & ((1 << GFBITS) - 1);
}

gf gf_inv(gf in) {
    gf tmp_11;
    gf tmp_1111;

    gf out = in;

    out = gf_sq(out);
    tmp_11 = gf_mul(out, in); // 11

    out = gf_sq(tmp_11);
    out = gf_sq(out);
    tmp_1111 = gf_mul(out, tmp_11); // 1111

    out = gf_sq(tmp_1111);
    out = gf_sq(out);
    out = gf_sq(out);
    out = gf_sq(out);
    out = gf_mul(out, tmp_1111); // 11111111

    out = gf_sq(out);
    out = gf_sq(out);
    out = gf_mul(out, tmp_11); // 1111111111

    out = gf_sq(out);
    out = gf_mul(out, in); // 11111111111

    return gf_sq(out); // 111111111110
}

/* input: field element den, num */
/* return: (num/den) */
gf gf_frac(gf den, gf num) {
    return gf_mul(gf_inv(den), num);
}

/* input: in0, in1 in GF((2^m)^t)*/
/* output: out = in0*in1 */
void GF_mul(gf *out, gf *in0, gf *in1) {
    int i, j;

    gf prod[ SYS_T * 2 - 1 ];

    for (i = 0; i < SYS_T * 2 - 1; i++) {
        prod[i] = 0;
    }

    for (i = 0; i < SYS_T; i++) {
        for (j = 0; j < SYS_T; j++) {
            prod[i + j] ^= gf_mul(in0[i], in1[j]);
        }
    }

    //

    for (i = (SYS_T - 1) * 2; i >= SYS_T; i--) {
        prod[i - SYS_T + 3] ^= prod[i];
        prod[i - SYS_T + 1] ^= prod[i];
        prod[i - SYS_T + 0] ^= gf_mul(prod[i], (gf) 2);
    }

    for (i = 0; i < SYS_T; i++) {
        out[i] = prod[i];
    }
}
