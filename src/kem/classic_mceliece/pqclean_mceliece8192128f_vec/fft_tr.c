/*
  This file is for transpose of the Gao-Mateer FFT
  Functions with names ending with _tr are (roughly) the transpose of the corresponding functions in fft.c
*/

#include "fft_tr.h"

#include "transpose.h"

#include <stdint.h>

static void radix_conversions_tr(vec in[][ GFBITS ]) {
    int i, j, k;

    const vec mask[6][2] = {
        {0x2222222222222222, 0x4444444444444444},
        {0x0C0C0C0C0C0C0C0C, 0x3030303030303030},
        {0x00F000F000F000F0, 0x0F000F000F000F00},
        {0x0000FF000000FF00, 0x00FF000000FF0000},
        {0x00000000FFFF0000, 0x0000FFFF00000000},
        {0xFFFFFFFF00000000, 0x00000000FFFFFFFF}
    };

    const vec s[6][4][GFBITS] = {
#include "scalars_4x.inc"
    };

    //

    for (j = 6; j >= 0; j--) {
        if (j < 6) {
            PQCLEAN_MCELIECE8192128F_VEC_vec_mul(in[0], in[0], s[j][0]); // scaling
            PQCLEAN_MCELIECE8192128F_VEC_vec_mul(in[1], in[1], s[j][1]); // scaling
            PQCLEAN_MCELIECE8192128F_VEC_vec_mul(in[2], in[2], s[j][2]); // scaling
            PQCLEAN_MCELIECE8192128F_VEC_vec_mul(in[3], in[3], s[j][3]); // scaling
        }

        for (k = j; k <= 4; k++) {
            for (i = 0; i < GFBITS; i++) {
                in[0][i] ^= (in[0][i] & mask[k][0]) << (1 << k);
                in[0][i] ^= (in[0][i] & mask[k][1]) << (1 << k);
                in[1][i] ^= (in[1][i] & mask[k][0]) << (1 << k);
                in[1][i] ^= (in[1][i] & mask[k][1]) << (1 << k);
                in[2][i] ^= (in[2][i] & mask[k][0]) << (1 << k);
                in[2][i] ^= (in[2][i] & mask[k][1]) << (1 << k);
                in[3][i] ^= (in[3][i] & mask[k][0]) << (1 << k);
                in[3][i] ^= (in[3][i] & mask[k][1]) << (1 << k);
            }
        }

        if (j <= 5) {
            for (i = 0; i < GFBITS; i++) {
                in[1][i] ^= in[0][i] >> 32;
                in[1][i] ^= in[1][i] << 32;

                in[3][i] ^= in[2][i] >> 32;
                in[3][i] ^= in[3][i] << 32;
            }
        }

        for (i = 0; i < GFBITS; i++) {
            in[3][i] ^= in[2][i] ^= in[1][i];
        }
    }
}

