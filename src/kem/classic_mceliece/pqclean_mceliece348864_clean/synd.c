/*
  This file is for syndrome computation
*/

#include "synd.h"

#include "params.h"
#include "root.h"


/* input: Goppa polynomial f, support L, received word r */
/* output: out, the syndrome of length 2t */
void PQCLEAN_MCELIECE348864_CLEAN_synd(gf *out, gf *f, gf *L, const unsigned char *r) {
    int i, j;
    gf e, e_inv, c;

    for (j = 0; j < 2 * SYS_T; j++) {
        out[j] = 0;
    }

    for (i = 0; i < SYS_N; i++) {
        c = (r[i / 8] >> (i % 8)) & 1;

        e = PQCLEAN_MCELIECE348864_CLEAN_eval(f, L[i]);
        e_inv = PQCLEAN_MCELIECE348864_CLEAN_gf_inv(PQCLEAN_MCELIECE348864_CLEAN_gf_mul(e, e));

        for (j = 0; j < 2 * SYS_T; j++) {
            out[j] = PQCLEAN_MCELIECE348864_CLEAN_gf_add(out[j], PQCLEAN_MCELIECE348864_CLEAN_gf_mul(e_inv, c));
            e_inv = PQCLEAN_MCELIECE348864_CLEAN_gf_mul(e_inv, L[i]);
        }
    }
}

