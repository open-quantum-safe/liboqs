// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file utils_randombytes.c
/// @brief wrappers for randombytes().
///

#include "utils_randombytes.h"


#if defined (_UTILS_OQS_)
#include <oqs/rand.h>

void randombytes(unsigned char *x, unsigned long long xlen) {
    OQS_randombytes(x, xlen);
}


#elif defined(_UTILS_SUPERCOP_)|| defined(_UTILS_PQM4_)

// nothing to do.

#elif defined(_NIST_KAT_)

// randombytes() is defined in rng.h/c provided from nist.

#elif defined(_UTILS_OPENSSL_)

#include <openssl/rand.h>

void randombytes(unsigned char *x, unsigned long long xlen) {
    RAND_bytes(x, xlen);
}

#elif defined( _DEBUG_RANDOMBYTES_ )

#include <stdlib.h>
#include <string.h>

void randombytes(unsigned char *x, unsigned long long xlen) {
    while (xlen--) {
        *x++ = rand() & 0xff;
    }
}

#else

ERROR -- no implementation for randombytes()

#endif