static void butterflies_tr(vec out[][ GFBITS ], vec in[][ GFBITS ]) {
    int i, j, k, s, b;

    vec tmp[ GFBITS ];
    vec pre[6][2][ GFBITS ];
    vec buf[2][64];

    const vec consts[ 128 ][ GFBITS ] = {
#include "consts.inc"
    };

    uint64_t consts_ptr = 128;

    const unsigned char reversal[128] = {
        0, 64, 32, 96, 16, 80, 48, 112,
        8, 72, 40, 104, 24, 88, 56, 120,
        4, 68, 36, 100, 20, 84, 52, 116,
        12, 76, 44, 108, 28, 92, 60, 124,
        2, 66, 34, 98, 18, 82, 50, 114,
        10, 74, 42, 106, 26, 90, 58, 122,
        6, 70, 38, 102, 22, 86, 54, 118,
        14, 78, 46, 110, 30, 94, 62, 126,
        1, 65, 33, 97, 17, 81, 49, 113,
        9, 73, 41, 105, 25, 89, 57, 121,
        5, 69, 37, 101, 21, 85, 53, 117,
        13, 77, 45, 109, 29, 93, 61, 125,
        3, 67, 35, 99, 19, 83, 51, 115,
        11, 75, 43, 107, 27, 91, 59, 123,
        7, 71, 39, 103, 23, 87, 55, 119,
        15, 79, 47, 111, 31, 95, 63, 127
    };

    const uint16_t beta[6] = {5246, 5306, 6039, 6685, 4905, 6755};

    //

    for (i = 6; i >= 0; i--) {
        s = 1 << i;
        consts_ptr -= s;

        for (j = 0; j < 128; j += 2 * s) {
            for (k = j; k < j + s; k++) {
                for (b = 0; b < GFBITS; b++) {
                    in[k][b] ^= in[k + s][b];
                }

                PQCLEAN_MCELIECE8192128F_VEC_vec_mul(tmp, in[k], consts[ consts_ptr + (k - j) ]);

                for (b = 0; b < GFBITS; b++) {
                    in[k + s][b] ^= tmp[b];
                }
            }
        }
    }

    for (i = 0; i < GFBITS; i++) {
        for (k = 0; k < 128; k++) {
            (&buf[0][0])[ k ] = in[ reversal[k] ][i];
        }

        PQCLEAN_MCELIECE8192128F_VEC_transpose_64x64(buf[0], buf[0]);
        PQCLEAN_MCELIECE8192128F_VEC_transpose_64x64(buf[1], buf[1]);

        for (k = 0; k < 2; k++) {
            pre[0][k][i] = buf[k][32];
            buf[k][33] ^= buf[k][32];
            pre[1][k][i] = buf[k][33];
            buf[k][35] ^= buf[k][33];
            pre[0][k][i] ^= buf[k][35];
            buf[k][34] ^= buf[k][35];
            pre[2][k][i] = buf[k][34];
            buf[k][38] ^= buf[k][34];
            pre[0][k][i] ^= buf[k][38];
            buf[k][39] ^= buf[k][38];
            pre[1][k][i] ^= buf[k][39];
            buf[k][37] ^= buf[k][39];
            pre[0][k][i] ^= buf[k][37];
            buf[k][36] ^= buf[k][37];
            pre[3][k][i] = buf[k][36];
            buf[k][44] ^= buf[k][36];
            pre[0][k][i] ^= buf[k][44];
            buf[k][45] ^= buf[k][44];
            pre[1][k][i] ^= buf[k][45];
            buf[k][47] ^= buf[k][45];
            pre[0][k][i] ^= buf[k][47];
            buf[k][46] ^= buf[k][47];
            pre[2][k][i] ^= buf[k][46];
            buf[k][42] ^= buf[k][46];
            pre[0][k][i] ^= buf[k][42];
            buf[k][43] ^= buf[k][42];
            pre[1][k][i] ^= buf[k][43];
            buf[k][41] ^= buf[k][43];
            pre[0][k][i] ^= buf[k][41];
            buf[k][40] ^= buf[k][41];
            pre[4][k][i] = buf[k][40];
            buf[k][56] ^= buf[k][40];
            pre[0][k][i] ^= buf[k][56];
            buf[k][57] ^= buf[k][56];
            pre[1][k][i] ^= buf[k][57];
            buf[k][59] ^= buf[k][57];
            pre[0][k][i] ^= buf[k][59];
            buf[k][58] ^= buf[k][59];
            pre[2][k][i] ^= buf[k][58];
            buf[k][62] ^= buf[k][58];
            pre[0][k][i] ^= buf[k][62];
            buf[k][63] ^= buf[k][62];
            pre[1][k][i] ^= buf[k][63];
            buf[k][61] ^= buf[k][63];
            pre[0][k][i] ^= buf[k][61];
            buf[k][60] ^= buf[k][61];
            pre[3][k][i] ^= buf[k][60];
            buf[k][52] ^= buf[k][60];
            pre[0][k][i] ^= buf[k][52];
            buf[k][53] ^= buf[k][52];
            pre[1][k][i] ^= buf[k][53];
            buf[k][55] ^= buf[k][53];
            pre[0][k][i] ^= buf[k][55];
            buf[k][54] ^= buf[k][55];
            pre[2][k][i] ^= buf[k][54];
            buf[k][50] ^= buf[k][54];
            pre[0][k][i] ^= buf[k][50];
            buf[k][51] ^= buf[k][50];
            pre[1][k][i] ^= buf[k][51];
            buf[k][49] ^= buf[k][51];
            pre[0][k][i] ^= buf[k][49];
            buf[k][48] ^= buf[k][49];
            pre[5][k][i] = buf[k][48];
            buf[k][16] ^= buf[k][48];
            pre[0][k][i] ^= buf[k][16];
            buf[k][17] ^= buf[k][16];
            pre[1][k][i] ^= buf[k][17];
            buf[k][19] ^= buf[k][17];
            pre[0][k][i] ^= buf[k][19];
            buf[k][18] ^= buf[k][19];
            pre[2][k][i] ^= buf[k][18];
            buf[k][22] ^= buf[k][18];
            pre[0][k][i] ^= buf[k][22];
            buf[k][23] ^= buf[k][22];
            pre[1][k][i] ^= buf[k][23];
            buf[k][21] ^= buf[k][23];
            pre[0][k][i] ^= buf[k][21];
            buf[k][20] ^= buf[k][21];
            pre[3][k][i] ^= buf[k][20];
            buf[k][28] ^= buf[k][20];
            pre[0][k][i] ^= buf[k][28];
            buf[k][29] ^= buf[k][28];
            pre[1][k][i] ^= buf[k][29];
            buf[k][31] ^= buf[k][29];
            pre[0][k][i] ^= buf[k][31];
            buf[k][30] ^= buf[k][31];
            pre[2][k][i] ^= buf[k][30];
            buf[k][26] ^= buf[k][30];
            pre[0][k][i] ^= buf[k][26];
            buf[k][27] ^= buf[k][26];
            pre[1][k][i] ^= buf[k][27];
            buf[k][25] ^= buf[k][27];
            pre[0][k][i] ^= buf[k][25];
            buf[k][24] ^= buf[k][25];
            pre[4][k][i] ^= buf[k][24];
            buf[k][8] ^= buf[k][24];
            pre[0][k][i] ^= buf[k][8];
            buf[k][9] ^= buf[k][8];
            pre[1][k][i] ^= buf[k][9];
            buf[k][11] ^= buf[k][9];
            pre[0][k][i] ^= buf[k][11];
            buf[k][10] ^= buf[k][11];
            pre[2][k][i] ^= buf[k][10];
            buf[k][14] ^= buf[k][10];
            pre[0][k][i] ^= buf[k][14];
            buf[k][15] ^= buf[k][14];
            pre[1][k][i] ^= buf[k][15];
            buf[k][13] ^= buf[k][15];
            pre[0][k][i] ^= buf[k][13];
            buf[k][12] ^= buf[k][13];
            pre[3][k][i] ^= buf[k][12];
            buf[k][4] ^= buf[k][12];
            pre[0][k][i] ^= buf[k][4];
            buf[k][5] ^= buf[k][4];
            pre[1][k][i] ^= buf[k][5];
            buf[k][7] ^= buf[k][5];
            pre[0][k][i] ^= buf[k][7];
            buf[k][6] ^= buf[k][7];
            pre[2][k][i] ^= buf[k][6];
            buf[k][2] ^= buf[k][6];
            pre[0][k][i] ^= buf[k][2];
            buf[k][3] ^= buf[k][2];
            pre[1][k][i] ^= buf[k][3];
            buf[k][1] ^= buf[k][3];

            pre[0][k][i] ^= buf[k][1];
            out[k][i] = buf[k][0] ^ buf[k][1];
        }
    }

    for (j = 0; j < GFBITS; j++) {
        tmp[j] = PQCLEAN_MCELIECE8192128F_VEC_vec_setbits((beta[0] >> j) & 1);
    }

    PQCLEAN_MCELIECE8192128F_VEC_vec_mul(out[2], pre[0][0], tmp);
    PQCLEAN_MCELIECE8192128F_VEC_vec_mul(out[3], pre[0][1], tmp);

    for (i = 1; i < 6; i++) {
        for (j = 0; j < GFBITS; j++) {
            tmp[j] = PQCLEAN_MCELIECE8192128F_VEC_vec_setbits((beta[i] >> j) & 1);
        }

        PQCLEAN_MCELIECE8192128F_VEC_vec_mul(pre[i][0], pre[i][0], tmp);
        PQCLEAN_MCELIECE8192128F_VEC_vec_mul(pre[i][1], pre[i][1], tmp);

        for (b = 0; b < GFBITS; b++) {
            out[2][b] ^= pre[i][0][b];
            out[3][b] ^= pre[i][1][b];
        }
    }

}

void PQCLEAN_MCELIECE8192128F_VEC_fft_tr(vec out[][GFBITS], vec in[][ GFBITS ]) {
    butterflies_tr(out, in);

    radix_conversions_tr(out);
}

