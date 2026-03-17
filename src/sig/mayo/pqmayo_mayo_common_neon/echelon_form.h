// SPDX-License-Identifier: Apache-2.0

#include <arithmetic_common.h>
#include <mem.h>
#include <arm_neon.h>
#include <stdint.h>


#define MAYO_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MAYO_MIN(x, y) (((x) < (y)) ? (x) : (y))

/* put matrix in row echelon form with ones on first nonzero entries in constant time*/
static inline void EF(unsigned char *A, int _nrows, int _ncols) {

    (void) _nrows;
    (void) _ncols;

    #define nrows M_MAX
    #define ncols (K_MAX * O_MAX + 1)

    #define NEON_REGS_PER_ROW ((K_MAX * O_MAX + 1 + 15) / 16)
    #define MAX_COLS (NEON_REGS_PER_ROW * 16)

    uint8x16_t _pivot_row[NEON_REGS_PER_ROW];
    uint8x16_t A_neon[NEON_REGS_PER_ROW* M_MAX];

    unsigned char* pivot_row_bytes = (unsigned char*) _pivot_row;
    unsigned char* A_bytes = (unsigned char*) A_neon;

    // load A in the tail of NEON registers
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++)
        {
            A_bytes[i*MAX_COLS + (MAX_COLS - ncols) + j] = A[ i*ncols + j ];
        }
    }

    // pivot row is secret, pivot col is not
    unsigned char inverse;
    int pivot_row = 0;
    int pivot_col = MAYO_MAX(MAX_COLS - ncols,0);
    for (; pivot_col < MAX_COLS; pivot_col++) {
        #include "echelon_form_loop.h"
    }

    // write the matrix A back
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            A[i * ncols + j] = A_bytes[i*NEON_REGS_PER_ROW*16 + (MAX_COLS - ncols) + j];
        }
    }
    mayo_secure_clear(_pivot_row, NEON_REGS_PER_ROW * 16);
    mayo_secure_clear(A_neon, NEON_REGS_PER_ROW * 16 * nrows);
}

