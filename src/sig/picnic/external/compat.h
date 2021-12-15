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

#include <stddef.h>

#if defined(HAVE_CONFIG_H)
#include <config.h>
#elif !defined(OQS)
/* in case cmake checks were not run, define HAVE_* for known good configurations */
#include "macros.h"
#if defined(__OpenBSD__)
#include <sys/param.h>
#endif /* __OpenBSD__ */

#if !defined(HAVE_ALIGNED_ALLOC) && !defined(__APPLE__) && !defined(__MINGW32__) &&                \
    !defined(__MINGW64__) && !defined(_MSC_VER) &&                                                 \
    (defined(_ISOC11_SOURCE) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L))
/* aligned_alloc was introduced in ISO C 2011 */
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
#endif /* HAVE_CONFIG_H */

#if defined(HAVE_ALIGNED_ALLOC)
#include <stdlib.h>

#define picnic_aligned_alloc(alignment, size) aligned_alloc((alignment), (size))
#define picnic_aligned_free(ptr) free((ptr))
#else
/**
 * Compatibility implementation of aligned_alloc from ISO C 2011.
 */
void* picnic_aligned_alloc(size_t alignment, size_t size);
/**
 * Some aligned_alloc compatbility implementations require custom free
 * functions, so we provide one too.
 */
void picnic_aligned_free(void* ptr);
#endif /* HAVE_ALIGNED_ALLOC */

#include "endian_compat.h"

#if defined(HAVE_TIMINGSAFE_BCMP)
#define picnic_timingsafe_bcmp(a, b, len) timingsafe_bcmp((a), (b), (len))
#else
/**
 * Compatibility implementation of timingsafe_bcmp from OpenBSD 4.9 and FreeBSD 12.0.
 */
int picnic_timingsafe_bcmp(const void* a, const void* b, size_t len);
#endif /* HAVE_TIMINGSAFE_BCMP */

#if defined(HAVE_EXPLICIT_BZERO)
#define picnic_explicit_bzero(ptr, len) explicit_bzero((ptr), (len))
#else
/**
 * Compatibility implementation of explicit_bzero
 */
void picnic_explicit_bzero(void* a, size_t len);
#endif /* HAVE_EXPLICIT_BZERO */

#if defined(OQS)
#define picnic_aligned_alloc(alignment, size) OQS_MEM_aligned_alloc((alignment), (size))
#define picnic_aligned_free(ptr) OQS_MEM_aligned_free((ptr))
#define picnic_timingsafe_bcmp(a, b, len) OQS_MEM_secure_bcmp((a), (b), (len))
#define picnic_explicit_bzero(ptr, len) OQS_MEM_cleanse(ptr, len)
#endif
#endif
