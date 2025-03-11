// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file gf256_tabs.h
/// @brief Defining the constant tables for performing GF arithmetics.
///

#ifndef _GF256_TABS_H_
#define _GF256_TABS_H_

#include <stdint.h>
#include "params.h"

//IF_CRYPTO_CORE:define CRYPTO_NAMESPACE

#ifdef  __cplusplus
extern  "C" {
#endif
#define __gf256_mulbase_avx PQOV_NAMESPACE(__gf256_mulbase_avx)
extern const unsigned char __gf256_mulbase_avx[];



#ifdef  __cplusplus
}
#endif



#endif // _GF16_TABS_H_
