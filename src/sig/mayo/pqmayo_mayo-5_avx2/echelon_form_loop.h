// SPDX-License-Identifier: Apache-2.0

int pivot_col_rounded = pivot_col/32;

int pivot_row_lower_bound = MAYO_MAX(0, pivot_col + nrows - MAX_COLS);
int pivot_row_upper_bound = MAYO_MIN(nrows - 1, pivot_col - MAX_COLS + ncols);
/* the pivot row is guaranteed to be between these lower and upper bounds if A has full rank*/

/* per-row masks vs pivot_row, via vector compares */
int eq32[M_MAX + 8];
int gt32[M_MAX + 8];
ef_row_masks(pivot_row, nrows, eq32, gt32);

/* zero out pivot row */
for (int i = pivot_col_rounded; i < AVX_REGS_PER_ROW; i++) {
    _pivot_row[i] = _mm256_set1_epi8(0);
}

/* try to get a pivot row in constant time: select every candidate up to and including
   the first nonzero one, as a lowest set bit isolate rather than a prefix AND carried
   one row at a time, so the rows only feed OR reductions. Addresses and trip counts
   come from the public row/pivot_col and shifts are by the public row offset. */
const int ef_last = MAYO_MIN(nrows - 1, pivot_row_upper_bound + 32);
const int ef_n = ef_last - pivot_row_lower_bound + 1;
uint64_t ef_nz = 0, ef_cand = 0;
for (int i = 0; i < ef_n; i++) {
    const int row = pivot_row_lower_bound + i;
    const unsigned char v = A_bytes[row * AVX_REGS_PER_ROW * 32 + pivot_col];
    ef_val[i] = v;
    ef_nz   |= (uint64_t) ((v + 0xff) >> 8) << i;
    ef_cand |= (uint64_t) (((uint32_t) (eq32[row] | gt32[row])) >> 31) << i;
}
const uint64_t ef_first = (ef_nz & ef_cand) & (~(ef_nz & ef_cand) + 1);
const uint64_t ef_pfx = ef_cand & (ef_first | (ef_first - 1));
unsigned char pivot = 0;
for (int i = 0; i < ef_n; i++) {
    const uint32_t m = (uint32_t) -(int32_t) ((ef_pfx >> i) & 1);
    ef_sel[pivot_row_lower_bound + i] = m;
    pivot ^= ef_val[i] & (unsigned char) m;
}
uint32_t pivot_is_zero = ~ct_compare_32((int) pivot, 0);
for (int row = pivot_row_lower_bound; row <= ef_last; row++) {
    __m256i mask = _mm256_set1_epi32( (int) ef_sel[row] );
    for (int j = pivot_col_rounded; j < AVX_REGS_PER_ROW; j++) {
        _pivot_row[j] ^= mask & A_avx[row * AVX_REGS_PER_ROW + j];
    }
}

/* multiply pivot row by inverse of pivot */
inverse = inverse_f_ct(pivot);
__m256i inverse_multab = tbl32_gf16_multab(inverse);

for (int j = pivot_col_rounded; j < AVX_REGS_PER_ROW; j++) {
    _pivot_row[j] = _mm256_shuffle_epi8(inverse_multab, _pivot_row[j]);
}

/* conditionally write pivot row to the correct row, if there is a nonzero pivot */
/* eliminate entries below pivot */
for (int row = pivot_row_lower_bound; row < nrows; row++) {
    unsigned char below_pivot =  (unsigned char) gt32[row];
    unsigned char elt_to_elim = A_bytes[row*AVX_REGS_PER_ROW*32 + pivot_col];

    __m256i multab = tbl32_gf16_multab(below_pivot & elt_to_elim);
    if (row <= pivot_row_upper_bound) {
        __m256i mask = _mm256_set1_epi32( (int)((uint32_t)eq32[row] & ~pivot_is_zero) );
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
