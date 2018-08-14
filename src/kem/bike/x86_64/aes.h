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

#define AES256_KEY_SIZE 32ULL
#define AES256_KEY_BITS (AES256_KEY_SIZE * 8)
#define AES256_BLOCK_SIZE 16ULL

#define MAX_AES_INVOKATION (MASK(32))

void AES_256_Key_Expansion(OUT uint8_t *ks, IN const uint8_t *key);

void AES256_Enc_Intrinsic(OUT const uint8_t *ct,
                          IN const uint8_t *pt,
                          IN const uint8_t *ks);

#endif //__AES_H_INCLUDED__
