/*
  This file is for syndrome computation
*/

#include "synd.h"

#include "params.h"
#include "root.h"

#include <stdio.h>

/* input: Goppa polynomial f, support L, received word r */
/* output: out, the syndrome of length 2t */
void synd(gf *out, gf *f, gf *L, const unsigned char *r) {
    int i, j;
    gf e, e_inv, c;

    for (j = 0; j < 2 * SYS_T; j++) {
        out[j] = 0;
    }

    for (i = 0; i < SYS_N; i++) {
        c = (r[i / 8] >> (i % 8)) & 1;

        e = eval(f, L[i]);
        e_inv = gf_inv(gf_mul(e, e));

        for (j = 0; j < 2 * SYS_T; j++) {
            out[j] = gf_add(out[j], gf_mul(e_inv, c));
            e_inv = gf_mul(e_inv, L[i]);
        }
    }
}
