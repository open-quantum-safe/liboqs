/*
 *  SPDX-License-Identifier: MIT
 *
 *  Generated for liboqs integration.
 */

#ifndef CONFIG_H
#define CONFIG_H

/* Only oqs/common.h is needed here, for the OQS_MEM_* allocators used by the
 * override below; the aes/rand/sha3/oqsconfig headers are included directly by
 * the faest-ref sources that use them. The HAVE_*MEMALIGN defines are omitted:
 * under OQS the aligned-alloc fallback in compat.c is compiled out
 * (`#if !defined(HAVE_ALIGNED_ALLOC) && !defined(OQS)`), so they have no effect. */
#include <oqs/common.h>

/* Route libc allocators through OQS when built for liboqs. */
#define malloc(size) OQS_MEM_malloc(size) // IGNORE memory-check
#define calloc(nmemb, size) OQS_MEM_calloc(nmemb, size) // IGNORE memory-check
#define free(ptr) OQS_MEM_insecure_free(ptr) // IGNORE memory-check

#include "faest_namespace.h"

#endif
