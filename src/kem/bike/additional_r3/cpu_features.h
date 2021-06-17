/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker, Shay Gueron and Dusan Kostic,
 * AWS Cryptographic Algorithms Group.
 */

#pragma once

#include "defs.h"

#include <oqs/common.h>
#include <stdint.h>

#if defined(X86_64)

_INLINE_ uint32_t is_avx2_enabled(void) { return OQS_CPU_has_extension(OQS_CPU_EXT_AVX2); }
_INLINE_ uint32_t is_avx512_enabled(void) { return OQS_CPU_has_extension(OQS_CPU_EXT_AVX512); }
_INLINE_ uint32_t is_pclmul_enabled(void) { return OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ); }
_INLINE_ uint32_t is_vpclmul_enabled(void) { return OQS_CPU_has_extension(OQS_CPU_EXT_VPCLMULQDQ); }

#else

_INLINE_ uint32_t is_avx2_enabled(void) { return 0; }
_INLINE_ uint32_t is_avx512_enabled(void) { return 0; }
_INLINE_ uint32_t is_pclmul_enabled(void) { return 0; }
_INLINE_ uint32_t is_vpclmul_enabled(void) { return 0; }

#endif
