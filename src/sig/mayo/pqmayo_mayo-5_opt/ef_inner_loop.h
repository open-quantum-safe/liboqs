// SPDX-License-Identifier: Apache-2.0

int pivot_row_lower_bound = MAYO_MAX(0, pivot_col + nrows - ncols);
int pivot_row_upper_bound = MAYO_MIN(nrows - 1, pivot_col);
// the pivot row is guaranteed to be between these lower and upper bounds if
// A has full rank

// zero out pivot row
for (int i = offset; i < row_len; i++) {
    _pivot_row[i] = 0;
    _pivot_row2[i] = 0;
}

// try to get a pivot row in constant time
unsigned char pivot = 0;
uint64_t pivot_is_zero = -1;
for (int row = pivot_row_lower_bound;
        row <= MAYO_MIN(nrows - 1, pivot_row_upper_bound + 32); row++) {

    uint64_t is_pivot_row = ~ct_compare_64(row, pivot_row);
    uint64_t below_pivot_row = ct_64_is_greater_than(row, pivot_row);

    for (int j = offset; j < row_len; j++) {
        _pivot_row[j] ^= (is_pivot_row | (below_pivot_row & pivot_is_zero)) &
                            packed_A[row * row_len + j];
    }
    pivot = m_extract_element(_pivot_row, pivot_col);
    pivot_is_zero = ~ct_compare_64((int) pivot, 0);
}

// multiply pivot row by inverse of pivot
inverse = inverse_f(pivot);
vec_mul_add_u64(row_len - offset, _pivot_row + offset, inverse, _pivot_row2 + offset);

// conditionally write pivot row to the correct row, if there is a nonzero
// pivot
for (int row = pivot_row_lower_bound; row <= pivot_row_upper_bound; row++) {
    uint64_t do_copy = ~ct_compare_64(row, pivot_row) & ~pivot_is_zero;
    uint64_t do_not_copy = ~do_copy;
    for (int col = offset; col < row_len; col++) {
        packed_A[row * row_len + col] =
            (do_not_copy & packed_A[row * row_len + col]) +
            (do_copy & _pivot_row2[col]);
    }
}

// eliminate entries below pivot
for (int row = pivot_row_lower_bound; row < nrows; row++) {
    unsigned char below_pivot = (row > pivot_row);
    unsigned char elt_to_elim = m_extract_element(packed_A + row * row_len, pivot_col);

    vec_mul_add_u64(row_len - offset, _pivot_row2 + offset, below_pivot * elt_to_elim,
                            packed_A + row * row_len + offset);                            
}

pivot_row += (-(int32_t)(~pivot_is_zero));
