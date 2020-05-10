/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: configuration file
*
* SPDX-License-Identifier: MIT
**************************************************************************************/

#ifndef __QTESLA_CONFIG_H__
#define __QTESLA_CONFIG_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Definition of operating system (removed in OQS)

// Definition of compiler (removed in OQS)

// Definition of the targeted architecture and basic data types

#define TARGET_AMD64 1
#define TARGET_x86 2
#define TARGET_ARM 3
#define TARGET_ARM64 4

#if defined(_AMD64_)
#define TARGET TARGET_AMD64
#define RADIX 64
#define RADIX32 32
typedef uint64_t digit_t; // Unsigned 64-bit digit
typedef int64_t sdigit_t; // Signed 64-bit digit
#elif defined(_X86_)
#define TARGET TARGET_x86
#define RADIX 32
#define RADIX32 32
typedef uint32_t digit_t; // Unsigned 32-bit digit
typedef int32_t sdigit_t; // Signed 32-bit digit
#elif defined(_ARM_)
#define TARGET TARGET_ARM
#define RADIX 32
#define RADIX32 32
typedef uint32_t digit_t; // Unsigned 32-bit digit
typedef int32_t sdigit_t; // Signed 32-bit digit
#elif defined(_ARM64_)
#define TARGET TARGET_ARM64
#define RADIX 64
#define RADIX32 32
typedef uint64_t digit_t; // Unsigned 64-bit digit
typedef int64_t sdigit_t; // Signed 64-bit digit
#else
#error-- "Unsupported ARCHITECTURE"
#endif

#endif
