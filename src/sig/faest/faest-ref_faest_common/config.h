/*
 *  SPDX-License-Identifier: MIT
 *
 *  Generated for liboqs integration.
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifndef OQS
#define OQS
#endif

#include <oqs/aes.h>
#include <oqs/oqsconfig.h>
#include <oqs/common.h>
#include <oqs/rand.h>
#include <oqs/sha3.h>

/* Route libc allocators through OQS when built for liboqs. */
#define malloc(size) OQS_MEM_malloc(size) // IGNORE memory-check
#define calloc(nmemb, size) OQS_MEM_calloc(nmemb, size) // IGNORE memory-check
#define free(ptr) OQS_MEM_insecure_free(ptr) // IGNORE memory-check

#if defined(__APPLE__) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
#define HAVE_POSIX_MEMALIGN
#endif

#if defined(__linux__)
#define HAVE_MEMALIGN
#endif

#endif
