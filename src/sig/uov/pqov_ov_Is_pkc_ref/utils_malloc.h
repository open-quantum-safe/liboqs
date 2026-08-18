// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file utils_malloc.h
/// @brief the interface for adapting malloc functions.
///
///

#ifndef _UTILS_MALLOC_H_
#define _UTILS_MALLOC_H_

#include <oqs/common.h>
#define ov_malloc OQS_MEM_malloc
#define ov_free OQS_MEM_secure_free

#if defined(__GNUC__) || defined(__clang__)
#define PQOV_ALIGN  __attribute__((aligned(32)))
#elif defined(_MSC_VER)
#define PQOV_ALIGN  __declspec(align(32))
#else
#define PQOV_ALIGN
#endif

#endif // _UTILS_MALLOC_H_


