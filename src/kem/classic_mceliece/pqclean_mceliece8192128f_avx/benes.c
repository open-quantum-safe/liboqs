/*
  This file is for Benes network related functions
*/

#include "benes.h"

#include "params.h"
#include "transpose.h"
#include "util.h"

static void layer_x(vec128 *data, vec128 *bits) {
    int i;
    vec128 v0, v1;
    vec128 d;

    for (i = 0; i < 64; i += 2) {
        v0 = PQCLEAN_MCELIECE8192128F_AVX_vec128_unpack_low(data[i + 0], data[i + 1]);
        v1 = PQCLEAN_MCELIECE8192128F_AVX_vec128_unpack_high(data[i + 0], data[i + 1]);

        d = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(v0, v1);
        d = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(d, *bits++);
        v0 = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(v0, d);
        v1 = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(v1, d);

        data[i + 0] = PQCLEAN_MCELIECE8192128F_AVX_vec128_unpack_low(v0, v1);
        data[i + 1] = PQCLEAN_MCELIECE8192128F_AVX_vec128_unpack_high(v0, v1);
    }
}

static void layer_0(vec128 *bs, vec128 *cond) {
    int x;
    vec128 diff;

    for (x = 0; x < (1 << 6); x += 2) {
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x ], bs[ x + 1 ]);
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, *cond++);
        bs[ x ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x ], diff);
        bs[ x + 1 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 1 ], diff);
    }
}

static void layer_1(vec128 *bs, vec128 *cond) {
    int x;
    vec128 diff;

    for (x = 0; x < (1 << 6); x += 4) {
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 0 ], bs[ x + 2 ]);
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[0]);
        bs[ x + 0 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 0 ], diff);
        bs[ x + 2 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 2 ], diff);

        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 1 ], bs[ x + 3 ]);
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[1]);
        bs[ x + 1 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 1 ], diff);
        bs[ x + 3 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 3 ], diff);

        cond += 2;
    }
}

static void layer_2(vec128 *bs, vec128 *cond) {
    int x;
    vec128 diff;

    for (x = 0; x < (1 << 6); x += 8) {
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 0 ], bs[ x + 4 ]);
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[0]);
        bs[ x + 0 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 0 ], diff);
        bs[ x + 4 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 4 ], diff);

        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 1 ], bs[ x + 5 ]);
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[1]);
        bs[ x + 1 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 1 ], diff);
        bs[ x + 5 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 5 ], diff);

        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 2 ], bs[ x + 6 ]);
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[2]);
        bs[ x + 2 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 2 ], diff);
        bs[ x + 6 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 6 ], diff);

        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 3 ], bs[ x + 7 ]);
        diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[3]);
        bs[ x + 3 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 3 ], diff);
        bs[ x + 7 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ x + 7 ], diff);

        cond += 4;
    }
}

static void layer_3(vec128 *bs, vec128 *cond) {
    int x, s;
    vec128 diff;

    for (x = 0; x < (1 << 6); x += 16) {
        for (s = x; s < x + 8; s += 4) {
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 0 ], bs[ s + 8 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[0]);
            bs[ s + 0 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 0 ], diff);
            bs[ s + 8 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 8 ], diff);

            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 1 ], bs[ s + 9 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[1]);
            bs[ s + 1 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 1 ], diff);
            bs[ s + 9 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 9 ], diff);

            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 2 ], bs[ s + 10 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[2]);
            bs[ s + 2 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 2 ], diff);
            bs[ s + 10 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 10 ], diff);

            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 3 ], bs[ s + 11 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[3]);
            bs[ s + 3 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 3 ], diff);
            bs[ s + 11 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 11 ], diff);

            cond += 4;
        }
    }
}

static void layer_4(vec128 *bs, vec128 *cond) {
    int x, s;
    vec128 diff;

    for (x = 0; x < (1 << 6); x += 32) {
        for (s = x; s < x + 16; s += 4) {
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 0 ], bs[ s + 16 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[0]);
            bs[ s + 0 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 0 ], diff);
            bs[ s + 16 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 16 ], diff);

            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 1 ], bs[ s + 17 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[1]);
            bs[ s + 1 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 1 ], diff);
            bs[ s + 17 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 17 ], diff);

            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 2 ], bs[ s + 18 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[2]);
            bs[ s + 2 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 2 ], diff);
            bs[ s + 18 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 18 ], diff);

            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 3 ], bs[ s + 19 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[3]);
            bs[ s + 3 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 3 ], diff);
            bs[ s + 19 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 19 ], diff);

            cond += 4;
        }
    }
}

static void layer_5(vec128 *bs, vec128 *cond) {
    int x, s;
    vec128 diff;

    for (x = 0; x < (1 << 6); x += 64) {
        for (s = x; s < x + 32; s += 4) {
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 0 ], bs[ s + 32 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[0]);
            bs[ s + 0 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 0 ], diff);
            bs[ s + 32 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 32 ], diff);

            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 1 ], bs[ s + 33 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[1]);
            bs[ s + 1 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 1 ], diff);
            bs[ s + 33 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 33 ], diff);

            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 2 ], bs[ s + 34 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[2]);
            bs[ s + 2 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 2 ], diff);
            bs[ s + 34 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 34 ], diff);

            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 3 ], bs[ s + 35 ]);
            diff = PQCLEAN_MCELIECE8192128F_AVX_vec128_and(diff, cond[3]);
            bs[ s + 3 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 3 ], diff);
            bs[ s + 35 ] = PQCLEAN_MCELIECE8192128F_AVX_vec128_xor(bs[ s + 35 ], diff);

            cond += 4;
        }
    }
}

