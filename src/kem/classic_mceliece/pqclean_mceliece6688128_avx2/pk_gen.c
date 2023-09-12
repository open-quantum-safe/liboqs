/*
  This file is for public-key generation
*/

#include "pk_gen.h"

#include "controlbits.h"
#include "crypto_declassify.h"
#include "crypto_uint64.h"
#include "fft.h"
#include "int32_sort.h"
#include "params.h"
#include "uint64_sort.h"
#include "util.h"

static crypto_uint64 uint64_is_equal_declassify(uint64_t t, uint64_t u) {
    crypto_uint64 mask = crypto_uint64_equal_mask(t, u);
    crypto_declassify(&mask, sizeof mask);
    return mask;
}

static crypto_uint64 uint64_is_zero_declassify(uint64_t t) {
    crypto_uint64 mask = crypto_uint64_zero_mask(t);
    crypto_declassify(&mask, sizeof mask);
    return mask;
}

#include <stdint.h>

#define par_width 7

/* set m to 11...1 if the i-th bit of x is 0 and the i-th bit of y is 1 */
/* set m to 00...0 otherwise */
static inline void extract_01_masks(uint16_t *m, const uint64_t *x, const uint64_t *y, int i) {
    *m = (((~x[ i >> 6 ]) & y[ i >> 6 ]) >> (i & 63)) & 1;
    *m = -(*m);
}

/* return a 128-bit vector of which each bits is set to the i-th bit of v */
static inline vec256 extract_mask256(const uint64_t v[], int i) {
    uint32_t mask;

    mask = (v[ i >> 6 ] >> (i & 63)) & 1;
    mask = -mask;

    return vec256_set1_32b(mask);
}

// swap x and y if m = 11...1
static inline void uint16_cswap(uint16_t *x, uint16_t *y, uint16_t m) {
    uint16_t d;

    d = *x ^ *y;
    d &= m;
    *x ^= d;
    *y ^= d;
}

// swap x and y if m = 11...1
static inline void vec256_cswap(vec256 *x, vec256 *y, vec256 m) {
    vec256 d;

    d = *x ^ *y;
    d &= m;
    *x ^= d;
    *y ^= d;
}

/* swap   x[i0] and   x[i1]  if x[i1] > x[i0] */
/* swap mat[i0] and mat[i1]  if x[i1] > x[i0] */
static inline void minmax_rows(uint16_t *x, vec256 (*mat)[par_width], int i0, int i1) {
    int i;
    uint16_t m;
    vec256 mm;

    m = x[i1] - x[i0];
    m >>= 15;
    m = -m;
    mm = vec256_set1_16b(m);

    uint16_cswap(&x[i0], &x[i1], m);

    for (i = 0; i < par_width; i++) {
        vec256_cswap(&mat[i0][i], &mat[i1][i], mm);
    }
}

/* merge first half of x[0],x[step],...,x[(2*n-1)*step] with second half */
/* requires n to be a power of 2 */
static void merge_rows(int n, int bound, uint16_t *x, vec256 (*mat)[par_width], int off, int step) {
    int i;

    if (n == 1) {
        if (off + step < bound) {
            minmax_rows(x, mat, off, off + step);
        }
    } else {
        merge_rows(n / 2, bound, x, mat, off, step * 2);
        merge_rows(n / 2, bound, x, mat, off + step, step * 2);

        for (i = 1; i < 2 * n - 1 && off + (i + 1) * step < bound; i += 2) {
            minmax_rows(x, mat, off + i * step, off + (i + 1)*step);
        }
    }
}

/* permute the rows of mat by sorting x */
static void sort_rows(int n, int bound, uint16_t *x, vec256 (*mat)[par_width], int off) {
    if (n <= 1) {
        return;
    }
    sort_rows(n / 2, bound, x, mat, off);
    sort_rows(n / 2, bound, x, mat, off + n / 2);
    merge_rows(n / 2, bound, x, mat, off, 1);
}

/* extract numbers represented in bitsliced form */
static void de_bitslicing(uint64_t *out, vec256 in[][GFBITS]) {
    int i, j, r;
    uint64_t u = 0;

    for (i = 0; i < (1 << GFBITS); i++) {
        out[i] = 0 ;
    }

    for (i = 0; i < 32; i++) {
        for (j = GFBITS - 1; j >= 0; j--) {
            u = vec256_extract(in[i][j], 0);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 0 * 64 + r] <<= 1;
                out[i * 256 + 0 * 64 + r] |= (u >> r) & 1;
            }
            u = vec256_extract(in[i][j], 1);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 1 * 64 + r] <<= 1;
                out[i * 256 + 1 * 64 + r] |= (u >> r) & 1;
            }
            u = vec256_extract(in[i][j], 2);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 2 * 64 + r] <<= 1;
                out[i * 256 + 2 * 64 + r] |= (u >> r) & 1;
            }
            u = vec256_extract(in[i][j], 3);
            for (r = 0; r < 64; r++) {
                out[i * 256 + 3 * 64 + r] <<= 1;
                out[i * 256 + 3 * 64 + r] |= (u >> r) & 1;
            }
        }
    }
}

