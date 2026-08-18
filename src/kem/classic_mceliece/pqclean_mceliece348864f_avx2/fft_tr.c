/*
  This file is for transpose of the Gao-Mateer FFT
  Functions with names ending with _tr are (roughly) the transpose of the corresponding functions in fft.c

  For the implementation strategy, see
  https://eprint.iacr.org/2017/793.pdf
*/

#include "fft_tr.h"

#include "transpose.h"
#include "vec.h"

#include <stdint.h>

static void radix_conversions_tr(vec128 in[ GFBITS ]) {
    int i, j, k;

    const vec128 mask[10] = {
        vec128_set2x(0x2222222222222222, 0x2222222222222222),
        vec128_set2x(0x4444444444444444, 0x4444444444444444),
        vec128_set2x(0x0C0C0C0C0C0C0C0C, 0x0C0C0C0C0C0C0C0C),
        vec128_set2x(0x3030303030303030, 0x3030303030303030),
        vec128_set2x(0x00F000F000F000F0, 0x00F000F000F000F0),
        vec128_set2x(0x0F000F000F000F00, 0x0F000F000F000F00),
        vec128_set2x(0x0000FF000000FF00, 0x0000FF000000FF00),
        vec128_set2x(0x00FF000000FF0000, 0x00FF000000FF0000),
        vec128_set2x(0x00000000FFFF0000, 0x00000000FFFF0000),
        vec128_set2x(0x0000FFFF00000000, 0x0000FFFF00000000)
    };

    const vec128 s[5][GFBITS] = {
#include "scalars_2x.data"
    };

    uint64_t v0, v1;

    //

    for (j = 5; j >= 0; j--) {

        if (j < 5) {
            vec128_mul(in, in, s[j]);
        }

        for (i = 0; i < GFBITS; i++) {
            for (k = j; k <= 4; k++) {
                in[i] ^= vec128_sll_2x(in[i] & mask[2 * k + 0], 1 << k);
                in[i] ^= vec128_sll_2x(in[i] & mask[2 * k + 1], 1 << k);
            }
        }

        for (i = 0; i < GFBITS; i++) {
            v0 = vec128_extract(in[i], 0);
            v1 = vec128_extract(in[i], 1);

            v1 ^= v0 >> 32;
            v1 ^= v1 << 32;

            in[i] = vec128_set2x(v0, v1);
        }
    }
}

