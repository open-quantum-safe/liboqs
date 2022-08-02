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

#if !defined(HAVE_USEFUL_ATTR_ALIGNED)
#error "SIMD support requires that the compiler supports some method to specify alignment."
#endif

#if defined(_MSC_VER)
#include <immintrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#elif defined(__GNUC__) && defined(__ARM_NEON)
#include <arm_neon.h>
#endif

#include "cpu.h"

#if defined(__x86_64__) || defined(_M_X64)
#if defined(WITH_SSE2) || defined(WITH_AVX2)
#define NO_UINT64_FALLBACK
#endif
#endif

#if defined(__aarch64__)
#if defined(WITH_NEON)
#define NO_UINT64_FALLBACK
#endif
#endif

#if defined(_MSC_VER)
#define restrict __restrict
#endif

#if defined(WITH_SSE2)
#define ATTR_TARGET_S128 ATTR_TARGET_SSE2
#else
#define ATTR_TARGET_S128
#endif

#if defined(WITH_AVX2)
#define ATTR_TARGET_S256 ATTR_TARGET_AVX2
#else
#define ATTR_TARGET_S256
#endif

#define FN_ATTRIBUTES_S256 ATTR_ARTIFICIAL ATTR_ALWAYS_INLINE ATTR_TARGET_S256
#define FN_ATTRIBUTES_S128 ATTR_ARTIFICIAL ATTR_ALWAYS_INLINE ATTR_TARGET_S128

#define FN_ATTRIBUTES_S256_PURE FN_ATTRIBUTES_S256 ATTR_PURE
#define FN_ATTRIBUTES_S128_PURE FN_ATTRIBUTES_S128 ATTR_PURE

#define FN_ATTRIBUTES_S256_CONST FN_ATTRIBUTES_S256 ATTR_CONST
#define FN_ATTRIBUTES_S128_CONST FN_ATTRIBUTES_S128 ATTR_CONST

#if defined(WITH_SSE2)
/* backwards compatibility macros for GCC 4.8 and 4.9
 *
 * bs{l,r}i was introduced in GCC 5 and in clang as macros sometime in 2015.
 * */
#if (!defined(__clang__) && defined(__GNUC__) && __GNUC__ < 5) ||                                  \
    ((defined(__clang__) || defined(_MSC_VER)) && !defined(_mm_bslli_si128))
#define _mm_bslli_si128(a, imm) _mm_slli_si128((a), (imm))
#define _mm_bsrli_si128(a, imm) _mm_srli_si128((a), (imm))
#endif
#endif

#if defined(WITH_AVX2)
typedef __m256i word256;

#if defined(__GNUC__) || defined(__clang__)
#define _mm256_set_m128i(v0, v1) _mm256_insertf128_si256(_mm256_castsi128_si256(v1), (v0), 1)
#define _mm256_setr_m128i(v0, v1) _mm256_set_m128i((v1), (v0))
#endif

#define mm256_zero _mm256_setzero_si256()
#define mm256_load(s) _mm256_load_si256((const word256*)s)
#define mm256_set_4(v0, v1, v2, v3) _mm256_set_epi64x(v3, v2, v1, v0)
#define mm256_store(d, s) _mm256_store_si256((word256*)d, s)
#define mm256_xor(l, r) _mm256_xor_si256((l), (r))
#define mm256_and(l, r) _mm256_and_si256((l), (r))
#define mm256_xor_mask(lhs, rhs, mask) mm256_xor((lhs), mm256_and((rhs), (mask)))

#define mm256_shift_left(data, count)                                                              \
  _mm256_or_si256(_mm256_slli_epi64(data, count),                                                  \
                  _mm256_blend_epi32(mm256_zero,                                                   \
                                     _mm256_permute4x64_epi64(_mm256_srli_epi64(data, 64 - count), \
                                                              _MM_SHUFFLE(2, 1, 0, 0)),            \
                                     _MM_SHUFFLE(3, 3, 3, 0)))

#define mm256_shift_right(data, count)                                                             \
  _mm256_or_si256(_mm256_srli_epi64(data, count),                                                  \
                  _mm256_blend_epi32(mm256_zero,                                                   \
                                     _mm256_permute4x64_epi64(_mm256_slli_epi64(data, 64 - count), \
                                                              _MM_SHUFFLE(0, 3, 2, 1)),            \
                                     _MM_SHUFFLE(0, 3, 3, 3)))

#define mm256_rotate_left(data, count)                                                             \
  _mm256_or_si256(                                                                                 \
      _mm256_slli_epi64(data, count),                                                              \
      _mm256_permute4x64_epi64(_mm256_srli_epi64(data, 64 - count), _MM_SHUFFLE(2, 1, 0, 3)))

#define mm256_rotate_right(data, count)                                                            \
  _mm256_or_si256(                                                                                 \
      _mm256_srli_epi64(data, count),                                                              \
      _mm256_permute4x64_epi64(_mm256_slli_epi64(data, 64 - count), _MM_SHUFFLE(0, 3, 2, 1)))
