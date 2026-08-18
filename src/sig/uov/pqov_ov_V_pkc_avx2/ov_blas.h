// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file ov_blas.h
/// @brief Defining the functions used in ov.c according to the definitions in params.h
///
///  Defining the functions used in ov.c according to the definitions in params.h

#ifndef _OV_BLAS_H_
#define _OV_BLAS_H_

#include "blas.h"

#include "parallel_matrix_op.h"

#include "params.h"



#ifdef _USE_GF16

#define gfv_get_ele     gf16v_get_ele
#define gfv_mul_scalar  gf16v_mul_scalar
#define gfv_madd        gf16v_madd

#define gfmat_prod      gf16mat_prod

#define batch_trimat_madd    batch_trimat_madd_gf16
#define batch_trimatTr_madd  batch_trimatTr_madd_gf16
#define batch_2trimat_madd   batch_2trimat_madd_gf16
#define batch_matTr_madd     batch_matTr_madd_gf16
#define batch_upper_matTr_x_mat     batch_upper_matTr_x_mat_gf16

// TODO: this should be cleaner
#if defined( _BLAS_M4F_)
#include "blas_matrix_m4f.h"
#define batch_quad_trimat_eval batch_quad_trimat_eval_gf16_m4f
#else
#define batch_quad_trimat_eval batch_quad_trimat_eval_gf16
#endif



#define gfmat_inv       gf16mat_inv

#if defined(_MUL_WITH_MULTAB_)
#define gfv_generate_multabs   gf16v_generate_multabs
#define gfmat_prod_multab      gf16mat_prod_multab
#define gfv_madd_multab        gf16v_madd_multab
#define batch_trimat_madd_multab    batch_trimat_madd_multab_gf16
#define batch_trimatTr_madd_multab  batch_trimatTr_madd_multab_gf16
#define batch_2trimat_madd_multab   batch_2trimat_madd_multab_gf16
#define batch_matTr_madd_multab     batch_matTr_madd_multab_gf16
#define batch_upper_matTr_x_mat_multab     batch_upper_matTr_x_mat_multab_gf16
#endif

#else

#define gfv_get_ele     gf256v_get_ele
#define gfv_mul_scalar  gf256v_mul_scalar
#define gfv_madd        gf256v_madd

#define gfmat_prod      gf256mat_prod

#define batch_trimat_madd    batch_trimat_madd_gf256
#define batch_trimatTr_madd  batch_trimatTr_madd_gf256
#define batch_2trimat_madd   batch_2trimat_madd_gf256
#define batch_matTr_madd     batch_matTr_madd_gf256
#define batch_upper_matTr_x_mat     batch_upper_matTr_x_mat_gf256

#if defined(_MUL_WITH_MULTAB_)
#define gfv_generate_multabs   gf256v_generate_multabs
#define gfmat_prod_multab      gf256mat_prod_multab
#define gfv_madd_multab        gf256v_madd_multab
#define batch_trimat_madd_multab    batch_trimat_madd_multab_gf256
#define batch_trimatTr_madd_multab  batch_trimatTr_madd_multab_gf256
#define batch_2trimat_madd_multab   batch_2trimat_madd_multab_gf256
#define batch_matTr_madd_multab     batch_matTr_madd_multab_gf256
#define batch_upper_matTr_x_mat_multab     batch_upper_matTr_x_mat_multab_gf256
#endif

// TODO: this should be cleaner
#if defined( _BLAS_M4F_)
#include "blas_matrix_m4f.h"
#define batch_quad_trimat_eval batch_quad_trimat_eval_gf256_m4f
#else
#define batch_quad_trimat_eval batch_quad_trimat_eval_gf256
#endif


#define gfmat_inv       gf256mat_inv

#endif




#endif  // _OV_BLAS_H_

