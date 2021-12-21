/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef PICNIC_MACROS_H
#define PICNIC_MACROS_H

#include "oqs_picnic_macros.h"

/* __FUNCTION__ generates a warning on Linux with -Wpedantic and newer versions
 * of GCC (tested with 5.4).  So we use __func__ in all source and define it on
 * Windows.
 */
#if defined(__WINDOWS__)
#define __func__ __FUNCTION__
#endif

/* compatibility with clang and other compilers */
#if !defined(__has_attribute)
#define __has_attribute(a) 0
#endif

#if !defined(__has_builtin)
#define __has_builtin(b) 0
#endif

/* gcc version check macro */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define GNUC_CHECK(maj, min)                                                                       \
  (((__GNUC__ << 20) + (__GNUC_MINOR__ << 10)) >= (((maj) << 20) + ((min) << 10)))
#else
#define GNUC_CHECK(maj, min) 0
#endif

/* glibc version check macro */
#if defined(__GLIBC__)
#define GLIBC_CHECK(maj, min) __GLIBC_PREREQ(maj, min)
#else
#define GLIBC_CHECK(maj, min) 0
#endif

/* FreeBSD version check macro */
#if defined(__FreeBSD__)
#define FREEBSD_CHECK(maj, min) (__FreeBSD__ >= (maj))
#else
#define FREEBSD_CHECK(maj, min) 0
#endif

/* NetBSD version check macro */
#if defined(__NetBSD__)
#include <sys/param.h>
#define NETBSD_CHECK(maj, min) (__NetBSD_Version__ >= ((maj)*1000000000 + (min)*10000000))
#else
#define NETBSD_CHECK(maj, min) 0
#endif

/* Apple version check macro */
#if defined(__APPLE__)
#include <Availability.h>
#define MACOSX_CHECK(maj, min, rev)                                                                \
  (__MAC_OS_X_VERSION_MIN_REQUIRED >= ((maj)*10000 + (min)*100 + (rev)))
#else
#define MACOSX_CHECK(maj, min, rev) 0
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/* assume */
#if GNUC_CHECK(4, 5) || __has_builtin(__builtin_unreachable)
#define ASSUME(p)                                                                                  \
  if (!(p))                                                                                        \
  __builtin_unreachable()
#elif defined(_MSC_VER)
#define ASSUME(p) __assume(p)
#else
#define ASSUME(p) (void)(p)
#endif

/* nonnull attribute */
#if GNUC_CHECK(3, 3) || __has_attribute(nonnull)
#define ATTR_NONNULL __attribute__((nonnull))
#define ATTR_NONNULL_ARG(i) __attribute__((nonnull(i)))
#else
#define ATTR_NONNULL
#define ATTR_NONNULL_ARG(i)
#endif

/* destructor attribute */
#if GNUC_CHECK(2, 7) || __has_attribute(destructor)
#define ATTR_DTOR __attribute__((destructor))
#else
#define ATTR_DTOR
#endif

/* assumed aligned attribute */
#if GNUC_CHECK(4, 9) || __has_attribute(assume_aligned)
#define ATTR_ASSUME_ALIGNED(i) __attribute__((assume_aligned(i)))
#else
#define ATTR_ASSUME_ALIGNED(i)
#endif

/* aligned attribute */
/* note that C11's alignas will only do the job once DR 444 is implemented */
#if GNUC_CHECK(4, 9) || __has_attribute(aligned)
#define ATTR_ALIGNED(i) __attribute__((aligned((i))))
#define HAVE_USEFUL_ATTR_ALIGNED
/* #elif defined(_MSC_VER)
#define ATTR_ALIGNED(i) __declspec(align((i)))
#define HAVE_USEFUL_ATTR_ALIGNED */
#else
#define ATTR_ALIGNED(i)
#endif

/* round size to meet alignment requirements */
#define ALIGNT(s, t) (((s) + sizeof(t) - 1) & ~(sizeof(t) - 1))
#define ALIGNU64T(s) ALIGNT(s, uint64_t)

/* unreachable builtin */
#if GNUC_CHECK(4, 5) || __has_builtin(__builtin_unreachable)
#define UNREACHABLE __builtin_unreachable()
#define HAVE_USEFUL_UNREACHABLE
/* #elif defined(_MSC_VER)
#define UNREACHABLE __assume(0) */
#else
#define UNREACHABLE
#endif

/* assume aligned builtin */
#if GNUC_CHECK(4, 9) || __has_builtin(__builtin_assume_aligned)
#define ASSUME_ALIGNED(p, a) __builtin_assume_aligned((p), (a))
#elif defined(HAVE_USEFUL_UNREACHABLE) && defined(HAVE_USEFUL_ATTR_ALIGNED)
#define ASSUME_ALIGNED(p, a) (((((uintptr_t)(p)) % (a)) == 0) ? (p) : (UNREACHABLE, (p)))
#else
#define ASSUME_ALIGNED(p, a) (p)
#endif

/* always inline attribute */
#if GNUC_CHECK(4, 0) || __has_attribute(always_inline)
#define ATTR_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define ATTR_ALWAYS_INLINE __forceinline
#else
#define ATTR_ALWAYS_INLINE
#endif

/* pure attribute */
#if defined(__GNUC__) || __has_attribute(pure)
#define ATTR_PURE __attribute__((pure))
#else
#define ATTR_PURE
#endif

/* const attribute */
#if defined(__GNUC__) || __has_attribute(const)
#define ATTR_CONST __attribute__((const))
#else
#define ATTR_CONST
#endif

