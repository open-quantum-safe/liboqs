/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#include "macros.h"

/* define HAVE_* for more known good configurations */
#if !defined(HAVE_POSIX_MEMALIGN) &&                                                               \
    ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || defined(__APPLE__))
/* defined in POSIX and available on OS X */
#define HAVE_POSIX_MEMALIGN
#endif /* HAVE_POSIX_MEMALIGN */

#if !defined(HAVE_MEMALIGN) && defined(__linux__)
/* always available on Linux */
#define HAVE_MEMALIGN
#endif /* HAVE_MEMALIGN */

#if !defined(HAVE_CONSTTIME_MEMEQUAL) && NETBSD_CHECK(7, 0)
/* consttime_memequal was introduced in NetBSD 7.0 */
#define HAVE_CONSTTIME_MEMEQUAL
#endif /* HAVE_CONSTTIME_MEMEQUAL */
#endif /* HAVE_CONFIG_H */

#include "compat.h"

#if !defined(HAVE_ALIGNED_ALLOC)
#include <errno.h>
#include <stdlib.h>
#if !defined(HAVE_POSIX_MEMALIGN) || defined(__MING32__) || defined(__MING64__) || defined(_MSC_VER)
#include <malloc.h>
#endif

void* aligned_alloc(size_t alignment, size_t size) {
  /* check alignment (power of 2) and size (multiple of alignment) */
  if (alignment & (alignment - 1) || size & (alignment - 1)) {
    errno = EINVAL;
    return NULL;
  }

#if defined(HAVE_POSIX_MEMALIGN)
  /* check alignment (needs to be >= sizeof(void*)) */
  if (alignment < sizeof(void*)) {
    alignment = sizeof(void*);
  }

  void* ptr     = NULL;
  const int err = posix_memalign(&ptr, alignment, size);
  if (err) {
    errno = err;
  }
  return ptr;
#elif defined(HAVE_MEMALIGN)
  return memalign(alignment, size);
#elif defined(__MINGW32__) || defined(__MINGW64__)
  return __mingw_aligned_malloc(size, alignment);
#elif defined(_MSC_VER)
  return _aligned_malloc(size, alignment);
#else
  if (size > 0) {
    errno = ENOMEM;
  }
  return NULL;
#endif
}

void aligned_free(void* ptr) {
#if defined(HAVE_POSIX_MEMALIGN) || defined(HAVE_MEMALIGN)
  free(ptr);
#elif defined(__MINGW32__) || defined(__MINGW64__)
  __mingw_aligned_free(ptr);
#elif defined(_MSC_VER)
  _aligned_free(ptr);
#endif
}
#endif /* HAVE_ALIGNED_ALLOC */

#if !defined(HAVE_TIMINGSAFE_BCMP)
int timingsafe_bcmp(const void* a, const void* b, size_t len) {
#if defined(HAVE_CONSTTIME_MEMEQUAL)
  return !consttime_memequal(a, b, len);
#else
  const unsigned char* p1 = a;
  const unsigned char* p2 = b;

  unsigned int res = 0;
  for (; len; --len, ++p1, ++p2) {
    res |= *p1 ^ *p2;
  }
  return res;
#endif
}
#endif /* HAVE_TIMINGSAFE_BCMP */

#if !defined(HAVE_EXPLICIT_BZERO)
#if defined(_WIN32)
#include <windows.h>
#endif

void explicit_bzero(void* a, size_t len) {
#if defined(_WIN32)
  SecureZeroMemory(a, len);
#else
  volatile char* p = a;
  for (; len; ++p, --len) {
    *p = 0;
  }
#endif
}
#endif /* HAVE_EXPLICIT_BZERO */
