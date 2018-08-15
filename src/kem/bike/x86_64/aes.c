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

#include "../functions_renaming.h"
#include "aes.h"
#include <wmmintrin.h>
#include <tmmintrin.h>

void AES256_Enc_Intrinsic(OUT const uint8_t *CT,
                          IN const uint8_t *PT,
                          IN const uint8_t *KS) {
	uint32_t i;
	__m128i block = _mm_loadu_si128((__m128i *) PT);
	block = _mm_xor_si128(block, ((__m128i *) KS)[0]);
	for (i = 1; i < 14; i++) {
		block = _mm_aesenc_si128(block, ((__m128i *) KS)[i]);
	}
	block = _mm_aesenclast_si128(block, ((__m128i *) KS)[14]);
	_mm_storeu_si128((__m128i *) (CT), block);
}
