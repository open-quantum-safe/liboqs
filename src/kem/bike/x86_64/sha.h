/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#ifndef _MB_SHA_H_
#define _MB_SHA_H_

#include "types.h"

#define SHA384_HASH_SIZE 48ULL
#define SHA384_HASH_QWORDS (SHA384_HASH_SIZE / 8)

#define SHA512_HASH_SIZE 64ULL
#define SHA512_HASH_QWORDS (SHA512_HASH_SIZE / 8)

typedef struct sha384_hash_s {
	union {
		uint8_t raw[SHA384_HASH_SIZE];
		uint64_t qw[SHA384_HASH_QWORDS];
	} u;
} sha384_hash_t;
static_assert(sizeof(sha384_hash_t) == SHA384_HASH_SIZE, sha384_hash_size);

typedef struct sha512_hash_s {
	union {
		uint8_t raw[SHA512_HASH_SIZE];
		uint64_t qw[SHA512_HASH_QWORDS];
	} u;
} sha512_hash_t;
static_assert(sizeof(sha512_hash_t) == SHA512_HASH_SIZE, sha512_hash_size);

typedef struct {
	uint8_t *ptr;
	uint32_t blocks;
} hash_desc;

#include "sha384.h"

int sha(OUT sha_hash_t *hash_out,
        IN const uint32_t byte_length,
        IN const uint8_t *msg);

void sha_mb(OUT sha_hash_t *hash_out,
            IN const uint8_t *msg,
            IN const uint32_t byte_length,
            IN const uint32_t num);

#endif // _MB_SHA_H_
