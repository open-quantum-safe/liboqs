#ifndef _BLAS_H_
#define _BLAS_H_
/// @file blas.h
/// @brief Defining the implementations for linear algebra functions depending on the machine architecture.
///

#include "blas_comm.h"
#include "blas_u32.h"
#include "rainbow_config.h"

#define gf256v_predicated_add PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf256v_predicated_add_u32
#define gf256v_add PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf256v_add_u32


#define gf16v_mul_scalar PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_mul_scalar_u32
#define gf16v_madd PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_madd_u32
#define gf16v_dot PQCLEAN_RAINBOWIACYCLIC_CLEAN_gf16v_dot_u32


#endif // _BLAS_H_

