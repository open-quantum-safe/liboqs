/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation".
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__

#include "../../../../config.h"
#include <oqs/oqsconfig.h>

// Map BIKE internal errors to OQS ones
#define OQS_ERR_KEM_BIKE_DECODING_FAILURE OQS_ERROR
#define OQS_ERR_KEM_BIKE_AES_SET_KEY OQS_ERROR
#define OQS_ERR_KEM_BIKE_WEIGHT_IS_NOT_T OQS_ERROR
#define OQS_ERR_KEM_BIKE_AES_CTR_PRF_INIT OQS_ERROR
#define OQS_ERR_KEM_BIKE_AES_OVER_USED OQS_ERROR

////////////////////////////////////////////
//             Basic defs
///////////////////////////////////////////

// For code clarity.
#define IN
#define OUT

#define ALIGN(n) __attribute__((aligned(n)))
#define BIKE_UNUSED(x) (void) (x)
#define BIKE_UNUSED_ATT __attribute__((unused))

#define _INLINE_ static inline

// In asm the symbols '==' and '?' are not allowed therefore if using divide_and_ceil
// in asm files we must ensure with static_assert its validity
#ifdef __ASM_FILE__
#define static_assert(COND, MSG)
#elif defined(__cplusplus)
#define static_assert(COND, MSG) static_assert(COND, "MSG")
#else
#define static_assert(COND, MSG) typedef char static_assertion_##MSG[(COND) ? 1 : -1] BIKE_UNUSED_ATT
#endif

// Divide by the divider and round up to next integer
#define DIVIDE_AND_CEIL(x, divider) ((x + divider) / divider)

// Bit manipations
#ifdef __ASM_FILE__
#define BIT(len) (1 << (len))
#else
#define BIT(len) (1UL << (len))
#endif

#define MASK(len) (BIT(len) - 1)
#define SIZEOF_BITS(b) (sizeof(b) * 8)

#define XMM_SIZE 0x10
#define YMM_SIZE 0x20
#define ZMM_SIZE 0x40

#define ALL_YMM_SIZE (16 * YMM_SIZE)
#define ALL_ZMM_SIZE (32 * ZMM_SIZE)

////////////////////////////////////////////
//             Debug
///////////////////////////////////////////

#ifndef VERBOSE
#define VERBOSE 0
#endif

#ifndef __ASM_FILE__
#if (VERBOSE == 3)
#define MSG(...) \
	{ printf(__VA_ARGS__); }
#define DMSG(...) MSG(__VA_ARGS__)
#define EDMSG(...) MSG(__VA_ARGS__)
#define SEDMSG(...) MSG(__VA_ARGS__)
#elif (VERBOSE == 2)
#define MSG(...) \
	{ printf(__VA_ARGS__); }
#define DMSG(...) MSG(__VA_ARGS__)
#define EDMSG(...) MSG(__VA_ARGS__)
#define SEDMSG(...)
#elif (VERBOSE == 1)
#define MSG(...) \
	{ printf(__VA_ARGS__); }
#define DMSG(...) MSG(__VA_ARGS__)
#define EDMSG(...)
#define SEDMSG(...)
#else
#define MSG(...) \
	{ printf(__VA_ARGS__); }
#define DMSG(...)
#define EDMSG(...)
#define SEDMSG(...)
#endif
#endif

////////////////////////////////////////////
//              Printing
///////////////////////////////////////////

// #define PRINT_IN_BE
// #define NO_SPACE
// #define NO_NEWLINE

#endif // __TYPES_H_INCLUDED__
