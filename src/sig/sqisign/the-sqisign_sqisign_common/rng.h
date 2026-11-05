// SPDX-License-Identifier: Apache-2.0

// liboqs integration shim: route SQIsign's randombytes() to OQS_randombytes().
// This header overrides the in-tree include/rng.h when SQIsign is built inside liboqs;
// the AES-CTR-DRBG randombytes implementations are excluded from the liboqs source list.

#ifndef rng_h
#define rng_h

#include <oqs/oqs.h>

static inline int
randombytes(unsigned char *x, unsigned long long xlen)
{
    OQS_randombytes(x, xlen);
    return 0;
}

#endif // rng_h
