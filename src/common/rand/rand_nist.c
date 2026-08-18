//
//  rng.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
/*
NIST-developed software is provided by NIST as a public service. You may use, copy, and distribute copies of the software in any medium, provided that you keep intact this entire notice. You may improve, modify, and create derivative works of the software or any portion of the software, and you may copy and distribute such modifications or works. Modified works should carry a notice stating that you changed the software and should note the date and nature of any such change. Please explicitly acknowledge the National Institute of Standards and Technology as the source of the software.

NIST-developed software is expressly provided "AS IS." NIST MAKES NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT, OR ARISING BY OPERATION OF LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND DATA ACCURACY. NIST NEITHER REPRESENTS NOR WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST DOES NOT WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE USE OF THE SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.

You are solely responsible for determining the appropriateness of using and distributing the software and you assume all risks associated with its use, including but not limited to the risks and costs of program errors, compliance with applicable laws, damage to or loss of data, programs or equipment, and the unavailability or interruption of operation. This software is not intended to be used in any situation where a failure could cause risk of injury or damage to property. The software developed by NIST employees is not subject to copyright protection within the United States.
*/
//  SPDX-License-Identifier: Unknown
//  Modified for liboqs by Douglas Stebila and Spencer Wilson
//

#include <assert.h>
#include <string.h>

#include <oqs/common.h>
#include <oqs/rand.h>
#include <oqs/rand_nist.h>

#ifdef OQS_USE_OPENSSL
#include "../ossl_helpers.h"
#else
#include <oqs/aes.h>
#endif

void OQS_randombytes_nist_kat(unsigned char *x, size_t xlen);

static OQS_NIST_DRBG_struct DRBG_ctx;
static void AES256_CTR_DRBG_Update(unsigned char *provided_data, unsigned char *Key, unsigned char *V);

// Use whatever AES implementation you have. This uses AES from openSSL library
//    key - 256-bit AES key
//    ctr - a 128-bit plaintext value
//    buffer - a 128-bit ciphertext value
static void AES256_ECB(unsigned char *key, unsigned char *ctr, unsigned char *buffer) {
#ifdef OQS_USE_OPENSSL
	EVP_CIPHER_CTX *ctx;

	int len;

	/* Create and initialise the context */
	ctx = OSSL_FUNC(EVP_CIPHER_CTX_new)();
	OQS_EXIT_IF_NULLPTR(ctx, "OpenSSL");

	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptInit_ex)(ctx, oqs_aes_256_ecb(), NULL, key, NULL));
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptUpdate)(ctx, buffer, &len, ctr, 16));

	/* Clean up */
	OSSL_FUNC(EVP_CIPHER_CTX_free)(ctx);
#else
	void *schedule = NULL;
	OQS_AES256_ECB_load_schedule(key, &schedule);
	OQS_AES256_ECB_enc(ctr, 16, key, buffer);
	OQS_AES256_free_schedule(schedule);
#endif
}

void OQS_randombytes_nist_kat_init_256bit(const uint8_t *entropy_input, const uint8_t *personalization_string) {
	unsigned char seed_material[48];

	memcpy(seed_material, entropy_input, 48);
	if (personalization_string)
		for (int i = 0; i < 48; i++) {
			seed_material[i] ^= personalization_string[i];
		}
	OQS_MEM_cleanse(DRBG_ctx.Key, 32);
	memset(DRBG_ctx.V, 0x00, 16);
	AES256_CTR_DRBG_Update(seed_material, DRBG_ctx.Key, DRBG_ctx.V);
	DRBG_ctx.reseed_counter = 1;
}

void OQS_randombytes_nist_kat(unsigned char *x, size_t xlen) {
	unsigned char block[16];
	int i = 0;

	while (xlen > 0) {
		//increment V
		for (int j = 15; j >= 0; j--) {
			if (DRBG_ctx.V[j] == 0xff) {
				DRBG_ctx.V[j] = 0x00;
			} else {
				DRBG_ctx.V[j]++;
				break;
			}
		}
		AES256_ECB(DRBG_ctx.Key, DRBG_ctx.V, block);
		if (xlen > 15) {
			memcpy(x + i, block, 16);
			i += 16;
			xlen -= 16;
		} else {
			memcpy(x + i, block, xlen);
			xlen = 0;
		}
	}
	AES256_CTR_DRBG_Update(NULL, DRBG_ctx.Key, DRBG_ctx.V);
	DRBG_ctx.reseed_counter++;
}

void OQS_randombytes_nist_kat_get_state(void *out) {
	OQS_NIST_DRBG_struct *out_state = (OQS_NIST_DRBG_struct *)out;
	if (out_state != NULL) {
		memcpy(out_state->Key, DRBG_ctx.Key, sizeof(DRBG_ctx.Key));
		memcpy(out_state->V, DRBG_ctx.V, sizeof(DRBG_ctx.V));
		out_state->reseed_counter = DRBG_ctx.reseed_counter;
	}
}

void OQS_randombytes_nist_kat_set_state(const void *in) {
	const OQS_NIST_DRBG_struct *in_state = (const OQS_NIST_DRBG_struct *)in;
	if (in_state != NULL) {
		memcpy(DRBG_ctx.Key, in_state->Key, sizeof(DRBG_ctx.Key));
		memcpy(DRBG_ctx.V, in_state->V, sizeof(DRBG_ctx.V));
		DRBG_ctx.reseed_counter = in_state->reseed_counter;
	}
}

static void AES256_CTR_DRBG_Update(unsigned char *provided_data, unsigned char *Key, unsigned char *V) {
	unsigned char temp[48];

	for (int i = 0; i < 3; i++) {
		//increment V
		for (int j = 15; j >= 0; j--) {
			if (V[j] == 0xff) {
				V[j] = 0x00;
			} else {
				V[j]++;
				break;
			}
		}

		AES256_ECB(Key, V, temp + 16 * i);
	}
	if (provided_data != NULL)
		for (int i = 0; i < 48; i++) {
			temp[i] ^= provided_data[i];
		}
	memcpy(Key, temp, 32);
	memcpy(V, temp + 32, 16);
}
