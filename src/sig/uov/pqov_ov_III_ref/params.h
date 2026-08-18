// SPDX-License-Identifier: CC0 OR Apache-2.0
#ifndef _PARAMS_H_
#define _PARAMS_H_


/*
 *   This parameter file contains all options that affect testvectors
 *   config.h contains implementation options.
 */

// -----------------------------------------------------------------------------

//#define _4ROUND_AES_

#if (!defined(_OV16_160_64))&&(!defined(_OV256_112_44))&&(!defined(_OV256_184_72))&&(!defined(_OV256_244_96))

//#define _OV16_160_64
#define _OV256_112_44
// #define _OV256_184_72
// #define _OV256_244_96
#endif

#if (!defined(_OV_CLASSIC))&&(!defined(_OV_PKC))&&(!defined(_OV_PKC_SKC))
#define _OV_CLASSIC
//#define _OV_PKC
//#define _OV_PKC_SKC
#endif

// -----------------------------------------------------------------------------

#if defined _OV16_160_64
#define _USE_GF16
#define _GFSIZE 16
#define _PUB_N 160
#define _PUB_M 64
#define _HASH_LEN 32
#define PQOV_NAMESPACE_PARAM Is

#elif defined _OV256_112_44
#define _GFSIZE 256
#define _PUB_N 112
#define _PUB_M 44
#define _HASH_LEN 32
#define PQOV_NAMESPACE_PARAM Ip

#elif defined _OV256_184_72
#define _GFSIZE 256
#define _PUB_N 184
#define _PUB_M 72
#define _HASH_LEN 48
#define PQOV_NAMESPACE_PARAM III

#elif defined _OV256_244_96
#define _GFSIZE 256
#define _PUB_N 244
#define _PUB_M 96
#define _HASH_LEN 64
#define PQOV_NAMESPACE_PARAM V
#endif

#define _O (_PUB_M)
#define _V ((_PUB_N)-(_O))


#define STR1(x) #x
#define THE_NAME(gf,n,m) "OV(" STR1(gf) "," STR1(n) "," STR1(m) ")"
#define OV_PARAMNAME THE_NAME(_GFSIZE,_PUB_N,_PUB_M)


/// size of variables, in # bytes.
#ifdef _USE_GF16
// GF16
#define _V_BYTE (_V/2)
#define _O_BYTE (_O/2)
#define _PUB_N_BYTE  (_PUB_N/2)
#define _PUB_M_BYTE  (_PUB_M/2)
#else
// GF256
#define _V_BYTE (_V)
#define _O_BYTE (_O)
#define _PUB_N_BYTE  (_PUB_N)
#define _PUB_M_BYTE  (_PUB_M)
#endif


/// length of seed for public key, in # bytes
#define LEN_PKSEED 16

/// length of seed for secret key, in # bytes
#define LEN_SKSEED 32

/// length of salt for a signature, in # bytes
#define _SALT_BYTE 16

/// length of a signature
#define OV_SIGNATUREBYTES (_PUB_N_BYTE + _SALT_BYTE )

#define N_TRIANGLE_TERMS(n_var) ((n_var)*((n_var)+1)/2)

/// length of components of public key, in # bytes
#define _PK_BYTE       (_PUB_M_BYTE * N_TRIANGLE_TERMS(_PUB_N))
#define _PK_P1_BYTE   (_O_BYTE * N_TRIANGLE_TERMS(_V))
#define _PK_P2_BYTE   (_O_BYTE * _V * _O)
#define _PK_P3_BYTE   (_O_BYTE * N_TRIANGLE_TERMS(_O))


#define OV_SK_UNCOMPRESSED_BYTES (LEN_SKSEED                     + \
                                  _V_BYTE*_O                     + \
                                  _O_BYTE * N_TRIANGLE_TERMS(_V) + \
                                  _O_BYTE * _V*_O)
#define OV_PK_UNCOMPRESSED_BYTES (_PUB_M_BYTE * N_TRIANGLE_TERMS(_PUB_N))
#define OV_SK_COMPRESSED_BYTES   (LEN_SKSEED)
#define OV_PK_COMPRESSED_BYTES   (LEN_PKSEED + _O_BYTE * N_TRIANGLE_TERMS(_O))


#if defined _OV_CLASSIC
#define PQOV_NAMESPACE_VARIANT
#define OV_ALGNAME (OV_PARAMNAME "-classic")
#define OV_SECRETKEYBYTES OV_SK_UNCOMPRESSED_BYTES
#define OV_PUBLICKEYBYTES OV_PK_UNCOMPRESSED_BYTES

#elif defined _OV_PKC
#define PQOV_NAMESPACE_VARIANT _pkc
#define OV_ALGNAME (OV_PARAMNAME "-pkc")
#define OV_SECRETKEYBYTES OV_SK_UNCOMPRESSED_BYTES
#define OV_PUBLICKEYBYTES OV_PK_COMPRESSED_BYTES

#elif defined _OV_PKC_SKC
#define PQOV_NAMESPACE_VARIANT _pkc_skc
#define OV_ALGNAME (OV_PARAMNAME "-pkc-skc")
#define OV_SECRETKEYBYTES OV_SK_COMPRESSED_BYTES
#define OV_PUBLICKEYBYTES OV_PK_COMPRESSED_BYTES
#endif


#if defined(_BLAS_NEON_)
#define PQOV_NAMESPACE_IMPL neon
#elif defined(_BLAS_AVX2_)
#define PQOV_NAMESPACE_IMPL avx2
#else
#define PQOV_NAMESPACE_IMPL ref
#endif

#define PQOV_CONCAT_(x1,x2,x3,x4,x5) x1##_##x2##x3##_##x4##_##x5
#define PQOV_CONCAT(x1,x2,x3,x4,x5) PQOV_CONCAT_(x1,x2,x3,x4,x5)
#define PQOV_NAMESPACE(s) \
  PQOV_CONCAT(pqov_uov,PQOV_NAMESPACE_PARAM,PQOV_NAMESPACE_VARIANT,PQOV_NAMESPACE_IMPL,s)





#endif