static void butterflies_tr(vec128 out[ GFBITS ], vec256 in[][ GFBITS ]) {
    int i, j, k, s, b;

    uint64_t tmp[ GFBITS ];
    uint64_t pre[6][ GFBITS ];
    uint64_t out64[2][64];

    vec256 p2[ 6 ];
    vec256 buf[64];
    vec256 x[ GFBITS ], y[ GFBITS ];
    vec256 tmp256[ GFBITS ];

    const vec256 consts[ 17 ][ GFBITS ] = {
#include "consts.data"
    };

    uint64_t consts_ptr = 17;

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

    const uint16_t beta[6] = {8, 1300, 3408, 1354, 2341, 1154};

    // butterflies

    for (i = 3; i >= 0; i--) {
        s = 1 << i;
        consts_ptr -= s;

        for (j = 0; j < 16; j += 2 * s) {
            for (k = j; k < j + s; k++) {
                for (b = 0; b < GFBITS; b++) {
                    in[k][b] ^= in[k + s][b];
                }
                vec256_mul(tmp256, in[k], consts[ consts_ptr + (k - j) ]);
                for (b = 0; b < GFBITS; b++) {
                    in[k + s][b] ^= tmp256[b];
                }
            }
        }
    }

    for (i = 0; i < 16; i += 2) {
        for (b = 0; b < GFBITS; b++) {
            x[b] = vec256_unpack_low(in[i + 0][b], in[i + 1][b]);
        }
        for (b = 0; b < GFBITS; b++) {
            y[b] = vec256_unpack_high(in[i + 0][b], in[i + 1][b]);
        }

        for (b = 0; b < GFBITS; b++) {
            x[b] ^= y[b];
        }
        vec256_mul(tmp256, x, consts[ 1 ]);
        for (b = 0; b < GFBITS; b++) {
            y[b] ^= tmp256[b];
        }

        for (b = 0; b < GFBITS; b++) {
            in[i + 0][b] = vec256_unpack_low_2x(x[b], y[b]);
        }
        for (b = 0; b < GFBITS; b++) {
            in[i + 1][b] = vec256_unpack_high_2x(x[b], y[b]);
        }
    }

    for (i = 0; i < 16; i += 2) {
        for (b = 0; b < GFBITS; b++) {
            in[i + 0][b] ^= in[i + 1][b];
        }
        vec256_mul(tmp256, in[i + 0], consts[ 0 ]);
        for (b = 0; b < GFBITS; b++) {
            in[i + 1][b] ^= tmp256[b];
        }
    }

    // transpose

    for (i = 0; i < GFBITS; i += 4) {
        for (j = 0; j < 64; j += 8) {
            buf[ reversal[j + 0] ] = vec256_set4x(vec256_extract(in[j / 4 + 0][i + 0], 0),
                                                  vec256_extract(in[j / 4 + 0][i + 1], 0),
                                                  vec256_extract(in[j / 4 + 0][i + 2], 0),
                                                  vec256_extract(in[j / 4 + 0][i + 3], 0));
            buf[ reversal[j + 1] ] = vec256_set4x(vec256_extract(in[j / 4 + 1][i + 0], 0),
                                                  vec256_extract(in[j / 4 + 1][i + 1], 0),
                                                  vec256_extract(in[j / 4 + 1][i + 2], 0),
                                                  vec256_extract(in[j / 4 + 1][i + 3], 0));
            buf[ reversal[j + 2] ] = vec256_set4x(vec256_extract(in[j / 4 + 0][i + 0], 1),
                                                  vec256_extract(in[j / 4 + 0][i + 1], 1),
                                                  vec256_extract(in[j / 4 + 0][i + 2], 1),
                                                  vec256_extract(in[j / 4 + 0][i + 3], 1));
            buf[ reversal[j + 3] ] = vec256_set4x(vec256_extract(in[j / 4 + 1][i + 0], 1),
                                                  vec256_extract(in[j / 4 + 1][i + 1], 1),
                                                  vec256_extract(in[j / 4 + 1][i + 2], 1),
                                                  vec256_extract(in[j / 4 + 1][i + 3], 1));
            buf[ reversal[j + 4] ] = vec256_set4x(vec256_extract(in[j / 4 + 0][i + 0], 2),
                                                  vec256_extract(in[j / 4 + 0][i + 1], 2),
                                                  vec256_extract(in[j / 4 + 0][i + 2], 2),
                                                  vec256_extract(in[j / 4 + 0][i + 3], 2));
            buf[ reversal[j + 5] ] = vec256_set4x(vec256_extract(in[j / 4 + 1][i + 0], 2),
                                                  vec256_extract(in[j / 4 + 1][i + 1], 2),
                                                  vec256_extract(in[j / 4 + 1][i + 2], 2),
                                                  vec256_extract(in[j / 4 + 1][i + 3], 2));
            buf[ reversal[j + 6] ] = vec256_set4x(vec256_extract(in[j / 4 + 0][i + 0], 3),
                                                  vec256_extract(in[j / 4 + 0][i + 1], 3),
                                                  vec256_extract(in[j / 4 + 0][i + 2], 3),
                                                  vec256_extract(in[j / 4 + 0][i + 3], 3));
            buf[ reversal[j + 7] ] = vec256_set4x(vec256_extract(in[j / 4 + 1][i + 0], 3),
                                                  vec256_extract(in[j / 4 + 1][i + 1], 3),
                                                  vec256_extract(in[j / 4 + 1][i + 2], 3),
                                                  vec256_extract(in[j / 4 + 1][i + 3], 3));
        }

        transpose_64x256_sp(buf);

        p2[0] = buf[32];
        buf[33] = vec256_xor(buf[33], buf[32]);
        p2[1] = buf[33];
        buf[35] = vec256_xor(buf[35], buf[33]);
        p2[0] = vec256_xor(p2[0], buf[35]);
        buf[34] = vec256_xor(buf[34], buf[35]);
        p2[2] = buf[34];
        buf[38] = vec256_xor(buf[38], buf[34]);
        p2[0] = vec256_xor(p2[0], buf[38]);
        buf[39] = vec256_xor(buf[39], buf[38]);
        p2[1] = vec256_xor(p2[1], buf[39]);
        buf[37] = vec256_xor(buf[37], buf[39]);
        p2[0] = vec256_xor(p2[0], buf[37]);
        buf[36] = vec256_xor(buf[36], buf[37]);
        p2[3] = buf[36];
        buf[44] = vec256_xor(buf[44], buf[36]);
        p2[0] = vec256_xor(p2[0], buf[44]);
        buf[45] = vec256_xor(buf[45], buf[44]);
        p2[1] = vec256_xor(p2[1], buf[45]);
        buf[47] = vec256_xor(buf[47], buf[45]);
        p2[0] = vec256_xor(p2[0], buf[47]);
        buf[46] = vec256_xor(buf[46], buf[47]);
        p2[2] = vec256_xor(p2[2], buf[46]);
        buf[42] = vec256_xor(buf[42], buf[46]);
        p2[0] = vec256_xor(p2[0], buf[42]);
        buf[43] = vec256_xor(buf[43], buf[42]);
        p2[1] = vec256_xor(p2[1], buf[43]);
        buf[41] = vec256_xor(buf[41], buf[43]);
        p2[0] = vec256_xor(p2[0], buf[41]);
        buf[40] = vec256_xor(buf[40], buf[41]);
        p2[4] = buf[40];
        buf[56] = vec256_xor(buf[56], buf[40]);
        p2[0] = vec256_xor(p2[0], buf[56]);
        buf[57] = vec256_xor(buf[57], buf[56]);
        p2[1] = vec256_xor(p2[1], buf[57]);
        buf[59] = vec256_xor(buf[59], buf[57]);
        p2[0] = vec256_xor(p2[0], buf[59]);
        buf[58] = vec256_xor(buf[58], buf[59]);
        p2[2] = vec256_xor(p2[2], buf[58]);
        buf[62] = vec256_xor(buf[62], buf[58]);
        p2[0] = vec256_xor(p2[0], buf[62]);
        buf[63] = vec256_xor(buf[63], buf[62]);
        p2[1] = vec256_xor(p2[1], buf[63]);
        buf[61] = vec256_xor(buf[61], buf[63]);
        p2[0] = vec256_xor(p2[0], buf[61]);
        buf[60] = vec256_xor(buf[60], buf[61]);
        p2[3] = vec256_xor(p2[3], buf[60]);
        buf[52] = vec256_xor(buf[52], buf[60]);
        p2[0] = vec256_xor(p2[0], buf[52]);
        buf[53] = vec256_xor(buf[53], buf[52]);
        p2[1] = vec256_xor(p2[1], buf[53]);
        buf[55] = vec256_xor(buf[55], buf[53]);
        p2[0] = vec256_xor(p2[0], buf[55]);
        buf[54] = vec256_xor(buf[54], buf[55]);
        p2[2] = vec256_xor(p2[2], buf[54]);
        buf[50] = vec256_xor(buf[50], buf[54]);
        p2[0] = vec256_xor(p2[0], buf[50]);
        buf[51] = vec256_xor(buf[51], buf[50]);
        p2[1] = vec256_xor(p2[1], buf[51]);
        buf[49] = vec256_xor(buf[49], buf[51]);
        p2[0] = vec256_xor(p2[0], buf[49]);
        buf[48] = vec256_xor(buf[48], buf[49]);
        p2[5] = buf[48];
        buf[16] = vec256_xor(buf[16], buf[48]);
        p2[0] = vec256_xor(p2[0], buf[16]);
        buf[17] = vec256_xor(buf[17], buf[16]);
        p2[1] = vec256_xor(p2[1], buf[17]);
        buf[19] = vec256_xor(buf[19], buf[17]);
        p2[0] = vec256_xor(p2[0], buf[19]);
        buf[18] = vec256_xor(buf[18], buf[19]);
        p2[2] = vec256_xor(p2[2], buf[18]);
        buf[22] = vec256_xor(buf[22], buf[18]);
        p2[0] = vec256_xor(p2[0], buf[22]);
        buf[23] = vec256_xor(buf[23], buf[22]);
        p2[1] = vec256_xor(p2[1], buf[23]);
        buf[21] = vec256_xor(buf[21], buf[23]);
        p2[0] = vec256_xor(p2[0], buf[21]);
        buf[20] = vec256_xor(buf[20], buf[21]);
        p2[3] = vec256_xor(p2[3], buf[20]);
        buf[28] = vec256_xor(buf[28], buf[20]);
        p2[0] = vec256_xor(p2[0], buf[28]);
        buf[29] = vec256_xor(buf[29], buf[28]);
        p2[1] = vec256_xor(p2[1], buf[29]);
        buf[31] = vec256_xor(buf[31], buf[29]);
        p2[0] = vec256_xor(p2[0], buf[31]);
        buf[30] = vec256_xor(buf[30], buf[31]);
        p2[2] = vec256_xor(p2[2], buf[30]);
        buf[26] = vec256_xor(buf[26], buf[30]);
        p2[0] = vec256_xor(p2[0], buf[26]);
        buf[27] = vec256_xor(buf[27], buf[26]);
        p2[1] = vec256_xor(p2[1], buf[27]);
        buf[25] = vec256_xor(buf[25], buf[27]);
        p2[0] = vec256_xor(p2[0], buf[25]);
        buf[24] = vec256_xor(buf[24], buf[25]);
        p2[4] = vec256_xor(p2[4], buf[24]);
        buf[8] = vec256_xor(buf[8], buf[24]);
        p2[0] = vec256_xor(p2[0], buf[8]);
        buf[9] = vec256_xor(buf[9], buf[8]);
        p2[1] = vec256_xor(p2[1], buf[9]);
        buf[11] = vec256_xor(buf[11], buf[9]);
        p2[0] = vec256_xor(p2[0], buf[11]);
        buf[10] = vec256_xor(buf[10], buf[11]);
        p2[2] = vec256_xor(p2[2], buf[10]);
        buf[14] = vec256_xor(buf[14], buf[10]);
        p2[0] = vec256_xor(p2[0], buf[14]);
        buf[15] = vec256_xor(buf[15], buf[14]);
        p2[1] = vec256_xor(p2[1], buf[15]);
        buf[13] = vec256_xor(buf[13], buf[15]);
        p2[0] = vec256_xor(p2[0], buf[13]);
        buf[12] = vec256_xor(buf[12], buf[13]);
        p2[3] = vec256_xor(p2[3], buf[12]);
        buf[4] = vec256_xor(buf[4], buf[12]);
        p2[0] = vec256_xor(p2[0], buf[4]);
        buf[5] = vec256_xor(buf[5], buf[4]);
        p2[1] = vec256_xor(p2[1], buf[5]);
        buf[7] = vec256_xor(buf[7], buf[5]);
        p2[0] = vec256_xor(p2[0], buf[7]);
        buf[6] = vec256_xor(buf[6], buf[7]);
        p2[2] = vec256_xor(p2[2], buf[6]);
        buf[2] = vec256_xor(buf[2], buf[6]);
        p2[0] = vec256_xor(p2[0], buf[2]);
        buf[3] = vec256_xor(buf[3], buf[2]);
        p2[1] = vec256_xor(p2[1], buf[3]);
        buf[1] = vec256_xor(buf[1], buf[3]);

        p2[0] = vec256_xor(p2[0], buf[1]);
        buf[0] = vec256_xor(buf[0], buf[1]);

        for (j = 0; j < 6; j++) {
            pre[j][i + 0] = vec256_extract(p2[j], 0);
            pre[j][i + 1] = vec256_extract(p2[j], 1);
            pre[j][i + 2] = vec256_extract(p2[j], 2);
            pre[j][i + 3] = vec256_extract(p2[j], 3);
        }

        out64[0][i + 0] = vec256_extract(buf[0], 0);
        out64[0][i + 1] = vec256_extract(buf[0], 1);
        out64[0][i + 2] = vec256_extract(buf[0], 2);
        out64[0][i + 3] = vec256_extract(buf[0], 3);
    }

    //

    for (j = 0; j < GFBITS; j++) {
        tmp[j] = (beta[0] >> j) & 1;
        tmp[j] = -tmp[j];
    }

    vec_mul(out64[1], pre[0], tmp);

    for (i = 1; i < 6; i++) {
        for (j = 0; j < GFBITS; j++) {
            tmp[j] = (beta[i] >> j) & 1;
            tmp[j] = -tmp[j];
        }

        vec_mul(tmp, pre[i], tmp);
        vec_add(out64[1], out64[1], tmp);
    }

    for (i = 0; i < GFBITS; i++) {
        out[i] = vec128_set2x(out64[0][i], out64[1][i]);
    }
}

void fft_tr(vec128 out[GFBITS], vec256 in[][ GFBITS ]) {
    butterflies_tr(out, in);
    radix_conversions_tr(out);
}
