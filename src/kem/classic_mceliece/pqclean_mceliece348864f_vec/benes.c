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
void PQCLEAN_MCELIECE348864F_VEC_benes(uint64_t *r, const unsigned char *bits, int rev) {
    int i;

    const unsigned char *cond_ptr;
    int inc, low;

    uint64_t cond[64];

    //

    if (rev == 0) {
        inc = 256;
        cond_ptr = bits;
    } else {
        inc = -256;
        cond_ptr = bits + (2 * GFBITS - 2) * 256;
    }

    //

    PQCLEAN_MCELIECE348864F_VEC_transpose_64x64(r, r);

    for (low = 0; low <= 5; low++) {
        for (i = 0; i < 64; i++) {
            cond[i] = PQCLEAN_MCELIECE348864F_VEC_load4(cond_ptr + i * 4);
        }
        PQCLEAN_MCELIECE348864F_VEC_transpose_64x64(cond, cond);
        layer(r, cond, low);
        cond_ptr += inc;
    }

    PQCLEAN_MCELIECE348864F_VEC_transpose_64x64(r, r);

    for (low = 0; low <= 5; low++) {
        for (i = 0; i < 32; i++) {
            cond[i] = PQCLEAN_MCELIECE348864F_VEC_load8(cond_ptr + i * 8);
        }
        layer(r, cond, low);
        cond_ptr += inc;
    }
    for (low = 4; low >= 0; low--) {
        for (i = 0; i < 32; i++) {
            cond[i] = PQCLEAN_MCELIECE348864F_VEC_load8(cond_ptr + i * 8);
        }
        layer(r, cond, low);
        cond_ptr += inc;
    }

    PQCLEAN_MCELIECE348864F_VEC_transpose_64x64(r, r);

    for (low = 5; low >= 0; low--) {
        for (i = 0; i < 64; i++) {
            cond[i] = PQCLEAN_MCELIECE348864F_VEC_load4(cond_ptr + i * 4);
        }
        PQCLEAN_MCELIECE348864F_VEC_transpose_64x64(cond, cond);
        layer(r, cond, low);
        cond_ptr += inc;
    }

    PQCLEAN_MCELIECE348864F_VEC_transpose_64x64(r, r);
}

