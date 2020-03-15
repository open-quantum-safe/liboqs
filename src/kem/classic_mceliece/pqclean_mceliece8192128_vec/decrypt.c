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
#include "vec.h"

#include <stdio.h>

static void scaling(vec out[][GFBITS], vec inv[][GFBITS], const unsigned char *sk, const vec *recv) {
    int i, j;

    vec irr_int[2][ GFBITS ];
    vec eval[128][ GFBITS ];
    vec tmp[ GFBITS ];

    //

    PQCLEAN_MCELIECE8192128_VEC_irr_load(irr_int, sk);

    PQCLEAN_MCELIECE8192128_VEC_fft(eval, irr_int);

    for (i = 0; i < 128; i++) {
        PQCLEAN_MCELIECE8192128_VEC_vec_sq(eval[i], eval[i]);
    }

    PQCLEAN_MCELIECE8192128_VEC_vec_copy(inv[0], eval[0]);

    for (i = 1; i < 128; i++) {
        PQCLEAN_MCELIECE8192128_VEC_vec_mul(inv[i], inv[i - 1], eval[i]);
    }

    PQCLEAN_MCELIECE8192128_VEC_vec_inv(tmp, inv[127]);

    for (i = 126; i >= 0; i--) {
        PQCLEAN_MCELIECE8192128_VEC_vec_mul(inv[i + 1], tmp, inv[i]);
        PQCLEAN_MCELIECE8192128_VEC_vec_mul(tmp, tmp, eval[i + 1]);
    }

    PQCLEAN_MCELIECE8192128_VEC_vec_copy(inv[0], tmp);

    //

    for (i = 0; i < 128; i++) {
        for (j = 0; j < GFBITS; j++) {
            out[i][j] = inv[i][j] & recv[i];
        }
    }
}

static void scaling_inv(vec out[][GFBITS], vec inv[][GFBITS], const vec *recv) {
    int i, j;

    for (i = 0; i < 128; i++) {
        for (j = 0; j < GFBITS; j++) {
            out[i][j] = inv[i][j] & recv[i];
        }
    }
}

static void preprocess(vec *recv, const unsigned char *s) {
    int i;

    recv[0] = 0;

    for (i = 1; i < 128; i++) {
        recv[i] = recv[0];
    }

    for (i = 0; i < SYND_BYTES / 8; i++) {
        recv[i] = PQCLEAN_MCELIECE8192128_VEC_load8(s + i * 8);
    }
}

static uint16_t weight(const vec *v) {
    uint16_t i, w = 0;

    for (i = 0; i < SYS_N; i++) {
        w += (uint16_t)((v[i / 64] >> (i % 64)) & 1);
    }

    return w;
}

static uint16_t synd_cmp(vec s0[][ GFBITS ], vec s1[][ GFBITS ]) {
    int i, j;
    vec diff = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < GFBITS; j++) {
            diff |= (s0[i][j] ^ s1[i][j]);
        }
    }

    return (uint16_t)PQCLEAN_MCELIECE8192128_VEC_vec_testz(diff);
}

/* Niederreiter decryption with the Berlekamp decoder */
/* intput: sk, secret key */
/*         c, ciphertext (syndrome) */
/* output: e, error vector */
/* return: 0 for success; 1 for failure */
int PQCLEAN_MCELIECE8192128_VEC_decrypt(unsigned char *e, const unsigned char *sk, const unsigned char *c) {
    int i;

    uint16_t check_synd;
    uint16_t check_weight;

    vec inv[ 128 ][ GFBITS ];
    vec scaled[ 128 ][ GFBITS ];
    vec eval[ 128 ][ GFBITS ];

    vec error[ 128 ];

    vec s_priv[ 4 ][ GFBITS ];
    vec s_priv_cmp[ 4 ][ GFBITS ];
    vec locator[2][ GFBITS ];

    vec recv[ 128 ];
    vec allone;

    // Berlekamp decoder

    preprocess(recv, c);

    PQCLEAN_MCELIECE8192128_VEC_benes(recv, sk + IRR_BYTES, 1);
    scaling(scaled, inv, sk, recv);
    PQCLEAN_MCELIECE8192128_VEC_fft_tr(s_priv, scaled);
    PQCLEAN_MCELIECE8192128_VEC_bm(locator, s_priv);

    PQCLEAN_MCELIECE8192128_VEC_fft(eval, locator);

    // reencryption and weight check

    allone = PQCLEAN_MCELIECE8192128_VEC_vec_setbits(1);

    for (i = 0; i < 128; i++) {
        error[i] = PQCLEAN_MCELIECE8192128_VEC_vec_or_reduce(eval[i]);
        error[i] ^= allone;
    }

    check_weight = weight(error) ^ SYS_T;
    check_weight -= 1;
    check_weight >>= 15;

    scaling_inv(scaled, inv, error);
    PQCLEAN_MCELIECE8192128_VEC_fft_tr(s_priv_cmp, scaled);

    check_synd = synd_cmp(s_priv, s_priv_cmp);

    //

    PQCLEAN_MCELIECE8192128_VEC_benes(error, sk + IRR_BYTES, 0);

    for (i = 0; i < 128; i++) {
        PQCLEAN_MCELIECE8192128_VEC_store8(e + i * 8, error[i]);
    }

    return 1 - (check_synd & check_weight);
}

