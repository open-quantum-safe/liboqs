// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file utils_randombytes.h
/// @brief wrappers for randombytes().
///
///

#ifndef _UTILS_RANDOMBYTES_H_
#define _UTILS_RANDOMBYTES_H_


#ifdef  __cplusplus
extern  "C" {
#endif


#include "config.h"
#include "params.h"


#if defined(_UTILS_SUPERCOP_)||defined(_UTILS_PQM4_)

#include "randombytes.h"

#elif defined(_NIST_KAT_) && defined(_UTILS_OPENSSL_)

#include "rng.h"

#else
#define randombytes PQOV_NAMESPACE(randombytes)
void randombytes(unsigned char *x, unsigned long long xlen);

#endif


#ifdef  __cplusplus
}
#endif

#endif // _UTILS_RANDOMBYTES_H_


