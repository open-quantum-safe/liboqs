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
#else
/* in case cmake checks were not run, define HAVE_* for known good configurations */
#if !defined(HAVE_ALIGNED_ALLOC) && !defined(__APPLE__) && !defined(__MINGW32__) &&                \
    !defined(__MINGW64__) &&                                                                       \
    (defined(_ISOC11_SOURCE) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L))
#define HAVE_ALIGNED_ALLOC
#endif
#endif

#if defined(HAVE_ALIGNED_ALLOC)
#include <stdlib.h>

#define aligned_free free
#else
#include <stddef.h>

/**
 * Compatibility implementation of aligned_alloc from ISO C 2011.
 */
void* aligned_alloc(size_t alignment, size_t size);
/**
 * Some aligned_alloc compatbility implementations require custom free
 * functions, so we provide one too.
 */
void aligned_free(void* ptr);
#endif

#include "endian_compat.h"

#endif
