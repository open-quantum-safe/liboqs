// SPDX-License-Identifier: Apache-2.0

#include <immintrin.h>
#include <stdint.h>


#define MAYO_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MAYO_MIN(x, y) (((x) < (y)) ? (x) : (y))


//
// generate multiplication table for '4-bit' variable 'b'. From https://eprint.iacr.org/2023/059/.
//
static inline __m256i tbl32_gf16_multab( uint8_t b ) {
    __m256i bx = _mm256_set1_epi16( b & 0xf );
    __m256i b1 = _mm256_srli_epi16( bx, 1 );

    const __m256i tab0 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 0));
    const __m256i tab1 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 1));
    const __m256i tab2 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 2));
    const __m256i tab3 = _mm256_load_si256((__m256i const *) (__gf16_mulbase + 32 * 3));

    __m256i mask_1  = _mm256_set1_epi16(1);
    __m256i mask_4  = _mm256_set1_epi16(4);
    __m256i mask_0  = _mm256_setzero_si256();

    return ( tab0 & _mm256_cmpgt_epi16( bx & mask_1, mask_0) )
           ^ ( tab1 & _mm256_cmpgt_epi16( b1 & mask_1, mask_0) )
           ^ ( tab2 & _mm256_cmpgt_epi16( bx & mask_4, mask_0) )
           ^ ( tab3 & _mm256_cmpgt_epi16( b1 & mask_4, mask_0) );
}

/* put matrix in row echelon form with ones on first nonzero entries in constant time*/
static inline void EF(unsigned char *A, int _nrows, int _ncols) {

    (void) _nrows;
    (void) _ncols;

    #define nrows M_MAX
    #define ncols (K_MAX * O_MAX + 1)

    #define AVX_REGS_PER_ROW ((K_MAX * O_MAX + 1 + 31) / 32)
    #define MAX_COLS (AVX_REGS_PER_ROW * 32)

    __m256i _pivot_row[AVX_REGS_PER_ROW];
    __m256i A_avx[AVX_REGS_PER_ROW* M_MAX];

    unsigned char* pivot_row_bytes = (unsigned char*) _pivot_row;
    unsigned char* A_bytes = (unsigned char*) A_avx;

    // load A in the tail of AVX2 registers
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
    for (; pivot_col < MAX_COLS-160; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS-128; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS-96; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS-64; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS-32; pivot_col++) {
        #include "echelon_form_loop.h"
    }
    for (; pivot_col < MAX_COLS; pivot_col++) {
        #include "echelon_form_loop.h"
    }

    // write the matrix A back
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            A[i * ncols + j] = A_bytes[i*AVX_REGS_PER_ROW*32 + (MAX_COLS - ncols) + j];
        }
    }
    mayo_secure_clear(_pivot_row, AVX_REGS_PER_ROW * 32);
    mayo_secure_clear(A_avx, AVX_REGS_PER_ROW * 32 * nrows);
}

