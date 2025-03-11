// SPDX-License-Identifier: CC0 OR Apache-2.0
#ifndef CONFIG_H_
#define CONFIG_H_

#ifndef NDEBUG
#define NDEBUG
#endif

/*
 *   This file contains all implementation options that do not affect testvectors
 *   params.h contains other options.
 */


//
// This macro set the max steps for conditional row-swaps in the constant time
// Gaussian Elimination algorithm.
// Default value : OFF
//
//#define _GE_CONST_TIME_CADD_EARLY_STOP_
#define _GE_EARLY_STOP_STEPS_GF16_    16
#define _GE_EARLY_STOP_STEPS_GF256_   8


//
// The following macros choose optimizations for basic linear algebra functions.
// It is currently defined from the makefile
//
//#define _BLAS_UINT64_
//#define _BLAS_SSE_
//#define _BLAS_AVX2_
//#define _BLAS_NEON_
//#define _BLAS_M4F_

// GF multiplication with multiplication tables
//#define _MUL_WITH_MULTAB_


//
// choose implementations for SHAKE256, AES128CTR, and randombytes() functions
//
#if !(defined(_UTILS_OPENSSL_)||defined(_UTILS_SUPERCOP_)||defined(_UTILS_PQM4_)||defined(_UTILS_OQS_))
// default:
#define _UTILS_OPENSSL_

//#define _UTILS_SUPERCOP_
//#define _UTILS_PQM4_
#endif  // !(defined(_UTILS_OPENSSL_)||defined(_UTILS_SUPERCOP_)||defined(_UTILS_PQM4_))

//
// Options for AES128CTR
// The following macros will implement AES128CTR with native aes instructions.
//
// x86 aes instrucitons
//#define _UTILS_AESNI_
// arm aes instructions
//#define _UTILS_NEONAES_
// arm neon bitslice aes
//#define _UTILS_NEONBSAES_

#if ! (defined(_UTILS_PQM4_)||defined(_UTILS_AESNI_)||defined(_UTILS_NEONAES_)||defined(_UTILS_NEONBSAES_)||defined(_UTILS_OQS_))

#if defined(_BLAS_AVX2_)
#define _UTILS_AESNI_
#elif defined(_BLAS_NEON_)&&defined(_APPLE_SILICON_)
#define _UTILS_NEONAES_
#elif defined(_BLAS_NEON_)
#define _UTILS_NEONBSAES_
#endif

#endif


#if defined(_UTILS_OQS_)
#include <oqs/common.h>
#if defined(OQS_ENABLE_TEST_CONSTANT_TIME)
#define _VALGRIND_
#endif
#endif

//
// Options for randombytes()
//
//
// The following macro will implement randombytes() with utils/nistkat/rng.[hc]
// It uses AEC256CTR and AES256_ECB from openssl.
//
//#define _NIST_KAT_
//
// The following macro will implement randombytes() with C rand()
// Turn on only when there is no proper implementation
//
//#define _DEBUG_RANDOMBYTES_


#endif
