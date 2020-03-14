/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/

#include "fft.h"

#include "transpose.h"

#include <stdint.h>

/* input: in, polynomial in bitsliced form */
/* output: in, result of applying the radix conversions on in */
static void radix_conversions(vec in[][GFBITS]) {
    int i, j, k;

    const vec mask[5][2] = {
        {0x8888888888888888, 0x4444444444444444},
        {0xC0C0C0C0C0C0C0C0, 0x3030303030303030},
        {0xF000F000F000F000, 0x0F000F000F000F00},
        {0xFF000000FF000000, 0x00FF000000FF0000},
        {0xFFFF000000000000, 0x0000FFFF00000000}
    };

    const vec s[5][2][GFBITS] = {
#include "scalars_2x.inc"
    };

    for (j = 0; j <= 5; j++) {
        for (i = 0; i < GFBITS; i++) {
            in[1][i] ^= in[1][i] >> 32;
            in[0][i] ^= in[1][i] << 32;
        }

        for (i = 0; i < GFBITS; i++) {
            for (k = 4; k >= j; k--) {
                in[0][i] ^= (in[0][i] & mask[k][0]) >> (1 << k);
                in[0][i] ^= (in[0][i] & mask[k][1]) >> (1 << k);
                in[1][i] ^= (in[1][i] & mask[k][0]) >> (1 << k);
                in[1][i] ^= (in[1][i] & mask[k][1]) >> (1 << k);
            }
        }

        if (j < 5) {
            PQCLEAN_MCELIECE6688128F_VEC_vec_mul(in[0], in[0], s[j][0]);
            PQCLEAN_MCELIECE6688128F_VEC_vec_mul(in[1], in[1], s[j][1]);
        }
    }
}

