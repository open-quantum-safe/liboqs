// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas_matrix.h
/// @brief linear algebra functions for matrix op.
///
#ifndef _BLAS_MATRIX_H_
#define _BLAS_MATRIX_H_

#include <stdint.h>
#include "params.h"


#ifdef  __cplusplus
extern  "C" {
#endif


///////////////// Section: multiplications  ////////////////////////////////
///// matrix-vector /////

/// @brief matrix-vector multiplication:  c = matA * b , in GF(16)
///
/// @param[out]  c         - the output vector c
/// @param[in]   matA      - a column-major matrix A.
/// @param[in]   n_A_vec_byte  - the size of column vectors in bytes.
/// @param[in]   n_A_width   - the width of matrix A.
/// @param[in]   b          - the vector b.
///
#define gf16mat_prod PQOV_NAMESPACE(gf16mat_prod)
void gf16mat_prod(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b);


/// @brief matrix-vector multiplication:  c = matA * b , in GF(256)
///
/// @param[out]  c         - the output vector c
/// @param[in]   matA      - a column-major matrix A.
/// @param[in]   n_A_vec_byte  - the size of column vectors in bytes.
/// @param[in]   n_A_width   - the width of matrix A.
/// @param[in]   b          - the vector b.
///
#define gf256mat_prod PQOV_NAMESPACE(gf256mat_prod)
void gf256mat_prod(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b);


/// @brief matrix-vector multiplication:  c = matA * b , in GF(16)
///
/// @param[out]  c         - the output vector c
/// @param[in]   matA      - a column-major matrix A.
/// @param[in]   n_A_vec_byte  - the size of column vectors in bytes.
/// @param[in]   n_A_width   - the width of matrix A.
/// @param[in]   multab_b     - the multiplication tables of the vector b.
///
#define gf16mat_prod_multab PQOV_NAMESPACE(gf16mat_prod_multab)
void gf16mat_prod_multab(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *multab_b);


/// @brief matrix-vector multiplication:  c = matA * b , in GF(256)
///
/// @param[out]  c         - the output vector c
/// @param[in]   matA      - a column-major matrix A.
/// @param[in]   n_A_vec_byte  - the size of column vectors in bytes.
/// @param[in]   n_A_width   - the width of matrix A.
/// @param[in]   multab_b     - the multiplication tabls of the vector b.
///
#define gf256mat_prod_multab PQOV_NAMESPACE(gf256mat_prod_multab)
void gf256mat_prod_multab(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *multab_b);


//////////////////////////  Gaussian for solving lienar equations ///////////////////////////


/// @brief Computing the row echelon form of a matrix, in GF(16)
///
/// @param[in,out]  sq_col_mat_a   - square matrix parts of a linear system. a is a column major matrix.
///                                  The returned matrix of row echelon form is a row major matrix.
/// @param[in,out]  constant       - constant parts of a linear system.
/// @param[in]           len       - the width of the matrix a, i.e., the number of column vectors.
/// @return   1(true) if success. 0(false) if the matrix is singular.
///
#define gf16mat_gaussian_elim PQOV_NAMESPACE(gf16mat_gaussian_elim)
unsigned gf16mat_gaussian_elim(uint8_t *sq_col_mat_a, uint8_t *constant, unsigned len);

/// @brief Back substitution of the constant terms with a row echelon form of a matrix, in GF(16)
///
/// @param[in,out]  constant       - constant parts of a linear system.
/// @param[in]     sq_row_mat_a    - row echelon form of a linear system.
/// @param[in]           len       - the height of the matrix a, i.e., the number of row vectors.
///
#define gf16mat_back_substitute PQOV_NAMESPACE(gf16mat_back_substitute)
void gf16mat_back_substitute( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len);

/// @brief Computing the row echelon form of a matrix, in GF(256)
///
/// @param[in,out]  sq_col_mat_a   - square matrix parts of a linear system. a is a column major matrix.
///                                  The returned matrix of row echelon form is a row major matrix.
/// @param[in,out]  constant       - constant parts of a linear system.
/// @param[in]           len       - the width of the matrix a, i.e., the number of column vectors.
/// @return   1(true) if success. 0(false) if the matrix is singular.
///
#define gf256mat_gaussian_elim PQOV_NAMESPACE(gf256mat_gaussian_elim)
unsigned gf256mat_gaussian_elim(uint8_t *sq_col_mat_a, uint8_t *constant, unsigned len);

/// @brief Back substitution of the constant terms with a row echelon form of a matrix, in GF(16)
///
/// @param[in,out]  constant       - constant parts of a linear system.
/// @param[in]     sq_row_mat_a    - row echelon form of a linear system.
/// @param[in]           len       - the height of the matrix a, i.e., the number of row vectors.
///
#define gf256mat_back_substitute PQOV_NAMESPACE(gf256mat_back_substitute)
void gf256mat_back_substitute( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len);




#ifdef  __cplusplus
}
#endif

#endif  // _BLAS_MATRIX_H_

