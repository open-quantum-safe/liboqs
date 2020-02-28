/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef SIMD_H
#define SIMD_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "macros.h"

#if defined(_MSC_VER)
#include <immintrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#elif defined(__GNUC__) && defined(__ARM_NEON)
#include <arm_neon.h>
#endif

#if defined(__GNUC__) && !(defined(__APPLE__) && (__clang_major__ <= 8)) &&                        \
    !defined(__MINGW32__) && !defined(__MINGW64__)
#define BUILTIN_CPU_SUPPORTED
#endif

#include "cpu.h"

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#if defined(BUILTIN_CPU_SUPPORTED)
#define CPU_SUPPORTS_AVX2 (__builtin_cpu_supports("avx2") && __builtin_cpu_supports("bmi2"))
#define CPU_SUPPORTS_POPCNT __builtin_cpu_supports("popcnt")
#else
#define CPU_SUPPORTS_AVX2 cpu_supports(CPU_CAP_AVX2)
#define CPU_SUPPORTS_POPCNT cpu_supports(CPU_CAP_POPCNT)
#endif
#endif

#if defined(__x86_64__) || defined(_M_X64)
// X86-64 CPUs always support SSE2
#define CPU_SUPPORTS_SSE2 1
#elif defined(__i386__) || defined(_M_IX86)
#if defined(BUILTIN_CPU_SUPPORTED)
#define CPU_SUPPORTS_SSE2 __builtin_cpu_supports("sse2")
#else
#define CPU_SUPPORTS_SSE2 cpu_supports(CPU_CAP_SSE2)
#endif
#else
#define CPU_SUPPORTS_SSE2 0
#endif

#if defined(__aarch64__)
#define CPU_SUPPORTS_NEON 1
#elif defined(__arm__)
#define CPU_SUPPRTS_NEON cpu_supports(CPU_CAP_NEON)
#else
#define CPU_SUPPORTS_NEON 0
#endif

#if defined(_MSC_VER)
#define restrict __restrict
#endif

#define apply_region(name, type, xor, attributes)                                                  \
  static inline void attributes name(type* restrict dst, type const* restrict src,                 \
                                     unsigned int count) {                                         \
    for (unsigned int i = count; i; --i, ++dst, ++src) {                                           \
      *dst = xor(*dst, *src);                                                                      \
    }                                                                                              \
  }

#define apply_mask(name, type, xor, and, attributes)                                               \
  static inline type attributes name(const type lhs, const type rhs, const type mask) {            \
    return xor(lhs, and(rhs, mask));                                                               \
  }

#define apply_mask_region(name, type, xor, and, attributes)                                        \
  static inline void attributes name(type* restrict dst, type const* restrict src,                 \
                                     type const mask, unsigned int count) {                        \
    for (unsigned int i = count; i; --i, ++dst, ++src) {                                           \
      *dst = xor(*dst, and(*src, mask));                                                           \
    }                                                                                              \
  }

#define apply_array(name, type, xor, count, attributes)                                            \
  static inline void attributes name(type dst[count], type const lhs[count],                       \
                                     type const rhs[count]) {                                      \
    type* d       = dst;                                                                           \
    const type* l = lhs;                                                                           \
    const type* r = rhs;                                                                           \
    for (unsigned int i = count; i; --i, ++d, ++l, ++r) {                                          \
      *d = xor(*l, *r);                                                                            \
    }                                                                                              \
  }

#if defined(WITH_AVX2)
typedef __m256i word256;

#if defined(__GNUC__) || defined(__clang__)
#define _mm256_set_m128i(v0, v1) _mm256_insertf128_si256(_mm256_castsi128_si256(v1), (v0), 1)
#define _mm256_setr_m128i(v0, v1) _mm256_set_m128i((v1), (v0))
#endif

#define mm256_zero _mm256_setzero_si256()
#define mm256_xor(l, r) _mm256_xor_si256(l, r)
#define mm256_and(l, r) _mm256_and_si256(l, r)

apply_region(mm256_xor_region, word256, mm256_xor, FN_ATTRIBUTES_AVX2)
apply_mask_region(mm256_xor_mask_region, word256, mm256_xor, mm256_and, FN_ATTRIBUTES_AVX2)
apply_mask(mm256_xor_mask, word256, mm256_xor, mm256_and, FN_ATTRIBUTES_AVX2_CONST)
#endif

#if defined(WITH_SSE2)
typedef __m128i word128;

#define mm128_zero _mm_setzero_si128()
#define mm128_xor(l, r) _mm_xor_si128(l, r)
#define mm128_and(l, r) _mm_and_si128(l, r)

apply_region(mm128_xor_region, word128, mm128_xor, FN_ATTRIBUTES_SSE2)
apply_mask_region(mm128_xor_mask_region, word128, mm128_xor, mm128_and, FN_ATTRIBUTES_SSE2)
apply_mask(mm128_xor_mask, word128, mm128_xor, mm128_and, FN_ATTRIBUTES_SSE2_CONST)
apply_array(mm256_xor_sse, word128, mm128_xor, 2, FN_ATTRIBUTES_SSE2)
apply_array(mm256_and_sse, word128, mm128_and, 2, FN_ATTRIBUTES_SSE2)
#endif

#if defined(WITH_NEON)
typedef uint64x2_t word128;

#define mm128_zero vmovq_n_u64(0)
#define mm128_xor(l, r) veorq_u64(l, r)
#define mm128_and(l, r) vandq_u64(l, r)

apply_region(mm128_xor_region, word128, mm128_xor, FN_ATTRIBUTES_NEON);
apply_mask_region(mm128_xor_mask_region, word128, mm128_xor, mm128_and, FN_ATTRIBUTES_NEON);
apply_mask(mm128_xor_mask, word128, mm128_xor, mm128_and, FN_ATTRIBUTES_NEON_CONST);
apply_array(mm256_xor, word128, mm128_xor, 2, FN_ATTRIBUTES_NEON);
apply_array(mm256_and, word128, mm128_and, 2, FN_ATTRIBUTES_NEON);
#endif

#if defined(_MSC_VER)
#undef restrict
#endif

#undef apply_region
#undef apply_mask_region
#undef apply_array
#undef BUILTIN_CPU_SUPPORTED

#endif
