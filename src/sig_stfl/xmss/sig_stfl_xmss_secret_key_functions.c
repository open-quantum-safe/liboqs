// SPDX-License-Identifier: MIT

#include <oqs/oqs.h>
#include <string.h>
#include "sig_stfl_xmss.h"

#if defined(__GNUC__) || defined(__clang__)
#define XMSS_UNUSED_ATT __attribute__((unused))
#else
#define XMSS_UNUSED_ATT
#endif

/* Serialize XMSS secret key data into a byte string */
OQS_STATUS OQS_SECRET_KEY_XMSS_serialize_key(const OQS_SIG_STFL_SECRET_KEY *sk, size_t *sk_len, uint8_t **sk_buf_ptr) {
	if (sk == NULL || sk_len == NULL || sk_buf_ptr == NULL) {
		return OQS_ERROR;
	}

	// Lock the key if possible
	if (sk->lock_key != NULL && sk->mutex != NULL) {
		sk->lock_key(sk->mutex);
	}

	uint8_t *sk_buf = malloc(sk->length_secret_key * sizeof(uint8_t));
	if (sk_buf == NULL) {
		return OQS_ERROR;
	}

	// Simply copy byte string of secret_key_data
	memcpy(sk_buf, sk->secret_key_data, sk->length_secret_key);

	*sk_buf_ptr = sk_buf;
	*sk_len = sk->length_secret_key;

	// Unlock the key if possible
	if (sk->unlock_key != NULL && sk->mutex != NULL) {
		sk->unlock_key(sk->mutex);
	}

	return OQS_SUCCESS;
}

/* Deserialize XMSS byte string into an XMSS secret key data */
OQS_STATUS OQS_SECRET_KEY_XMSS_deserialize_key(OQS_SIG_STFL_SECRET_KEY *sk, const size_t sk_len, const uint8_t *sk_buf, XMSS_UNUSED_ATT void *context) {
	if (sk == NULL || sk_buf == NULL || (sk_len != sk->length_secret_key)) {
		return OQS_ERROR;
	}

	// Lock the key if possible
	if (sk->lock_key != NULL && sk->mutex != NULL) {
		sk->lock_key(sk->mutex);
	}

	sk->secret_key_data = malloc(sk_len);
	if (sk->secret_key_data == NULL) {
		return OQS_ERROR;
	}

	sk->context = context;
	memcpy(sk->secret_key_data, sk_buf, sk_len);

	// Unlock the key if possible
	if (sk->unlock_key != NULL && sk->mutex != NULL) {
		sk->unlock_key(sk->mutex);
	}

	return OQS_SUCCESS;
}

void OQS_SECRET_KEY_XMSS_set_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context) {
	if (sk == NULL || store_cb == NULL) {
		return;
	}
	sk->secure_store_scrt_key = store_cb;
	sk->context = context;
}

void OQS_SECRET_KEY_XMSS_free(OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return;
	}

	OQS_MEM_secure_free(sk->secret_key_data, sk->length_secret_key);
	sk->secret_key_data = NULL;
}
