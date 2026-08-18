// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file gf16_tabs.h
/// @brief Defining the constant tables for performing GF arithmetics.
///

#ifndef _GF16_TABS_H_
#define _GF16_TABS_H_

#include <stdint.h>
#include "params.h"

//IF_CRYPTO_CORE:define CRYPTO_NAMESPACE

#ifdef  __cplusplus
extern  "C" {
#endif

#define __gf16_inv PQOV_NAMESPACE(__gf16_inv)
extern const unsigned char __gf16_inv[];
#define __gf16_squ PQOV_NAMESPACE(__gf16_squ)
extern const unsigned char __gf16_squ[];
#define __gf16_exp PQOV_NAMESPACE(__gf16_exp)
extern const unsigned char __gf16_exp[];
#define __gf16_log PQOV_NAMESPACE(__gf16_log)
extern const unsigned char __gf16_log[];

#define __gf16_mulbase PQOV_NAMESPACE(__gf16_mulbase)
extern const unsigned char __gf16_mulbase[];
#define __gf16_mul PQOV_NAMESPACE(__gf16_mul)
extern const unsigned char __gf16_mul[];

#define __gf256_squ PQOV_NAMESPACE(__gf256_squ)
extern const unsigned char __gf256_squ[];
#define __gf256_mulbase PQOV_NAMESPACE(__gf256_mulbase)
extern const unsigned char __gf256_mulbase[];
#define __gf256_mul PQOV_NAMESPACE(__gf256_mul)
extern const unsigned char __gf256_mul[];


#ifdef  __cplusplus
}
#endif



#endif // _GF16_TABS_H_
