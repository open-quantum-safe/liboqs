/*
  This file is for Niederreiter decryption
*/

#include "decrypt.h"

#include "benes.h"
#include "bm.h"
#include "fft.h"
#include "fft_tr.h"
#include "params.h"
#include "util.h"

#include <stdio.h>

static void scaling(vec256 out[][GFBITS], vec256 inv[][GFBITS], const unsigned char *sk, vec256 *recv) {
    int i, j;

    vec128 sk_int[ GFBITS ];
    vec256 eval[32][ GFBITS ];
    vec256 tmp[ GFBITS ];

    // computing inverses

    irr_load(sk_int, sk);

    fft(eval, sk_int);

    for (i = 0; i < 32; i++) {
        vec256_sq(eval[i], eval[i]);
    }

    vec256_copy(inv[0], eval[0]);

    for (i = 1; i < 32; i++) {
        vec256_mul(inv[i], inv[i - 1], eval[i]);
    }

    vec256_inv(tmp, inv[31]);

    for (i = 30; i >= 0; i--) {
        vec256_mul(inv[i + 1], tmp, inv[i]);
        vec256_mul(tmp, tmp, eval[i + 1]);
    }

    vec256_copy(inv[0], tmp);

    //

    for (i = 0; i < 32; i++) {
        for (j = 0; j < GFBITS; j++) {
            out[i][j] = vec256_and(inv[i][j], recv[i]);
        }
    }
}

static void scaling_inv(vec256 out[][GFBITS], vec256 inv[][GFBITS], vec256 *recv) {
    int i, j;

    for (i = 0; i < 32; i++) {
        for (j = 0; j < GFBITS; j++) {
            out[i][j] = vec256_and(inv[i][j], recv[i]);
        }
    }
}

static void preprocess(vec128 *recv, const unsigned char *s) {
    int i;

    recv[0] = vec128_setbits(0);

    for (i = 1; i < 64; i++) {
        recv[i] = recv[0];
    }

    for (i = 0; i < SYND_BYTES / 16; i++) {
        recv[i] = load16(s + i * 16);
    }
}

static int weight(vec256 *v) {
    int i, w = 0;

    for (i = 0; i < 32; i++) {
        w += __builtin_popcountll( vec256_extract(v[i], 0) );
        w += __builtin_popcountll( vec256_extract(v[i], 1) );
        w += __builtin_popcountll( vec256_extract(v[i], 2) );
        w += __builtin_popcountll( vec256_extract(v[i], 3) );
    }

    return w;
}

static uint64_t synd_cmp(vec256 *s0, vec256 *s1) {
    int i;
    vec256 diff;

    diff = vec256_xor(s0[0], s1[0]);

    for (i = 1; i < GFBITS; i++) {
        diff = vec256_or(diff, vec256_xor(s0[i], s1[i]));
    }

    return vec256_testz(diff);
}

static void reformat_128to256(vec256 *out, vec128 *in) {
    int i;
    uint64_t v[4];

    for (i = 0; i < 32; i++) {
        v[0] = vec128_extract(in[2 * i + 0], 0);
        v[1] = vec128_extract(in[2 * i + 0], 1);
        v[2] = vec128_extract(in[2 * i + 1], 0);
        v[3] = vec128_extract(in[2 * i + 1], 1);

        out[i] = vec256_set4x(v[0], v[1], v[2], v[3]);
    }
}

static void reformat_256to128(vec128 *out, vec256 *in) {
    int i;
    uint64_t v[4];

    for (i = 0; i < 32; i++) {
        v[0] = vec256_extract(in[i], 0);
        v[1] = vec256_extract(in[i], 1);
        v[2] = vec256_extract(in[i], 2);
        v[3] = vec256_extract(in[i], 3);

        out[2 * i + 0] = vec128_set2x(v[0], v[1]);
        out[2 * i + 1] = vec128_set2x(v[2], v[3]);
    }
}

/* Niederreiter decryption with the Berlekamp decoder */
/* intput: sk, secret key */
/*         s, ciphertext (syndrome) */
/* output: e, error vector */
/* return: 0 for success; 1 for failure */
int decrypt(unsigned char *e, const unsigned char *sk, const unsigned char *s) {
    int i;

    uint16_t check_synd;
    uint16_t check_weight;

    vec256 inv[ 64 ][ GFBITS ];
    vec256 scaled[ 64 ][ GFBITS ];
    vec256 eval[ 64 ][ GFBITS ];

    vec128 error128[ 64 ];
    vec256 error256[ 32 ];

    vec256 s_priv[ GFBITS ];
    vec256 s_priv_cmp[ GFBITS ];
    vec128 locator[ GFBITS ];

    vec128 recv128[ 64 ];
    vec256 recv256[ 32 ];
    vec256 allone;

    vec128 bits_int[25][32];

    // Berlekamp decoder

    preprocess(recv128, s);

    load_bits(bits_int, sk + IRR_BYTES);
    benes(recv128, bits_int, 1);
    reformat_128to256(recv256, recv128);

    scaling(scaled, inv, sk, recv256); // scaling
    fft_tr(s_priv, scaled); // transposed FFT
    bm(locator, s_priv); // Berlekamp Massey

    fft(eval, locator); // FFT

    // reencryption and weight check

    allone = vec256_set1_16b(0xFFFF);

    for (i = 0; i < 32; i++) {
        error256[i] = vec256_or_reduce(eval[i]);
        error256[i] = vec256_xor(error256[i], allone);
    }

    check_weight = weight(error256) ^ SYS_T;
    check_weight -= 1;
    check_weight >>= 15;

    scaling_inv(scaled, inv, error256);
    fft_tr(s_priv_cmp, scaled);

    check_synd = synd_cmp(s_priv, s_priv_cmp);

    //

    reformat_256to128(error128, error256);
    benes(error128, bits_int, 0);

    for (i = 0; i < 64; i++) {
        store16(e + i * 16, error128[i]);
    }

    return 1 - (check_synd & check_weight);
}
