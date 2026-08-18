/*
  This file is for matrix transposition
*/

#include "transpose.h"

#include <stdint.h>

/* input: in, a 64x64 matrix over GF(2) */
/* output: out, transpose of in */
void transpose_64x64(uint64_t *out, const uint64_t *in) {
    int i, j, s, d;

    uint64_t x, y;
    uint64_t masks[6][2] = {
        {0x5555555555555555, 0xAAAAAAAAAAAAAAAA},
        {0x3333333333333333, 0xCCCCCCCCCCCCCCCC},
        {0x0F0F0F0F0F0F0F0F, 0xF0F0F0F0F0F0F0F0},
        {0x00FF00FF00FF00FF, 0xFF00FF00FF00FF00},
        {0x0000FFFF0000FFFF, 0xFFFF0000FFFF0000},
        {0x00000000FFFFFFFF, 0xFFFFFFFF00000000}
    };

    for (i = 0; i < 64; i++) {
        out[i] = in[i];
    }

    for (d = 5; d >= 0; d--) {
        s = 1 << d;

        for (i = 0; i < 64; i += s * 2) {
            for (j = i; j < i + s; j++) {
                x = (out[j] & masks[d][0]) | ((out[j + s] & masks[d][0]) << s);
                y = ((out[j] & masks[d][1]) >> s) | (out[j + s] & masks[d][1]);

                out[j + 0] = x;
                out[j + s] = y;
            }
        }
    }
}
