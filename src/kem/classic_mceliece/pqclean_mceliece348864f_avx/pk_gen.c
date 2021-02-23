/*
  This file is for public-key generation
*/

#include "pk_gen.h"

#include "benes.h"
#include "controlbits.h"
#include "fft.h"
#include "params.h"
#include "transpose.h"
#include "util.h"

#include <stdint.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

static void de_bitslicing(uint64_t *out, vec256 in[][GFBITS]) {
    int i, j, r;
    uint64_t u = 0;

    for (i = 0; i < (1 << GFBITS); i++) {
        out[i] = 0 ;
    }

    for (i = 0; i < 16; i++) {
        for (j = GFBITS - 1; j >= 0; j--) {
            u = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(in[i][j], 0);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 0 * 64 + r] <<= 1;
                out[i * 256 + 0 * 64 + r] |= (u >> r) & 1;
            }
            u = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(in[i][j], 1);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 1 * 64 + r] <<= 1;
                out[i * 256 + 1 * 64 + r] |= (u >> r) & 1;
            }
            u = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(in[i][j], 2);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 2 * 64 + r] <<= 1;
                out[i * 256 + 2 * 64 + r] |= (u >> r) & 1;
            }
            u = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(in[i][j], 3);
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

    for (i = 0; i < 16; i++) {
        for (j = GFBITS - 1; j >= 0; j--) {
            for (k = 0; k < 4; k++) {
                for (r = 63; r >= 0; r--) {
                    u[k] <<= 1;
                    u[k] |= (in[i * 256 + k * 64 + r] >> (j + GFBITS)) & 1;
                }
            }

            out1[i][j] = PQCLEAN_MCELIECE348864F_AVX_vec256_set4x(u[0], u[1], u[2], u[3]);
        }

        for (j = GFBITS - 1; j >= 0; j--) {
            for (k = 0; k < 4; k++) {
                for (r = 63; r >= 0; r--) {
                    u[k] <<= 1;
                    u[k] |= (in[i * 256 + k * 64 + r] >> j) & 1;
                }
            }

            out0[i][GFBITS - 1 - j] = PQCLEAN_MCELIECE348864F_AVX_vec256_set4x(u[0], u[1], u[2], u[3]);
        }
    }
}

/* return number of trailing zeros of the non-zero input in */
static inline int ctz(uint64_t in) {
    return (int)_tzcnt_u64(in);
}

static inline uint64_t same_mask(uint16_t x, uint16_t y) {
    uint64_t mask;

    mask = x ^ y;
    mask -= 1;
    mask >>= 63;
    mask = -mask;

    return mask;
}

static int mov_columns(uint64_t mat[][ ((SYS_N + 255) / 256) * 4 ], uint32_t *perm) {
    int i, j, k, s, block_idx, row;
    uint64_t buf[64], ctz_list[32], t, d, mask;

    row = GFBITS * SYS_T - 32;
    block_idx = row / 64;

    // extract the 32x64 matrix

    for (i = 0; i < 32; i++) {
        buf[i] = (mat[ row + i ][ block_idx + 0 ] >> 32) |
                 (mat[ row + i ][ block_idx + 1 ] << 32);
    }

    // compute the column indices of pivots by Gaussian elimination.
    // the indices are stored in ctz_list

    for (i = 0; i < 32; i++) {
        t = buf[i];
        for (j = i + 1; j < 32; j++) {
            t |= buf[j];
        }

        if (t == 0) {
            return -1;    // return if buf is not full rank
        }

        ctz_list[i] = s = ctz(t);

        for (j = i + 1; j < 32; j++) {
            mask = (buf[i] >> s) & 1;
            mask -= 1;
            buf[i] ^= buf[j] & mask;
        }
        for (j =   0; j <  i; j++) {
            mask = (buf[j] >> s) & 1;
            mask = -mask;
            buf[j] ^= buf[i] & mask;
        }
        for (j = i + 1; j < 32; j++) {
            mask = (buf[j] >> s) & 1;
            mask = -mask;
            buf[j] ^= buf[i] & mask;
        }
    }

    // updating permutation

    for (j = 0;   j < 32; j++) {
        for (k = j + 1; k < 64; k++) {
            d = perm[ row + j ] ^ perm[ row + k ];
            d &= same_mask(k, ctz_list[j]);
            perm[ row + j ] ^= d;
            perm[ row + k ] ^= d;
        }
    }

    // moving columns of mat according to the column indices of pivots

    for (i = 0; i < GFBITS * SYS_T; i += 64) {

        for (j = 0; j < min(64, GFBITS * SYS_T - i); j++) {
            buf[j] = (mat[ i + j ][ block_idx + 0 ] >> 32) |
                     (mat[ i + j ][ block_idx + 1 ] << 32);
        }

        PQCLEAN_MCELIECE348864F_AVX_transpose_64x64(buf);

        for (j = 0; j < 32; j++) {
            for (k = j + 1; k < 64; k++) {
                d = buf[ j ] ^ buf[ k ];
                d &= same_mask(k, ctz_list[j]);
                buf[ j ] ^= d;
                buf[ k ] ^= d;
            }
        }

        PQCLEAN_MCELIECE348864F_AVX_transpose_64x64(buf);

        for (j = 0; j < min(64, GFBITS * SYS_T - i); j++) {
            mat[ i + j ][ block_idx + 0 ] = (mat[ i + j ][ block_idx + 0 ] << 32 >> 32) | (buf[j] << 32);
            mat[ i + j ][ block_idx + 1 ] = (mat[ i + j ][ block_idx + 1 ] >> 32 << 32) | (buf[j] >> 32);
        }
    }

    return 0;
}

