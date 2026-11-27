/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef SLH_SYS_H
#define SLH_SYS_H

/* Try to find endianness, if not forced through CFLAGS already */
#if !defined(SLH_SYS_LITTLE_ENDIAN) && !defined(SLH_SYS_BIG_ENDIAN)
#if defined(__BYTE_ORDER__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define SLH_SYS_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define SLH_SYS_BIG_ENDIAN
#else
#error "__BYTE_ORDER__ defined, but don't recognize value."
#endif
#endif /* __BYTE_ORDER__ */

/* MSVC does not define __BYTE_ORDER__. However, MSVC only supports
 * little endian x86, x86_64, and AArch64. It is, hence, safe to assume
 * little endian. */
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || \
                          defined(_M_IX86) || defined(_M_ARM64))
#define SLH_SYS_LITTLE_ENDIAN
#endif

#endif /* !SLH_SYS_LITTLE_ENDIAN && !SLH_SYS_BIG_ENDIAN */

/*
 * C90 does not have the inline compiler directive yet.
 * We don't use it in C90 builds.
 * However, in that case the compiler warns about some inline functions in
 * header files not being used in every compilation unit that includes that
 * header. To work around it we silence that warning in that case using
 * __attribute__((unused)).
 */

/* Do not use inline for C90 builds*/
#if !defined(SLH_INLINE)
#if !defined(inline)
#if defined(_MSC_VER)
#define SLH_INLINE __inline
/* Don't combine __inline and __forceinline */
#define SLH_ALWAYS_INLINE __forceinline
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define SLH_INLINE inline
#define SLH_ALWAYS_INLINE SLH_INLINE __attribute__((always_inline))
#else
#define SLH_INLINE __attribute__((unused))
#define SLH_ALWAYS_INLINE SLH_INLINE
#endif

#else /* !inline */
#define SLH_INLINE inline
#define SLH_ALWAYS_INLINE SLH_INLINE __attribute__((always_inline))
#endif /* inline */
#endif /* !SLH_INLINE */

/*
 * Constant-time testing with valgrind: secret data is marked as undefined,
 * so that valgrind reports any branch or memory access depending on it.
 * Values that are public by design are declassified when they are computed.
 */
#if defined(SLH_CONFIG_CT_TESTING_ENABLED)
#include <valgrind/memcheck.h>
#define SLH_CT_TESTING_SECRET(ptr, len) \
  VALGRIND_MAKE_MEM_UNDEFINED((ptr), (len))
#define SLH_CT_TESTING_DECLASSIFY(ptr, len) \
  VALGRIND_MAKE_MEM_DEFINED((ptr), (len))
#else /* SLH_CONFIG_CT_TESTING_ENABLED */
#define SLH_CT_TESTING_SECRET(ptr, len) \
  do                                    \
  {                                     \
  } while (0)
#define SLH_CT_TESTING_DECLASSIFY(ptr, len) \
  do                                        \
  {                                         \
  } while (0)
#endif /* !SLH_CONFIG_CT_TESTING_ENABLED */

#endif /* SLH_SYS_H */