#endif

#if defined(WITH_SSE2) || defined(WITH_AVX2)
typedef __m128i word128;

#define mm128_zero _mm_setzero_si128()
#define mm128_load(s) _mm_load_si128((const word128*)(s))
#define mm128_loadu(s) _mm_loadu_si128((const void*)(s))
#define mm128_store(d, s) _mm_store_si128((word128*)d, (s))
#define mm128_xor(l, r) _mm_xor_si128((l), (r))
#define mm128_and(l, r) _mm_and_si128((l), (r))
#define mm128_broadcast_u64(x) _mm_set1_epi64x((x))
/* bit shifts up to 63 bits */
#define mm128_sl_u64(x, s) _mm_slli_epi64((x), (s))
#define mm128_sr_u64(x, s) _mm_srli_epi64((x), (s))

#define mm128_shift_left(data, count)                                                              \
  _mm_or_si128(_mm_slli_epi64(data, count), _mm_srli_epi64(_mm_bslli_si128(data, 8), 64 - count))

#define mm128_shift_right(data, count)                                                             \
  _mm_or_si128(_mm_srli_epi64(data, count), _mm_slli_epi64(_mm_bsrli_si128(data, 8), 64 - count))

#define mm128_rotate_left(data, count)                                                             \
  _mm_or_si128(_mm_slli_epi64(data, count),                                                        \
               _mm_shuffle_epi32(_mm_srli_epi64(data, 64 - count), _MM_SHUFFLE(1, 0, 3, 2)))

#define mm128_rotate_right(data, count)                                                            \
  _mm_or_si128(_mm_srli_epi64(data, count),                                                        \
               _mm_shuffle_epi32(_mm_slli_epi64(data, 64 - count), _MM_SHUFFLE(1, 0, 3, 2)))

#define mm128_shift_right_256(res, data, count)                                                    \
  do {                                                                                             \
    const __m128i total_carry = _mm_slli_epi64(_mm_bslli_si128(data[1], 8), 64 - count);           \
    __m128i carry             = _mm_slli_epi64(_mm_bsrli_si128(data[0], 8), 64 - count);           \
    res[0]                    = _mm_or_si128(_mm_srli_epi64(data[0], count), carry);               \
    carry                     = _mm_slli_epi64(_mm_bsrli_si128(data[1], 8), 64 - count);           \
    res[1]                    = _mm_or_si128(_mm_srli_epi64(data[1], count), carry);               \
    res[0]                    = _mm_or_si128(res[0], total_carry);                                 \
  } while (0)

#define mm128_shift_left_256(res, data, count)                                                     \
  do {                                                                                             \
    const __m128i total_carry = _mm_srli_epi64(_mm_bsrli_si128(data[0], 8), 64 - count);           \
    __m128i carry             = _mm_srli_epi64(_mm_bslli_si128(data[0], 8), 64 - count);           \
    res[0]                    = _mm_or_si128(_mm_slli_epi64(data[0], count), carry);               \
    carry                     = _mm_srli_epi64(_mm_bslli_si128(data[1], 8), 64 - count);           \
    res[1]                    = _mm_or_si128(_mm_slli_epi64(data[1], count), carry);               \
    res[1]                    = _mm_or_si128(res[1], total_carry);                                 \
  } while (0)

/* shift left by 64 to 127 bits */
#define mm128_shift_left_64_127(data, count) _mm_slli_epi64(_mm_bslli_si128(data, 8), count - 64)
/* shift right by 64 to 127 bits */
#define mm128_shift_right_64_127(data, count) _mm_srli_epi64(_mm_bsrli_si128(data, 8), count - 64)

#define mm128_rotate_left_256(res, data, count)                                                    \
  do {                                                                                             \
    const __m128i carry = mm128_shift_right_64_127(data[0], 128 - count);                          \
                                                                                                   \
    res[0] = _mm_or_si128(mm128_shift_left(data[0], count),                                        \
                          mm128_shift_right_64_127(data[1], 128 - count));                         \
    res[1] = _mm_or_si128(mm128_shift_left(data[1], count), carry);                                \
  } while (0)

#define mm128_rotate_right_256(res, data, count)                                                   \
  do {                                                                                             \
    const __m128i carry = mm128_shift_left_64_127(data[0], 128 - count);                           \
                                                                                                   \
    res[0] = _mm_or_si128(mm128_shift_right(data[0], count),                                       \
                          mm128_shift_left_64_127(data[1], 128 - count));                          \
    res[1] = _mm_or_si128(mm128_shift_right(data[1], count), carry);                               \
  } while (0)
#endif

#if defined(WITH_NEON)
typedef uint64x2_t word128;

