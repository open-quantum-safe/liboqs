/*
  This file is for Benes network related functions

  For the implementation strategy, see
  https://eprint.iacr.org/2017/793.pdf
*/

#include "util.h"
#include "benes.h"
#include "params.h"
#include "transpose.h"

/* middle layers of the benes network */
static void layer_in(uint64_t data[2][64], uint64_t *bits, int lgs) {
    int i, j, s;

    uint64_t d;

    s = 1 << lgs;

    for (i = 0; i < 64; i += s * 2) {
        for (j = i; j < i + s; j++) {

            d = (data[0][j + 0] ^ data[0][j + s]);
            d &= (*bits++);
            data[0][j + 0] ^= d;
            data[0][j + s] ^= d;

            d = (data[1][j + 0] ^ data[1][j + s]);
            d &= (*bits++);
            data[1][j + 0] ^= d;
            data[1][j + s] ^= d;
        }
    }
}

/* first and last layers of the benes network */
static void layer_ex(uint64_t *data, uint64_t *bits, int lgs) {
    int i, j, s;

    uint64_t d;

    s = 1 << lgs;

    for (i = 0; i < 128; i += s * 2) {
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
void apply_benes(unsigned char *r, const unsigned char *bits, int rev) {
    int i, iter, inc;

    unsigned char *r_ptr = r;
    const unsigned char *bits_ptr;

    uint64_t r_int_v[2][64];
    uint64_t r_int_h[2][64];
    uint64_t b_int_v[64];
    uint64_t b_int_h[64];

    //

    if (rev) {
        bits_ptr = bits + 12288;
        inc = -1024;
    } else     {
        bits_ptr = bits;
        inc = 0;
    }

    for (i = 0; i < 64; i++) {
        r_int_v[0][i] = load8(r_ptr + i * 16 + 0);
        r_int_v[1][i] = load8(r_ptr + i * 16 + 8);
    }

    transpose_64x64(r_int_h[0], r_int_v[0]);
    transpose_64x64(r_int_h[1], r_int_v[1]);

    for (iter = 0; iter <= 6; iter++) {
        for (i = 0; i < 64; i++) {
            b_int_v[i] = load8(bits_ptr);
            bits_ptr += 8;
        }

        bits_ptr += inc;

        transpose_64x64(b_int_h, b_int_v);

        layer_ex(r_int_h[0], b_int_h, iter);
    }

    transpose_64x64(r_int_v[0], r_int_h[0]);
    transpose_64x64(r_int_v[1], r_int_h[1]);

    for (iter = 0; iter <= 5; iter++) {
        for (i = 0; i < 64; i++) {
            b_int_v[i] = load8(bits_ptr);
            bits_ptr += 8;
        }
        bits_ptr += inc;

        layer_in(r_int_v, b_int_v, iter);
    }

    for (iter = 4; iter >= 0; iter--) {
        for (i = 0; i < 64; i++) {
            b_int_v[i] = load8(bits_ptr);
            bits_ptr += 8;
        }
        bits_ptr += inc;

        layer_in(r_int_v, b_int_v, iter);
    }

    transpose_64x64(r_int_h[0], r_int_v[0]);
    transpose_64x64(r_int_h[1], r_int_v[1]);

    for (iter = 6; iter >= 0; iter--) {
        for (i = 0; i < 64; i++) {
            b_int_v[i] = load8(bits_ptr);
            bits_ptr += 8;
        }

        bits_ptr += inc;

        transpose_64x64(b_int_h, b_int_v);

        layer_ex(r_int_h[0], b_int_h, iter);
    }

    transpose_64x64(r_int_v[0], r_int_h[0]);
    transpose_64x64(r_int_v[1], r_int_h[1]);

    for (i = 0; i < 64; i++) {
        store8(r_ptr + i * 16 + 0, r_int_v[0][i]);
        store8(r_ptr + i * 16 + 8, r_int_v[1][i]);
    }
}

/* input: condition bits c */
/* output: support s */
void support_gen(gf *s, const unsigned char *c) {
    gf a;
    int i, j;
    unsigned char L[ GFBITS ][ (1 << GFBITS) / 8 ];

    for (i = 0; i < GFBITS; i++) {
        for (j = 0; j < (1 << GFBITS) / 8; j++) {
            L[i][j] = 0;
        }
    }

    for (i = 0; i < (1 << GFBITS); i++) {
        a = bitrev((gf) i);

        for (j = 0; j < GFBITS; j++) {
            L[j][ i / 8 ] |= ((a >> j) & 1) << (i % 8);
        }
    }

    for (j = 0; j < GFBITS; j++) {
        apply_benes(L[j], c, 0);
    }

    for (i = 0; i < SYS_N; i++) {
        s[i] = 0;
        for (j = GFBITS - 1; j >= 0; j--) {
            s[i] <<= 1;
            s[i] |= (L[j][i / 8] >> (i % 8)) & 1;
        }
    }
}
