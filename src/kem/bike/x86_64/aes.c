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

#include "aes.h"

void aes256_enc_intrinsic(OUT const uint8_t *ct,
                          IN const uint8_t *pt,
                          IN const aes256_ks_t *ks) {
	uint32_t i = 0;
	volatile __m128i block = _mm_loadu_si128((__m128i *) pt);

	block = _mm_xor_si128(block, ks->keys[0]);
	for (i = 1; i < AES256_ROUNDS; i++) {
		block = _mm_aesenc_si128(block, ks->keys[i]);
	}
	block = _mm_aesenclast_si128(block, ks->keys[AES256_ROUNDS]);

	_mm_storeu_si128((__m128i *) (ct), block);

	// Clear the secret data when done
	block = _mm_setzero_si128();
}
