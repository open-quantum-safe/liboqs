/*
  This file is for the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857
*/

#include "bm.h"

#include "gf.h"
#include "util.h"

#include <stdint.h>

static inline uint64_t mask_nonzero(gf a) {
    uint64_t ret = a;

    ret -= 1;
    ret >>= 63;
    ret -= 1;

    return ret;
}

static inline uint64_t mask_leq(uint16_t a, uint16_t b) {
    uint64_t a_tmp = a;
    uint64_t b_tmp = b;
    uint64_t ret = b_tmp - a_tmp;

    ret >>= 63;
    ret -= 1;

    return ret;
}

static inline void vec_cmov(vec *out, const vec *in, uint16_t mask) {
    int i;

    vec m0, m1;

    m0 = PQCLEAN_MCELIECE348864_VEC_vec_set1_16b(mask);
    m1 = ~m0;

    for (i = 0; i < GFBITS; i++) {
        out[i] = (in[i] & m0) | (out[i] & m1);
        out[i] = (in[i] & m0) | (out[i] & m1);
    }
}

static inline void interleave(vec *in, int idx0, int idx1, const vec *mask, int b) {
    int s = 1 << b;

    vec x, y;

    x = (in[idx0] & mask[0]) | ((in[idx1] & mask[0]) << s);
    y = ((in[idx0] & mask[1]) >> s) | (in[idx1] & mask[1]);

    in[idx0] = x;
    in[idx1] = y;
}

/* input: in, field elements in bitsliced form */
/* output: out, field elements in non-bitsliced form */
static inline void get_coefs(gf *out, const vec *in) {
    int i, k;

    vec mask[4][2];
    vec buf[16];

    for (i =  0; i < GFBITS; i++) {
        buf[i] = in[i];
    }
    for (i = GFBITS; i < 16; i++) {
        buf[i] = 0;
    }

    mask[0][0] = PQCLEAN_MCELIECE348864_VEC_vec_set1_16b(0x5555);
    mask[0][1] = PQCLEAN_MCELIECE348864_VEC_vec_set1_16b(0xAAAA);
    mask[1][0] = PQCLEAN_MCELIECE348864_VEC_vec_set1_16b(0x3333);
    mask[1][1] = PQCLEAN_MCELIECE348864_VEC_vec_set1_16b(0xCCCC);
    mask[2][0] = PQCLEAN_MCELIECE348864_VEC_vec_set1_16b(0x0F0F);
    mask[2][1] = PQCLEAN_MCELIECE348864_VEC_vec_set1_16b(0xF0F0);
    mask[3][0] = PQCLEAN_MCELIECE348864_VEC_vec_set1_16b(0x00FF);
    mask[3][1] = PQCLEAN_MCELIECE348864_VEC_vec_set1_16b(0xFF00);

    interleave(buf,  0,  8, mask[3], 3);
    interleave(buf,  1,  9, mask[3], 3);
    interleave(buf,  2, 10, mask[3], 3);
    interleave(buf,  3, 11, mask[3], 3);
    interleave(buf,  4, 12, mask[3], 3);
    interleave(buf,  5, 13, mask[3], 3);
    interleave(buf,  6, 14, mask[3], 3);
    interleave(buf,  7, 15, mask[3], 3);

    interleave(buf,  0,  4, mask[2], 2);
    interleave(buf,  1,  5, mask[2], 2);
    interleave(buf,  2,  6, mask[2], 2);
    interleave(buf,  3,  7, mask[2], 2);
    interleave(buf,  8, 12, mask[2], 2);
    interleave(buf,  9, 13, mask[2], 2);
    interleave(buf, 10, 14, mask[2], 2);
    interleave(buf, 11, 15, mask[2], 2);

    interleave(buf,  0,  2, mask[1], 1);
    interleave(buf,  1,  3, mask[1], 1);
    interleave(buf,  4,  6, mask[1], 1);
    interleave(buf,  5,  7, mask[1], 1);
    interleave(buf,  8, 10, mask[1], 1);
    interleave(buf,  9, 11, mask[1], 1);
    interleave(buf, 12, 14, mask[1], 1);
    interleave(buf, 13, 15, mask[1], 1);

    interleave(buf,  0,  1, mask[0], 0);
    interleave(buf,  2,  3, mask[0], 0);
    interleave(buf,  4,  5, mask[0], 0);
    interleave(buf,  6,  7, mask[0], 0);
    interleave(buf,  8,  9, mask[0], 0);
    interleave(buf, 10, 11, mask[0], 0);
    interleave(buf, 12, 13, mask[0], 0);
    interleave(buf, 14, 15, mask[0], 0);

    for (i = 0; i < 16; i++) {
        for (k = 0; k <  4; k++) {
            out[ k * 16 + i ] = (buf[i] >> (k * 16)) & GFMASK;
        }
    }
}

