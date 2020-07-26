/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker and Shay Gueron
 * AWS Cryptographic Algorithms Group.
 * (ndrucker@amazon.com, gueron@amazon.com)
 */

#pragma once

#include "cleanup.h"
#include <oqs/aes.h>

#define MAX_AES_INVOKATION (MASK(32))

#define AES256_KEY_SIZE   (32U)
#define AES256_KEY_BITS   (AES256_KEY_SIZE * 8)
#define AES256_BLOCK_SIZE (16U)
#define AES256_ROUNDS     (14U)

typedef ALIGN(16) struct aes256_key_s {
	uint8_t raw[AES256_KEY_SIZE];
} aes256_key_t;

_INLINE_ void
aes256_key_cleanup(aes256_key_t *o) {
	secure_clean(o->raw, sizeof(*o));
}

// Using OpenSSL structures
typedef void *aes256_ks_t;

_INLINE_ ret_t
aes256_key_expansion(OUT aes256_ks_t *ks, IN const aes256_key_t *key) {
	OQS_AES256_ECB_load_schedule(key->raw, ks, 1);
	return SUCCESS;
}

_INLINE_ ret_t
aes256_enc(OUT uint8_t *ct, IN const uint8_t *pt, IN const aes256_ks_t ks) {
	OQS_AES256_ECB_enc_sch(pt, AES256_BLOCK_SIZE, ks, ct);
	return SUCCESS;
}

_INLINE_ void
aes256_free_ks(OUT aes256_ks_t *ks) {
	OQS_AES256_free_schedule(*ks);
	ks = NULL;
}
