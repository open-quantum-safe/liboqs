#define update_asm CRYPTO_NAMESPACE(update_asm)
#define _update_asm _CRYPTO_NAMESPACE(update_asm)
#define vec_reduce_asm CRYPTO_NAMESPACE(vec_reduce_asm)
#define _vec_reduce_asm _CRYPTO_NAMESPACE(vec_reduce_asm)
/*
  This file is for implementating the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857

  For the implementation strategy, see
  https://eprint.iacr.org/2017/793.pdf
*/

#include "bm.h"

#include "gf.h"
#include "util.h"
#include "vec.h"
#include "vec128.h"

#include <assert.h>
#include <stdint.h>

extern void update_asm(void *, gf, int);
extern gf vec_reduce_asm(uint64_t *);

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

static inline void vec_cmov(uint64_t out[][2], uint64_t mask) {
    int i;

    for (i = 0; i < GFBITS; i++) {
        out[i][0] = (out[i][0] & ~mask) | (out[i][1] & mask);
    }
}

static inline void interleave(vec128 *in, int idx0, int idx1, vec128 *mask, int b) {
    int s = 1 << b;

    vec128 x, y;

    x = vec128_or(vec128_and(in[idx0], mask[0]),
                  vec128_sll_2x(vec128_and(in[idx1], mask[0]), s));

    y = vec128_or(vec128_srl_2x(vec128_and(in[idx0], mask[1]), s),
                  vec128_and(in[idx1], mask[1]));

    in[idx0] = x;
    in[idx1] = y;
}

/* input: in, field elements in bitsliced form */
/* output: out, field elements in non-bitsliced form */
static inline void get_coefs(gf *out, vec128 *in) {
    int i, k;

    vec128 mask[4][2];
    vec128 buf[16];

    for (i =  0; i < GFBITS; i++) {
        buf[i] = in[i];
    }
    for (i = GFBITS; i < 16; i++) {
        buf[i] = vec128_setzero();
    }

    mask[0][0] = vec128_set1_16b(0x5555);
    mask[0][1] = vec128_set1_16b(0xAAAA);
    mask[1][0] = vec128_set1_16b(0x3333);
    mask[1][1] = vec128_set1_16b(0xCCCC);
    mask[2][0] = vec128_set1_16b(0x0F0F);
    mask[2][1] = vec128_set1_16b(0xF0F0);
    mask[3][0] = vec128_set1_16b(0x00FF);
    mask[3][1] = vec128_set1_16b(0xFF00);

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
            out[ (4 * 0 + k) * 16 + i ] = (vec128_extract(buf[i], 0) >> (k * 16)) & GFMASK;
            out[ (4 * 1 + k) * 16 + i ] = (vec128_extract(buf[i], 1) >> (k * 16)) & GFMASK;
        }
    }
}

/* input: in, field elements in bitsliced form */
/* output: out, field elements in non-bitsliced form */
void bm(uint64_t out[ GFBITS ], vec128 in[ GFBITS ]) {
    uint16_t i;
    uint16_t N, L;

    uint64_t prod[ GFBITS ];
    uint64_t in_tmp[ GFBITS ];

    uint64_t db[ GFBITS ][ 2 ];
    uint64_t BC_tmp[ GFBITS ][ 2 ];
    uint64_t BC[ GFBITS ][ 2 ];

    uint64_t mask, t;

    gf d, b, c0 = 1;

    gf coefs[SYS_T * 2];

    // init

    BC[0][1] = 0;
    BC[0][0] = 1;
    BC[0][0] <<= 63;

    for (i = 1; i < GFBITS; i++) {
        BC[i][0] = BC[i][1] = 0;
    }

    b = 1;
    L = 0;

    //

    get_coefs(coefs, in);

    for (i = 0; i < GFBITS; i++) {
        in_tmp[i] = 0;
    }

    for (N = 0; N < SYS_T * 2; N++) {
        // computing d

        vec_mul_sp(prod, in_tmp, &BC[0][0]);

        update_asm(in_tmp, coefs[N], 8);

        d = vec_reduce_asm(prod);

        t = gf_mul2(c0, coefs[N], b);

        d ^= t & 0xFFFFFFFF;

        // 3 cases

        mask = mask_nonzero(d) & mask_leq(L * 2, N);

        for (i = 0; i < GFBITS; i++) {
            db[i][0] = (d >> i) & 1;
            db[i][0] = -db[i][0];
            db[i][1] = (b >> i) & 1;
            db[i][1] = -db[i][1];
        }

        vec128_mul((vec128 *) BC_tmp, (vec128 *) db, (vec128 *) BC);

        vec_cmov(BC, mask);

        update_asm(BC, mask & c0, 16);

        for (i = 0; i < GFBITS; i++) {
            BC[i][1] = BC_tmp[i][0] ^ BC_tmp[i][1];
        }

        c0 = t >> 32;
        b = (d & mask) | (b & ~mask);
        L = ((N + 1 - L) & mask) | (L & ~mask);

    }

    c0 = gf_inv(c0);

    for (i = 0; i < GFBITS; i++) {
        out[i] = (c0 >> i) & 1;
        out[i] = -out[i];
    }

    vec_mul_sp(out, out, &BC[0][0]);
}