static inline gf vec_reduce(const vec *in) {
    int i;
    vec tmp;
    gf ret = 0;

    for (i = GFBITS - 1; i >= 0; i--) {
        tmp = in[i];

        tmp ^= tmp >> 32;
        tmp ^= tmp >> 16;
        tmp ^= tmp >> 8;
        tmp ^= tmp >> 4;
        tmp ^= tmp >> 2;
        tmp ^= tmp >> 1;

        ret <<= 1;
        ret |= tmp & 1;
    }

    return ret;
}

static void update(vec *in, const gf e) {
    int i;
    vec tmp;

    for (i = 0; i < GFBITS; i++) {
        tmp = (e >> i) & 1;

        in[i] = (in[i] >> 1) | (tmp << 63);
    }
}

/* input: in, sequence of field elements */
/* output: out, minimal polynomial of in */
void PQCLEAN_MCELIECE348864_VEC_bm(vec *out, vec in[][ GFBITS ]) {
    uint16_t i;
    uint16_t N, L;

    vec prod[ GFBITS ];
    vec in_tmp[ GFBITS ];

    vec d_vec[ GFBITS ];
    vec b_vec[ GFBITS ];
    vec B[ GFBITS ], C[ GFBITS ];
    vec B_tmp[ GFBITS ], C_tmp[ GFBITS ];

    vec mask, t;

    gf d, b, c0 = 1;

    gf coefs[SYS_T * 2];

    // init

    get_coefs(&coefs[  0], in[0]);
    get_coefs(&coefs[ 64], in[1]);

    C[0] = 0;
    B[0] = 1;
    B[0] <<= 63;

    for (i = 1; i < GFBITS; i++) {
        B[i] = C[i] = 0;
    }

    b = 1;
    L = 0;

    //

    for (i = 0; i < GFBITS; i++) {
        in_tmp[i] = 0;
    }

    for (N = 0; N < SYS_T * 2; N++) {
        // computing d

        PQCLEAN_MCELIECE348864_VEC_vec_mul(prod, in_tmp, C);

        update(in_tmp, coefs[N]);

        d = vec_reduce(prod);

        t = PQCLEAN_MCELIECE348864_VEC_gf_mul2(c0, coefs[N], b);
        d ^= t & 0xFFFFFFFF;

        // 3 cases

        mask = mask_nonzero(d) & mask_leq(L * 2, N);

        for (i = 0; i < GFBITS; i++) {
            d_vec[i] = PQCLEAN_MCELIECE348864_VEC_vec_setbits((d >> i) & 1);
            b_vec[i] = PQCLEAN_MCELIECE348864_VEC_vec_setbits((b >> i) & 1);
        }

        PQCLEAN_MCELIECE348864_VEC_vec_mul(B_tmp, d_vec, B);
        PQCLEAN_MCELIECE348864_VEC_vec_mul(C_tmp, b_vec, C);

        vec_cmov(B, C, (uint16_t)mask);
        update(B, mask & c0);

        for (i = 0; i < GFBITS; i++) {
            C[i] = B_tmp[i] ^ C_tmp[i];
        }

        c0 = (gf)(t >> 32);
        b = (d & mask) | (b & ~mask);
        L = ((N + 1 - L) & mask) | (L & ~mask);

    }

    c0 = PQCLEAN_MCELIECE348864_VEC_gf_inv(c0);

    for (i = 0; i < GFBITS; i++) {
        out[i] = PQCLEAN_MCELIECE348864_VEC_vec_setbits((c0 >> i) & 1);
    }

    PQCLEAN_MCELIECE348864_VEC_vec_mul(out, out, C);
}

