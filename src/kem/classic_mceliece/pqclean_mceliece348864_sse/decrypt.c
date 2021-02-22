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

static void scaling(vec128 out[][GFBITS], vec128 inv[][GFBITS], const unsigned char *sk, vec128 *recv) {
    int i, j;

    uint64_t irr_int[ GFBITS ];
    vec128 eval[32][ GFBITS ];
    vec128 tmp[ GFBITS ];

    //

    PQCLEAN_MCELIECE348864_SSE_irr_load(irr_int, sk);

    PQCLEAN_MCELIECE348864_SSE_fft(eval, irr_int);

    for (i = 0; i < 32; i++) {
        PQCLEAN_MCELIECE348864_SSE_vec128_sq(eval[i], eval[i]);
    }

    PQCLEAN_MCELIECE348864_SSE_vec128_copy(inv[0], eval[0]);

    for (i = 1; i < 32; i++) {
        PQCLEAN_MCELIECE348864_SSE_vec128_mul(inv[i], inv[i - 1], eval[i]);
    }

    PQCLEAN_MCELIECE348864_SSE_vec128_inv(tmp, inv[31]);

    for (i = 30; i >= 0; i--) {
        PQCLEAN_MCELIECE348864_SSE_vec128_mul(inv[i + 1], tmp, inv[i]);
        PQCLEAN_MCELIECE348864_SSE_vec128_mul(tmp, tmp, eval[i + 1]);
    }

    PQCLEAN_MCELIECE348864_SSE_vec128_copy(inv[0], tmp);

    //

    for (i = 0; i < 32; i++) {
        for (j = 0; j < GFBITS; j++) {
            out[i][j] = PQCLEAN_MCELIECE348864_SSE_vec128_and(inv[i][j], recv[i]);
        }
    }
}

static void preprocess(vec128 *recv, const unsigned char *s) {
    int i;
    uint8_t r[ 512 ];

    for (i = 0; i < SYND_BYTES; i++) {
        r[i] = s[i];
    }

    for (i = SYND_BYTES; i < 512; i++) {
        r[i] = 0;
    }

    for (i = 0; i < 32; i++) {
        recv[i] = PQCLEAN_MCELIECE348864_SSE_load16(r + i * 16);
    }
}

static void postprocess(unsigned char *e, vec128 *err) {
    int i;
    unsigned char error8[ (1 << GFBITS) / 8 ];
    uint64_t v[2];

    for (i = 0; i < 32; i++) {
        v[0] = PQCLEAN_MCELIECE348864_SSE_vec128_extract(err[i], 0);
        v[1] = PQCLEAN_MCELIECE348864_SSE_vec128_extract(err[i], 1);

        PQCLEAN_MCELIECE348864_SSE_store8(error8 + i * 16 + 0, v[0]);
        PQCLEAN_MCELIECE348864_SSE_store8(error8 + i * 16 + 8, v[1]);
    }

    for (i = 0; i < SYS_N / 8; i++) {
        e[i] = error8[i];
    }
}

static void scaling_inv(vec128 out[][GFBITS], vec128 inv[][GFBITS], vec128 *recv) {
    int i, j;

    for (i = 0; i < 32; i++) {
        for (j = 0; j < GFBITS; j++) {
            out[i][j] = PQCLEAN_MCELIECE348864_SSE_vec128_and(inv[i][j], recv[i]);
        }
    }
}

static uint16_t weight_check(unsigned char *e, vec128 *error) {
    int i;
    uint16_t w0 = 0;
    uint16_t w1 = 0;
    uint16_t check;

    for (i = 0; i < 32; i++) {
        w0 += _mm_popcnt_u64( PQCLEAN_MCELIECE348864_SSE_vec128_extract(error[i], 0) );
        w0 += _mm_popcnt_u64( PQCLEAN_MCELIECE348864_SSE_vec128_extract(error[i], 1) );
    }

    for (i = 0; i < SYS_N / 8; i++) {
        w1 += _mm_popcnt_u32( e[i] );
    }

    check = (w0 ^ SYS_T) | (w1 ^ SYS_T);
    check -= 1;
    check >>= 15;

    return check;
}

static uint64_t synd_cmp(vec128 s0[ GFBITS ], vec128 s1[ GFBITS ]) {
    int i;
    vec128 diff;

    diff = PQCLEAN_MCELIECE348864_SSE_vec128_xor(s0[0], s1[0]);

    for (i = 1; i < GFBITS; i++) {
        diff = PQCLEAN_MCELIECE348864_SSE_vec128_or(diff, PQCLEAN_MCELIECE348864_SSE_vec128_xor(s0[i], s1[i]));
    }

    return PQCLEAN_MCELIECE348864_SSE_vec128_testz(diff);
}

/* Niederreiter decryption with the Berlekamp decoder */
/* intput: sk, secret key */
/*         c, ciphertext (syndrome) */
/* output: e, error vector */
/* return: 0 for success; 1 for failure */
int PQCLEAN_MCELIECE348864_SSE_decrypt(unsigned char *e, const unsigned char *sk, const unsigned char *c) {
    int i;

    uint16_t check_synd;
    uint16_t check_weight;

    vec128 inv[ 32 ][ GFBITS ];
    vec128 scaled[ 32 ][ GFBITS ];
    vec128 eval[ 32 ][ GFBITS ];

    vec128 error[ 32 ];

    vec128 s_priv[ GFBITS ];
    vec128 s_priv_cmp[ GFBITS ];

    uint64_t locator[ GFBITS ];

    vec128 recv[ 32 ];
    vec128 allone;

    uint64_t bits_int[23][32];

    // Berlekamp decoder

    preprocess(recv, c);

    PQCLEAN_MCELIECE348864_SSE_load_bits(bits_int, sk + IRR_BYTES);
    PQCLEAN_MCELIECE348864_SSE_benes((uint64_t *) recv, bits_int, 1);

    scaling(scaled, inv, sk, recv);

    PQCLEAN_MCELIECE348864_SSE_fft_tr(s_priv, scaled);

    PQCLEAN_MCELIECE348864_SSE_bm(locator, s_priv);

    PQCLEAN_MCELIECE348864_SSE_fft(eval, locator);

    // reencryption and weight check

    allone = PQCLEAN_MCELIECE348864_SSE_vec128_setbits(1);

    for (i = 0; i < 32; i++) {
        error[i] = PQCLEAN_MCELIECE348864_SSE_vec128_or_reduce(eval[i]);
        error[i] = PQCLEAN_MCELIECE348864_SSE_vec128_xor(error[i], allone);
    }

    scaling_inv(scaled, inv, error);
    PQCLEAN_MCELIECE348864_SSE_fft_tr(s_priv_cmp, scaled);

    check_synd = synd_cmp(s_priv, s_priv_cmp);

    //

    PQCLEAN_MCELIECE348864_SSE_benes((uint64_t *) error, bits_int, 0);

    postprocess(e, error);

    check_weight = weight_check(e, error);

    return 1 - (check_synd & check_weight);
}

