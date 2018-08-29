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

#ifndef __AES_H_INCLUDED__
#define __AES_H_INCLUDED__

#include "types.h"
#include <wmmintrin.h>
#include <tmmintrin.h>

#define MAX_AES_INVOKATION (MASK(32))

#define AES256_KEY_SIZE (32ULL)
#define AES256_KEY_BITS (AES256_KEY_SIZE * 8)
#define AES256_BLOCK_SIZE (16ULL)
#define AES256_ROUNDS (14ULL)

typedef ALIGN(16) struct aes256_key_s {
	uint8_t raw[AES256_KEY_SIZE];
} aes256_key_t;

typedef ALIGN(16) struct aes256_ks_s {
	__m128i keys[AES256_ROUNDS + 1];
} aes256_ks_t;

// The ks parameter must be 16 bytes aligned!
void aes256_key_expansion(OUT aes256_ks_t *ks,
                          IN const aes256_key_t *key);

void aes256_enc_intrinsic(OUT const uint8_t *ct,
                          IN const uint8_t *pt,
                          IN const aes256_ks_t *ks);

#endif // __AES_H_INCLUDED__
