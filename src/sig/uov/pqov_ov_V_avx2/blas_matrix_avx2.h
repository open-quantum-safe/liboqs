// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas_matrix_avx2.h
/// @brief Linear algebra functions for matrix operations, specialized for AVX2 instruction set.
///

#ifndef _BLAS_MATRIX_AVX2_H_
#define _BLAS_MATRIX_AVX2_H_


#include "stdint.h"
#include "params.h"



#ifdef  __cplusplus
extern  "C" {
#endif


///////////////////////////////  GF( 16 ) ////////////////////////////////////////////////////

/// @brief  c = matA * b , GF(16)
///
/// @param[out]   c         - the output vector c
/// @param[in]   matA          - the matrix A.
/// @param[in]   n_A_vec_byte    - the size of column vectors in A.
/// @param[in]   n_A_width       - the width of matrix A.
/// @param[in]   b_multab        - the vector b, in multiplication tables.
///
#define gf16mat_prod_multab_avx2 PQOV_NAMESPACE(gf16mat_prod_multab_avx2)
void gf16mat_prod_multab_avx2( uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b_multab );

/// @brief  c = mat_a * b , GF(16)
///
/// @param[out]   c         - the output vector c
/// @param[in]   mat_a       - the matrix a.
/// @param[in]   a_h_byte    - the size of column vectors in a.
/// @param[in]   a_w        - the width of matrix a.
/// @param[in]   b           - the vector b.
///
#define gf16mat_prod_avx2 PQOV_NAMESPACE(gf16mat_prod_avx2)
void gf16mat_prod_avx2( uint8_t *c, const uint8_t *mat_a, unsigned a_h_byte, unsigned a_w, const uint8_t *b );


/// @brief Gaussian elimination to row-echelon form for a linear system, in GF(16)
///
/// @param[in,out]  sq_col_mat_a     - input of col-major-sq-matrix a. output of row-major-sq-matrix.
/// @param[in,out]  constant         - constant terms of the linear system.
/// @param[in]      len              - the dimension of input matrix a.
/// @return   1(true) if success. 0(false) if the matrix is singular.
///
#define gf16mat_gaussian_elim_avx2 PQOV_NAMESPACE(gf16mat_gaussian_elim_avx2)
unsigned gf16mat_gaussian_elim_avx2(uint8_t *sq_col_mat_a, uint8_t *constant, unsigned len);

/// @brief Performing back-substituion to solve a linear system, in GF(16)
///
/// @param[in,out]  constant   - constant terms of the linear system.
/// @param[in]   sq_row_mat_a  - row echelon form of the square row-major matrix a.
/// @param[in]   len           - the dimension of input matrix a.
///
#define gf16mat_back_substitute_avx2 PQOV_NAMESPACE(gf16mat_back_substitute_avx2)
void gf16mat_back_substitute_avx2( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len);



///////////////////////////////  GF( 256 ) ////////////////////////////////////////////////////

/// @brief  c = matA * b , GF(256)
///
/// @param[out]   c         - the output vector c
/// @param[in]   matA       - the matrix A.
/// @param[in]   n_A_vec_byte    - the size of column vectors in A.
/// @param[in]   n_A_width       - the widht of matrix A.
/// @param[in]   b_multab        - the vector b, in multiplication tables.
///
#define gf256mat_prod_multab_avx2 PQOV_NAMESPACE(gf256mat_prod_multab_avx2)
void gf256mat_prod_multab_avx2( uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b_multab );

/// @brief  c = matA * b , GF(256)
///
/// @param[out]   c         - the output vector c
/// @param[in]   matA       - the matrix A.
/// @param[in]   n_A_vec_byte   - the size of column vectors in a.
/// @param[in]   n_A_width    - the width of matrix a.
/// @param[in]   b           - the vector b.
///
#define gf256mat_prod_avx2 PQOV_NAMESPACE(gf256mat_prod_avx2)
void gf256mat_prod_avx2( uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b );


/// @brief Gaussian elimination to row-echelon form for a linear system, in GF(256)
///
/// @param[in,out]  sq_col_mat_a     - input of col-major-sq-matrix a. output of row-major-sq-matrix.
/// @param[in,out]  constant         - constant terms of the linear system.
/// @param[in]      len              - the dimension of input matrix a.
/// @return   1(true) if success. 0(false) if the matrix is singular.
///
#define gf256mat_gaussian_elim_avx2 PQOV_NAMESPACE(gf256mat_gaussian_elim_avx2)
unsigned gf256mat_gaussian_elim_avx2(uint8_t *sq_col_mat_a, uint8_t *constant, unsigned len);

/// @brief Performing back-substituion to solve a linear system, in GF(256)
///
/// @param[in,out]  constant   - constant terms of the linear system.
/// @param[in]   sq_row_mat_a  - row echelon form of the square row-major matrix a.
/// @param[in]   len           - the dimension of input matrix a.
///
#define gf256mat_back_substitute_avx2 PQOV_NAMESPACE(gf256mat_back_substitute_avx2)
void gf256mat_back_substitute_avx2( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len);



#ifdef  __cplusplus
}
#endif



#endif // _BLAS_MATRIX_AVX2_H_

