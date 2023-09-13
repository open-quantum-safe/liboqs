/*
  This file is for Benes network related functions

  For the implementation strategy, see
  https://eprint.iacr.org/2017/793.pdf
*/

#include "util.h"
#include "benes.h"
#include "params.h"
#include "transpose.h"

static void layer_0(uint64_t *bs, uint64_t *cond) {
    int x;
    uint64_t diff;

    for (x = 0; x < (1 << 6); x += 2) {
        diff = bs[ x ] ^ bs[ x + 1 ];
        diff &= *cond++;
        bs[ x ] ^= diff;
        bs[ x + 1 ] ^= diff;
    }
}

static void layer_1(uint64_t *bs, const uint64_t *cond) {
    int x;
    uint64_t diff;

    for (x = 0; x < (1 << 6); x += 4) {
        diff = bs[ x + 0 ] ^ bs[ x + 2 ];
        diff &= cond[0];
        bs[ x + 0 ] ^= diff;
        bs[ x + 2 ] ^= diff;

        diff = bs[ x + 1 ] ^ bs[ x + 3 ];
        diff &= cond[1];
        bs[ x + 1 ] ^= diff;
        bs[ x + 3 ] ^= diff;

        cond += 2;
    }
}

static void layer_2(uint64_t *bs, const uint64_t *cond) {
    int x;
    uint64_t diff;

    for (x = 0; x < (1 << 6); x += 8) {
        diff = bs[ x + 0 ] ^ bs[ x + 4 ];
        diff &= cond[0];
        bs[ x + 0 ] ^= diff;
        bs[ x + 4 ] ^= diff;

        diff = bs[ x + 1 ] ^ bs[ x + 5 ];
        diff &= cond[1];
        bs[ x + 1 ] ^= diff;
        bs[ x + 5 ] ^= diff;

        diff = bs[ x + 2 ] ^ bs[ x + 6 ];
        diff &= cond[2];
        bs[ x + 2 ] ^= diff;
        bs[ x + 6 ] ^= diff;

        diff = bs[ x + 3 ] ^ bs[ x + 7 ];
        diff &= cond[3];
        bs[ x + 3 ] ^= diff;
        bs[ x + 7 ] ^= diff;

        cond += 4;
    }
}

static void layer_3(uint64_t *bs, const uint64_t *cond) {
    int x, s;
    uint64_t diff;

    for (x = 0; x < (1 << 6); x += 16) {
        for (s = x; s < x + 8; s += 4) {
            diff = bs[ s + 0 ] ^ bs[ s + 8 ];
            diff &= cond[0];
            bs[ s + 0 ] ^= diff;
            bs[ s + 8 ] ^= diff;

            diff = bs[ s + 1 ] ^ bs[ s + 9 ];
            diff &= cond[1];
            bs[ s + 1 ] ^= diff;
            bs[ s + 9 ] ^= diff;

            diff = bs[ s + 2 ] ^ bs[ s + 10 ];
            diff &= cond[2];
            bs[ s + 2 ] ^= diff;
            bs[ s + 10 ] ^= diff;

            diff = bs[ s + 3 ] ^ bs[ s + 11 ];
            diff &= cond[3];
            bs[ s + 3 ] ^= diff;
            bs[ s + 11 ] ^= diff;

            cond += 4;
        }
    }
}

static void layer_4(uint64_t *bs, const uint64_t *cond) {
    int x, s;
    uint64_t diff;

    for (x = 0; x < (1 << 6); x += 32) {
        for (s = x; s < x + 16; s += 4) {
            diff = bs[ s + 0 ] ^ bs[ s + 16 ];
            diff &= cond[0];
            bs[ s + 0 ] ^= diff;
            bs[ s + 16 ] ^= diff;

            diff = bs[ s + 1 ] ^ bs[ s + 17 ];
            diff &= cond[1];
            bs[ s + 1 ] ^= diff;
            bs[ s + 17 ] ^= diff;

            diff = bs[ s + 2 ] ^ bs[ s + 18 ];
            diff &= cond[2];
            bs[ s + 2 ] ^= diff;
            bs[ s + 18 ] ^= diff;

            diff = bs[ s + 3 ] ^ bs[ s + 19 ];
            diff &= cond[3];
            bs[ s + 3 ] ^= diff;
            bs[ s + 19 ] ^= diff;

            cond += 4;
        }
    }
}

