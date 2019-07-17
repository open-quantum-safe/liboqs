/**
 *  m4r.c
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  Methods of Four Russians
 *
 *  References:
 *  [1]  Martin Albrecht, Gregory Bard and William Hart, (2010),
 *       "Algorithm 898 : efficient multiplication of dense matrices over GF(2)",
 *       ACM Transactions on Mathematical Software, Volume 37 (Number 1),
 *       Article: 9, ISSN 0098-3500
 *  [2]  Martin Albrecht, Gregory Bard and Clement Pernet, (2011),
 *       "Efficient Dense Gaussian Elimination over the Finite Field with Two Elements",
 *       Research Report, https://hal.inria.fr/hal-00796873
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#include <stdlib.h>
#include <string.h>
#include "bits.h"
#include "random.h"
#include "m4r.h"

#define STRIPE_SIZE         8
#define STRIPE_SIZE_LOG     3
#define NUM_GRAY_TABLES     8
#define NUM_GRAY_TABLES_LOG 3
#define M4R_ROW(M, r)       ((uint8_t *)(M)->v + ((r) * (M)->stride))
#define GRAY_TO_BIN(x)      ((x) ^ ((x) >> 1))

const uint8_t _gray_codes_lut2[] = {
    0, 1, 0, 1
};
const uint8_t _gray_codes_lut3[] = {
    0, 1, 0, 2, 0, 1, 0, 2
};
const uint8_t _gray_codes_lut4[] = {
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 3
};
const uint8_t _gray_codes_lut5[] = {
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4
};
const uint8_t _gray_codes_lut6[] = {
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5
};
const uint8_t _gray_codes_lut7[] = {
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6
};
const uint8_t _gray_codes_lut8[] = {
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7
};
const uint8_t* _gray_codes_lut[] = {
    NULL, NULL, _gray_codes_lut2, _gray_codes_lut3,
    _gray_codes_lut4, _gray_codes_lut5, _gray_codes_lut6,
    _gray_codes_lut7, _gray_codes_lut8
};

static void _m4ri_make_table_rev(matrix_ff2* T,
                          const matrix_ff2* B,
                          uint32_t r_end,
                          uint32_t c_end,
                          uint32_t k)
{
    int32_t i, j, is_zero, nblocks;
    const uint8_t *gray_codes_lut;
    packed_t *B_ptr = NULL;
    packed_t *S_ptr = NULL;
    packed_t *T_ptr = NULL;
    packed_t *U_ptr = NULL;
    packed_t mask = 0;
    
    nblocks = ((c_end + MOD) >> LOG2);

    /**
     * This is equivalent to
     *    mask = (ONE << (c_end & MOD))-1;
     *    mask = (c_end & MOD) ? mask : ~mask;
     **/
    mask = (ONE << (c_end & MOD))-1;
    is_zero = ~((c_end & MOD) | -(c_end & MOD)) >> 31;
    mask = mask ^ (-is_zero & (~mask ^ mask));

    /* Note: the 0-th row of T is always 00...0 */
    memset(M4R_ROW(T, 0), 0, T->stride);
    
    /* 1-st row of T, starting at block l */
    memset(M4R_ROW(T, 1), 0, T->stride);
    S_ptr = T_ptr = ((packed_t *)M4R_ROW(T, 1));
    /* r_start-th row of B, starting at block l */
    B_ptr = ((packed_t *)M4R_ROW(B, r_end-k));
    for (j=0; j<nblocks; j++) {
        *T_ptr = *B_ptr++;
        T_ptr++;
    }
    S_ptr[nblocks-1] &= mask; /* Clear unwanted bits */
    
    /* S_ptr is a pointer to the previous row of T */
    
    /* Subsequent rows of T */
    gray_codes_lut = _gray_codes_lut[k];
    for (i=2; i<(1 << k); i++) {
        /* i-th row of T, starting at block l */
        j = GRAY_TO_BIN(i);
        memset(M4R_ROW(T, j), 0, T->stride);
        U_ptr = T_ptr = ((packed_t *)M4R_ROW(T, j));
        /* (r+Gray[i])-th row of B, starting at block l */
        B_ptr = ((packed_t *)M4R_ROW(B, r_end - k + gray_codes_lut[i-1]));
        for (j=0; j<nblocks; j++) {
            *T_ptr = (*S_ptr++ ^ *B_ptr++);
            T_ptr++;
        }
        U_ptr[nblocks-1] &= mask; /* Clear unwanted bits */
        S_ptr = U_ptr; /* Pointer to the previous row of T */
    }
}

static inline void _m4ri_swap_rows(packed_t *a, packed_t *b, uint32_t nblocks)
{
    int32_t i;
    packed_t v = 0;
    packed_t *a_ptr = a, *b_ptr = b;
    
    for (i=0; i<(int32_t)nblocks; i++) {
        v = *a_ptr; *a_ptr = *b_ptr; *b_ptr = v;
        a_ptr++; b_ptr++;
    }
}

/**
 *  Add row `dst` with row `src` from column `c_start` and store
 *  the output at row `dst`
 **/
