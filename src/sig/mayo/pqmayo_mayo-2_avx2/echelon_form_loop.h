// SPDX-License-Identifier: Apache-2.0

int pivot_col_rounded = pivot_col/32;

int pivot_row_lower_bound = MAYO_MAX(0, pivot_col + nrows - MAX_COLS);
int pivot_row_upper_bound = MAYO_MIN(nrows - 1, pivot_col - MAX_COLS + ncols);
/* the pivot row is guaranteed to be between these lower and upper bounds if A has full rank*/

/* zero out pivot row */
for (int i = pivot_col_rounded; i < AVX_REGS_PER_ROW; i++) {
    _pivot_row[i] = _mm256_set1_epi8(0);
}

/* try to get a pivot row in constant time */
unsigned char pivot = 0;
uint32_t pivot_is_zero = -1;
for (int row = pivot_row_lower_bound;
        row <= MAYO_MIN(nrows - 1, pivot_row_upper_bound + 32); row++) {
    uint32_t is_pivot_row = ~ct_compare_32(row, pivot_row);
    uint32_t below_pivot_row = ct_is_greater_than(row, pivot_row);
    __m256i mask = _mm256_set1_epi32( is_pivot_row | (below_pivot_row & pivot_is_zero) );
    for (int j = pivot_col_rounded; j < AVX_REGS_PER_ROW; j++) {
        _pivot_row[j] ^= mask & A_avx[row * AVX_REGS_PER_ROW + j];
    }
    pivot = pivot_row_bytes[pivot_col];
    pivot_is_zero = ~ct_compare_32((int) pivot, 0);
}

/* multiply pivot row by inverse of pivot */
inverse = inverse_f(pivot);
__m256i inverse_multab = tbl32_gf16_multab(inverse);

for (int j = pivot_col_rounded; j < AVX_REGS_PER_ROW; j++) {
    _pivot_row[j] = _mm256_shuffle_epi8(inverse_multab, _pivot_row[j]);
}

/* conditionally write pivot row to the correct row, if there is a nonzero pivot */
/* eliminate entries below pivot */
for (int row = pivot_row_lower_bound; row < nrows; row++) {
    unsigned char below_pivot =  (unsigned char) (ct_is_greater_than(row, pivot_row));
    unsigned char elt_to_elim = A_bytes[row*AVX_REGS_PER_ROW*32 + pivot_col];

    __m256i multab = tbl32_gf16_multab(below_pivot & elt_to_elim);
    if (row <= pivot_row_upper_bound) {
        __m256i mask = _mm256_set1_epi32(~ct_compare_32(row, pivot_row) & ~pivot_is_zero);
        for (int col = pivot_col_rounded; col < AVX_REGS_PER_ROW; col++) { 
            A_avx[row*AVX_REGS_PER_ROW + col] = _mm256_blendv_epi8(A_avx[row*AVX_REGS_PER_ROW + col], _pivot_row[col], mask) ^
                                                    _mm256_shuffle_epi8(multab, _pivot_row[col]);
        }
    } else {
        for (int j = pivot_col_rounded; j < AVX_REGS_PER_ROW; j++) {
            A_avx[row*AVX_REGS_PER_ROW + j] ^= _mm256_shuffle_epi8(multab, _pivot_row[j]);
        }
    }
}

pivot_row += (-(int32_t)(~pivot_is_zero));

