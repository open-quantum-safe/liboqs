// SPDX-License-Identifier: MIT

/**
 * Glue code between SNOVA and liboqs
 */

#include <oqs/oqs.h>

#include "snova.h"

// Size of the message digest in the hash-and-sign paragdigm.
#define SNOVA_BYTES_DIGEST 64

OQS_STATUS SNOVA_NAMESPACE(keypair)(uint8_t *pk, uint8_t *sk) {
	uint8_t seed_pair[seed_length];
	uint8_t *pt_private_key_seed;
	uint8_t *pt_public_key_seed;

	OQS_randombytes(seed_pair, seed_length);
	pt_public_key_seed = seed_pair;
	pt_private_key_seed = seed_pair + seed_length_public;

#if sk_is_seed
	int res = generate_keys_ssk(pk, sk, pt_public_key_seed, pt_private_key_seed);
#else
	int res = generate_keys_esk(pk, sk, pt_public_key_seed, pt_private_key_seed);
#endif

	if (res) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_STATUS SNOVA_NAMESPACE(sign)(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len,
                                 const uint8_t *secret_key) {
	uint8_t digest[SNOVA_BYTES_DIGEST];
	uint8_t salt[bytes_salt];

	shake256(message, message_len, digest, SNOVA_BYTES_DIGEST);

	OQS_randombytes(salt, bytes_salt);
#if sk_is_seed
	int res = sign_digest_ssk(signature, digest, SNOVA_BYTES_DIGEST, salt, secret_key);
#else
	int res = sign_digest_esk(signature, digest, SNOVA_BYTES_DIGEST, salt, secret_key);
#endif
	*signature_len = bytes_sig_with_salt;

	if (res) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_STATUS SNOVA_NAMESPACE(verify)(const uint8_t *signature, size_t signature_len, const uint8_t *message, size_t message_len,
                                   const uint8_t *pk) {
	if (signature_len != bytes_sig_with_salt) {
		return OQS_ERROR;
	}

	uint8_t digest[SNOVA_BYTES_DIGEST];
	shake256(message, message_len, digest, SNOVA_BYTES_DIGEST);

	if (verify_signture(digest, SNOVA_BYTES_DIGEST, signature, pk)) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}
