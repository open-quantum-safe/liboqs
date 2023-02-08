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

_INLINE_ uint32_t is_avx2_enabled(void)
{
#if defined(OQS_DIST_X86_64_BUILD)
	return OQS_CPU_has_extension(OQS_CPU_EXT_AVX2);
#elif defined(OQS_USE_AVX2_INSTRUCTIONS)
	return 1;
#else
	return 0;
#endif
}

_INLINE_ uint32_t is_avx512_enabled(void)
{
// TODO: Determine minimal set of AVX512 features
#if defined(OQS_DIST_X86_64_BUILD)
	return OQS_CPU_has_extension(OQS_CPU_EXT_AVX512);
#elif defined(OQS_USE_AVX512_INSTRUCTIONS)
	return 1;
#else
	return 0;
#endif
}

_INLINE_ uint32_t is_pclmul_enabled(void)
{
#if defined(OQS_DIST_X86_64_BUILD)
	return OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ);
#elif defined(OQS_USE_PCLMULQDQ_INSTRUCTIONS)
	return 1;
#else
	return 0;
#endif
}

_INLINE_ uint32_t is_vpclmul_enabled(void) {
#if defined(OQS_DIST_X86_64_BUILD)
	return OQS_CPU_has_extension(OQS_CPU_EXT_VPCLMULQDQ);
#elif defined(OQS_USE_VPCLMULQDQ_INSTRUCTIONS)
	return 1;
#else
	return 0;
#endif
}
