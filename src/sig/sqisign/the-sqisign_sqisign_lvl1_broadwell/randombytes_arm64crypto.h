// SPDX-License-Identifier: Apache-2.0

#ifndef RANDOMBYTES_ARM64CRYPTO_H
#define RANDOMBYTES_ARM64CRYPTO_H

#include <stdio.h>

#define RNG_SUCCESS      0
#define RNG_BAD_MAXLEN  -1
#define RNG_BAD_OUTBUF  -2
#define RNG_BAD_REQ_LEN -3

typedef struct {
    unsigned char   buffer[16];
    int             buffer_pos;
    unsigned long   length_remaining;
    unsigned char   key[32];
    unsigned char   ctr[16];
} AES_XOF_struct;

typedef struct {
    unsigned char   Key[32];
    unsigned char   V[16];
    int             reseed_counter;
} AES256_CTR_DRBG_struct;

#endif /* RANDOMBYTES_ARM64CRYPTO_H */
