/*
  This file is for public-key generation
*/

#include "pk_gen.h"

#include "benes.h"
#include "controlbits.h"
#include "fft.h"
#include "params.h"
#include "util.h"

#include <stdint.h>

static void de_bitslicing(uint64_t *out, vec in[][GFBITS]) {
    int i, j, r;

    for (i = 0; i < (1 << GFBITS); i++) {
        out[i] = 0 ;
    }

    for (i = 0; i < 128; i++) {
        for (j = GFBITS - 1; j >= 0; j--) {
            for (r = 0; r < 64; r++) {
                out[i * 64 + r] <<= 1;
                out[i * 64 + r] |= (in[i][j] >> r) & 1;
            }
        }
    }
}

static void to_bitslicing_2x(vec out0[][GFBITS], vec out1[][GFBITS], const uint64_t *in) {
    int i, j, r;

    for (i = 0; i < 128; i++) {
        for (j = GFBITS - 1; j >= 0; j--) {
            for (r = 63; r >= 0; r--) {
                out1[i][j] <<= 1;
                out1[i][j] |= (in[i * 64 + r] >> (j + GFBITS)) & 1;
            }
        }

        for (j = GFBITS - 1; j >= 0; j--) {
            for (r = 63; r >= 0; r--) {
                out0[i][GFBITS - 1 - j] <<= 1;
                out0[i][GFBITS - 1 - j] |= (in[i * 64 + r] >> j) & 1;
            }
        }
    }
}

