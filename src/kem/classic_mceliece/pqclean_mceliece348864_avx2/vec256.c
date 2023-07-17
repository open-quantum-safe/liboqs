/*
  This file is for functions related to 256-bit vectors
  including functions for bitsliced field operations
*/

#include "vec256.h"

/* bitsliced field squarings */
void vec256_sq(vec256 *out, vec256 *in) {
    int i;
    vec256 result[GFBITS];

    result[0] = in[0] ^ in[6];
    result[1] = in[11];
    result[2] = in[1] ^ in[7];
    result[3] = in[6];
    result[4] = in[2] ^ in[11] ^ in[8];
    result[5] = in[7];
    result[6] = in[3] ^ in[9];
    result[7] = in[8];
    result[8] = in[4] ^ in[10];
    result[9] = in[9];
    result[10] = in[5] ^ in[11];
    result[11] = in[10];

    for (i = 0; i < GFBITS; i++) {
        out[i] = result[i];
    }
}

/* bitsliced field inverses */
void vec256_inv(vec256 *out, vec256 *in) {
    vec256 tmp_11[ GFBITS ];
    vec256 tmp_1111[ GFBITS ];

    vec256_copy(out, in);

    vec256_sq(out, out);
    vec256_mul(tmp_11, out, in); // ^11

    vec256_sq(out, tmp_11);
    vec256_sq(out, out);
    vec256_mul(tmp_1111, out, tmp_11); // ^1111

    vec256_sq(out, tmp_1111);
    vec256_sq(out, out);
    vec256_sq(out, out);
    vec256_sq(out, out);
    vec256_mul(out, out, tmp_1111); // ^11111111

    vec256_sq(out, out);
    vec256_sq(out, out);
    vec256_mul(out, out, tmp_11); // ^1111111111
    vec256_sq(out, out);
    vec256_mul(out, out, in); // ^11111111111

    vec256_sq(out, out); // ^111111111110
}
