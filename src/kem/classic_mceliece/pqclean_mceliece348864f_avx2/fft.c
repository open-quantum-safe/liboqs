/*
  This file is for implementing the Gao-Mateer FFT, see
  http://www.math.clemson.edu/~sgao/papers/GM10.pdf

  For the implementation strategy, see
  https://eprint.iacr.org/2017/793.pdf
*/

#include "fft.h"

#include "vec.h"

/* input: in, polynomial in bitsliced form */
/* output: in, result of applying the radix conversions on in */
static void radix_conversions(uint64_t *in) {
    int i, j, k;

    const uint64_t mask[5][2] = {
        {0x8888888888888888, 0x4444444444444444},
        {0xC0C0C0C0C0C0C0C0, 0x3030303030303030},
        {0xF000F000F000F000, 0x0F000F000F000F00},
        {0xFF000000FF000000, 0x00FF000000FF0000},
        {0xFFFF000000000000, 0x0000FFFF00000000}
    };

    const uint64_t s[5][GFBITS] = {
#include "scalars.data"
    };

    //

    for (j = 0; j <= 4; j++) {
        for (i = 0; i < GFBITS; i++) {
            for (k = 4; k >= j; k--) {
                in[i] ^= (in[i] & mask[k][0]) >> (1 << k);
                in[i] ^= (in[i] & mask[k][1]) >> (1 << k);
            }
        }

        vec_mul(in, in, s[j]); // scaling
    }
}

/* input: in, result of applying the radix conversions to the input polynomial */
/* output: out, evaluation results (by applying the FFT butterflies) */
static void butterflies(vec256 out[][ GFBITS ], const uint64_t *in) {
    int i, j, k, s, b;

    uint64_t t0, t1, t2, t3;

    const vec256 consts[ 17 ][ GFBITS ] = {
#include "consts.data"
    };

    uint64_t consts_ptr = 0;

    const unsigned char reversal[64] = {
        0, 32, 16, 48,  8, 40, 24, 56,
        4, 36, 20, 52, 12, 44, 28, 60,
        2, 34, 18, 50, 10, 42, 26, 58,
        6, 38, 22, 54, 14, 46, 30, 62,
        1, 33, 17, 49,  9, 41, 25, 57,
        5, 37, 21, 53, 13, 45, 29, 61,
        3, 35, 19, 51, 11, 43, 27, 59,
        7, 39, 23, 55, 15, 47, 31, 63
    };

    // boradcast

    vec256 tmp256[ GFBITS ];
    vec256 x[ GFBITS ], y[ GFBITS ];

    for (j = 0; j < 64; j += 8) {
        for (i = 0; i < GFBITS; i++) {
            t0 = (in[i] >> reversal[j + 0]) & 1;
            t0 = -t0;
            t1 = (in[i] >> reversal[j + 2]) & 1;
            t1 = -t1;
            t2 = (in[i] >> reversal[j + 4]) & 1;
            t2 = -t2;
            t3 = (in[i] >> reversal[j + 6]) & 1;
            t3 = -t3;

            out[j / 4 + 0][i] = vec256_set4x(t0, t1, t2, t3);

            t0 = (in[i] >> reversal[j + 1]) & 1;
            t0 = -t0;
            t1 = (in[i] >> reversal[j + 3]) & 1;
            t1 = -t1;
            t2 = (in[i] >> reversal[j + 5]) & 1;
            t2 = -t2;
            t3 = (in[i] >> reversal[j + 7]) & 1;
            t3 = -t3;

            out[j / 4 + 1][i] = vec256_set4x(t0, t1, t2, t3);
        }
    }

    //

    for (i = 0; i < 16; i += 2) {
        vec256_mul(tmp256, out[i + 1], consts[ 0 ]);

        for (b = 0; b < GFBITS; b++) {
            out[i + 0][b] ^= tmp256[b];
        }
        for (b = 0; b < GFBITS; b++) {
            out[i + 1][b] ^= out[i + 0][b];
        }
    }

    for (i = 0; i < 16; i += 2) {
        for (b = 0; b < GFBITS; b++) {
            x[b] = vec256_unpack_low_2x(out[i + 0][b], out[i + 1][b]);
        }
        for (b = 0; b < GFBITS; b++) {
            y[b] = vec256_unpack_high_2x(out[i + 0][b], out[i + 1][b]);
        }

        vec256_mul(tmp256, y, consts[ 1 ]);

        for (b = 0; b < GFBITS; b++) {
            x[b] ^= tmp256[b];
        }
        for (b = 0; b < GFBITS; b++) {
            y[b] ^= x[b];
        }

        for (b = 0; b < GFBITS; b++) {
            out[i + 0][b] = vec256_unpack_low(x[b], y[b]);
        }
        for (b = 0; b < GFBITS; b++) {
            out[i + 1][b] = vec256_unpack_high(x[b], y[b]);
        }
    }

    consts_ptr = 2;

    for (i = 0; i <= 3; i++) {
        s = 1 << i;

        for (j = 0; j < 16; j += 2 * s) {
            for (k = j; k < j + s; k++) {
                vec256_mul(tmp256, out[k + s], consts[ consts_ptr + (k - j) ]);

                for (b = 0; b < GFBITS; b++) {
                    out[k][b] ^= tmp256[b];
                }
                for (b = 0; b < GFBITS; b++) {
                    out[k + s][b] ^= out[k][b];
                }
            }
        }

        consts_ptr += s;
    }

    // adding the part contributed by x^64

    vec256 powers[16][GFBITS] = {
#include "powers.data"
    };

    for (i = 0; i < 16; i++) {
        for (b = 0; b < GFBITS; b++) {
            out[i][b] = vec256_xor(out[i][b], powers[i][b]);
        }
    }
}

void fft(vec256 out[][ GFBITS ], uint64_t *in) {
    radix_conversions(in);
    butterflies(out, in);
}