/* convert numbers into bitsliced form */
static void to_bitslicing_2x(vec256 out0[][GFBITS], vec256 out1[][GFBITS], const uint64_t *in) {
    int i, j, k, r;
    uint64_t u[2][4] = {{0}};

    for (i = 0; i < 32; i++) {
        for (j = GFBITS - 1; j >= 0; j--) {
            for (k = 0; k < 4; k++) {
                for (r = 63; r >= 0; r--) {
                    u[0][k] <<= 1;
                    u[0][k] |= (in[i * 256 + k * 64 + r] >> (GFBITS - 1 - j)) & 1;

                    u[1][k] <<= 1;
                    u[1][k] |= (in[i * 256 + k * 64 + r] >> (j + GFBITS)) & 1;
                }
            }

            out0[i][j] = vec256_set4x(u[0][0], u[0][1], u[0][2], u[0][3]);
            out1[i][j] = vec256_set4x(u[1][0], u[1][1], u[1][2], u[1][3]);
        }
    }
}

/* y[pi[i]] = x[i] */
/* requires pi to be a permutation */
static void composeinv(int n, uint16_t y[n], const uint16_t x[n], const uint16_t pi[n]) {
    int i;
    int32_t t[n];

    for (i = 0; i < n; ++i) {
        t[i] = pi[i];
        t[i] <<= 16;
        t[i] |= x[i];
    }

    int32_sort(t, n);

    for (i = 0; i < n; ++i) {
        y[i] = t[i] & 0xFFFF;
    }
}