#define mm128_zero vmovq_n_u64(0)
#define mm128_load(s) vld1q_u64(s)
#define mm128_store(d, s) vst1q_u64(d, s)
#define mm128_xor(l, r) veorq_u64((l), (r))
#define mm128_and(l, r) vandq_u64((l), (r))
#define mm128_broadcast_u64(x) vdupq_n_u64((x))
/* bit shifts up to 63 bits */
#define mm128_sl_u64(x, s) vshlq_n_u64((x), (s))
#define mm128_sr_u64(x, s) vshrq_n_u64((x), (s))

/* shift left by 64 to 127 bits */
#define mm128_shift_left_64_127(data, count)                                                       \
  mm128_sl_u64(vextq_u64(mm128_zero, data, 1), count - 64)
/* shift right by 64 to 127 bits */
#define mm128_shift_right_64_127(data, count)                                                      \
  mm128_sr_u64(vextq_u64(data, mm128_zero, 1), count - 64)

#define mm128_shift_left(data, count)                                                              \
  vorrq_u64(mm128_sl_u64(data, count), mm128_sr_u64(vextq_u64(mm128_zero, data, 1), 64 - count))

#define mm128_shift_right(data, count)                                                             \
  vorrq_u64(mm128_sr_u64(data, count), mm128_sl_u64(vextq_u64(data, mm128_zero, 1), 64 - count))

#define mm128_rotate_left(data, count)                                                             \
  vorrq_u64(mm128_shift_left(data, count), mm128_shift_right_64_127(data, 128 - count))

#define mm128_rotate_right(data, count)                                                            \
  vorrq_u64(mm128_shift_right(data, count), mm128_shift_left_64_127(data, 128 - count))

#define mm128_shift_left_256(res, data, count)                                                     \
  do {                                                                                             \
    res[1] = vorrq_u64(mm128_shift_left(data[1], count),                                           \
                       mm128_shift_right_64_127(data[0], 128 - count));                            \
    res[0] = mm128_shift_left(data[0], count);                                                     \
  } while (0)

#define mm128_shift_right_256(res, data, count)                                                    \
  do {                                                                                             \
    res[0] = vorrq_u64(mm128_shift_right(data[0], count),                                          \
                       mm128_shift_left_64_127(data[1], 128 - count));                             \
    res[1] = mm128_shift_right(data[1], count);                                                    \
  } while (0)

#define mm128_rotate_left_256(res, data, count)                                                    \
  do {                                                                                             \
    const word128 carry = mm128_shift_right_64_127(data[1], 128 - count);                          \
                                                                                                   \
    res[1] = vorrq_u64(mm128_shift_left(data[1], count),                                           \
                       mm128_shift_right_64_127(data[0], 128 - count));                            \
    res[0] = vorrq_u64(mm128_shift_left(data[0], count), carry);                                   \
  } while (0)

#define mm128_rotate_right_256(res, data, count)                                                   \
  do {                                                                                             \
    const word128 carry = mm128_shift_left_64_127(data[0], 128 - count);                           \
                                                                                                   \
    res[0] = vorrq_u64(mm128_shift_right(data[0], count),                                          \
                       mm128_shift_left_64_127(data[1], 128 - count));                             \
    res[1] = vorrq_u64(mm128_shift_right(data[1], count), carry);                                  \
  } while (0)
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
FN_ATTRIBUTES_S128_CONST
static inline word128 mm128_xor_mask(const word128 lhs, const word128 rhs, const word128 mask) {
  return mm128_xor(lhs, mm128_and(rhs, mask));
}

FN_ATTRIBUTES_S128
static inline void mm128_xor_region(word128* restrict dst, const word128* restrict src,
                                    unsigned int count) {
  for (unsigned int i = 0; i != count; ++i) {
    dst[i] = mm128_xor(dst[i], src[i]);
  }
}

FN_ATTRIBUTES_S128
static inline void mm128_xor_mask_region(word128* restrict dst, const uint64_t* restrict src,
                                         const word128 mask, unsigned int count) {
  for (unsigned int i = 0; i != count; ++i) {
    dst[i] = mm128_xor(dst[i],
                       mm128_and(mm128_load(&src[i * sizeof(word128) / sizeof(uint64_t)]), mask));
  }
}

FN_ATTRIBUTES_S128
static inline void mm128_xor_256(word128 dst[2], const word128 lhs[2], const word128 rhs[2]) {
  dst[0] = mm128_xor(lhs[0], rhs[0]);
  dst[1] = mm128_xor(lhs[1], rhs[1]);
}

FN_ATTRIBUTES_S128
static inline void mm128_and_256(word128 dst[2], const word128 lhs[2], const word128 rhs[2]) {
  dst[0] = mm128_and(lhs[0], rhs[0]);
  dst[1] = mm128_and(lhs[1], rhs[1]);
}
#endif

#if defined(_MSC_VER)
#undef restrict
#endif

#undef BUILTIN_CPU_SUPPORTED

#endif