static inline void _m4ri_add_rows_rev(packed_t *dst,
                                      const packed_t *src,
                                      uint32_t c_end)
{
    int32_t i, is_zero, nblocks;
    packed_t mask, *dst_ptr = NULL;
    const packed_t *src_ptr = NULL;
    
    nblocks = (int32_t)((c_end + MOD) >> LOG2);
    
    /**
     * This is equivalent to
     *    mask = (ONE << (c_end & MOD))-1;
     *    mask = (c_end & MOD) ? mask : ~mask;
     **/
    mask = (ONE << (c_end & MOD))-1;
    is_zero = ~((c_end & MOD) | -(c_end & MOD)) >> 31;
    mask = mask ^ (-is_zero & (~mask ^ mask));
    
    dst_ptr = dst; src_ptr = src;
    for (i=0; i<(int32_t)(nblocks-1); i++) {
        *dst_ptr ^= *src_ptr++;
        dst_ptr++;
    }
    dst[nblocks-1] ^= (*src_ptr & mask);
}

static inline void _m4ri_add_rows_rev_from_gray_table(matrix_ff2* A,
                                                      const matrix_ff2 *T,
                                                      uint32_t r_start,
                                                      uint32_t r_end,
                                                      uint32_t c_end,
                                                      uint32_t k)
{
    int32_t i;
    uint8_t *ptr, x, x0, x1, mask;
    uint32_t hi, lo;
    uint8_t mask_hi = 0, mask_lo = 0;
    
    hi = (c_end-1) >> 3;
    lo = (c_end-k) >> 3;
    mask_hi = (1 << (c_end & 7))-1;
    mask_lo = ~((1 << ((c_end-k) & 7))-1);
    mask = (mask_hi & mask_lo);
    mask = CT_mux(CT_is_equal_zero(mask_hi), mask_lo, mask);
    
    for (i=(int32_t)r_start-1; i>=(int32_t)r_end; i--) {
        ptr = M4R_ROW(A, i) + hi;
        x0 = (((*ptr & mask_hi) << (STRIPE_SIZE - ((c_end-k) & 7))) |
              ((*(ptr-1) & mask_lo) >> ((c_end-k) & 7)));
        x1 = (*ptr & mask) >> ((c_end-k) & 7);
        x = (uint8_t)CT_mux(CT_is_equal(hi, lo), x1, x0);
        x &= ((1 << k)-1);
        _m4ri_add_rows_rev((packed_t *)M4R_ROW(A, i), (const packed_t *)M4R_ROW(T, x), c_end);
    }
}

static uint32_t _m4ri_gauss_submatrix(matrix_ff2* A,
                               uint32_t r,
                               uint32_t c,
                               uint32_t r_end,
                               uint32_t k)
{
    int32_t i, j, l, found;
    uint32_t r_start = r;
    packed_t *u = NULL, *v = NULL;
    
    for (j=c-1; j>=(int32_t)(c-k); j--) {
        found = 0;
        for (i=r_start-1; i>=(int32_t)r_end; i--) {
            v = (packed_t *)M4R_ROW(A, i);
            for (l=0; l<(int32_t)(c-j-1); l++) {
                if (is_bit_set(v, (c-l-1))) {
                    _m4ri_add_rows_rev((packed_t *)M4R_ROW(A, i),
                                       (const packed_t *)M4R_ROW(A, (r-l-1)),
                                       c-l);
                }
            }
            if (is_bit_set(v, j)) {
                _m4ri_swap_rows((packed_t *)M4R_ROW(A, i),
                                (packed_t *)M4R_ROW(A, r_start-1),
                                A->nblocks);
                for (l=r-1; l>=(int32_t)r_start; l--) {
                    u = (packed_t *)M4R_ROW(A, l);
                    if (is_bit_set(u, j)) {
                        _m4ri_add_rows_rev((packed_t *)M4R_ROW(A, l),
                                           (const packed_t *)M4R_ROW(A, r_start-1),
                                           (uint32_t)j+1);
                    }
                }
                r_start--;
                found = 1;
                break;
            }
        }
        if (!found)
            return (uint32_t)(c-j-1);
    }
    
    return (uint32_t)(c-j-1);
}

uint32_t PQCLEAN_NTSKEM1264_CLEAN_m4r_rref(matrix_ff2* A)
{
    int32_t r = 0, c = 0, rank = 0;
    int32_t k = STRIPE_SIZE, rk;
    matrix_ff2 *T = NULL;
    
    if (!(T = PQCLEAN_NTSKEM1264_CLEAN_alloc_matrix_ff2(sizeof(_gray_codes_lut8), A->ncols)))
        return 0;
    
    r = A->nrows;
    c = A->ncols;
    while (c > 0) {
        if (c - k < 0) {
            k = c;
        }
        rk = _m4ri_gauss_submatrix(A, r, c, 0, k);
        if (rk > 0) {
            _m4ri_make_table_rev(T, A, r, c, rk);
            _m4ri_add_rows_rev_from_gray_table(A, T, r-rk, 0, c, rk);
            _m4ri_add_rows_rev_from_gray_table(A, T, A->nrows, r, c, rk);
        }
        r -= rk;
        c -= rk;
        rank += rk;
        if (rk != k)
            --c;
    }
    
    PQCLEAN_NTSKEM1264_CLEAN_free_matrix_ff2(T);
    
    return (uint32_t)rank;

}
