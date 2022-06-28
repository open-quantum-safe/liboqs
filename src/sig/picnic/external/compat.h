/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef PICNIC_COMPAT_H
#define PICNIC_COMPAT_H

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include "macros.h"

#include <stddef.h>

#if !defined(HAVE_CONFIG_H) && !defined(OQS)
/* In case cmake checks were not run, define HAVE_* for known good configurations. We skip those if
 * building for OQS, as the compat functions from there can be used instead. */
#if defined(__OpenBSD__)
#include <sys/param.h>
#endif /* __OpenBSD__ */

#if !defined(HAVE_ALIGNED_ALLOC) && !defined(__APPLE__) && !defined(__MINGW32__) &&                \
    !defined(__MINGW64__) && !defined(_MSC_VER) && !defined(__ANDROID__) &&                        \
    (defined(_ISOC11_SOURCE) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L))
/* aligned_alloc was introduced in ISO C 2011. Even if building with -std=c11, some toolchains do
 * not provide aligned_alloc, including toolchains for Android, OS X, MinGW, and others. */
#define HAVE_ALIGNED_ALLOC
#endif /* HAVE_ALIGNED_ALLOC */

#if !defined(HAVE_EXPLICIT_BZERO) &&                                                               \
    (GLIBC_CHECK(2, 25) || (defined(__OpenBSD__) && OpenBSD >= 201405) || FREEBSD_CHECK(11, 0) ||  \
     NETBSD_CHECK(8, 0))
/* explicit_bzero was introduced in glibc 2.35, OpenBSD 5.5, FreeBSD 11.0 and NetBSD 8.0 */
#define HAVE_EXPLICIT_BZERO
#endif /* HAVE_EXPLICIT_BZERO */

#if !defined(HAVE_TIMINGSAFE_BCMP) && ((defined(__OpenBSD__) && OpenBSD >= 201105) ||              \
                                       FREEBSD_CHECK(12, 0) || MACOSX_CHECK(10, 12, 1))
/* timingsafe_bcmp was introduced in OpenBSD 4.9, FreeBSD 12.0, and MacOS X 10.12 */
#define HAVE_TIMINGSAFE_BCMP
#endif /* HAVE_TIMINGSAFE_BCMP */
#endif /* !HAVE_CONFIG_H && !OQS */

#if defined(HAVE_ALIGNED_ALLOC)
#include <stdlib.h>

#define picnic_aligned_alloc(alignment, size) aligned_alloc((alignment), (size))
#define picnic_aligned_free(ptr) free((ptr))
#else
PICNIC_BEGIN_C_DECL

/**
 * Compatibility implementation of aligned_alloc from ISO C 2011.
 */
void* picnic_aligned_alloc(size_t alignment, size_t size);
/**
 * Some aligned_alloc compatbility implementations require custom free
 * functions, so we provide one too.
 */
void picnic_aligned_free(void* ptr);

PICNIC_END_C_DECL
#endif /* HAVE_ALIGNED_ALLOC */

#include "endian_compat.h"

#if defined(HAVE_TIMINGSAFE_BCMP)
#define picnic_timingsafe_bcmp(a, b, len) timingsafe_bcmp((a), (b), (len))
#else
PICNIC_BEGIN_C_DECL

/**
 * Compatibility implementation of timingsafe_bcmp from OpenBSD 4.9 and FreeBSD 12.0.
 */
int picnic_timingsafe_bcmp(const void* a, const void* b, size_t len);

PICNIC_END_C_DECL
#endif /* HAVE_TIMINGSAFE_BCMP */

#if defined(HAVE_EXPLICIT_BZERO)
#define picnic_explicit_bzero(ptr, len) explicit_bzero((ptr), (len))
#else
PICNIC_BEGIN_C_DECL

/**
 * Compatibility implementation of explicit_bzero
 */
void picnic_explicit_bzero(void* a, size_t len);

PICNIC_END_C_DECL
#endif /* HAVE_EXPLICIT_BZERO */

#if defined(OQS)
#define picnic_aligned_alloc(alignment, size) OQS_MEM_aligned_alloc((alignment), (size))
#define picnic_aligned_free(ptr) OQS_MEM_aligned_free((ptr))
#define picnic_timingsafe_bcmp(a, b, len) OQS_MEM_secure_bcmp((a), (b), (len))
#define picnic_explicit_bzero(ptr, len) OQS_MEM_cleanse(ptr, len)
#endif

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

#endif
