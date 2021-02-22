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

static void de_bitslicing(uint64_t *out, vec256 in[][GFBITS]) {
    int i, j, r;
    uint64_t u = 0;

    for (i = 0; i < (1 << GFBITS); i++) {
        out[i] = 0 ;
    }

    for (i = 0; i < 32; i++) {
        for (j = GFBITS - 1; j >= 0; j--) {
            u = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(in[i][j], 0);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 0 * 64 + r] <<= 1;
                out[i * 256 + 0 * 64 + r] |= (u >> r) & 1;
            }
            u = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(in[i][j], 1);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 1 * 64 + r] <<= 1;
                out[i * 256 + 1 * 64 + r] |= (u >> r) & 1;
            }
            u = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(in[i][j], 2);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 2 * 64 + r] <<= 1;
                out[i * 256 + 2 * 64 + r] |= (u >> r) & 1;
            }
            u = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(in[i][j], 3);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 3 * 64 + r] <<= 1;
                out[i * 256 + 3 * 64 + r] |= (u >> r) & 1;
            }
        }
    }
}

static void to_bitslicing_2x(vec256 out0[][GFBITS], vec256 out1[][GFBITS], const uint64_t *in) {
    int i, j, k, r;
    uint64_t u[4] = {0};

    for (i = 0; i < 32; i++) {
        for (j = GFBITS - 1; j >= 0; j--) {
            for (k = 0; k < 4; k++) {
                for (r = 63; r >= 0; r--) {
                    u[k] <<= 1;
                    u[k] |= (in[i * 256 + k * 64 + r] >> (j + GFBITS)) & 1;
                }
            }

            out1[i][j] = PQCLEAN_MCELIECE8192128_AVX_vec256_set4x(u[0], u[1], u[2], u[3]);
        }

        for (j = GFBITS - 1; j >= 0; j--) {
            for (k = 0; k < 4; k++) {
                for (r = 63; r >= 0; r--) {
                    u[k] <<= 1;
                    u[k] |= (in[i * 256 + k * 64 + r] >> j) & 1;
                }
            }

            out0[i][GFBITS - 1 - j] = PQCLEAN_MCELIECE8192128_AVX_vec256_set4x(u[0], u[1], u[2], u[3]);
        }
    }
}

#define NBLOCKS2_H ((SYS_N + 255) / 256)
#define NBLOCKS2_I ((GFBITS * SYS_T + 255) / 256)
int PQCLEAN_MCELIECE8192128_AVX_pk_gen(unsigned char *pk, uint32_t *perm, const unsigned char *sk) {
    int i, j, k;
    int row, c, d;

    uint64_t mat[ GFBITS * SYS_T ][ 128 ];
    uint64_t ops[ GFBITS * SYS_T ][ GFBITS * SYS_T / 64 ];

    uint64_t mask;

    vec128 sk_int[ GFBITS ];

    vec256 consts[ 32 ][ GFBITS ];
    vec256 eval[ 32 ][ GFBITS ];
    vec256 prod[ 32 ][ GFBITS ];
    vec256 tmp[ GFBITS ];

    uint64_t list[1 << GFBITS];
    uint64_t one_row[ (SYS_N - GFBITS * SYS_T) / 64 ];

    // compute the inverses

    PQCLEAN_MCELIECE8192128_AVX_irr_load(sk_int, sk);

    PQCLEAN_MCELIECE8192128_AVX_fft(eval, sk_int);

    PQCLEAN_MCELIECE8192128_AVX_vec256_copy(prod[0], eval[0]);

    for (i = 1; i < 32; i++) {
        PQCLEAN_MCELIECE8192128_AVX_vec256_mul(prod[i], prod[i - 1], eval[i]);
    }

    PQCLEAN_MCELIECE8192128_AVX_vec256_inv(tmp, prod[31]);

    for (i = 30; i >= 0; i--) {
        PQCLEAN_MCELIECE8192128_AVX_vec256_mul(prod[i + 1], prod[i], tmp);
        PQCLEAN_MCELIECE8192128_AVX_vec256_mul(tmp, tmp, eval[i + 1]);
    }

    PQCLEAN_MCELIECE8192128_AVX_vec256_copy(prod[0], tmp);

    // fill matrix

    de_bitslicing(list, prod);

    for (i = 0; i < (1 << GFBITS); i++) {
        list[i] <<= GFBITS;
        list[i] |= i;
        list[i] |= ((uint64_t) perm[i]) << 31;
    }

    PQCLEAN_MCELIECE8192128_AVX_sort_63b(1 << GFBITS, list);

    to_bitslicing_2x(consts, prod, list);

    for (i = 0; i < (1 << GFBITS); i++) {
        perm[i] = list[i] & GFMASK;
    }

    for (j = 0; j < NBLOCKS2_I; j++) {
        for (k = 0; k < GFBITS; k++) {
            mat[ k ][ 4 * j + 0 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 0);
            mat[ k ][ 4 * j + 1 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 1);
            mat[ k ][ 4 * j + 2 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 2);
            mat[ k ][ 4 * j + 3 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 3);
        }
    }

    for (i = 1; i < SYS_T; i++) {
        for (j = 0; j < NBLOCKS2_I; j++) {
            PQCLEAN_MCELIECE8192128_AVX_vec256_mul(prod[j], prod[j], consts[j]);

            for (k = 0; k < GFBITS; k++) {
                mat[ i * GFBITS + k ][ 4 * j + 0 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 0);
                mat[ i * GFBITS + k ][ 4 * j + 1 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 1);
                mat[ i * GFBITS + k ][ 4 * j + 2 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 2);
                mat[ i * GFBITS + k ][ 4 * j + 3 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 3);
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

    for (j = NBLOCKS2_I; j < NBLOCKS2_H; j++) {
        for (k = 0; k < GFBITS; k++) {
            mat[ k ][ 4 * j + 0 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 0);
            mat[ k ][ 4 * j + 1 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 1);
            mat[ k ][ 4 * j + 2 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 2);
            mat[ k ][ 4 * j + 3 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 3);
        }
    }

    for (i = 1; i < SYS_T; i++) {
        for (j = NBLOCKS2_I; j < NBLOCKS2_H; j++) {
            PQCLEAN_MCELIECE8192128_AVX_vec256_mul(prod[j], prod[j], consts[j]);

            for (k = 0; k < GFBITS; k++) {
                mat[ i * GFBITS + k ][ 4 * j + 0 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 0);
                mat[ i * GFBITS + k ][ 4 * j + 1 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 1);
                mat[ i * GFBITS + k ][ 4 * j + 2 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 2);
                mat[ i * GFBITS + k ][ 4 * j + 3 ] = PQCLEAN_MCELIECE8192128_AVX_vec256_extract(prod[ j ][ k ], 3);
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
                PQCLEAN_MCELIECE8192128_AVX_store8(pk, one_row[ k ]);
                pk += 8;
            }
        }
    }

    //

    return 0;
}

