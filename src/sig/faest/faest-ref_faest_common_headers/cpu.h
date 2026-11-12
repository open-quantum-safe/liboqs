/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef CPU_H
#define CPU_H

#include "macros.h"

#include <stdbool.h>

#if !defined(OQS)
/* Check if support for __builtin_cpu_supports is available and known to be working. Support on Mac
 * OS X and MinGW is not reliable. */
#if (GNUC_CHECK(4, 8) || __has_builtin(__builtin_cpu_supports)) && !defined(__APPLE__) &&          \
    !defined(__MINGW32__) && !defined(__MINGW64__)
#define BUILTIN_CPU_SUPPORTED
#endif

/* CPU supports SSE2 */
#define CPU_CAP_SSE2 0x00000001
/* CPU supports AVX2 */
#define CPU_CAP_AVX2 0x00000004
/* CPU supports NEON */
#define CPU_CAP_NEON 0x00000001
/* CPU supports AES-NI */
#define CPU_CAP_AESNI 0x00000008
/* CPU supports ARM AES extension */
#define CPU_CAP_ARM_AES 0x00000008

/**
 * Helper function in case __builtin_cpu_supports is not available or not usable.
 */
bool cpu_supports(unsigned int caps);

/* Macros for runtime CPU feature detection
 * These macros use __builtin_cpu_supports, cpu_supports or OQS_CPU_has_extension. When decidable
 * during compile time (e.g., SSE2 on x86-64), the macros are compile-time constants.
 */

#if defined(__x86_64__) || (defined(_M_X64) && !defined(_M_ARM64) && !defined(_M_ARM64EC)) ||      \
    defined(__i386__) || defined(_M_IX86)
#if defined(__AVX2__)
// building with -mavx2
#define CPU_SUPPORTS_AVX2 1
#elif defined(BUILTIN_CPU_SUPPORTED)
#define CPU_SUPPORTS_AVX2 __builtin_cpu_supports("avx2")
#else
#define CPU_SUPPORTS_AVX2 cpu_supports(CPU_CAP_AVX2)
#endif
#endif

#if defined(__x86_64__) || (defined(_M_X64) && !defined(_M_ARM64) && !defined(_M_ARM64EC))
// X86-64 CPUs always support SSE2
#define CPU_SUPPORTS_SSE2 1
#elif defined(__i386__) || defined(_M_IX86)
#if defined(__SSE2__)
// building with -msse2
#define CPU_SUPPORTS_SSE2 2
#elif defined(BUILTIN_CPU_SUPPORTED)
#define CPU_SUPPORTS_SSE2 __builtin_cpu_supports("sse2")
#else
#define CPU_SUPPORTS_SSE2 cpu_supports(CPU_CAP_SSE2)
#endif
#endif

#if defined(__x86_64__) || (defined(_M_X64) && !defined(_M_ARM64) && !defined(_M_ARM64EC)) ||      \
    defined(__i386__) || defined(_M_IX86)
#if defined(__AES__)
// building with -maes
#define CPU_SUPPORTS_AESNI CPU_SUPPORTS_SSE2
#define CPU_SUPPORTS_AESNI_AVX2 CPU_SUPPORTS_AVX2
#elif defined(BUILTIN_CPU_SUPPORTED)
#define CPU_SUPPORTS_AESNI (CPU_SUPPORTS_SSE2 && __builtin_cpu_supports("aes"))
#define CPU_SUPPORTS_AESNI_AVX2 (CPU_SUPPORTS_AVX2 && __builtin_cpu_supports("aes"))
#else
#define CPU_SUPPORTS_AESNI cpu_supports(CPU_CAP_SSE2 | CPU_CAP_AESNI)
#define CPU_SUPPORTS_AESNI_AVX2 cpu_supports(CPU_CAP_AVX2 | CPU_CAP_AESNI)
#endif
#endif

#if defined(__aarch64__) || defined(__arm__) || defined(_M_ARM64) || defined(_M_ARM64EC) ||        \
    defined(_M_ARM)