/* target attribute */
#if defined(__GNUC__) || __has_attribute(target)
#define ATTR_TARGET(x) __attribute__((target((x))))
#define ATTR_TARGET_AVX2 __attribute__((target("avx2,bmi2")))
#define ATTR_TARGET_SSE2 __attribute__((target("sse2")))
#else
#define ATTR_TARGET(x)
#define ATTR_TARGET_AVX2
#define ATTR_TARGET_SSE2
#endif

/* artificial attribute */
#if GNUC_CHECK(4, 7) || __has_attribute(__artificial__)
#define ATTR_ARTIFICIAL __attribute__((__artificial__))
#else
#define ATTR_ARTIFICIAL
#endif

#define FN_ATTRIBUTES_AVX2 ATTR_ARTIFICIAL ATTR_ALWAYS_INLINE ATTR_TARGET_AVX2
#define FN_ATTRIBUTES_SSE2 ATTR_ARTIFICIAL ATTR_ALWAYS_INLINE ATTR_TARGET_SSE2
#define FN_ATTRIBUTES_NEON ATTR_ARTIFICIAL ATTR_ALWAYS_INLINE

#define FN_ATTRIBUTES_AVX2_PURE FN_ATTRIBUTES_AVX2 ATTR_PURE
#define FN_ATTRIBUTES_SSE2_PURE FN_ATTRIBUTES_SSE2 ATTR_PURE
#define FN_ATTRIBUTES_NEON_PURE FN_ATTRIBUTES_NEON ATTR_PURE

#define FN_ATTRIBUTES_AVX2_CONST FN_ATTRIBUTES_AVX2 ATTR_CONST
#define FN_ATTRIBUTES_SSE2_CONST FN_ATTRIBUTES_SSE2 ATTR_CONST
#define FN_ATTRIBUTES_NEON_CONST FN_ATTRIBUTES_NEON ATTR_CONST

/* concatenation */
#define CONCAT2(a, b) a##_##b
#define CONCAT(a, b) CONCAT2(a, b)

/* helper macros/functions for checked integer subtraction */
#if GNUC_CHECK(5, 0) || __has_builtin(__builtin_add_overflow)
#define sub_overflow_size_t(x, y, diff) __builtin_sub_overflow(x, y, diff)
#else
#include <stdbool.h>
#include <stddef.h>

ATTR_ARTIFICIAL
static inline bool sub_overflow_size_t(const size_t x, const size_t y, size_t* diff) {
  *diff = x - y;
  return x < y;
}
#endif

#include <stdint.h>

/* helper functions for parity computations */
#if GNUC_CHECK(4, 9) || __has_builtin(__builtin_parity)
ATTR_CONST ATTR_ARTIFICIAL static inline uint64_t parity64_uint64(uint64_t in) {
  return __builtin_parityll(in);
}
#else
/* byte parity from: https://graphics.stanford.edu/~seander/bithacks.html#ParityWith64Bits */
ATTR_CONST ATTR_ARTIFICIAL static inline uint64_t parity64_uint64(uint64_t in) {
  in ^= in >> 1;
  in ^= in >> 2;
  in = (in & 0x1111111111111111) * 0x1111111111111111;
  return (in >> 60) & 1;
}
#endif

/* helper functions to compute number of leading zeroes */
#if GNUC_CHECK(4, 7) || __has_builtin(__builtin_clz)
ATTR_CONST ATTR_ARTIFICIAL static inline uint32_t clz(uint32_t x) {
  return x ? __builtin_clz(x) : 32;
}
#elif defined(_MSC_VER)
#include <intrin.h>
ATTR_CONST ATTR_ARTIFICIAL static inline uint32_t clz(uint32_t x) {
  unsigned long index = 0;
  if (_BitScanReverse(&index, x)) {
    return 31 - index;
  }
  return 32;
}
#else
/* Number of leading zeroes of x.
 * From the book
 * H.S. Warren, *Hacker's Delight*, Pearson Education, 2003.
 * http://www.hackersdelight.org/hdcodetxt/nlz.c.txt
 */
ATTR_CONST ATTR_ARTIFICIAL static inline uint32_t clz(uint32_t x) {
  if (!x) {
    return 32;
  }

  uint32_t n = 1;
  if (!(x >> 16)) {
    n = n + 16;
    x = x << 16;
  }
  if (!(x >> 24)) {
    n = n + 8;
    x = x << 8;
  }
  if (!(x >> 28)) {
    n = n + 4;
    x = x << 4;
  }
  if (!(x >> 30)) {
    n = n + 2;
    x = x << 2;
  }
  n = n - (x >> 31);

  return n;
}
#endif

ATTR_CONST ATTR_ARTIFICIAL static inline uint32_t ceil_log2(uint32_t x) {
  if (!x) {
    return 0;
  }
  return 32 - clz(x - 1);
}

#if defined(__WIN32__)
#define SIZET_FMT "%Iu"
#else
#define SIZET_FMT "%zu"
#endif

/* crypto_declassify wrapper */
#if defined(TIMECOP)
#include "crypto_declassify.h"
#define picnic_declassify(x, len) crypto_declassify(x, len)
#elif defined(WITH_VALGRIND)
#include <valgrind/memcheck.h>
#define picnic_declassify(x, len) VALGRIND_MAKE_MEM_DEFINED(x, len)
#else
#define picnic_declassify(x, len)
#endif

#endif
