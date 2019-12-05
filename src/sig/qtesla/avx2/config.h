/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: configuration file
**************************************************************************************/

#ifndef __QTESLA_CONFIG_H__
#define __QTESLA_CONFIG_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Definition of operating system

#define OS_LINUX 1
#define OS_TARGET OS_LINUX

// Definition of compiler (removed in OQS)

// Definition of the targeted architecture and basic data types

#define TARGET_AMD64 1

#if defined(_AMD64_)
#define TARGET TARGET_AMD64
#define RADIX 64
#define RADIX32 32
typedef uint64_t digit_t; // Unsigned 64-bit digit
typedef int64_t sdigit_t; // Signed 64-bit digit
#else
#error-- "Unsupported ARCHITECTURE"
#endif

// Instruction support

#if defined(WITH_AVX2)
#define USE_AVX2 // AVX2 support selection
#else
#error-- "This implementation uses AVX2 instructions"
#endif

#endif
