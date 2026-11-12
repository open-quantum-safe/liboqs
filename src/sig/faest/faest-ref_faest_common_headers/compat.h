/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_COMPAT_H
#define FAEST_COMPAT_H

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include "macros.h"

#include <stddef.h>

/* In case meson checks were not run, define HAVE_* for known good configurations here. When
 * building for OQS, skip those where compat functions are provided by OQS. */
#if !defined(HAVE_CONFIG_H) && !defined(OQS)
#if !defined(HAVE_ALIGNED_ALLOC) && !defined(__APPLE__) && !defined(__MINGW32__) &&                \
    !defined(__MINGW64__) && !defined(_MSC_VER) && !defined(__ANDROID__) &&                        \
    (defined(_ISOC11_SOURCE) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L))
/* aligned_alloc was introduced in ISO C 2011. Even if building with -std=c11, some toolchains do
 * not provide aligned_alloc, including toolchains for Android, OS X, MinGW, and others. */
#define HAVE_ALIGNED_ALLOC
#endif /* HAVE_ALIGNED_ALLOC */

#if !defined(HAVE_EXPLICIT_BZERO) &&                                                               \
    (GLIBC_CHECK(2, 25) || OPENBSD_CHECK(201405) || FREEBSD_CHECK(11, 0) || NETBSD_CHECK(8, 0))
/* explicit_bzero was introduced in glibc 2.35, OpenBSD 5.5, FreeBSD 11.0 and NetBSD 8.0 */
#define HAVE_EXPLICIT_BZERO
#endif /* HAVE_EXPLICIT_BZERO */

#if !defined(HAVE_MEMSET_EXPLICIT) && (GLIBC_CHECK(2, 43) || NETBSD_CHECK(11, 0))
/* memset_explicit was introduced in glibc 2.43, NetBSD 11.0 */
#define HAVE_MEMSET_EXPLICIT
#endif

#if !defined(HAVE_TIMINGSAFE_BCMP) &&                                                              \
    (OPENBSD_CHECK(201105) || FREEBSD_CHECK(12, 0) || MACOSX_CHECK(10, 12, 1))
/* timingsafe_bcmp was introduced in OpenBSD 4.9, FreeBSD 12.0, and MacOS X 10.12 */
#define HAVE_TIMINGSAFE_BCMP
#endif /* HAVE_TIMINGSAFE_BCMP */
#endif /* !HAVE_CONFIG_H && !OQS */

/* HAVE_* for compact functions not provided by OQS. */
#if !defined(HAVE_CONFIG_H)
#if !defined(HAVE_MEMPCPY) && GLIBC_CHECK(2, 1)
/* mempcpy was introduced in glibc 2.1*/
#define HAVE_MEMPCPY
#endif /* HAVE_MEMPCPY */

/* HAVE_* for compiler support.
 * These macros define whether thecompiler supports the compilation of these ISA extensions and the
 * corresponding intrinsics. Runtime detection is provided by the CPU_SUPPORTS_ macros.
 */
#if defined(__x86_64__) || defined(__i386__) || defined(_M_IX86) ||                                \
    (defined(_M_X64) && !defined(_M_ARM64) && !defined(_M_ARM64EC))
#if __has_include(<wmmintrin.h>)
#define HAVE_AESNI
#define HAVE_PCLMUL
#endif
#if __has_include(<emmintrin.h>)
#define HAVE_SSE2
#endif
#if __has_include(<immintrin.h>)
#define HAVE_AVX2
#endif

#if GNUC_CHECK(9, 0) || CLANG_CHECK(8)
#define HAVE_MM_LOADU_SI64
#endif
#endif

#if defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC)
#define HAVE_NEON
// TODO: fix detection of compiler support
#if defined(__ARM_FEATURE_AES) || defined(__ARM_FEATURE_CRYPTO)
#define HAVE_ARM_AES
#endif
#elif (defined(__arm__) || defined(_M_ARM)) && __has_include(<arm_neon.h>)
#define HAVE_NEON
#endif
#endif /* !HAVE_CONFIG_H */

#if defined(OQS)
#include <oqs/common.h>
#endif

