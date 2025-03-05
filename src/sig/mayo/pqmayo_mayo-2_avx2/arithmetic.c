// SPDX-License-Identifier: Apache-2.0

#include <arithmetic.h>
#include <simple_arithmetic.h>
#include <mem.h>
#include <echelon_form.h>
#include <stdalign.h>
#include <stdlib.h>
#include <string.h>
#ifdef ENABLE_CT_TESTING
#include <valgrind/memcheck.h>
#endif

void m_upper(const mayo_params_t* p, const uint64_t *in, uint64_t *out, int size) {
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif
    // Look into AVX2'ing this 
    const int m_vec_limbs = PARAM_m_vec_limbs(p);
    int m_vecs_stored = 0;
    for (int r = 0; r < size; r++) {
        for (int c = r; c < size; c++) {
            m_vec_copy(m_vec_limbs, in + m_vec_limbs * (r * size + c), out + m_vec_limbs * m_vecs_stored );
            if (r != c) {
                m_vec_add(m_vec_limbs, in + m_vec_limbs * (c * size + r), out + m_vec_limbs * m_vecs_stored );
            }
            m_vecs_stored ++;
        }
    }
}


// sample a solution x to Ax = y, with r used as randomness
// require:
// - A is a matrix with m rows and k*o+1 collumns (values in the last collum are
// not important, they will be overwritten by y) in row major order
// - y is a vector with m elements
// - r and x are k*o bytes long
// return: 1 on success, 0 on failure
int sample_solution(const mayo_params_t *p, unsigned char *A,
                           const unsigned char *y, const unsigned char *r,
                           unsigned char *x, int k, int o, int m, int A_cols) {
    #ifdef MAYO_VARIANT
    (void) p;
    #endif
    unsigned char finished;
    int col_upper_bound;
    unsigned char correct_column;

    // x <- r
    for (int i = 0; i < k * o; i++) {
        x[i] = r[i];
    }

    // compute Ar;
    unsigned char Ar[M_MAX];
    for (int i = 0; i < m; i++) {
        A[k * o + i * (k * o + 1)] = 0; // clear last col of A
    }
    mat_mul(A, r, Ar, k * o + 1, m, 1);

    // move y - Ar to last column of matrix A
    for (int i = 0; i < m; i++) {
        A[k * o + i * (k * o + 1)] = sub_f(y[i], Ar[i]);
    }

    EF(A, m, k * o + 1);

    // check if last row of A (excluding the last entry of y) is zero
    unsigned char full_rank = 0;
    for (int i = 0; i < A_cols - 1; i++) {
        full_rank |= A[(m - 1) * A_cols + i];
    }

// It is okay to leak if we need to restart or not
#ifdef ENABLE_CT_TESTING
    VALGRIND_MAKE_MEM_DEFINED(&full_rank, 1);
#endif

    if (full_rank == 0) {
        return 0;
    }

    // back substitution in constant time
    // the index of the first nonzero entry in each row is secret, which makes
    // things less efficient

    for (int row = m - 1; row >= 0; row--) {
        finished = 0;
        col_upper_bound = MAYO_MIN(row + (32/(m-row)), k*o);
        // the first nonzero entry in row r is between r and col_upper_bound with probability at least ~1-q^{-32}

        for (int col = row; col <= col_upper_bound; col++) {

            // Compare two chars in constant time.
            // Returns 0x00 if the byte arrays are equal, 0xff otherwise.
            correct_column = ct_compare_8((A[row * A_cols + col]), 0) & ~finished;

            unsigned char u = correct_column & A[row * A_cols + A_cols - 1];
            x[col] ^= u;

            for (int i = 0; i < row; i += 8) {
                uint64_t tmp = ( (uint64_t) A[ i    * A_cols + col] <<  0) ^ ( (uint64_t) A[(i+1) * A_cols + col] <<  8)
                             ^ ( (uint64_t) A[(i+2) * A_cols + col] << 16) ^ ( (uint64_t) A[(i+3) * A_cols + col] << 24)
                             ^ ( (uint64_t) A[(i+4) * A_cols + col] << 32) ^ ( (uint64_t) A[(i+5) * A_cols + col] << 40)
                             ^ ( (uint64_t) A[(i+6) * A_cols + col] << 48) ^ ( (uint64_t) A[(i+7) * A_cols + col] << 56);

                tmp = mul_fx8(u, tmp);

                A[ i    * A_cols + A_cols - 1] ^= (tmp      ) & 0xf;
                A[(i+1) * A_cols + A_cols - 1] ^= (tmp >> 8 ) & 0xf;
                A[(i+2) * A_cols + A_cols - 1] ^= (tmp >> 16) & 0xf;
                A[(i+3) * A_cols + A_cols - 1] ^= (tmp >> 24) & 0xf;
                A[(i+4) * A_cols + A_cols - 1] ^= (tmp >> 32) & 0xf;
                A[(i+5) * A_cols + A_cols - 1] ^= (tmp >> 40) & 0xf;
                A[(i+6) * A_cols + A_cols - 1] ^= (tmp >> 48) & 0xf;
                A[(i+7) * A_cols + A_cols - 1] ^= (tmp >> 56) & 0xf;
            }

            finished = finished | correct_column;
        }
    }
    return 1;
}

volatile uint32_t uint32_t_blocker = 0;
volatile uint64_t uint64_t_blocker = 0;
volatile unsigned char unsigned_char_blocker = 0;
