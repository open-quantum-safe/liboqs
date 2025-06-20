/***************************************************************************
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*  
* Licensed under the Apache License, Version 2.0 (the "License").
* You may not use this file except in compliance with the License.
* A copy of the License is located at
*  
*     http://www.apache.org/licenses/LICENSE-2.0
*  
* or in the "license" file accompanying this file. This file is distributed 
* on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
* express or implied. See the License for the specific language governing 
* permissions and limitations under the License.
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#pragma once

#include <stdint.h>
#include <wmmintrin.h>
#include "defs.h"

#define MAX_AES_INVOKATION (MASK(32))

#define AES256_KEY_SIZE (32ULL)
#define AES256_KEY_BITS (AES256_KEY_SIZE * 8)
#define AES_BLOCK_SIZE (16ULL)
#define AES256_ROUNDS (14ULL)

#ifdef VAES256
#define PAR_AES_BLOCK_SIZE (AES_BLOCK_SIZE*2)
#elif defined(VAES512)
#define PAR_AES_BLOCK_SIZE (AES_BLOCK_SIZE*4)
#endif

typedef ALIGN(16) struct aes256_key_s {
    uint8_t raw[AES256_KEY_SIZE];
} aes256_key_t;

typedef ALIGN(16) struct aes256_ks_s {
    __m128i keys[AES256_ROUNDS + 1];
} aes256_ks_t;

// The ks parameter must be 16 bytes aligned!
EXTERNC void aes256_key_expansion(OUT aes256_ks_t *ks,
                                  IN const aes256_key_t *key);

// Encrypt one 128-bit block ct = E(pt,ks)
void aes256_enc(OUT uint8_t *ct,
                IN const uint8_t *pt,
                IN const aes256_ks_t *ks);

// Encrypt num_blocks 128-bit blocks 
// ct[15:0] = E(pt[15:0],ks)
// ct[31:16] = E(pt[15:0] + 1,ks)
// ...
// ct[16*num_blocks - 1:16*(num_blocks-1)] = E(pt[15:0] + num_blocks,ks)
void aes256_ctr_enc(OUT uint8_t *ct,
                    IN const uint8_t *pt,
                    IN const uint32_t num_blocks,
                    IN const aes256_ks_t *ks);

// Encrypt num_blocks 128-bit blocks using VAES (AVX-2)
// ct[15:0] = E(pt[15:0],ks)
// ct[31:16] = E(pt[15:0] + 1,ks)
// ...
// ct[16*num_blocks - 1:16*(num_blocks-1)] = E(pt[15:0] + num_blocks,ks)
void aes256_ctr_enc256(OUT uint8_t *ct,
                       IN const uint8_t *ctr,
                       IN const uint32_t num_blocks,
                       IN const aes256_ks_t *ks);

// Encrypt num_blocks 128-bit blocks using VAES (AVX512)
// ct[15:0] = E(pt[15:0],ks)
// ct[31:16] = E(pt[15:0] + 1,ks)
// ...
// ct[16*num_blocks - 1:16*(num_blocks-1)] = E(pt[15:0] + num_blocks,ks)
void aes256_ctr_enc512(OUT uint8_t *ct,
                       IN const uint8_t *ctr,
                       IN const uint32_t num_blocks,
                       IN const aes256_ks_t *ks);
