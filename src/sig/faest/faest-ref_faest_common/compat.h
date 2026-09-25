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

#if !defined(HAVE_CONFIG_H) && !defined(OQS)
/* In case meson checks were not run, define HAVE_* for known good configurations. We skip those if
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

#if !defined(HAVE_MEMSET_EXPLICIT) && GLIBC_CHECK(2, 43)
/* memset_explicit was introduced in glibc 2.43 */
#define HAVE_MEMSET_EXPLICIT
#endif

#if !defined(HAVE_TIMINGSAFE_BCMP) && ((defined(__OpenBSD__) && OpenBSD >= 201105) ||              \
                                       FREEBSD_CHECK(12, 0) || MACOSX_CHECK(10, 12, 1))
/* timingsafe_bcmp was introduced in OpenBSD 4.9, FreeBSD 12.0, and MacOS X 10.12 */
#define HAVE_TIMINGSAFE_BCMP
#endif /* HAVE_TIMINGSAFE_BCMP */

#if !defined(HAVE_MEMPCPY) && GLIBC_CHECK(2, 1)
/* mempcpy was introduced in glibc 2.1*/
#define HAVE_MEMPCPY
#endif /* HAVE_MEMPCPY */

#if defined(__x86_64__) || defined(__i386__) || defined(_M_IX86) || defined(_M_AMD64)
#if __has_include(<wmmintrin.h>)
#define HAVE_AESNI
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
#endif /* !HAVE_CONFIG_H && !OQS */

#if defined(HAVE_ALIGNED_ALLOC)
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

#if defined(HAVE_TIMINGSAFE_BCMP)
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

#if defined(HAVE_EXPLICIT_BZERO)
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
#if defined(_MSC_VER) || defined(__clang__)
#define faest_mempcpy(dst, src, len) ((void*)(((uint8_t*)memcpy((dst), (src), (len))) + (len)))
#else
#define faest_mempcpy(dst, src, len) (memcpy((dst), (src), (len)) + (len))
#endif
#endif /* HAVE_MEMPCPY */

#if defined(OQS)
#include <oqs/common.h>

#define faest_aligned_alloc(alignment, size) OQS_MEM_aligned_alloc((alignment), (size))
#define faest_aligned_free(ptr) OQS_MEM_aligned_free((ptr))
#define faest_timingsafe_bcmp(a, b, len) OQS_MEM_secure_bcmp((a), (b), (len))
#define faest_explicit_bzero(ptr, len) OQS_MEM_cleanse(ptr, len)
#endif

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
#endif

/* helper functions for byte parity: 0 if even number of bits are set, 1 if odd number of bts are
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
#endif

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
#endif

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
#endif

#if defined(HAVE_AVX2)
#if defined(_MSC_VER)
// workarounds for MSVC
#define __m256i_u __m256i
#elif !GNUC_CHECK(7, 0) && !CLANG_CHECK(9)
// workaround for gcc and clang
#define __m256i_u __m256i
#endif
#endif

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
#endif /* HAVE_AESNI */

#endif