int PQCLEAN_MCELIECE8192128_VEC_pk_gen(unsigned char *pk, uint32_t *perm, const unsigned char *sk) {
    int i, j, k;
    int row, c, d;

    uint64_t mat[ GFBITS * SYS_T ][ 128 ];
    uint64_t ops[ GFBITS * SYS_T ][ GFBITS * SYS_T / 64 ];

    uint64_t mask;

    vec irr_int[2][ GFBITS ];

    vec consts[ 128 ][ GFBITS ];
    vec eval[ 128 ][ GFBITS ];
    vec prod[ 128 ][ GFBITS ];
    vec tmp[ GFBITS ];

    uint64_t list[1 << GFBITS];
    uint64_t one_row[ (SYS_N - GFBITS * SYS_T) / 64 ];

    // compute the inverses

    PQCLEAN_MCELIECE8192128_VEC_irr_load(irr_int, sk);

    PQCLEAN_MCELIECE8192128_VEC_fft(eval, irr_int);

    PQCLEAN_MCELIECE8192128_VEC_vec_copy(prod[0], eval[0]);

    for (i = 1; i < 128; i++) {
        PQCLEAN_MCELIECE8192128_VEC_vec_mul(prod[i], prod[i - 1], eval[i]);
    }

    PQCLEAN_MCELIECE8192128_VEC_vec_inv(tmp, prod[127]);

    for (i = 126; i >= 0; i--) {
        PQCLEAN_MCELIECE8192128_VEC_vec_mul(prod[i + 1], prod[i], tmp);
        PQCLEAN_MCELIECE8192128_VEC_vec_mul(tmp, tmp, eval[i + 1]);
    }

    PQCLEAN_MCELIECE8192128_VEC_vec_copy(prod[0], tmp);

    // fill matrix

    de_bitslicing(list, prod);

    for (i = 0; i < (1 << GFBITS); i++) {
        list[i] <<= GFBITS;
        list[i] |= i;
        list[i] |= ((uint64_t) perm[i]) << 31;
    }

    PQCLEAN_MCELIECE8192128_VEC_sort_63b(1 << GFBITS, list);

    to_bitslicing_2x(consts, prod, list);

    for (i = 0; i < (1 << GFBITS); i++) {
        perm[i] = list[i] & GFMASK;
    }

    for (j = 0; j < (GFBITS * SYS_T + 63) / 64; j++) {
        for (k = 0; k < GFBITS; k++) {
            mat[ k ][ j ] = prod[ j ][ k ];
        }
    }

    for (i = 1; i < SYS_T; i++) {
        for (j = 0; j < (GFBITS * SYS_T + 63) / 64; j++) {
            PQCLEAN_MCELIECE8192128_VEC_vec_mul(prod[j], prod[j], consts[j]);

            for (k = 0; k < GFBITS; k++) {
                mat[ i * GFBITS + k ][ j ] = prod[ j ][ k ];
            }
        }
    }

    // gaussian elimination to obtain an upper triangular matrix
    // and keep track of the operations in ops

    for (i = 0; i < (GFBITS * SYS_T) / 64; i++) {
        for (j = 0; j < 64; j++) {
            row = i * 64 + j;
            for (c = 0; c < (GFBITS * SYS_T) / 64; c++) {
                ops[ row ][ c ] = 0;
            }
        }
    }

    for (i = 0; i < (GFBITS * SYS_T) / 64; i++) {
        for (j = 0; j < 64; j++) {
            row = i * 64 + j;

            ops[ row ][ i ] = 1;
            ops[ row ][ i ] <<= j;
        }
    }

    for (i = 0; i < (GFBITS * SYS_T) / 64; i++) {
        for (j = 0; j < 64; j++) {
            row = i * 64 + j;

            for (k = row + 1; k < GFBITS * SYS_T; k++) {
                mask = mat[ row ][ i ] >> j;
                mask &= 1;
                mask -= 1;

                for (c = 0; c < (GFBITS * SYS_T) / 64; c++) {
                    mat[ row ][ c ] ^= mat[ k ][ c ] & mask;
                    ops[ row ][ c ] ^= ops[ k ][ c ] & mask;
                }
            }

            if ( ((mat[ row ][ i ] >> j) & 1) == 0 ) { // return if not systematic
                return -1;
            }

            for (k = row + 1; k < GFBITS * SYS_T; k++) {
                mask = mat[ k ][ i ] >> j;
                mask &= 1;
                mask = -mask;

                for (c = 0; c < (GFBITS * SYS_T) / 64; c++) {
                    mat[ k ][ c ] ^= mat[ row ][ c ] & mask;
                    ops[ k ][ c ] ^= ops[ row ][ c ] & mask;
                }
            }
        }
    }

    // computing the lineaer map required to obatin the systematic form

    for (i = (GFBITS * SYS_T) / 64 - 1; i >= 0; i--) {
        for (j = 63; j >= 0; j--) {
            row = i * 64 + j;

            for (k = 0; k < row; k++) {
                {
                    mask = mat[ k ][ i ] >> j;
                    mask &= 1;
                    mask = -mask;

                    for (c = 0; c < (GFBITS * SYS_T) / 64; c++) {
                        ops[ k ][ c ] ^= ops[ row ][ c ] & mask;
                    }
                }
            }
        }
    }

    // apply the linear map to the non-systematic part

    for (j = (GFBITS * SYS_T + 63) / 64; j < 128; j++) {
        for (k = 0; k < GFBITS; k++) {
            mat[ k ][ j ] = prod[ j ][ k ];
        }
    }

    for (i = 1; i < SYS_T; i++) {
        for (j = (GFBITS * SYS_T + 63) / 64; j < 128; j++) {
            PQCLEAN_MCELIECE8192128_VEC_vec_mul(prod[j], prod[j], consts[j]);

            for (k = 0; k < GFBITS; k++) {
                mat[ i * GFBITS + k ][ j ] = prod[ j ][ k ];
            }
        }
    }

    for (i = 0; i < (GFBITS * SYS_T) / 64; i++) {
        for (j = 0; j < 64; j++) {
            row = i * 64 + j;

            for (k = 0; k < (SYS_N - GFBITS * SYS_T) / 64; k++) {
                one_row[ k ] = 0;
            }

            for (c = 0; c < (GFBITS * SYS_T) / 64; c++) {
                for (d = 0; d < 64; d++) {
                    mask = ops[ row ][ c ] >> d;
                    mask &= 1;
                    mask = -mask;

                    for (k = 0; k < (SYS_N - GFBITS * SYS_T) / 64; k++) {
                        one_row[ k ] ^= mat[ c * 64 + d ][ k + (GFBITS * SYS_T) / 64 ] & mask;
                    }
                }
            }

            for (k = 0; k < (SYS_N - GFBITS * SYS_T) / 64; k++) {
                PQCLEAN_MCELIECE8192128_VEC_store8(pk, one_row[ k ]);
                pk += 8;
            }
        }
    }

    //

    return 0;
}

