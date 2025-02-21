// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file blas.h
/// @brief Defining the implementations for linear algebra functions depending on the machine architecture.
///

#ifndef _BLAS_H_
#define _BLAS_H_


#include "config.h"

//
//  These are light-weighted inlined functions.
//  We use macros to link them to corresponding inlined functions.
//

#if defined( _BLAS_AVX2_ ) && defined( _BLAS_GFNI_ )

#define _BLAS_UNIT_LEN_ 32

#include "blas_avx2.h"
#include "blas_avx2_gfni.h"

#define gf16v_mul_scalar    gf16v_mul_scalar_gfni
#define gf16v_madd          gf16v_madd_gfni

#define gf256v_add          gf256v_add_avx2
#define gf256v_mul_scalar   gf256v_mul_scalar_avx2_gfni
#define gf256v_madd         gf256v_madd_avx2_gfni

#define gf16v_madd_multab   gf16v_madd_multab_gfni
#define gf16v_generate_multabs  gf16v_generate_multabs_gfni

#include "blas_u32.h"
#define gf256v_conditional_add      _gf256v_conditional_add_u32

#include "blas_comm.h"
#include "blas_matrix.h"
#include "blas_matrix_sse.h"
#include "blas_matrix_avx2.h"
#include "blas_matrix_avx2_gfni.h"

#elif defined( _BLAS_AVX2_ )

#define _BLAS_UNIT_LEN_ 32

#include "blas_avx2.h"

#define gf16v_mul_scalar    gf16v_mul_scalar_avx2
#define gf16v_madd          gf16v_madd_avx2

#define gf256v_add          gf256v_add_avx2
#define gf256v_mul_scalar   gf256v_mul_scalar_avx2
#define gf256v_madd         gf256v_madd_avx2

#define gf16v_madd_multab   gf16v_madd_multab_avx2
#define gf256v_madd_multab  gf256v_madd_multab_avx2
#define gf16v_generate_multabs  gf16v_generate_multabs_avx2
#define gf256v_generate_multabs  gf256v_generate_multabs_avx2

#include "blas_u32.h"
#define gf256v_conditional_add      _gf256v_conditional_add_u32

#include "blas_comm.h"
#include "blas_matrix.h"
#include "blas_matrix_sse.h"
#include "blas_matrix_avx2.h"

#elif defined( _BLAS_SSE_ )

#define _BLAS_UNIT_LEN_ 16

#include "blas_sse.h"

#define gf16v_mul_scalar    gf16v_mul_scalar_sse
#define gf16v_madd          gf16v_madd_sse

#define gf256v_add          gf256v_add_sse
#define gf256v_mul_scalar   gf256v_mul_scalar_sse
#define gf256v_madd         gf256v_madd_sse

#define gf16v_madd_multab   gf16v_madd_multab_sse
#define gf256v_madd_multab  gf256v_madd_multab_sse
#define gf16v_generate_multabs  gf16v_generate_multabs_sse
#define gf256v_generate_multabs  gf256v_generate_multabs_sse


#include "blas_u32.h"
#define gf256v_conditional_add      _gf256v_conditional_add_u32

#include "blas_comm.h"
#include "blas_matrix.h"
#include "blas_matrix_sse.h"



#elif defined( _BLAS_NEON_ )

#include "blas_neon.h"

#define gf256v_add          gf256v_add_neon

#define gf16v_mul_scalar    gf16v_mul_scalar_neon
#define gf16v_madd          gf16v_madd_neon

#define gf256v_mul_scalar   gf256v_mul_scalar_neon
#define gf256v_madd         gf256v_madd_neon

#define gf16v_madd_multab   gf16v_madd_multab_neon
#define gf256v_madd_multab  gf256v_madd_multab_neon
#define gf16v_generate_multabs  gf16v_generate_multabs_neon
#define gf256v_generate_multabs  gf256v_generate_multabs_neon


#define _BLAS_UNIT_LEN_ 16

#include "blas_u32.h"
#define gf256v_conditional_add      _gf256v_conditional_add_u32

#include "blas_comm.h"
#include "blas_matrix.h"
#include "blas_matrix_neon.h"

#elif defined( _BLAS_UINT64_ )

#define _BLAS_UNIT_LEN_ 8

#include "blas_u64.h"

#define gf16v_mul_scalar    _gf16v_mul_scalar_u64
#define gf16v_madd          _gf16v_madd_u64

#define gf256v_add          _gf256v_add_u64
#define gf256v_mul_scalar   _gf256v_mul_scalar_u64
#define gf256v_madd         _gf256v_madd_u64

#define gf256v_conditional_add      _gf256v_conditional_add_u64

#include "blas_comm.h"
#include "blas_matrix.h"

#else


#ifndef _BLAS_UNIT_LEN_
#define _BLAS_UNIT_LEN_ 4
#endif

#include "blas_u32.h"

#define gf16v_mul_scalar        _gf16v_mul_scalar_u32
#define gf16v_madd              _gf16v_madd_u32

#define gf256v_add              _gf256v_add_u32
#define gf256v_mul_scalar       _gf256v_mul_scalar_u32
#define gf256v_madd             _gf256v_madd_u32

#define gf256v_conditional_add  _gf256v_conditional_add_u32

#include "blas_comm.h"
#include "blas_matrix.h"

#endif




#endif // _BLAS_H_

