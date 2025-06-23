// SPDX-License-Identifier: Apache-2.0

#ifndef rng_h
#define rng_h

#include <oqs/oqs.h>

static int randombytes(unsigned char *x, unsigned long long xlen){
    OQS_randombytes(x, xlen);
    return 0;
}

#endif /* rng_h */
