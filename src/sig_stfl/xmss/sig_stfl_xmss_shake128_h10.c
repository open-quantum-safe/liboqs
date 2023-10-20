// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdlib.h>

#include <oqs/oqs.h>
#include "sig_stfl_xmss.h"

#include "external/xmss.h"

#if defined(__GNUC__) || defined(__clang__)
#define XMSS_UNUSED_ATT __attribute__((unused))
#else
#define XMSS_UNUSED_ATT
#endif

// ======================== XMSS-SHAKE_10_256 ======================== //

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake128_h10_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->oid = OQS_SIG_STFL_alg_xmss_shake128_h10_oid;
	sig->method_name = "XMSS-SHAKE_10_256";
	sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8391";
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmss_shake128_h10_length_pk;
	sig->length_secret_key = OQS_SIG_STFL_alg_xmss_shake128_h10_length_sk;
	sig->length_signature = OQS_SIG_STFL_alg_xmss_shake128_h10_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmss_shake128_h10_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmss_shake128_h10_sign;
	sig->verify = OQS_SIG_STFL_alg_xmss_shake128_h10_verify;
	sig->sigs_remaining = OQS_SIG_STFL_alg_xmss_shake128_h10_sigs_remaining;
	sig->sigs_total = OQS_SIG_STFL_alg_xmss_shake128_h10_sigs_total;

	return sig;
}

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE128_H10_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SIG_STFL_SECRET_KEY *sk = malloc(sizeof(OQS_SIG_STFL_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SIG_STFL_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmss_shake128_h10_length_sk;

	// Secret serialize/deserialize function
	sk->serialize_key = OQS_SECRET_KEY_XMSS_serialize_key;
	sk->deserialize_key = OQS_SECRET_KEY_XMSS_deserialize_key;

	// Initialize the key with length_secret_key amount of bytes.
	sk->secret_key_data = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));

	if (sk->secret_key_data == NULL) {
		OQS_MEM_insecure_free(sk);
		return NULL;
	}
	memset(sk->secret_key_data, 0, sk->length_secret_key);

	// Point to associated OQS_SIG_STFL object
	sk->sig = NULL;

	// Mutual exclusion struct
	sk->mutex = NULL;

	// Set Secret Key locking function
	sk->lock_key = NULL;

	// Set Secret Key unlocking / releasing function
	sk->unlock_key = NULL;

	// Set Secret Key saving function
	sk->secure_store_scrt_key = NULL;

	// Set Secret Key store callback function
	sk->set_scrt_key_store_cb = OQS_SECRET_KEY_XMSS_set_store_cb;

	// Set Secret Key free function
	sk->free_key = OQS_SECRET_KEY_XMSS_free;

	return sk;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_keypair(XMSS_UNUSED_ATT uint8_t *public_key, XMSS_UNUSED_ATT OQS_SIG_STFL_SECRET_KEY *secret_key) {

	if (public_key == NULL || secret_key == NULL || secret_key->secret_key_data == NULL) {
		return OQS_ERROR;
	}

	if (xmss_keypair(public_key, secret_key->secret_key_data, OQS_SIG_STFL_alg_xmss_shake128_h10_oid)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_sign(uint8_t *signature, size_t *signature_len, XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, XMSS_UNUSED_ATT OQS_SIG_STFL_SECRET_KEY *secret_key) {

	if (signature == NULL || signature_len == NULL || message == NULL || secret_key == NULL || secret_key->secret_key_data == NULL) {
		return OQS_ERROR;
	}

	unsigned long long sig_length = 0;
	if (xmss_sign(secret_key->secret_key_data, signature, &sig_length, message, message_len)) {
		return OQS_ERROR;
	}
	*signature_len = (size_t) sig_length;

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_verify(XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, const uint8_t *signature, size_t signature_len, XMSS_UNUSED_ATT const uint8_t *public_key) {

	if (message == NULL || signature == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	if (xmss_sign_open(message, (unsigned long long) message_len, signature, (unsigned long long) signature_len, public_key)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	if (remain == NULL || secret_key == NULL || secret_key->secret_key_data == NULL) {
		return OQS_ERROR;
	}

	if (xmss_remaining_signatures(remain, secret_key->secret_key_data)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	if (total == NULL || secret_key == NULL || secret_key->secret_key_data == NULL) {
		return OQS_ERROR;
	}

	if (xmss_total_signatures(total, secret_key->secret_key_data)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}