#define NBLOCKS1_H ((SYS_N + 63) / 64)
#define NBLOCKS2_H ((SYS_N + 255) / 256)
#define NBLOCKS1_I ((GFBITS * SYS_T + 63) / 64)
int PQCLEAN_MCELIECE348864F_AVX_pk_gen(unsigned char *pk, uint32_t *perm, const unsigned char *sk) {

    int i, j, k;
    int row, c;

    uint64_t mat[ GFBITS * SYS_T ][ NBLOCKS2_H * 4 ];

    uint64_t mask;

    uint64_t sk_int[ GFBITS ];

    vec256 consts[ 16 ][ GFBITS ];
    vec256 eval[ 16 ][ GFBITS ];
    vec256 prod[ 16 ][ GFBITS ];
    vec256 tmp[ GFBITS ];

    uint64_t list[1 << GFBITS];

    // compute the inverses

    PQCLEAN_MCELIECE348864F_AVX_irr_load(sk_int, sk);

    PQCLEAN_MCELIECE348864F_AVX_fft(eval, sk_int);

    PQCLEAN_MCELIECE348864F_AVX_vec256_copy(prod[0], eval[0]);

    for (i = 1; i < 16; i++) {
        PQCLEAN_MCELIECE348864F_AVX_vec256_mul(prod[i], prod[i - 1], eval[i]);
    }

    PQCLEAN_MCELIECE348864F_AVX_vec256_inv(tmp, prod[15]);

    for (i = 14; i >= 0; i--) {
        PQCLEAN_MCELIECE348864F_AVX_vec256_mul(prod[i + 1], prod[i], tmp);
        PQCLEAN_MCELIECE348864F_AVX_vec256_mul(tmp, tmp, eval[i + 1]);
    }

    PQCLEAN_MCELIECE348864F_AVX_vec256_copy(prod[0], tmp);

    // fill matrix

    de_bitslicing(list, prod);

    for (i = 0; i < (1 << GFBITS); i++) {
        list[i] <<= GFBITS;
        list[i] |= i;
        list[i] |= ((uint64_t) perm[i]) << 31;
    }

    PQCLEAN_MCELIECE348864F_AVX_sort_63b(1 << GFBITS, list);

    to_bitslicing_2x(consts, prod, list);

    for (i = 0; i < (1 << GFBITS); i++) {
        perm[i] = list[i] & GFMASK;
    }

    for (j = 0; j < NBLOCKS2_H; j++) {
        for (k = 0; k < GFBITS; k++) {
            mat[ k ][ 4 * j + 0 ] = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(prod[ j ][ k ], 0);
            mat[ k ][ 4 * j + 1 ] = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(prod[ j ][ k ], 1);
            mat[ k ][ 4 * j + 2 ] = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(prod[ j ][ k ], 2);
            mat[ k ][ 4 * j + 3 ] = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(prod[ j ][ k ], 3);
        }
    }

    for (i = 1; i < SYS_T; i++) {
        for (j = 0; j < NBLOCKS2_H; j++) {
            PQCLEAN_MCELIECE348864F_AVX_vec256_mul(prod[j], prod[j], consts[j]);

            for (k = 0; k < GFBITS; k++) {
                mat[ i * GFBITS + k ][ 4 * j + 0 ] = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(prod[ j ][ k ], 0);
                mat[ i * GFBITS + k ][ 4 * j + 1 ] = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(prod[ j ][ k ], 1);
                mat[ i * GFBITS + k ][ 4 * j + 2 ] = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(prod[ j ][ k ], 2);
                mat[ i * GFBITS + k ][ 4 * j + 3 ] = PQCLEAN_MCELIECE348864F_AVX_vec256_extract(prod[ j ][ k ], 3);
            }
        }
    }

    // gaussian elimination

    for (row = 0; row < PK_NROWS; row++) {
        i = row >> 6;
        j = row & 63;

        if (row == GFBITS * SYS_T - 32) {
            if (mov_columns(mat, perm)) {
                return -1;
            }
        }

        for (k = row + 1; k < PK_NROWS; k++) {
            mask = mat[ row ][ i ] >> j;
            mask &= 1;
            mask -= 1;

            for (c = 0; c < NBLOCKS1_H; c++) {
                mat[ row ][ c ] ^= mat[ k ][ c ] & mask;
            }
        }

        if ( ((mat[ row ][ i ] >> j) & 1) == 0 ) { // return if not systematic
            return -1;
        }

        for (k = 0; k < row; k++) {
            mask = mat[ k ][ i ] >> j;
            mask &= 1;
            mask = -mask;

            for (c = 0; c < NBLOCKS1_H; c++) {
                mat[ k ][ c ] ^= mat[ row ][ c ] & mask;
            }
        }

        for (k = row + 1; k < PK_NROWS; k++) {
            mask = mat[ k ][ i ] >> j;
            mask &= 1;
            mask = -mask;

            for (c = 0; c < NBLOCKS1_H; c++) {
                mat[ k ][ c ] ^= mat[ row ][ c ] & mask;
            }
        }
    }

    for (i = 0; i < GFBITS * SYS_T; i++) {
        for (j = NBLOCKS1_I; j < NBLOCKS1_H - 1; j++) {
            PQCLEAN_MCELIECE348864F_AVX_store8(pk, mat[i][j]);
            pk += 8;
        }

        PQCLEAN_MCELIECE348864F_AVX_store_i(pk, mat[i][j], PK_ROW_BYTES % 8);

        pk += PK_ROW_BYTES % 8;
    }

    //

    return 0;
}

