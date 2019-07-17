/**
 *  matrix_ff2.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef _MATRIX_FF2_H
#define _MATRIX_FF2_H

#include <stdio.h>
#include <stdint.h>
#include "bits.h"

/**
 *  For optimisation purposes, we may need to pad the structure
 *  so that it fits into the cache-line of the CPU
 **/
typedef struct {
    uint32_t nrows;     /* Number of rows in the matrix */
    uint32_t ncols;     /* Number of columns in the matrix */
    uint32_t nblocks;   /* Number of 'packed_t's in use per row */
    uint32_t stride;    /* Bytes allocedd per row, 128-bit aligned */
    uint8_t* v;         /* Pointer to a buffer containing the matrix values */
} matrix_ff2;

/**
 *  Allocate an (nrows x ncols) matrix F_2
 *
 *  @note
 *  The allocated matrix is not guaranteed to be all zeros
 *
 *  @param[in] nrows    Number of rows
 *  @param[in] ncols    Number of columns
 *  @return Pointer to matrix object
 **/
matrix_ff2* PQCLEAN_NTSKEM1264_CLEAN_alloc_matrix_ff2(int nrows, int ncols);

/**
 *  Allocate and zero an (nrows x ncols) matrix F_2
 *
 *  @param[in] nrows    Number of rows
 *  @param[in] ncols    Number of columns
 *  @return Pointer to matrix object
 **/
matrix_ff2* PQCLEAN_NTSKEM1264_CLEAN_calloc_matrix_ff2(int nrows, int ncols);

/**
 *  Deallocate an F_2 matrix
 *
 *  @param[in] M    Pointer to matrix object
 **/
void PQCLEAN_NTSKEM1264_CLEAN_free_matrix_ff2(matrix_ff2* M);

/**
 *  Zero an F_2 matrix
 *
 *  @param[in] M    Pointer to matrix object
 **/
void PQCLEAN_NTSKEM1264_CLEAN_zero_matrix_ff2(matrix_ff2* M);

/**
 *  Pointer to the r-th row of a matrix
 *
 *  @param[in] M    Pointer to a matrix object
 *  @param[in] r    The row index
 *  @return Pointer to the r-th row of a matrix
 **/
static inline uint8_t* row_ptr_matrix_ff2(const matrix_ff2* M, uint32_t r)
{
    return ((uint8_t *)(M)->v + ((r) * (M)->stride));
}

/**
 *  Swap columns `a` and `b` in matrix `M`
 *
 *  @param[in,out] M    The matrix to be column-swapped
 *  @param[in]     a    Column a
 *  @param[in]     b    Column b
 **/
void PQCLEAN_NTSKEM1264_CLEAN_column_swap_matrix_ff2(matrix_ff2* M, int32_t a, int32_t b);

/**
 *  Transform a matrix M into reduced row echelon form M = [A | I]
 *
 *  @note
 *  The transformation is done in-place and the resulting matrix
 *  is in the form [ A | I ].
 *
 *  @param[in,out] M    Pointer to a matrix
 *  @return The rank of matrix M
 **/
uint32_t PQCLEAN_NTSKEM1264_CLEAN_reduce_row_echelon_matrix_ff2(matrix_ff2 *M);

#endif /* _MATRIX_GF2_H */