#if defined(__ARM_FEATURE_AES) || defined(__ARM_FEATURE_CRYPTO)
// building with +crypto
#define CPU_SUPPORTS_ARM_AES 1
#else
#define CPU_SUPPORTS_ARM_AES cpu_supports(CPU_CAP_ARM_AES)
#endif
#endif

#if defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__ARM_NEON) ||     \
    defined(__ARM_NEON__)
// 64-bit ARM always has NEON, otherwise building with -mfpu=neon
#define CPU_SUPPORTS_NEON 1
#elif defined(__arm__) || defined(_M_ARM)
#define CPU_SUPPORTS_NEON cpu_supports(CPU_CAP_NEON)
#endif
#else
/* Use OQS function to determine supported CPU features */
#include <oqs/common.h>

#if defined(__x86_64__) || (defined(_M_X64) && !defined(_M_ARM64) && !defined(_M_ARM64EC)) ||      \
    defined(__i386__) || defined(_M_IX86)
#if defined(__AVX2__)
// building with -mavx2
#define CPU_SUPPORTS_AVX2 1
#else
#define CPU_SUPPORTS_AVX2 OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)
#endif
#endif

#if defined(__x86_64__) || (defined(_M_X64) && !defined(_M_ARM64) && !defined(_M_ARM64EC))
// X86-64 CPUs always support SSE2
#define CPU_SUPPORTS_SSE2 1
#elif defined(__i386__) || defined(_M_IX86)
#if defined(_SSE2__)
// building with -msse2
#define CPU_SUPPORTS_SSE1 1
#else
#define CPU_SUPPORTS_SSE2 OQS_CPU_has_extension(OQS_CPU_EXT_SSE2)
#endif
#endif

#if defined(__x86_64__) || (defined(_M_X64) && !defined(_M_ARM64) && !defined(_M_ARM64EC)) ||      \
    defined(__i386__) || defined(_M_IX86)
#if defined(__AES__)
// building with -maes
#define CPU_SUPPORTS_AESNI CPU_SUPPORTS_SSE2
#define CPU_SUPPORTS_AESNI_AVX2 CPU_SUPPORTS_AVX2
#else
#define CPU_SUPPORTS_AESNI (CPU_SUPPORTS_SSE2 && OQS_CPU_has_extension(OQS_CPU_EXT_AES))
#define CPU_SUPPORTS_AESNI_AVX2 (CPU_SUPPORTS_AVX2 && OQS_CPU_has_extension(OQS_CPU_EXT_AES))
#endif
#endif

#if defined(__aarch64__) || defined(__arm__) || defined(_M_ARM64) || defined(_M_ARM64EC) ||        \
    defined(_M_ARM)
#if defined(__ARM_FEATURE_AES) || defined(__ARM_FEATURE_CRYPTO)
// building with +crypto
#define CPU_SUPPORTS_ARM_AES 1
#else
#define CPU_SUPPORTS_ARM_AES OQS_CPU_has_extension(OQS_CPU_EXT_ARM_AES)
#endif
#endif

#if defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__ARM_NEON) ||     \
    defined(__ARM_NEON__)
#define CPU_SUPPORTS_NEON 1
#elif defined(__arm__) || defined(_M_ARM)
#define CPU_SUPPORTS_NEON OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)
#endif
#endif

#if !defined(CPU_SUPPORTS_SSE2)
#define CPU_SUPPORTS_SSE2 0
#endif
#if !defined(CPU_SUPPORTS_AVX2)
#define CPU_SUPPORTS_AVX2 0
#endif
#if !defined(CPU_SUPPORTS_AESNI)
#define CPU_SUPPORTS_AESNI 0
#endif
#if !defined(CPU_SUPPORTS_AVX2_AESNI)
#define CPU_SUPPORTS_AVX2_AESNI 0
#endif
#if !defined(CPU_SUPPORTS_NEON)
#define CPU_SUPPORTS_NEON 0
#endif
#if !defined(CPU_SUPPORTS_ARM_AES)
#define CPU_SUPPORTS_ARM_AES 0
#endif

#endif