/* input: irr, an irreducible polynomial */
/*        perm, a permutation represented as an array of 32-bit numbers */
/*        pi, same permutation represented as an array of 16-bit numbers */
/* output: pk, the public key*/
/* return: 0 if pk is successfully generated, -1 otherwise */
int pk_gen(unsigned char *pk, const unsigned char *irr, const uint32_t *perm, int16_t *pi) {
#define nBlocks_I ((PK_NROWS + 255) / 256)

    int i, j, k, b;
    int row, c;

    union {
        uint64_t w[ PK_NROWS ][ nBlocks_I * 4 ];
        vec256   v[ PK_NROWS ][ nBlocks_I ];
    } mat;

    union {
        uint64_t w[ PK_NROWS ][ 28 ];
        vec256 v[ PK_NROWS ][ 7 ];
    } par;

    uint16_t m;
    vec256 mm;

    vec128 sk_int[ GFBITS ];

    vec256 consts[ 32 ][ GFBITS ];
    vec256 eval[ 32 ][ GFBITS ];
    vec256 prod[ 32 ][ GFBITS ];
    vec256 tmp[ GFBITS ];

    uint64_t list[1 << GFBITS];
    uint64_t one = 1;
    uint64_t t;

    uint16_t ind[ PK_NROWS ];
    uint16_t ind_inv[ PK_NROWS ];

    // compute the inverses

    irr_load(sk_int, irr);

    fft(eval, sk_int);

    vec256_copy(prod[0], eval[0]);

    for (i = 1; i < 32; i++) {
        vec256_mul(prod[i], prod[i - 1], eval[i]);
    }

    vec256_inv(tmp, prod[31]);

    for (i = 30; i >= 0; i--) {
        vec256_mul(prod[i + 1], prod[i], tmp);
        vec256_mul(tmp, tmp, eval[i + 1]);
    }

    vec256_copy(prod[0], tmp);

    // fill matrix

    de_bitslicing(list, prod);

    for (i = 0; i < (1 << GFBITS); i++) {
        list[i] <<= GFBITS;
        list[i] |= i;
        list[i] |= ((uint64_t) perm[i]) << 31;
    }

    uint64_sort(list, 1 << GFBITS);

    for (i = 1; i < (1 << GFBITS); i++) {
        if (uint64_is_equal_declassify(list[i - 1] >> 31, list[i] >> 31)) {
            return -1;
        }
    }

    to_bitslicing_2x(consts, prod, list);

    for (i = 0; i < (1 << GFBITS); i++) {
        pi[i] = list[i] & GFMASK;
    }

    for (j = 0; j < nBlocks_I; j++) {
        for (k = 0; k < GFBITS; k++) {
            mat.v[ k ][ j ] = prod[ j ][ k ];
        }
    }

    for (i = 1; i < SYS_T; i++) {
        for (j = 0; j < nBlocks_I; j++) {
            vec256_mul(prod[j], prod[j], consts[j]);

            for (k = 0; k < GFBITS; k++) {
                mat.v[ i * GFBITS + k ][ j ] = prod[ j ][ k ];
            }
        }
    }

    // gaussian elimination to obtain L, U, and P such that LP M = U
    // L and U are stored in the space of M
    // P is stored in ind

    for (i = 0; i < PK_NROWS; i++) {
        ind_inv[i] = ind[ i ] = i;
    }

    for (row = 0; row < PK_NROWS; row++) {
        i = row >> 6;
        j = row & 63;

        for (k = row + 1; k < PK_NROWS; k++) {
            extract_01_masks(&m, mat.w[ row ], mat.w[ k ], row);
            uint16_cswap(&ind[row], &ind[k], m);

            mm = vec256_set1_16b(m);
            for (c = 0; c < nBlocks_I; c++) {
                vec256_cswap(&mat.v[ row ][ c ], &mat.v[ k ][ c ], mm);
            }
        }

        if ( uint64_is_zero_declassify((mat.w[ row ][ i ] >> j) & 1) ) { // return if not systematic
            return -1;
        }

        for (k = row + 1; k < PK_NROWS; k++) {
            t = mat.w[ k ][ i ] & (one << j);
            mm = extract_mask256(mat.w[k], row);

            for (c = 0; c < nBlocks_I; c++) {
                mat.v[ k ][ c ] ^= mat.v[ row ][ c ] & mm;
            }

            mat.w[ k ][ i ] |= t;
        }
    }

    // apply M^-1 to the remaining columns

    composeinv(PK_NROWS, ind_inv, ind_inv, ind);

    for (j = 6; j < 27; j += par_width) {
        for (k = 0; k < GFBITS; k++) {
            for (b = 0; b < par_width; b++) {
                par.v[ k ][ b ] = prod[ j + b ][ k ];
            }
        }

        for (i = 1; i < SYS_T; i++) {
            for (b = 0; b < par_width; b++) {
                vec256_mul(prod[ j + b ], prod[ j + b ], consts[ j + b ]);
            }

            for (k = 0; k < GFBITS; k++) {
                for (b = 0; b < par_width; b++) {
                    par.v[ i * GFBITS + k ][ b ] = prod[ j + b ][ k ];
                }
            }
        }

        // apply P

        for (i = 0; i < PK_NROWS; i++) {
            ind[i] = ind_inv[i];
        }

        sort_rows((1 << GFBITS) / 4, PK_NROWS, ind, par.v, 0);

        // apply L

        for (row = PK_NROWS - 1; row >= 0; row--) {
            for (i = row - 1; i >= 0; i--) {
                mm = extract_mask256(mat.w[row], i);

                for (k = 0; k < par_width; k++) {
                    par.v[ row ][ k ] ^= par.v[ i ][ k ] & mm;
                }
            }
        }

        // apply U^-1

        if (j == 6) {
            for (i = 0; i < PK_NROWS; i++) {
                par.v[i][0] = mat.v[i][nBlocks_I - 1];
            }
        }

        for (row = PK_NROWS - 1; row >= 0; row--) {
            for (i = PK_NROWS - 1; i > row; i--) {
                mm = extract_mask256(mat.w[row], i);

                for (k = 0; k < par_width; k++) {
                    par.v[ row ][ k ] ^= par.v[ i ][ k ] & mm;
                }
            }
        }

        if (j + par_width < 27) {
            for (row = 0; row < PK_NROWS; row++) {
                if (j == 6) {
                    k = 2;
                } else {
                    k = 0;
                }
                for (; k < 28; k++) {
                    store8(pk + PK_ROW_BYTES * row + j * 32 + k * 8 - 13 * 16, par.w[row][k]);
                }
            }
        } else {
            for (row = 0; row < PK_NROWS; row++) {
                for (k = 0; k < 24; k++) {
                    store8(pk + PK_ROW_BYTES * row + j * 32 + k * 8 - 13 * 16, par.w[row][k]);
                }

                store_i(pk + PK_ROW_BYTES * row + j * 32 + k * 8 - 13 * 16, par.w[row][k], 4);
            }
        }
    }

    //

    return 0;
}
