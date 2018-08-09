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

#include <oqs/config.h>

////////////////////////////////////////////
//             Basic defs
///////////////////////////////////////////

//Divide by the divider and round up to next integer.
//In asm the symbols '==' and '?' are not allowed therefore if using divide_and_ceil
//in asm files we must ensure with static_assert its validity.
#ifdef __ASM_FILE__
#define DIVIDE_AND_CEIL(x, divider) ((x / divider) + 1)
#define static_assert(COND, MSG)
#elif __cplusplus
#define DIVIDE_AND_CEIL(x, divider) ((x / divider) + (x % divider == 0 ? 0 : 1))
#define static_assert(COND, MSG) static_assert(COND, "MSG")
#else
#define DIVIDE_AND_CEIL(x, divider) ((x / divider) + (x % divider == 0 ? 0 : 1))
#define static_assert(COND, MSG) typedef char static_assertion_##MSG[(COND) ? 1 : -1]
#endif

#define BIKE_UNUSED(x) (void) (x)

#define ALIGN(n) __attribute__((aligned(n)))

//For clarity of the code.
#define IN
#define OUT

//Bit manipations
#ifdef __ASM_FILE__
#define BIT(len) (1 << (len))
#else
#define BIT(len) (1UL << (len))
#endif

#define MASK(len) (BIT(len) - 1)
#define SIZEOF_BITS(b) (sizeof(b) * 8)

#define _INLINE_ static inline

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

//Show timer rests in cycles.
//#define RDTSC

//#define PRINT_IN_BE
//#define NO_SPACE
//#define NO_NEWLINE

////////////////////////////////////////////
//              Testing
///////////////////////////////////////////
#ifndef NUM_OF_TESTS
#define NUM_OF_TESTS 1
#endif

//Disabled for random testing
//#define USE_NIST_RAND

#endif //__TYPES_H_INCLUDED__
