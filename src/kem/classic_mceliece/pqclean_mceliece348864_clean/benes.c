/*
  This file is for Benes network related functions
*/

#include "benes.h"

#include "params.h"
#include "transpose.h"
#include "util.h"

/* one layer of the benes network */
static void layer(uint64_t *data, uint64_t *bits, int lgs) {
    int i, j, s;

    uint64_t d;

    s = 1 << lgs;

    for (i = 0; i < 64; i += s * 2) {
        for (j = i; j < i + s; j++) {

            d = (data[j + 0] ^ data[j + s]);
            d &= (*bits++);
            data[j + 0] ^= d;
            data[j + s] ^= d;
        }
    }
}

/* input: r, sequence of bits to be permuted */
/*        bits, condition bits of the Benes network */
/*        rev, 0 for normal application; !0 for inverse */
/* output: r, permuted bits */
void PQCLEAN_MCELIECE348864_CLEAN_apply_benes(unsigned char *r, const unsigned char *bits, int rev) {
    int i;

    const unsigned char *cond_ptr;
    int inc, low;

    uint64_t bs[64];
    uint64_t cond[64];

    //

    for (i = 0; i < 64; i++) {
        bs[i] = PQCLEAN_MCELIECE348864_CLEAN_load8(r + i * 8);
    }

    if (rev == 0) {
        inc = 256;
        cond_ptr = bits;
    } else {
        inc = -256;
        cond_ptr = bits + (2 * GFBITS - 2) * 256;
    }

    //

    PQCLEAN_MCELIECE348864_CLEAN_transpose_64x64(bs, bs);

    for (low = 0; low <= 5; low++) {
        for (i = 0; i < 64; i++) {
            cond[i] = PQCLEAN_MCELIECE348864_CLEAN_load4(cond_ptr + i * 4);
        }
        PQCLEAN_MCELIECE348864_CLEAN_transpose_64x64(cond, cond);
        layer(bs, cond, low);
        cond_ptr += inc;
    }

    PQCLEAN_MCELIECE348864_CLEAN_transpose_64x64(bs, bs);

    for (low = 0; low <= 5; low++) {
        for (i = 0; i < 32; i++) {
            cond[i] = PQCLEAN_MCELIECE348864_CLEAN_load8(cond_ptr + i * 8);
        }
        layer(bs, cond, low);
        cond_ptr += inc;
    }
    for (low = 4; low >= 0; low--) {
        for (i = 0; i < 32; i++) {
            cond[i] = PQCLEAN_MCELIECE348864_CLEAN_load8(cond_ptr + i * 8);
        }
        layer(bs, cond, low);
        cond_ptr += inc;
    }

    PQCLEAN_MCELIECE348864_CLEAN_transpose_64x64(bs, bs);

    for (low = 5; low >= 0; low--) {
        for (i = 0; i < 64; i++) {
            cond[i] = PQCLEAN_MCELIECE348864_CLEAN_load4(cond_ptr + i * 4);
        }
        PQCLEAN_MCELIECE348864_CLEAN_transpose_64x64(cond, cond);
        layer(bs, cond, low);
        cond_ptr += inc;
    }

    PQCLEAN_MCELIECE348864_CLEAN_transpose_64x64(bs, bs);


    for (i = 0; i < 64; i++) {
        PQCLEAN_MCELIECE348864_CLEAN_store8(r + i * 8, bs[i]);
    }
}

/* input: condition bits c */
/* output: support s */
void PQCLEAN_MCELIECE348864_CLEAN_support_gen(gf *s, const unsigned char *c) {
    gf a;
    int i, j;
    unsigned char L[ GFBITS ][ (1 << GFBITS) / 8 ];

    for (i = 0; i < GFBITS; i++) {
        for (j = 0; j < (1 << GFBITS) / 8; j++) {
            L[i][j] = 0;
        }
    }

    for (i = 0; i < (1 << GFBITS); i++) {
        a = PQCLEAN_MCELIECE348864_CLEAN_bitrev((gf) i);

        for (j = 0; j < GFBITS; j++) {
            L[j][ i / 8 ] |= ((a >> j) & 1) << (i % 8);
        }
    }

    for (j = 0; j < GFBITS; j++) {
        PQCLEAN_MCELIECE348864_CLEAN_apply_benes(L[j], c, 0);
    }

    for (i = 0; i < SYS_N; i++) {
        s[i] = 0;
        for (j = GFBITS - 1; j >= 0; j--) {
            s[i] <<= 1;
            s[i] |= (L[j][i / 8] >> (i % 8)) & 1;
        }
    }
}