/* input: bits, control bits as array of bytes */
/* output: bits_int, control bits as array of 128-bit vectors */
void PQCLEAN_MCELIECE8192128F_AVX_load_bits(vec128 bits_int[][32], const unsigned char *bits) {
    int i, j;
    const unsigned char *ptr = bits;

    vec128 buf[64];

    for (i = 0; i <= 5; i += 2) {
        for (j = 0; j < 64; j++) {
            buf[j] = PQCLEAN_MCELIECE8192128F_AVX_vec128_set2x(PQCLEAN_MCELIECE8192128F_AVX_load8(ptr), PQCLEAN_MCELIECE8192128F_AVX_load8(ptr + 512));
            ptr += 8;
        }

        PQCLEAN_MCELIECE8192128F_AVX_transpose_64x128_sp( buf );

        for (j = 0; j < 32; j++) {
            bits_int[i + 0][j] = PQCLEAN_MCELIECE8192128F_AVX_vec128_unpack_low(buf[j], buf[j + 32]);
            bits_int[i + 1][j] = PQCLEAN_MCELIECE8192128F_AVX_vec128_unpack_high(buf[j], buf[j + 32]);
        }

        ptr += 512;
    }

    for (i = 6; i <= 18; i++) {
        for (j = 0; j < 32; j++) {
            bits_int[i][j] = PQCLEAN_MCELIECE8192128F_AVX_load16(ptr);
            ptr += 16;
        }
    }

    for (i = 19; i < 25; i += 2) {
        for (j = 0; j < 64; j++) {
            buf[j] = PQCLEAN_MCELIECE8192128F_AVX_vec128_set2x(PQCLEAN_MCELIECE8192128F_AVX_load8(ptr), PQCLEAN_MCELIECE8192128F_AVX_load8(ptr + 512));
            ptr += 8;
        }

        PQCLEAN_MCELIECE8192128F_AVX_transpose_64x128_sp( buf );

        for (j = 0; j < 32; j++) {
            bits_int[i + 0][j] = PQCLEAN_MCELIECE8192128F_AVX_vec128_unpack_low(buf[j], buf[j + 32]);
            bits_int[i + 1][j] = PQCLEAN_MCELIECE8192128F_AVX_vec128_unpack_high(buf[j], buf[j + 32]);
        }

        ptr += 512;
    }
}

/* input: r, sequence of bits to be permuted */
/*        b, control bits as array of 128-bit vectors  */
/*        rev, 0 for normal application; !0 for inverse */
/* output: r, permuted bits */
void PQCLEAN_MCELIECE8192128F_AVX_benes(vec128 *r, vec128 b[][32], int rev) {
    int inc;

    vec128 *b_ptr;

    if (rev == 0) {
        inc =  32;
        b_ptr = b[ 0];
    } else          {
        inc = -32;
        b_ptr = b[24];
    }

    //

    PQCLEAN_MCELIECE8192128F_AVX_transpose_64x128_sp( r );

    layer_0(r, b_ptr);
    b_ptr += inc;
    layer_1(r, b_ptr);
    b_ptr += inc;
    layer_2(r, b_ptr);
    b_ptr += inc;
    layer_3(r, b_ptr);
    b_ptr += inc;
    layer_4(r, b_ptr);
    b_ptr += inc;
    layer_5(r, b_ptr);
    b_ptr += inc;

    PQCLEAN_MCELIECE8192128F_AVX_transpose_64x128_sp( r );

    layer_x(r, b_ptr);
    b_ptr += inc;
    layer_0(r, b_ptr);
    b_ptr += inc;
    layer_1(r, b_ptr);
    b_ptr += inc;
    layer_2(r, b_ptr);
    b_ptr += inc;
    layer_3(r, b_ptr);
    b_ptr += inc;
    layer_4(r, b_ptr);
    b_ptr += inc;
    layer_5(r, b_ptr);
    b_ptr += inc;
    layer_4(r, b_ptr);
    b_ptr += inc;
    layer_3(r, b_ptr);
    b_ptr += inc;
    layer_2(r, b_ptr);
    b_ptr += inc;
    layer_1(r, b_ptr);
    b_ptr += inc;
    layer_0(r, b_ptr);
    b_ptr += inc;
    layer_x(r, b_ptr);
    b_ptr += inc;

    PQCLEAN_MCELIECE8192128F_AVX_transpose_64x128_sp( r );

    layer_5(r, b_ptr);
    b_ptr += inc;
    layer_4(r, b_ptr);
    b_ptr += inc;
    layer_3(r, b_ptr);
    b_ptr += inc;
    layer_2(r, b_ptr);
    b_ptr += inc;
    layer_1(r, b_ptr);
    b_ptr += inc;
    layer_0(r, b_ptr);
    //b_ptr += inc;

    PQCLEAN_MCELIECE8192128F_AVX_transpose_64x128_sp( r );
}

