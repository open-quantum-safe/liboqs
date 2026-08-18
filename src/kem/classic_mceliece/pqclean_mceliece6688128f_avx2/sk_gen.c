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
#include "vec128.h"

static inline crypto_uint16 gf_is_zero_declassify(gf t) {
    crypto_uint16 mask = crypto_uint16_zero_mask(t);
    crypto_declassify(&mask, sizeof mask);
    return mask;
}

/* input: v, a list of GF(2^m) elements in bitsliced form */
/* input: idx, an index */
/* return: the idx-th element in v */
static inline gf extract_gf(uint64_t v[GFBITS][2], int idx) {
    int i;
    gf ret;

    ret = 0;
    for (i = GFBITS - 1; i >= 0; i--) {
        ret <<= 1;
        ret |= (v[i][idx / 64] >> (idx & 63)) & 1;
    }

    return ret;
}

/* same as extract_gf but reduces return value to 1 bit */
static inline uint64_t extract_bit(uint64_t v[GFBITS][2], int idx) {
    int i;
    uint64_t ret;

    ret = 0;
    for (i = GFBITS - 1; i >= 0; i--) {
        ret |= v[i][idx / 64];
    }

    return (ret >> (idx & 63)) & 1;
}

static void transpose_128x128(uint64_t (*in)[2]) {
    int i, j, k;
    uint64_t m[2][2][64], t;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 64; k++) {
                m[i][j][k] = in[64 * i + k][j];
            }
        }
    }

    for (k = 0; k < 64; k++) {
        t = m[0][1][k];
        m[0][1][k] = m[1][0][k];
        m[1][0][k] = t;
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            transpose_64x64(m[i][j]);
        }
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 64; k++) {
                in[64 * i + k][j] = m[i][j][k];
            }
        }
    }
}

/* input: f, element in GF((2^m)^t) */
/* output: out, minimal polynomial of f */
/* return: 0 for success and -1 for failure */
int genpoly_gen(gf *out, gf *f) {
    int i, j, k;

    gf t, inv;

    uint64_t m;
    vec128 mask;

    union {
        uint64_t d[ GFBITS ][ 128 ][2];
        vec128   v[ GFBITS ][ 128 ];
    } buf;

    union {
        uint64_t d[GFBITS][2];
        vec128   v[GFBITS];
    } v;

    union {
        uint64_t d[SYS_T][GFBITS][2];
        vec128   v[SYS_T][GFBITS];
    } mat;

    // fill matrix

    buf.v[0][0] = vec128_set2x(1, 0);
    for (i = 1; i < GFBITS; i++) {
        buf.v[i][0] = vec128_setzero();
    }

    for (j = 0; j < GFBITS; j++) {
        v.v[j] = vec128_setzero();

        for (i = SYS_T - 1; i >= 0; i--) {
            v.d[j][1] <<= 1;
            v.d[j][1] |= v.d[j][0] >> 63;
            v.d[j][0] <<= 1;
            v.d[j][0] |= (f[i] >> j) & 1;
        }
    }

    for (i = 0; i < GFBITS; i++) {
        buf.v[i][1] = v.v[i];
    }

    for (k = 2; k <= SYS_T; k++) {
        vec128_mul_GF(v.v, v.v, f);

        if (k < SYS_T) {
            for (i = 0; i < GFBITS; i++) {
                buf.v[i][k] = v.v[i];
            }
        } else {
            for (i = 0; i < SYS_T; i++) {
                out[i] = extract_gf(v.d, i);
            }
        }
    }

    for (i = 0; i < GFBITS; i++) {
        transpose_128x128(buf.d[i]);
    }

    for (j = 0; j < SYS_T; j++) {
        for (i = 0; i < GFBITS; i++) {
            mat.v[j][i] = buf.v[i][j];
        }
    }

    // gaussian

    for (i = 0; i < SYS_T; i++) {
        for (j = i + 1; j < SYS_T; j++) {
            m = extract_bit(mat.d[i], i);
            m -= 1;
            mask = vec128_set2x(m, m);

            for (k = 0; k < GFBITS; k++) {
                mat.v[i][k] ^= mat.v[j][k] & mask;
            }

            out[i] ^= out[j] & m;
        }

        //

        t = extract_gf(mat.d[i], i);

        if (gf_is_zero_declassify(t)) {
            return -1;    // return if not systematic
        }

        //

        inv = gf_inv(t);
        vec128_mul_gf(mat.v[i], mat.v[i], inv);

        out[i] = gf_mul(out[i], inv);

        for (j = 0; j < SYS_T; j++) {
            if (j != i) {
                t = extract_gf(mat.d[j], i);

                vec128_mul_gf(v.v, mat.v[i], t);
                vec128_add(mat.v[j], mat.v[j], v.v);

                out[j] ^= gf_mul(out[i], t);
            }
        }
    }

    return 0;
}
