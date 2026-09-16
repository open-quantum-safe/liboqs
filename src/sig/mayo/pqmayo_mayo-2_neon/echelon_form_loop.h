// SPDX-License-Identifier: Apache-2.0

int pivot_col_rounded = pivot_col/16;

int pivot_row_lower_bound = MAYO_MAX(0, pivot_col + nrows - MAX_COLS);
int pivot_row_upper_bound = MAYO_MIN(nrows - 1, pivot_col - MAX_COLS + ncols);
/* the pivot row is guaranteed to be between these lower and upper bounds if A has full rank*/

/* per-row masks vs pivot_row, via vector compares */
int eq32[M_MAX + 8];
int gt32[M_MAX + 8];
ef_row_masks(pivot_row, nrows, eq32, gt32);

/* zero out pivot row */
for (int i = pivot_col_rounded; i < NEON_REGS_PER_ROW; i++) {
    _pivot_row[i] = vmovq_n_u8(0);
}

/* try to get a pivot row in constant time. Reading the running pivot back out of
   _pivot_row chained a vector store into a scalar load every row, so take the entries
   straight from A and carry only a prefix AND of "still zero". Addresses and trip
   counts come from the public row/pivot_col, the rest is arithmetic on masks. */
const int ef_last = MAYO_MIN(nrows - 1, pivot_row_upper_bound + 32);
unsigned char pivot = 0;
uint32_t still_zero = (uint32_t) -1;
for (int row = pivot_row_lower_bound; row <= ef_last; row++) {
    unsigned char val = A_bytes[row * NEON_REGS_PER_ROW * 16 + pivot_col];
    uint32_t eq = (uint32_t) eq32[row], gt = (uint32_t) gt32[row];
    uint32_t m = eq | (gt & still_zero);
    ef_sel[row] = m;
    pivot ^= (unsigned char) (m & val);
    still_zero &= ~ct_compare_32((int) val, 0) | ~(eq | gt);
}
uint32_t pivot_is_zero = ~ct_compare_32((int) pivot, 0);
for (int row = pivot_row_lower_bound; row <= ef_last; row++) {
    uint8x16_t mask = vmovq_n_u8( (unsigned char) ef_sel[row] );
    for (int j = pivot_col_rounded; j < NEON_REGS_PER_ROW; j++) {
        _pivot_row[j] ^= mask & A_neon[row * NEON_REGS_PER_ROW + j];
    }
}

/* multiply pivot row by inverse of pivot */
inverse = inverse_f_ct(pivot);
uint8x16_t inverse_multab = gf16v_get_multab(inverse);

for (int j = pivot_col_rounded; j < NEON_REGS_PER_ROW; j++) {
    _pivot_row[j] = vqtbl1q_u8(inverse_multab, _pivot_row[j]);
}

/* conditionally write pivot row to the correct row, if there is a nonzero pivot */
/* eliminate entries below pivot */
for (int row = pivot_row_lower_bound; row < nrows; row++) {
    unsigned char below_pivot =  (unsigned char) gt32[row];
    unsigned char elt_to_elim = A_bytes[row*NEON_REGS_PER_ROW*16 + pivot_col];

    uint8x16_t multab = gf16v_get_multab(below_pivot & elt_to_elim);
    if (row <= pivot_row_upper_bound) {
        uint8x16_t mask = vmovq_n_u8( (unsigned char)((uint32_t)eq32[row] & ~pivot_is_zero) );
        for (int col = pivot_col_rounded; col < NEON_REGS_PER_ROW; col++) {
            A_neon[row*NEON_REGS_PER_ROW + col] = 	vbslq_u8(mask, _pivot_row[col], A_neon[row*NEON_REGS_PER_ROW + col]) ^
                                                    vqtbl1q_u8(multab, _pivot_row[col]);
        }
    } else {
        for (int j = pivot_col_rounded; j < NEON_REGS_PER_ROW; j++) {
            A_neon[row*NEON_REGS_PER_ROW + j] ^= vqtbl1q_u8(multab, _pivot_row[j]);
        }
    }
}

pivot_row += (-(int32_t)(~pivot_is_zero));
