/*
  This file is for Niederreiter decryption
*/

#include "decrypt.h"

#include "benes.h"
#include "bm.h"
#include "gf.h"
#include "params.h"
#include "root.h"
#include "synd.h"
#include "util.h"

/* Niederreiter decryption with the Berlekamp decoder */
/* intput: sk, secret key */
/*         c, ciphertext */
/* output: e, error vector */
/* return: 0 for success; 1 for failure */
int PQCLEAN_MCELIECE348864_CLEAN_decrypt(unsigned char *e, const unsigned char *sk, const unsigned char *c) {
    int i, w = 0;
    uint16_t check;

    unsigned char r[ SYS_N / 8 ];

    gf g[ SYS_T + 1 ];
    gf L[ SYS_N ];

    gf s[ SYS_T * 2 ];
    gf s_cmp[ SYS_T * 2 ];
    gf locator[ SYS_T + 1 ];
    gf images[ SYS_N ];

    gf t;

    //

    for (i = 0; i < SYND_BYTES; i++) {
        r[i] = c[i];
    }
    for (i = SYND_BYTES; i < SYS_N / 8; i++) {
        r[i] = 0;
    }

    for (i = 0; i < SYS_T; i++) {
        g[i] = PQCLEAN_MCELIECE348864_CLEAN_load2(sk);
        g[i] &= GFMASK;
        sk += 2;
    }
    g[ SYS_T ] = 1;

    PQCLEAN_MCELIECE348864_CLEAN_support_gen(L, sk);

    PQCLEAN_MCELIECE348864_CLEAN_synd(s, g, L, r);

    PQCLEAN_MCELIECE348864_CLEAN_bm(locator, s);

    PQCLEAN_MCELIECE348864_CLEAN_root(images, locator, L);

    //

    for (i = 0; i < SYS_N / 8; i++) {
        e[i] = 0;
    }

    for (i = 0; i < SYS_N; i++) {
        t = PQCLEAN_MCELIECE348864_CLEAN_gf_iszero(images[i]) & 1;

        e[ i / 8 ] |= t << (i % 8);
        w += t;

    }

    PQCLEAN_MCELIECE348864_CLEAN_synd(s_cmp, g, L, e);

    //

    check = (uint16_t)w;
    check ^= SYS_T;

    for (i = 0; i < SYS_T * 2; i++) {
        check |= s[i] ^ s_cmp[i];
    }

    check -= 1;
    check >>= 15;

    return check ^ 1;
}

