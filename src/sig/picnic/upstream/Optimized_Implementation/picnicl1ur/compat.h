#ifndef PICNIC_COMPAT_H
#define PICNIC_COMPAT_H

/* in case cmake checks failed or were not run, define HAVE_* for known good
 * configurations */

#if !defined(HAVE_ALIGNED_ALLOC) &&                                                                \
    (defined(_ISOC11_SOURCE) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L &&       \
                                 !defined(__MINGW32__) && !defined(__MINGW64__)))
#define HAVE_ALIGNED_ALLOC
#endif

#if !defined(HAVE_POSIX_MEMALIGN) && defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
#define HAVE_POSIX_MEMALIGN
#endif

#if !defined(HAVE_MEMALIGN) && defined(__linux__)
#define HAVE_MEMALIGN
#endif

#if defined(HAVE_ALIGNED_ALLOC)
#include <stdlib.h>

#define aligned_free free
#else
#include <stddef.h>

/* compat implementation of aligned_alloc from ISO C 2011 */
void* aligned_alloc(size_t alignment, size_t size);
/* some aligned alloc implementations require custom free functions, so we
 * provide one too */
void aligned_free(void* ptr);
#endif

/* backwards compatibility macros for GCC 4.8 and 4.9
 *
 * bs{l,r}i was introduced in GCC 5 and in clang as macros sometime in 2015.
 * */

#include "endian_compat.h"

#endif
