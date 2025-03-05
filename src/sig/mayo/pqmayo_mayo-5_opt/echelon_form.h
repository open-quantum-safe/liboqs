
// SPDX-License-Identifier: Apache-2.0

#ifndef ECHELON_FORM_H
#define ECHELON_FORM_H

#include <stdalign.h>
#include <stdint.h>
#include <mem.h>
#include <arithmetic.h>

#define MAYO_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MAYO_MIN(x, y) (((x) < (y)) ? (x) : (y))

static inline unsigned char
m_extract_element(const uint64_t *in, int index) {
    const int leg = index / 16;
    const int offset = index % 16;

    return (in[leg] >> (offset*4)) & 0xF;
}

static inline void
ef_pack_m_vec(const unsigned char *in, uint64_t *out, int ncols) {
    int i;
    unsigned char *out8 = (unsigned char *)out;
    for(i = 0; i+1 < ncols; i += 2){
#ifdef TARGET_BIG_ENDIAN
        out8[(((i/2 + 8) / 8) * 8) - 1 - (i/2)%8]  = (in[i+0] << 0) | (in[i+1] << 4);
#else
        out8[i/2]  = (in[i+0] << 0) | (in[i+1] << 4);
#endif
    }
    if (ncols % 2 == 1){
#ifdef TARGET_BIG_ENDIAN
        out8[(((i/2 + 8) / 8) * 8) - 1 - (i/2)%8]  = (in[i+0] << 0);
#else
        out8[i/2]  = (in[i+0] << 0);
#endif
    }
}

static inline void
ef_unpack_m_vec(int legs, const uint64_t *in, unsigned char *out) {
    const unsigned char *in8 = (const unsigned char *)in;
    for(int i = 0; i < legs * 16; i += 2){
#ifdef TARGET_BIG_ENDIAN
        out[i]   = (in8[(((i/2 + 8) / 8) * 8) - 1 - (i/2)%8]) & 0xF;
        out[i+1] = (in8[(((i/2 + 8) / 8) * 8) - 1 - (i/2)%8] >> 4);
#else
        out[i]   = (in8[i/2]) & 0xF;
        out[i+1] = (in8[i/2] >> 4);
#endif
    }
}


// put matrix in row echelon form with ones on first nonzero entries *in
// constant time*
static inline void EF(unsigned char *A, int nrows, int ncols) {

    alignas (32) uint64_t _pivot_row[(K_MAX * O_MAX + 1 + 15) / 16];
    alignas (32) uint64_t _pivot_row2[(K_MAX * O_MAX + 1 + 15) / 16];
    alignas (32) uint64_t packed_A[((K_MAX * O_MAX + 1 + 15) / 16) * M_MAX] = {0};

    int row_len = (ncols + 15) / 16;

    // nibbleslice the matrix A
    for (int i = 0; i < nrows; i++) {
        ef_pack_m_vec(A + i * ncols, packed_A + i * row_len, ncols);
    }

    // pivot row is secret, pivot col is not

    unsigned char inverse;
    int pivot_row = 0;
    for (int pivot_col = 0; pivot_col < ncols; pivot_col++) {

        int pivot_row_lower_bound = MAYO_MAX(0, pivot_col + nrows - ncols);
        int pivot_row_upper_bound = MAYO_MIN(nrows - 1, pivot_col);
        // the pivot row is guaranteed to be between these lower and upper bounds if
        // A has full rank

        // zero out pivot row
        for (int i = 0; i < row_len; i++) {
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

            for (int j = 0; j < row_len; j++) {
                _pivot_row[j] ^= (is_pivot_row | (below_pivot_row & pivot_is_zero)) &
                                 packed_A[row * row_len + j];
            }
            pivot = m_extract_element(_pivot_row, pivot_col);
            pivot_is_zero = ~ct_compare_64((int) pivot, 0);
        }

        // multiply pivot row by inverse of pivot
        inverse = inverse_f(pivot);
        vec_mul_add_u64(row_len, _pivot_row, inverse, _pivot_row2);

        // conditionally write pivot row to the correct row, if there is a nonzero
        // pivot
        for (int row = pivot_row_lower_bound; row <= pivot_row_upper_bound; row++) {
            uint64_t do_copy = ~ct_compare_64(row, pivot_row) & ~pivot_is_zero;
            uint64_t do_not_copy = ~do_copy;
            for (int col = 0; col < row_len; col++) {
                packed_A[row * row_len + col] =
                    (do_not_copy & packed_A[row * row_len + col]) +
                    (do_copy & _pivot_row2[col]);
            }
        }

        // eliminate entries below pivot
        for (int row = pivot_row_lower_bound; row < nrows; row++) {
            unsigned char below_pivot = (row > pivot_row);
            unsigned char elt_to_elim = m_extract_element(packed_A + row * row_len, pivot_col);

            vec_mul_add_u64(row_len, _pivot_row2, below_pivot * elt_to_elim,
                                    packed_A + row * row_len);                            
        }

        pivot_row += (-(int64_t)(~pivot_is_zero));
    }

    unsigned char temp[(O_MAX * K_MAX + 1 + 15)];

    // unbitslice the matrix A
    for (int i = 0; i < nrows; i++) {
        ef_unpack_m_vec(row_len, packed_A + i * row_len, temp);
        for (int j = 0; j < ncols; j++) {
            A[i * ncols + j] = temp[j];
        }
    }

    mayo_secure_clear(temp, K_MAX * O_MAX + 1 + 15);
    mayo_secure_clear(_pivot_row, (K_MAX * O_MAX + 1 + 15) / 16 * 8);
    mayo_secure_clear(_pivot_row2, (K_MAX * O_MAX + 1 + 15) / 16 * 8);
    mayo_secure_clear(packed_A, ((K_MAX * O_MAX + 1 + 15) / 16) * M_MAX * 8);
}

#endif

