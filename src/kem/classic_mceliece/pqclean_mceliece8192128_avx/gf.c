/*
  this file is for functions for field arithmetic
*/

#include "gf.h"

#include "params.h"

#include <stdio.h>

/* 2 field multiplications */
uint64_t PQCLEAN_MCELIECE8192128_AVX_gf_mul2(gf a, gf b0, gf b1) {
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

/* field multiplication */
gf PQCLEAN_MCELIECE8192128_AVX_gf_mul(gf in0, gf in1) {
    int i;

    uint64_t tmp;
    uint64_t t0;
    uint64_t t1;
    uint64_t t;

    t0 = in0;
    t1 = in1;

    tmp = t0 * (t1 & 1);

    for (i = 1; i < GFBITS; i++) {
        tmp ^= (t0 * (t1 & ((uint64_t)1 << i)));
    }

    //

    t = tmp & 0x1FF0000;
    tmp ^= (t >> 9) ^ (t >> 10) ^ (t >> 12) ^ (t >> 13);

    t = tmp & 0x000E000;
    tmp ^= (t >> 9) ^ (t >> 10) ^ (t >> 12) ^ (t >> 13);

    return tmp & GFMASK;
}

/* 2 field squarings */
static inline gf gf_sq2(gf in) {
    int i;

    const uint64_t B[] = {0x1111111111111111,
                          0x0303030303030303,
                          0x000F000F000F000F,
                          0x000000FF000000FF
                         };

    const uint64_t M[] = {0x0001FF0000000000,
                          0x000000FF80000000,
                          0x000000007FC00000,
                          0x00000000003FE000
                         };

    uint64_t x = in;
    uint64_t t;

    x = (x | (x << 24)) & B[3];
    x = (x | (x << 12)) & B[2];
    x = (x | (x << 6)) & B[1];
    x = (x | (x << 3)) & B[0];

    for (i = 0; i < 4; i++) {
        t = x & M[i];
        x ^= (t >> 9) ^ (t >> 10) ^ (t >> 12) ^ (t >> 13);
    }

    return x & GFMASK;
}

/* square and multiply */
static inline gf gf_sqmul(gf in, gf m) {
    int i;

    uint64_t x;
    uint64_t t0;
    uint64_t t1;
    uint64_t t;

    const uint64_t M[] = {0x0000001FF0000000,
                          0x000000000FF80000,
                          0x000000000007E000
                         };

    t0 = in;
    t1 = m;

    x = (t1 << 6) * (t0 & (1 << 6));

    t0 ^= (t0 << 7);

    x ^= (t1 * (t0 & (0x04001)));
    x ^= (t1 * (t0 & (0x08002))) << 1;
    x ^= (t1 * (t0 & (0x10004))) << 2;
    x ^= (t1 * (t0 & (0x20008))) << 3;
    x ^= (t1 * (t0 & (0x40010))) << 4;
    x ^= (t1 * (t0 & (0x80020))) << 5;

    for (i = 0; i < 3; i++) {
        t = x & M[i];
        x ^= (t >> 9) ^ (t >> 10) ^ (t >> 12) ^ (t >> 13);
    }

    return x & GFMASK;
}

/* square twice and multiply */
static inline gf gf_sq2mul(gf in, gf m) {
    int i;

    uint64_t x;
    uint64_t t0;
    uint64_t t1;
    uint64_t t;

    const uint64_t M[] = {0x1FF0000000000000,
                          0x000FF80000000000,
                          0x000007FC00000000,
                          0x00000003FE000000,
                          0x0000000001FE0000,
                          0x000000000001E000
                         };

    t0 = in;
    t1 = m;

    x = (t1 << 18) * (t0 & (1 << 6));

    t0 ^= (t0 << 21);

    x ^= (t1 * (t0 & (0x010000001)));
    x ^= (t1 * (t0 & (0x020000002))) << 3;
    x ^= (t1 * (t0 & (0x040000004))) << 6;
    x ^= (t1 * (t0 & (0x080000008))) << 9;
    x ^= (t1 * (t0 & (0x100000010))) << 12;
    x ^= (t1 * (t0 & (0x200000020))) << 15;

    for (i = 0; i < 6; i++) {
        t = x & M[i];
        x ^= (t >> 9) ^ (t >> 10) ^ (t >> 12) ^ (t >> 13);
    }

    return x & GFMASK;
}

/* return num/den */
gf PQCLEAN_MCELIECE8192128_AVX_gf_frac(gf den, gf num) {
    gf tmp_11;
    gf tmp_1111;
    gf out;

    tmp_11 = gf_sqmul(den, den); // 11
    tmp_1111 = gf_sq2mul(tmp_11, tmp_11); // 1111
    out = gf_sq2(tmp_1111);
    out = gf_sq2mul(out, tmp_1111); // 11111111
    out = gf_sq2(out);
    out = gf_sq2mul(out, tmp_1111); // 111111111111

    return gf_sqmul(out, num); // 1111111111110
}

/* return 1/den */
gf PQCLEAN_MCELIECE8192128_AVX_gf_inv(gf in) {
    return PQCLEAN_MCELIECE8192128_AVX_gf_frac(in, ((gf) 1));
}

/* check if a == 0 */
gf PQCLEAN_MCELIECE8192128_AVX_gf_iszero(gf a) {
    uint32_t t = a;

    t -= 1;
    t >>= 19;

    return (gf) t;
}

/* multiplication in GF((2^m)^t) */
void PQCLEAN_MCELIECE8192128_AVX_GF_mul(gf *out, const gf *in0, const gf *in1) {
    int i, j;

    gf prod[255];

    for (i = 0; i < 255; i++) {
        prod[i] = 0;
    }

    for (i = 0; i < 128; i++) {
        for (j = 0; j < 128; j++) {
            prod[i + j] ^= PQCLEAN_MCELIECE8192128_AVX_gf_mul(in0[i], in1[j]);
        }
    }

    //

    for (i = 254; i >= 128; i--) {
        prod[i - 123] ^= PQCLEAN_MCELIECE8192128_AVX_gf_mul(prod[i], (gf) 7682);
        prod[i - 125] ^= PQCLEAN_MCELIECE8192128_AVX_gf_mul(prod[i], (gf) 2159);
        prod[i - 128] ^= PQCLEAN_MCELIECE8192128_AVX_gf_mul(prod[i], (gf) 3597);
    }

    for (i = 0; i < 128; i++) {
        out[i] = prod[i];
    }
}

