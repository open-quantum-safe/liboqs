// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file gf16_tabs.h
/// @brief Defining the constants for performing GF arithmetics.
///

#ifndef _GF16_TABS_NEON_H_
#define _GF16_TABS_NEON_H_

#include <stdint.h>
#include "params.h"

#ifdef  __cplusplus
extern  "C" {
#endif

#define __0_f PQOV_NAMESPACE(__0_f)
extern const unsigned char __0_f[];
#define __gf16_reduce PQOV_NAMESPACE(__gf16_reduce)
extern const unsigned char __gf16_reduce[];

#define __gf16_inv PQOV_NAMESPACE(__gf16_inv)
extern const unsigned char __gf16_inv[];
#define __gf16_squ PQOV_NAMESPACE(__gf16_squ)
extern const unsigned char __gf16_squ[];
#define __gf16_mulbase PQOV_NAMESPACE(__gf16_mulbase)
extern const unsigned char __gf16_mulbase[];
#define __gf16_mul PQOV_NAMESPACE(__gf16_mul)
extern const unsigned char __gf16_mul[];

#define __gf256_bit8_11_reduce PQOV_NAMESPACE(__gf256_bit8_11_reduce)
extern const unsigned char __gf256_bit8_11_reduce[];
#define __gf256_bit12_15_reduce PQOV_NAMESPACE(__gf256_bit12_15_reduce)
extern const unsigned char __gf256_bit12_15_reduce[];
#define __gf256_squ PQOV_NAMESPACE(__gf256_squ)
extern const unsigned char __gf256_squ[];
#define __gf256_mulbase PQOV_NAMESPACE(__gf256_mulbase)
extern const unsigned char __gf256_mulbase[];
#define __gf256_mul PQOV_NAMESPACE(__gf256_mul)
extern const unsigned char __gf256_mul[];


#ifdef  __cplusplus
}
#endif



#endif // _GF16_TABS_NEON_H_
