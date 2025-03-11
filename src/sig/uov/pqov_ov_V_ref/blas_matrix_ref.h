// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas_matrix_ref.h
/// @brief linear algebra functions for matrix op.
///
#ifndef _BLAS_MATRIX_REF_H_
#define _BLAS_MATRIX_REF_H_

#include <stdint.h>



#ifdef  __cplusplus
extern  "C" {
#endif


///////////////// Section: multiplications  ////////////////////////////////

// matrix-vector
#define gf16mat_prod_ref PQOV_NAMESPACE(gf16mat_prod_ref)
void gf16mat_prod_ref(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b);

#define gf256mat_prod_ref PQOV_NAMESPACE(gf256mat_prod_ref)
void gf256mat_prod_ref(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b);

/////////////////////////////////////////////////////
#define gf256mat_gaussian_elim_ref PQOV_NAMESPACE(gf256mat_gaussian_elim_ref)
unsigned gf256mat_gaussian_elim_ref(uint8_t *sq_col_mat_a, uint8_t *constant, unsigned len);

#define gf256mat_back_substitute_ref PQOV_NAMESPACE(gf256mat_back_substitute_ref)
void gf256mat_back_substitute_ref( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len);

#define gf16mat_gaussian_elim_ref PQOV_NAMESPACE(gf16mat_gaussian_elim_ref)
unsigned gf16mat_gaussian_elim_ref(uint8_t *sq_col_mat_a, uint8_t *constant, unsigned len);

#define gf16mat_back_substitute_ref PQOV_NAMESPACE(gf16mat_back_substitute_ref)
void gf16mat_back_substitute_ref( uint8_t *constant, const uint8_t *sq_row_mat_a, unsigned len);



#ifdef  __cplusplus
}
#endif

#endif  // _BLAS_MATRIX_REF_H_

