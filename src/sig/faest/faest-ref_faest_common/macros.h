/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_MACROS_H
#define FAEST_MACROS_H

#include "faest_defines.h"

/* compatibility with clang and other compilers */
#if !defined(__has_attribute)
#define __has_attribute(a) 0
#endif

#if !defined(__has_builtin)
#define __has_builtin(b) 0
#endif

#if !defined(__has_include)
#define __has_include(h) 0
#endif

/* gcc version check macro */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define GNUC_CHECK(maj, min)                                                                       \
  (((__GNUC__ << 20) + (__GNUC_MINOR__ << 10)) >= (((maj) << 20) + ((min) << 10)))
#else
#define GNUC_CHECK(maj, min) 0
#endif

/* clang version check macro */
#if defined(__clang__) && defined(__clang_major__)
#define CLANG_CHECK(maj) (__clang_major__ >= (maj))
#else
#define CLANG_CHECK(maj) 0
#endif

/* glibc version check macro */
#if __has_include(<features.h>)
#include <features.h>
#endif

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
#define NETBSD_CHECK(maj, min) (__NetBSD_Version__ >= ((maj) * 1000000000 + (min) * 10000000))
#else
#define NETBSD_CHECK(maj, min) 0
#endif

/* Apple version check macro */
#if defined(__APPLE__)
#include <Availability.h>
#define MACOSX_CHECK(maj, min, rev)                                                                \
  (__MAC_OS_X_VERSION_MIN_REQUIRED >= ((maj) * 10000 + (min) * 100 + (rev)))
#else
#define MACOSX_CHECK(maj, min, rev) 0
#endif

#if !defined(MIN)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#if !defined(MAX)
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

/* unused attributed */
#if defined(__GNUC__) || __has_attribute(unused)
#define FAEST_UNUSED(x) UNUSED_##x __attribute__((unused))
#else
#define FAEST_UNUSED(x) x
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
#if GNUC_CHECK(3, 2) || __has_attribute(aligned)
#define ATTR_ALIGNED(i) __attribute__((aligned((i))))
#define HAVE_USEFUL_ATTR_ALIGNED
#elif defined(_MSC_VER)
#define ATTR_ALIGNED(i) __declspec(align(i))
#define HAVE_USEFUL_ATTR_ALIGNED
#else
#define ATTR_ALIGNED(i)
#endif

/* round size to meet alignment requirements */
#define ALIGN_TO(s, a) (((s) + a - 1) & ~(a - 1))

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

/* pure attribute
  Functions can be marked as pure if their only effect is their return value. The return value
  itself may only be computed from reading global variables and the arguments.
 */
#if defined(__GNUC__) || __has_attribute(pure)
#define ATTR_PURE __attribute__((pure))
#else
#define ATTR_PURE
#endif

/* const attribute
  Functions can be marked as const if their only effect is their return value. The return value
  itself may only be computed from the arguments.
 */
#if defined(__GNUC__) || __has_attribute(const)
#define ATTR_CONST __attribute__((const))
#else
#define ATTR_CONST
#endif

/* target attribute */
#if defined(__GNUC__) || __has_attribute(target)
#define ATTR_TARGET(x) __attribute__((target((x))))
#define ATTR_TARGET_AVX2 __attribute__((target("avx2,bmi2,sse2")))
#define ATTR_TARGET_SSE2 __attribute__((target("sse2")))
#define ATTR_TARGET_AESNI __attribute__((target("sse2,aes")))
#define ATTR_TARGET_AESNI_AVX2 __attribute__((target("avx2,bmi2,aes")))
#else
#define ATTR_TARGET(x)
#define ATTR_TARGET_AVX2
#define ATTR_TARGET_SSE2
#define ATTR_TARGET_AESNI
#define ATTR_TARGET_AESNI_AVX2
#endif

/* artificial attribute */
#if GNUC_CHECK(4, 7) || __has_attribute(__artificial__)
#define ATTR_ARTIFICIAL __attribute__((__artificial__))
#else
#define ATTR_ARTIFICIAL
#endif

/* vector_size attribute */
#if GNUC_CHECK(4, 8) || __has_attribute(vector_size)
#define HAVE_ATTR_VECTOR_SIZE
#define ATTR_VECTOR_SIZE(s) __attribute__((vector_size(s)))
#elif __has_attribute(__vector_size__)
#define HAVE_ATTR_VECTOR_SIZE
#define ATTR_VECTOR_SIZE(s) __attribute__((__vector_size__(s)))
#else
#define ATTR_VECTOR_SIZE(s)
#endif

/* malloc attribute */
#if GNUC_CHECK(11, 0)
#define ATTR_MALLOC(arg) __attribute__((malloc, malloc(arg))) // IGNORE memory-check
#elif GNUC_CHECK(2, 96) || __has_attribute(malloc)
#define ATTR_MALLOC(arg) __attribute__((malloc))
#else
#define ATTR_MALLOC(arg)
#endif

/* alloc align attribute */
#if GNUC_CHECK(4, 9) || __has_attribute(alloc_align)
#define ATTR_ALLOC_ALIGN(arg) __attribute__((alloc_align(arg)))
#else
#define ATTR_ALLOC_ALIGN(arg)
#endif

/* alloc size attribute */
#if GNUC_CHECK(4, 3) || __has_attribute(alloc_size)
#define ATTR_ALLOC_SIZE(arg) __attribute__((alloc_size(arg)))
#else
#define ATTR_ALLOC_SIZE(arg)
#endif

/* deprecated attribute */
#if defined(_GCC__) || __has_attribute(deprecated)
#define ATTR_DEPRECATED __attribute__((deprecated))
#else
#define ATTR_DEPRECATED
#endif

/* concatenation */
#define CONCAT2(a, b) a##_##b
#define CONCAT(a, b) CONCAT2(a, b)

#if defined(__WIN32__)
#define SIZET_FMT "%Iu"
#else
#define SIZET_FMT "%zu"
#endif

/* crypto_declassify wrapper */
#if defined(TIMECOP) || defined(SUPERCOP)
#include "crypto_declassify.h"
#define faest_declassify(x, len) crypto_declassify((void*)x, len)
#elif defined(WITH_VALGRIND)
#include <valgrind/memcheck.h>
#define faest_declassify(x, len) VALGRIND_MAKE_MEM_DEFINED(x, len)
#else
#define faest_declassify(x, len)
#endif

/* number of elements in an array */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#endif
