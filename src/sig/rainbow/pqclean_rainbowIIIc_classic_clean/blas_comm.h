#ifndef _BLAS_COMM_H_
#define _BLAS_COMM_H_
/// @file blas_comm.h
/// @brief Common functions for linear algebra.
///

#include "rainbow_config.h"
#include <stdint.h>

/// @brief set a vector to 0.
///
/// @param[in,out]   b      - the vector b.
/// @param[in]  _num_byte   - number of bytes for the vector b.
///
void PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_gf256v_set_zero(uint8_t *b, unsigned int _num_byte);

/// @brief get an element from GF(256) vector .
///
/// @param[in]  a         - the input vector a.
/// @param[in]  i         - the index in the vector a.
/// @return  the value of the element.
///
uint8_t PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_gf256v_get_ele(const uint8_t *a, unsigned int i);

/// @brief check if a vector is 0.
///
/// @param[in]   a          - the vector a.
/// @param[in]  _num_byte   - number of bytes for the vector a.
/// @return  1(true) if a is 0. 0(false) else.
///
unsigned int PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_gf256v_is_zero(const uint8_t *a, unsigned int _num_byte);

/// @brief polynomial multiplication:  c = a*b
///
/// @param[out]   c         - the output polynomial c
/// @param[in]   a          - the vector a.
/// @param[in]   b          - the vector b.
/// @param[in]  _num   - number of elements for the polynomials a and b.
///
void PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_gf256v_polymul(uint8_t *c, const uint8_t *a, const uint8_t *b, unsigned int _num);

/// @brief matrix-vector multiplication:  c = matA * b , in GF(256)
///
/// @param[out]  c         - the output vector c
/// @param[in]   matA      - a column-major matrix A.
/// @param[in]   n_A_vec_byte  - the size of column vectors in bytes.
/// @param[in]   n_A_width   - the width of matrix A.
/// @param[in]   b          - the vector b.
///
void PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_gf256mat_prod(uint8_t *c, const uint8_t *matA, unsigned int n_A_vec_byte, unsigned int n_A_width, const uint8_t *b);

/// @brief matrix-matrix multiplication:  c = a * b , in GF(256)
///
/// @param[out]  c         - the output matrix c
/// @param[in]   c         - a matrix a.
/// @param[in]   b         - a matrix b.
/// @param[in]   len_vec   - the length of column vectors.
///
void PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_gf256mat_mul(uint8_t *c, const uint8_t *a, const uint8_t *b, unsigned int len_vec);

/// @brief Gauss elimination for a matrix, in GF(256)
///
/// @param[in,out]  mat    - the matrix.
/// @param[in]   h         - the height of the matrix.
/// @param[in]   w         - the width of the matrix.
/// @return   1(true) if success. 0(false) if the matrix is singular.
///
unsigned int PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_gf256mat_gauss_elim(uint8_t *mat, unsigned int h, unsigned int w);

/// @brief Solving linear equations, in GF(256)
///
/// @param[out]  sol       - the solutions.
/// @param[in]   inp_mat   - the matrix parts of input equations.
/// @param[in]   c_terms   - the constant terms of the input equations.
/// @param[in]   n         - the number of equations.
/// @return   1(true) if success. 0(false) if the matrix is singular.
///
unsigned int PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_gf256mat_solve_linear_eq(uint8_t *sol, const uint8_t *inp_mat, const uint8_t *c_terms, unsigned int n);

/// @brief Computing the inverse matrix, in GF(256)
///
/// @param[out]  inv_a     - the output of matrix a.
/// @param[in]   a         - a matrix a.
/// @param[in]   H         - height of matrix a, i.e., matrix a is an HxH matrix.
/// @param[in]   buffer    - The buffer for computations. it has to be as large as 2 input matrixes.
/// @return   1(true) if success. 0(false) if the matrix is singular.
///
unsigned int PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_gf256mat_inv(uint8_t *inv_a, const uint8_t *a, unsigned int H, uint8_t *buffer);

#endif // _BLAS_COMM_H_
