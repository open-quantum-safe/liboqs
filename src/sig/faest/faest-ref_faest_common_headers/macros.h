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
#endif /* __GLIBC__ */

/* FreeBSD version check macro */
#if defined(__FreeBSD__)
#define FREEBSD_CHECK(maj, min) (__FreeBSD__ >= (maj))
#else
#define FREEBSD_CHECK(maj, min) 0
#endif /* __FreeBSD__ */

/* NetBSD version check macro */
#if defined(__NetBSD__)
#include <sys/param.h>
#define NETBSD_CHECK(maj, min) (__NetBSD_Version__ >= ((maj) * 1000000000 + (min) * 10000000))
#else
#define NETBSD_CHECK(maj, min) 0
#endif /* __NetBSD__ */

/* OpenBSD version check macro */
#if defined(__OpenBSD__)
#include <sys/param.h>
#define OPENBSD_CHECK(ver) (OpenBSD >= (ver))
#else
#define OPENBSD_CHECK(ver) 0
#endif /* __OpenBSD__ */

/* Apple version check macro */
#if defined(__APPLE__)
#include <Availability.h>

#define MACOSX_CHECK(maj, min, rev)                                                                \
  (__MAC_OS_X_VERSION_MIN_REQUIRED >= ((maj) * 10000 + (min) * 100 + (rev)))
#else
#define MACOSX_CHECK(maj, min, rev) 0
#endif /* __APPLE__ */

#if !defined(MIN)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* !MIN */

#if !defined(MAX)
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif /* !MAX */

/* assume */
#if GNUC_CHECK(4, 5) || __has_builtin(__builtin_unreachable)
#define ASSUME(p)                                                                                  \
  do {                                                                                             \
    if (!(p)) {                                                                                    \
      __builtin_unreachable();                                                                     \
    }                                                                                              \
  } while (0)
#elif defined(_MSC_VER)
#define ASSUME(p) __assume(p)
#else
#define ASSUME(p) (void)(p)
#endif

/* unused attributed */
#if GNUC_CHECK(2, 7) || __has_attribute(unused)
#define FAEST_UNUSED(x) UNUSED_##x __attribute__((unused))
#else
#define FAEST_UNUSED(x) x
#endif

/* nonnull attribute */
#if GNUC_CHECK(3, 3) || __has_attribute(nonnull)
#define ATTR_NONNULL __attribute__((nonnull))
#define ATTR_NONNULL_ARG(i, ...) __attribute__((nonnull(i, __VA_ARGS__)))
#else
#define ATTR_NONNULL
#define ATTR_NONNULL_ARG(i, ...)
#endif

/* aligned attribute */
/* note that C11's alignas will only do the job once DR 444 is implemented */
#if GNUC_CHECK(3, 2) || __has_attribute(aligned)
#define ATTR_ALIGNED(i) __attribute__((aligned((i))))
#elif defined(_MSC_VER)
#define ATTR_ALIGNED(i) __declspec(align(i))
#else
#define ATTR_ALIGNED(i)
#endif

/* round size to meet alignment requirements */
#define ALIGN_TO(s, a) (((s) + a - 1) & ~(a - 1))

/* always inline attribute */
#if GNUC_CHECK(4, 0) || __has_attribute(always_inline)
#define ATTR_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define ATTR_ALWAYS_INLINE __forceinline
#else
#define ATTR_ALWAYS_INLINE
#endif

/* pure attribute
 * Functions can be marked as pure if their only effect is their return value.
 * The return value itself may only be computed from reading global variables
 * and the arguments.
 */
#if GNUC_CHECK(3, 0) || __has_attribute(pure)
#define ATTR_PURE __attribute__((pure))
#else
#define ATTR_PURE
#endif

/* const attribute
 * Functions can be marked as const if their only effect is their return value.
 * The return value itself may only be computed from the arguments.
 */
#if GNUC_CHECK(2, 5) || __has_attribute(const)
#define ATTR_CONST __attribute__((const))
#else
#define ATTR_CONST
#endif

/* target attribute */
#if GNUC_CHECK(4, 4) || __has_attribute(target)
#define ATTR_TARGET(x) __attribute__((target((x))))
#define ATTR_TARGET_AVX2 __attribute__((target("avx2,sse2")))
#define ATTR_TARGET_SSE2 __attribute__((target("sse2")))
#define ATTR_TARGET_AESNI __attribute__((target("sse2,aes")))
#define ATTR_TARGET_AESNI_AVX2 __attribute__((target("avx2,aes")))
#define ATTR_TARGET_PCLMUL __attribute__((target("pclmul")))
#if !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC)
#define ATTR_TARGET_NEON __attribute__((target("fpu=neon")))
#define ATTR_TARGET_ARM_AES
#else
#define ATTR_TARGET_NEON
#define ATTR_TARGET_ARM_AES __attribute__((target("+crypto")))
#endif
#else
#define ATTR_TARGET(x)
#define ATTR_TARGET_AVX2
#define ATTR_TARGET_SSE2
#define ATTR_TARGET_AESNI
#define ATTR_TARGET_AESNI_AVX2
#define ATTR_TARGET_PCLMUL
#define ATTR_TARGET_NEON
#define ATTR_TARGET_ARM_AES
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
#if GNUC_CHECK(3, 1) || __has_attribute(deprecated)
#define ATTR_DEPRECATED __attribute__((deprecated))
#else
#define ATTR_DEPRECATED
#endif

/* constructor attribute */
#if GNUC_CHECK(2, 7) || __has_attribute(constructor)
#define ATTR_CONSTRUCTOR __attribute__((constructor))
#endif

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