#if defined(OQS)
#define faest_malloc(size) OQS_MEM_malloc((size))         // IGNORE memory-check
#define faest_calloc(n, size) OQS_MEM_calloc((n), (size)) // IGNORE memory-check
#define faest_free(ptr) OQS_MEM_insecure_free((ptr))      // IGNORE memory-check
#else
#define faest_malloc(size) malloc((size)) // IGNORE memory-check
#define faest_calloc(n, size) calloc((n), (size)) // IGNORE memory-check
#define faest_free(ptr) free((ptr)) // IGNORE memory-check
#endif

#if defined(OQS)
#define faest_aligned_alloc(alignment, size) OQS_MEM_aligned_alloc((alignment), (size))
#define faest_aligned_free(ptr) OQS_MEM_aligned_free((ptr))
#elif defined(HAVE_ALIGNED_ALLOC)
#include <stdlib.h>

#define faest_aligned_alloc(alignment, size) aligned_alloc((alignment), (size))
#define faest_aligned_free(ptr) free((ptr)) // IGNORE memory-check
#else
FAEST_BEGIN_C_DECL

/**
 * Some aligned_alloc compatbility implementations require custom free
 * functions, so we provide one too.
 */
void faest_aligned_free(void* ptr);
/**
 * Compatibility implementation of aligned_alloc from ISO C 2011.
 */
void* faest_aligned_alloc(size_t alignment, size_t size) ATTR_MALLOC(faest_aligned_free)
    ATTR_ALLOC_ALIGN(1) ATTR_ALLOC_SIZE(2);

FAEST_END_C_DECL
#endif /* HAVE_ALIGNED_ALLOC */

#include "endian_compat.h"

#if defined(OQS)
#define faest_timingsafe_bcmp(a, b, len) OQS_MEM_secure_bcmp((a), (b), (len))
#elif defined(HAVE_TIMINGSAFE_BCMP)
#include <string.h>

#define faest_timingsafe_bcmp(a, b, len) timingsafe_bcmp((a), (b), (len))
#else
FAEST_BEGIN_C_DECL

/**
 * Compatibility implementation of timingsafe_bcmp from OpenBSD 4.9 and FreeBSD 12.0.
 */
int faest_timingsafe_bcmp(const void* a, const void* b, size_t len);

FAEST_END_C_DECL
#endif /* HAVE_TIMINGSAFE_BCMP */

#if defined(OQS)
#define faest_explicit_bzero(ptr, len) OQS_MEM_cleanse(ptr, len)
#elif defined(HAVE_EXPLICIT_BZERO)
#include <string.h>

#define faest_explicit_bzero(ptr, len) explicit_bzero((ptr), (len))
#elif defined(HAVE_MEMSET_EXPLICIT)
#include <string.h>

#define faest_explicit_bzero(ptr, len) memset_explicit((ptr), 0, (len))
#else
FAEST_BEGIN_C_DECL

/**
 * Compatibility implementation of explicit_bzero
 */
void faest_explicit_bzero(void* a, size_t len);

FAEST_END_C_DECL
#endif /* HAVE_EXPLICIT_BZERO */

#if defined(HAVE_MEMPCPY)
#include <string.h>

#define faest_mempcpy(dst, src, len) mempcpy((dst), (src), (len))
#else
#include <string.h>

/**
 * Compatibility implementation of mempcpy
 */
void* faest_mempcpy(void* dst, const void* src, size_t n) ATTR_NONNULL_ARG(1, 2);
#endif /* HAVE_MEMPCPY */

#include <limits.h>
#include <stdint.h>

/* helper functions for left and right rotations of bytes */
#if GNUC_CHECK(4, 9) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>

#define rotl8 __rolb
#define rotr8 __rorb
#define rotl32 __rold
#define rotr32 __rord
#elif __has_builtin(__builtin_rotateleft) && __has_builtin(__builtin_rotateright)
#define rotl8 __builtin_rotateleft8
#define rotr8 __builtin_rotateright8
#define rotl32 __builtin_rotateleft32
#define rotr32 __builtin_rotateright32
#else
ATTR_CONST static inline uint8_t rotl8(uint8_t n, unsigned int c) {
  const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
  c &= mask;
  return (n << c) | (n >> ((-c) & mask));
}