static void layer_5(uint64_t *bs, const uint64_t *cond) {
    int x, s;
    uint64_t diff;

    for (x = 0; x < (1 << 6); x += 64) {
        for (s = x; s < x + 32; s += 4) {
            diff = bs[ s + 0 ] ^ bs[ s + 32 ];
            diff &= cond[0];
            bs[ s + 0 ] ^= diff;
            bs[ s + 32 ] ^= diff;

            diff = bs[ s + 1 ] ^ bs[ s + 33 ];
            diff &= cond[1];
            bs[ s + 1 ] ^= diff;
            bs[ s + 33 ] ^= diff;

            diff = bs[ s + 2 ] ^ bs[ s + 34 ];
            diff &= cond[2];
            bs[ s + 2 ] ^= diff;
            bs[ s + 34 ] ^= diff;

            diff = bs[ s + 3 ] ^ bs[ s + 35 ];
            diff &= cond[3];
            bs[ s + 3 ] ^= diff;
            bs[ s + 35 ] ^= diff;

            cond += 4;
        }
    }
}

/* input: bits, control bits as array of bytes */
/* output: out, control bits as array of 64-bit vectors */
void load_bits(uint64_t out[][32], const unsigned char *bits) {
    int i, low, block = 0;

    uint64_t cond[64];

    //

    for (low = 0; low <= 5; low++) {
        for (i = 0; i < 64; i++) {
            cond[i] = load4(bits + block * 256 + i * 4);
        }
        transpose_64x64(cond);

        for (i = 0; i < 32; i++) {
            out[ block ][i] = cond[i];
        }
        block++;
    }

    for (low = 0; low <= 5; low++) {
        for (i = 0; i < 32; i++) {
            out[ block ][i] = load8(bits + block * 256 + i * 8);
        }
        block++;
    }

    for (low = 4; low >= 0; low--) {
        for (i = 0; i < 32; i++) {
            out[ block ][i] = load8(bits + block * 256 + i * 8);
        }
        block++;
    }

    for (low = 5; low >= 0; low--) {
        for (i = 0; i < 64; i++) {
            cond[i] = load4(bits + block * 256 + i * 4);
        }
        transpose_64x64(cond);

        for (i = 0; i < 32; i++) {
            out[ block ][i] = cond[i];
        }
        block++;
    }
}

/* input: r, sequence of bits to be permuted */
/*        cond, control bits as array of 64-bit vectors  */
/*        rev, 0 for normal application; !0 for inverse */
/* output: r, permuted bits */
void benes(uint64_t *r, uint64_t cond[][32], int rev) {
    int block, inc;

    uint64_t *bs = r;

    //

    if (rev == 0) {
        block =  0;
        inc =  1;
    } else          {
        block = 22;
        inc = -1;
    }

    transpose_64x64(bs);

    layer_0(bs, cond[ block ]);
    block += inc;
    layer_1(bs, cond[ block ]);
    block += inc;
    layer_2(bs, cond[ block ]);
    block += inc;
    layer_3(bs, cond[ block ]);
    block += inc;
    layer_4(bs, cond[ block ]);
    block += inc;
    layer_5(bs, cond[ block ]);
    block += inc;

    transpose_64x64(bs);

    layer_0(bs, cond[ block ]);
    block += inc;
    layer_1(bs, cond[ block ]);
    block += inc;
    layer_2(bs, cond[ block ]);
    block += inc;
    layer_3(bs, cond[ block ]);
    block += inc;
    layer_4(bs, cond[ block ]);
    block += inc;
    layer_5(bs, cond[ block ]);
    block += inc;
    layer_4(bs, cond[ block ]);
    block += inc;
    layer_3(bs, cond[ block ]);
    block += inc;
    layer_2(bs, cond[ block ]);
    block += inc;
    layer_1(bs, cond[ block ]);
    block += inc;
    layer_0(bs, cond[ block ]);
    block += inc;

    transpose_64x64(bs);

    layer_5(bs, cond[ block ]);
    block += inc;
    layer_4(bs, cond[ block ]);
    block += inc;
    layer_3(bs, cond[ block ]);
    block += inc;
    layer_2(bs, cond[ block ]);
    block += inc;
    layer_1(bs, cond[ block ]);
    block += inc;
    layer_0(bs, cond[ block ]);

    transpose_64x64(bs);
}
