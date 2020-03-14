/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
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
#include "scalars.inc"
    };

    //

    for (j = 0; j <= 4; j++) {
        for (i = 0; i < GFBITS; i++) {
            for (k = 4; k >= j; k--) {
                in[i] ^= (in[i] & mask[k][0]) >> (1 << k);
                in[i] ^= (in[i] & mask[k][1]) >> (1 << k);
            }
        }

        PQCLEAN_MCELIECE348864F_VEC_vec_mul(in, in, s[j]); // scaling
    }
}

/* input: in, result of applying the radix conversions to the input polynomial */
/* output: out, evaluation results (by applying the FFT butterflies) */
static void butterflies(uint64_t out[][ GFBITS ], const uint64_t *in) {
    int i, j, k, s, b;

    uint64_t tmp[ GFBITS ];
    uint64_t consts[ 63 ][ GFBITS ] = {
#include "consts.inc"
    };

    const vec powers[ 64 ][ GFBITS ] = {
#include "powers.inc"
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

    for (j = 0; j < 64; j++) {
        for (i = 0; i < GFBITS; i++) {
            out[j][i] = (in[i] >> reversal[j]) & 1;
            out[j][i] = -out[j][i];
        }
    }

    // butterflies

    for (i = 0; i <= 5; i++) {
        s = 1 << i;

        for (j = 0; j < 64; j += 2 * s) {
            for (k = j; k < j + s; k++) {
                PQCLEAN_MCELIECE348864F_VEC_vec_mul(tmp, out[k + s], consts[ consts_ptr + (k - j) ]);

                for (b = 0; b < GFBITS; b++) {
                    out[k][b] ^= tmp[b];
                }
                for (b = 0; b < GFBITS; b++) {
                    out[k + s][b] ^= out[k][b];
                }
            }
        }

        consts_ptr += ((uint64_t)1 << i);
    }

    //

    // adding the part contributed by x^64

    for (i = 0; i < 64; i++) {
        for (b = 0; b < GFBITS; b++) {
            out[i][b] ^= powers[i][b];
        }
    }

}

void PQCLEAN_MCELIECE348864F_VEC_fft(vec out[][ GFBITS ], vec *in) {
    radix_conversions(in);
    butterflies(out, in);
}