/* input: in, result of applying the radix conversions to the input polynomial */
/* output: out, evaluation results (by applying the FFT butterflies) */
static void butterflies(vec out[][ GFBITS ], vec in[][ GFBITS ]) {
    int i, j, k, s, b;

    vec tmp[ GFBITS ];
    vec pre[8][ GFBITS ];
    vec buf[128];

    uint64_t consts_ptr = 2;

    const vec consts[ 128 ][ GFBITS ] = {
#include "consts.inc"
    };

    const vec powers[ 128 ][ GFBITS ] = {
#include "powers.inc"
    };

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

    const uint16_t beta[7] = {2522, 7827, 7801, 8035, 6897, 8167, 3476};

    //

    for (i = 0; i < 7; i++) {
        for (j = 0; j < GFBITS; j++) {
            pre[i][j] = (beta[i] >> j) & 1;
            pre[i][j] = -pre[i][j];
        }

        PQCLEAN_MCELIECE6688128F_VEC_vec_mul(pre[i], in[1], pre[i]);
    }

    for (i = 0; i < GFBITS; i++) {
        buf[0] = in[0][i];

        buf[1] = buf[0] ^ pre[0][i];
        buf[32] = in[0][i] ^ pre[5][i];
        buf[3] = buf[1] ^ pre[1][i];
        buf[96] = buf[32] ^ pre[6][i];
        buf[97] = buf[96] ^ pre[0][i];
        buf[2] = in[0][i] ^ pre[1][i];
        buf[99] = buf[97] ^ pre[1][i];
        buf[6] = buf[2] ^ pre[2][i];
        buf[98] = buf[99] ^ pre[0][i];
        buf[7] = buf[6] ^ pre[0][i];
        buf[102] = buf[98] ^ pre[2][i];
        buf[5] = buf[7] ^ pre[1][i];
        buf[103] = buf[102] ^ pre[0][i];
        buf[101] = buf[103] ^ pre[1][i];
        buf[4] = in[0][i] ^ pre[2][i];
        buf[100] = buf[101] ^ pre[0][i];
        buf[12] = buf[4] ^ pre[3][i];
        buf[108] = buf[100] ^ pre[3][i];
        buf[13] = buf[12] ^ pre[0][i];
        buf[109] = buf[108] ^ pre[0][i];
        buf[15] = buf[13] ^ pre[1][i];
        buf[111] = buf[109] ^ pre[1][i];
        buf[14] = buf[15] ^ pre[0][i];
        buf[110] = buf[111] ^ pre[0][i];
        buf[10] = buf[14] ^ pre[2][i];
        buf[106] = buf[110] ^ pre[2][i];
        buf[11] = buf[10] ^ pre[0][i];
        buf[107] = buf[106] ^ pre[0][i];
        buf[9] = buf[11] ^ pre[1][i];
        buf[105] = buf[107] ^ pre[1][i];
        buf[104] = buf[105] ^ pre[0][i];
        buf[8] = in[0][i] ^ pre[3][i];
        buf[120] = buf[104] ^ pre[4][i];
        buf[24] = buf[8] ^ pre[4][i];
        buf[121] = buf[120] ^ pre[0][i];
        buf[25] = buf[24] ^ pre[0][i];
        buf[123] = buf[121] ^ pre[1][i];
        buf[27] = buf[25] ^ pre[1][i];
        buf[122] = buf[123] ^ pre[0][i];
        buf[26] = buf[27] ^ pre[0][i];
        buf[126] = buf[122] ^ pre[2][i];
        buf[30] = buf[26] ^ pre[2][i];
        buf[127] = buf[126] ^ pre[0][i];
        buf[31] = buf[30] ^ pre[0][i];
        buf[125] = buf[127] ^ pre[1][i];
        buf[29] = buf[31] ^ pre[1][i];
        buf[124] = buf[125] ^ pre[0][i];
        buf[28] = buf[29] ^ pre[0][i];
        buf[116] = buf[124] ^ pre[3][i];
        buf[20] = buf[28] ^ pre[3][i];
        buf[117] = buf[116] ^ pre[0][i];
        buf[21] = buf[20] ^ pre[0][i];
        buf[119] = buf[117] ^ pre[1][i];
        buf[23] = buf[21] ^ pre[1][i];
        buf[118] = buf[119] ^ pre[0][i];
        buf[22] = buf[23] ^ pre[0][i];
        buf[114] = buf[118] ^ pre[2][i];
        buf[18] = buf[22] ^ pre[2][i];
        buf[115] = buf[114] ^ pre[0][i];
        buf[19] = buf[18] ^ pre[0][i];
        buf[113] = buf[115] ^ pre[1][i];
        buf[17] = buf[19] ^ pre[1][i];
        buf[112] = buf[113] ^ pre[0][i];
        buf[80] = buf[112] ^ pre[5][i];
        buf[16] = in[0][i] ^ pre[4][i];
        buf[81] = buf[80] ^ pre[0][i];
        buf[48] = buf[16] ^ pre[5][i];
        buf[83] = buf[81] ^ pre[1][i];
        buf[49] = buf[48] ^ pre[0][i];
        buf[82] = buf[83] ^ pre[0][i];
        buf[51] = buf[49] ^ pre[1][i];
        buf[86] = buf[82] ^ pre[2][i];
        buf[50] = buf[51] ^ pre[0][i];
        buf[87] = buf[86] ^ pre[0][i];
        buf[54] = buf[50] ^ pre[2][i];
        buf[85] = buf[87] ^ pre[1][i];
        buf[55] = buf[54] ^ pre[0][i];
        buf[84] = buf[85] ^ pre[0][i];
        buf[53] = buf[55] ^ pre[1][i];
        buf[92] = buf[84] ^ pre[3][i];
        buf[52] = buf[53] ^ pre[0][i];
        buf[93] = buf[92] ^ pre[0][i];
        buf[60] = buf[52] ^ pre[3][i];
        buf[95] = buf[93] ^ pre[1][i];
        buf[61] = buf[60] ^ pre[0][i];
        buf[94] = buf[95] ^ pre[0][i];
        buf[63] = buf[61] ^ pre[1][i];
        buf[90] = buf[94] ^ pre[2][i];
        buf[62] = buf[63] ^ pre[0][i];
        buf[91] = buf[90] ^ pre[0][i];
        buf[58] = buf[62] ^ pre[2][i];
        buf[89] = buf[91] ^ pre[1][i];
        buf[59] = buf[58] ^ pre[0][i];
        buf[88] = buf[89] ^ pre[0][i];
        buf[57] = buf[59] ^ pre[1][i];
        buf[72] = buf[88] ^ pre[4][i];
        buf[56] = buf[57] ^ pre[0][i];
        buf[73] = buf[72] ^ pre[0][i];
        buf[40] = buf[56] ^ pre[4][i];
        buf[75] = buf[73] ^ pre[1][i];
        buf[41] = buf[40] ^ pre[0][i];
        buf[74] = buf[75] ^ pre[0][i];
        buf[43] = buf[41] ^ pre[1][i];
        buf[78] = buf[74] ^ pre[2][i];
        buf[42] = buf[43] ^ pre[0][i];
        buf[79] = buf[78] ^ pre[0][i];
        buf[46] = buf[42] ^ pre[2][i];
        buf[77] = buf[79] ^ pre[1][i];
        buf[47] = buf[46] ^ pre[0][i];
        buf[76] = buf[77] ^ pre[0][i];
        buf[45] = buf[47] ^ pre[1][i];
        buf[68] = buf[76] ^ pre[3][i];
        buf[44] = buf[45] ^ pre[0][i];
        buf[69] = buf[68] ^ pre[0][i];
        buf[36] = buf[44] ^ pre[3][i];
        buf[71] = buf[69] ^ pre[1][i];
        buf[37] = buf[36] ^ pre[0][i];
        buf[70] = buf[71] ^ pre[0][i];
        buf[39] = buf[37] ^ pre[1][i];
        buf[66] = buf[70] ^ pre[2][i];
        buf[38] = buf[39] ^ pre[0][i];
        buf[67] = buf[66] ^ pre[0][i];
        buf[34] = buf[38] ^ pre[2][i];
        buf[65] = buf[67] ^ pre[1][i];
        buf[35] = buf[34] ^ pre[0][i];
        buf[33] = buf[35] ^ pre[1][i];
        buf[64] = in[0][i] ^ pre[6][i];

        PQCLEAN_MCELIECE6688128F_VEC_transpose_64x64(buf +  0, buf +  0);
        PQCLEAN_MCELIECE6688128F_VEC_transpose_64x64(buf + 64, buf + 64);

        for (j = 0; j < 128; j++) {
            out[ reversal[j] ][i] = buf[j];
        }
    }

    for (i = 1; i <= 6; i++) {
        s = 1 << i;

        for (j = 0; j < 128; j += 2 * s) {
            for (k = j; k < j + s; k++) {
                PQCLEAN_MCELIECE6688128F_VEC_vec_mul(tmp, out[k + s], consts[ consts_ptr + (k - j) ]);

                for (b = 0; b < GFBITS; b++) {
                    out[k  ][b] ^= tmp[b];
                }
                for (b = 0; b < GFBITS; b++) {
                    out[k + s][b] ^= out[k][b];
                }
            }
        }

        consts_ptr += ((uint64_t)1 << i);
    }

    // adding the part contributed by x^128

    for (i = 0; i < 128; i++) {
        for (b = 0; b < GFBITS; b++) {
            out[i][b] ^= powers[i][b];
        }
    }
}

/* input: in, polynomial in bitsliced form */
/* output: out, bitsliced results of evaluating in all the field elements */
void PQCLEAN_MCELIECE6688128F_VEC_fft(vec out[][GFBITS], vec in[][GFBITS]) {
    radix_conversions(in);
    butterflies(out, in);
}