ATTR_CONST static inline uint8_t rotr8(uint8_t n, unsigned int c) {
  const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
  c &= mask;
  return (n >> c) | (n << ((-c) & mask));
}

ATTR_CONST static inline uint32_t rotl32(uint32_t n, unsigned int c) {
  const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
  c &= mask;
  return (n << c) | (n >> ((-c) & mask));
}

ATTR_CONST static inline uint32_t rotr32(uint32_t n, unsigned int c) {
  const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
  c &= mask;
  return (n >> c) | (n << ((-c) & mask));
}
#endif /* GNUC_CHECK(4, 9) */

/* helper functions for byte parity: 0 if even number of bits are set, 1 if odd number of bits are
 * set */
#if __has_builtin(__builtin_parity)
#define parity8 __builtin_parity
#elif __has_builtin(__builtin_popcount)
#define parity8(x) (__builtin_popcount(x) & 0x1)
#else
ATTR_CONST ATTR_ARTIFICIAL static inline uint8_t parity8(uint8_t n) {
  n ^= n >> 4;
  n ^= n >> 2;
  n ^= n >> 1;
  return !((~n) & 1);
}
#endif /* __has_builtin(__builtin_parity) */

#if __has_builtin(__builtin_parityll)
#define parity64 __builtin_parityll
#elif __has_builtin(__builtin_popcountll)
#define parity64(x) (__builtin_popcountll(x) & 0x1)
#else
/* byte parity from: https://graphics.stanford.edu/~seander/bithacks.html#ParityWith64Bits */
ATTR_CONST ATTR_ARTIFICIAL static inline uint64_t parity64(uint64_t in) {
  in ^= in >> 1;
  in ^= in >> 2;
  in = (in & 0x1111111111111111) * 0x1111111111111111;
  return (in >> 60) & 1;
}
#endif /* __has_builtin(__builtin_parityll) */

#if GNUC_CHECK(3, 4) || __has_builtin(__builtin_ctzll)
#define ctz64 __builtin_ctzll
#else
ATTR_CONST ATTR_ARTIFICIAL static inline int ctz64(uint64_t v) {
  int b = 0;
  while (!(v & 1)) {
    v >>= 1;
    ++b;
  }
  return b;
}
#endif

/* helper functions to compute number of leading zeroes with `clz(0) == 32` */
#if GNUC_CHECK(3, 4) || __has_builtin(__builtin_clz)
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

#if !defined(__cplusplus)
#include <assert.h>

/* static_assert fallback */
#if !defined(_MSC_VER) && !defined(static_assert)
#define static_assert _Static_assert
#endif
#endif

#if defined(HAVE_SSE2)
#if defined(_MSC_VER)
// workarounds for MSVC
#include <immintrin.h>

#define __m128i_u __m128i
#elif !GNUC_CHECK(7, 0) && !CLANG_CHECK(9)
// workaround for gcc and clang
#define __m128i_u __m128i
#endif /* _MSC_VER */

#if defined(HAVE_AVX2)
#if defined(_MSC_VER)
// workarounds for MSVC
#define __m256i_u __m256i
#elif !GNUC_CHECK(7, 0) && !CLANG_CHECK(9)
// workaround for gcc and clang
#define __m256i_u __m256i
#endif /* _MSC_VER */
#endif /* HAVE_AVX2 */

#if !defined(HAVE_MM_LOADU_SI64)
#include <string.h>>

ATTR_TARGET_SSE2 ATTR_ALWAYS_INLINE static inline __m128i _mm_loadu_si64(const void* src) {
  uint64_t u0;
  memcpy(&u0, src, sizeof(u0));
#if !defined(_MSC_VER) || defined(__x86_64__)
  return _mm_set_epi64x(0, u0);
#else
  // MS VC for x86 (untested)
  union {
    uint64_t q;
    uint32_t r[2];
  } u;
  u.q = u0;
  return _mm_setr_epi32(u.r[0], u.r[1], 0, 0);
#endif
}
#endif /* !HAVE_MM_LOADU_SI64 */
#endif /* HAVE_SSE2 */

#endif
