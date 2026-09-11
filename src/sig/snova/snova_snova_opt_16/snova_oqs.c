// SPDX-License-Identifier: MIT

/**
 * Glue code between SNOVA and liboqs
 *
 * Copyright (c) 2026 SNOVA TEAM
 */

#include <oqs/oqs.h>
#include <string.h>

#include "snova.h"

#define SKIP_PK_EXPAND

#include "symmetric.h"

OQS_STATUS SNOVA_NAMESPACE(oqs_keypair)(uint8_t *pk, uint8_t *sk) {
	uint8_t seed_pair[SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE];
	uint8_t *pt_public_key_seed;

	OQS_randombytes(seed_pair, SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE);
	pt_public_key_seed = seed_pair;

	int res = SNOVA_NAMESPACE(genkeys)(pk, sk, pt_public_key_seed);

	if (res) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_STATUS SNOVA_NAMESPACE(oqs_sign)(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len,
                                     const uint8_t *secret_key) {
	uint8_t digest[BYTES_DIGEST];
	uint8_t salt[BYTES_SALT];
	uint8_t sig[BYTES_SIGNATURE];
	int res;

	OQS_randombytes(salt, BYTES_SALT);

#if SNOVA_OPT == 5
	expanded_SK *pskx = (expanded_SK *)secret_key;
#else
	expanded_SK skx_d;
	expanded_SK *pskx = &skx_d;
	res = SNOVA_NAMESPACE(sk_expand)(pskx, secret_key);
	if (res) {
		return OQS_ERROR;
	}
#endif

	shake256(digest, BYTES_DIGEST, message, message_len);

	res = SNOVA_NAMESPACE(sign)(pskx, sig, digest, BYTES_DIGEST, salt);
	if (res) {
		return OQS_ERROR;
	} else {
		memcpy(signature, sig, BYTES_SIGNATURE);
		*signature_len = BYTES_SIGNATURE;
		return OQS_SUCCESS;
	}
}

OQS_STATUS SNOVA_NAMESPACE(oqs_verify)(const uint8_t *signature, size_t signature_len, const uint8_t *message,
                                       size_t message_len, const uint8_t *pk) {
	if (signature_len != BYTES_SIGNATURE) {
		return OQS_ERROR;
	}

	int res;

#if SNOVA_OPT == 5
	expanded_PK *ppkx = (expanded_PK *)pk;
#else
	expanded_PK pkx;
	expanded_PK *ppkx = &pkx;
	res = SNOVA_NAMESPACE(pk_expand)(ppkx, pk);
	if (res) {
		return OQS_ERROR;
	}
#endif

	uint8_t digest[BYTES_DIGEST];
	shake256(digest, BYTES_DIGEST, message, message_len);

	res = SNOVA_NAMESPACE(verify)(ppkx, signature, digest, BYTES_DIGEST);
	if (res) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}
