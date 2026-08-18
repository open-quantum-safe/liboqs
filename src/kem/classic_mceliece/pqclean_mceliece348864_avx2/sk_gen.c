/*
  This file is for secret-key generation
*/

#include "sk_gen.h"

#include "controlbits.h"
#include "crypto_declassify.h"
#include "crypto_uint16.h"
#include "gf.h"
#include "params.h"
#include "randombytes.h"
#include "transpose.h"
#include "util.h"
#include "vec.h"

static inline crypto_uint16 gf_is_zero_declassify(gf t) {
    crypto_uint16 mask = crypto_uint16_zero_mask(t);
    crypto_declassify(&mask, sizeof mask);
    return mask;
}

/* input: v, a list of GF(2^m) elements in bitsliced form */
/* input: idx, an index */
/* return: the idx-th element in v */
static inline gf extract_gf(const vec v[GFBITS], int idx) {
    int i;
    gf ret;

    ret = 0;
    for (i = GFBITS - 1; i >= 0; i--) {
        ret <<= 1;
        ret |= (v[i] >> idx) & 1;
    }

    return ret;
}

/* same as extract_gf but reduces return value to 1 bit */
static inline vec extract_bit(const vec v[GFBITS], int idx) {
    int i;
    vec ret;

    ret = 0;
    for (i = GFBITS - 1; i >= 0; i--) {
        ret |= v[i];
    }

    return (ret >> idx) & 1;
}

/* input: f, element in GF((2^m)^t) */
/* output: out, minimal polynomial of f */
/* return: 0 for success and -1 for failure */
int genpoly_gen(gf *out, gf *f) {
    int i, j, k;

    gf t, inv;

    vec v[ GFBITS ], buf[ GFBITS ][ 64 ], mat[ 64 ][ GFBITS ], mask;

    // fill matrix

    buf[0][0] = 1;
    for (i = 1; i < GFBITS; i++) {
        buf[i][0] = 0;
    }

    for (j = 0; j < GFBITS; j++) {
        for (i = SYS_T - 1; i >= 0; i--) {
            v[j] <<= 1;
            v[j] |= (f[i] >> j) & 1;
        }
    }

    for (i = 0; i < GFBITS; i++) {
        buf[i][1] = v[i];
    }

    for (k = 2; k <= SYS_T; k++) {
        vec_GF_mul(v, v, f);

        if (k < SYS_T) {
            for (i = 0; i < GFBITS; i++) {
                buf[i][k] = v[i];
            }
        } else {
            for (i = 0; i < SYS_T; i++) {
                out[i] = extract_gf(v, i);
            }
        }
    }

    for (i = 0; i < GFBITS; i++) {
        transpose_64x64(buf[i]);
    }

    for (j = 0; j < SYS_T; j++) {
        for (i = 0; i < GFBITS; i++) {
            mat[j][i] = buf[i][j];
        }
    }

    // gaussian

    for (i = 0; i < SYS_T; i++) {
        for (j = i + 1; j < SYS_T; j++) {
            mask = extract_bit(mat[i], i);
            mask -= 1;

            for (k = 0; k < GFBITS; k++) {
                mat[i][k] ^= mat[j][k] & mask;
            }

            out[i] ^= out[j] & mask;
        }

        //

        t = extract_gf(mat[i], i);

        if (gf_is_zero_declassify(t)) {
            return -1;    // return if not systematic
        }

        //

        inv = gf_inv(t);
        vec_mul_gf(mat[i], mat[i], inv);

        out[i] = gf_mul(out[i], inv);

        for (j = 0; j < SYS_T; j++) {
            if (j != i) {
                t = extract_gf(mat[j], i);

                vec_mul_gf(v, mat[i], t);
                vec_add(mat[j], mat[j], v);

                out[j] ^= gf_mul(out[i], t);
            }
        }
    }

    return 0;
}
