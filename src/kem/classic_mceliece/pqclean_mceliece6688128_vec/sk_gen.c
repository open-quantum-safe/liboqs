/*
  This file is for secret-key generation
*/

#include "sk_gen.h"

#include "controlbits.h"
#include "gf.h"
#include "params.h"
#include "util.h"

/* input: f, element in GF((2^m)^t) */
/* output: out, minimal polynomial of f */
/* return: 0 for success and -1 for failure */
int PQCLEAN_MCELIECE6688128_VEC_genpoly_gen(gf *out, gf *f) {
    int i, j, k, c;

    gf mat[ SYS_T + 1 ][ SYS_T ];
    gf mask, inv, t;

    // fill matrix

    mat[0][0] = 1;

    for (i = 1; i < SYS_T; i++) {
        mat[0][i] = 0;
    }

    for (i = 0; i < SYS_T; i++) {
        mat[1][i] = f[i];
    }

    for (j = 2; j <= SYS_T; j++) {
        PQCLEAN_MCELIECE6688128_VEC_GF_mul(mat[j], mat[j - 1], f);
    }

    // gaussian

    for (j = 0; j < SYS_T; j++) {
        for (k = j + 1; k < SYS_T; k++) {
            mask = PQCLEAN_MCELIECE6688128_VEC_gf_iszero(mat[ j ][ j ]);

            for (c = j; c < SYS_T + 1; c++) {
                mat[ c ][ j ] ^= mat[ c ][ k ] & mask;
            }

        }

        if ( mat[ j ][ j ] == 0 ) { // return if not systematic
            return -1;
        }

        inv = PQCLEAN_MCELIECE6688128_VEC_gf_inv(mat[j][j]);

        for (c = j; c < SYS_T + 1; c++) {
            mat[ c ][ j ] = PQCLEAN_MCELIECE6688128_VEC_gf_mul(mat[ c ][ j ], inv) ;
        }

        for (k = 0; k < SYS_T; k++) {
            if (k != j) {
                t = mat[ j ][ k ];

                for (c = j; c < SYS_T + 1; c++) {
                    mat[ c ][ k ] ^= PQCLEAN_MCELIECE6688128_VEC_gf_mul(mat[ c ][ j ], t);
                }
            }
        }
    }

    for (i = 0; i < SYS_T; i++) {
        out[i] = mat[ SYS_T ][ i ];
    }

    return 0;
}

/* input: permutation p represented as a list of 32-bit intergers */
/* output: -1 if some interger repeats in p */
/*          0 otherwise */
int PQCLEAN_MCELIECE6688128_VEC_perm_check(const uint32_t *p) {
    int i;
    uint64_t list[1 << GFBITS];

    for (i = 0; i < (1 << GFBITS); i++) {
        list[i] = p[i];
    }

    PQCLEAN_MCELIECE6688128_VEC_sort_63b(1 << GFBITS, list);

    for (i = 1; i < (1 << GFBITS); i++) {
        if (list[i - 1] == list[i]) {
            return -1;
        }
    }

    return 0;
}

