// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0

#include <oqs/oqs.h>
#include <string.h>
#include <stdbool.h>
#include "sig_stfl_xmss.h"

#if defined(__GNUC__) || defined(__clang__)
#define XMSS_UNUSED_ATT __attribute__((unused))
#else
#define XMSS_UNUSED_ATT
#endif

extern inline OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_new(size_t length_secret_key) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SIG_STFL_SECRET_KEY *sk = OQS_MEM_malloc(sizeof(OQS_SIG_STFL_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	OQS_MEM_cleanse(sk, sizeof(OQS_SIG_STFL_SECRET_KEY));

	sk->length_secret_key = length_secret_key;

	// Secret serialize/deserialize function
	sk->serialize_key = OQS_SECRET_KEY_XMSS_serialize_key;
	sk->deserialize_key = OQS_SECRET_KEY_XMSS_deserialize_key;

	// Initialize the key with length_secret_key amount of bytes.
	sk->secret_key_data = (uint8_t *)OQS_MEM_malloc(sk->length_secret_key * sizeof(uint8_t));

	if (sk->secret_key_data == NULL) {
		OQS_MEM_insecure_free(sk);
		return NULL;
	}

	OQS_MEM_cleanse(sk->secret_key_data, sk->length_secret_key);

	// Set application specific context
	sk->context = NULL;

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

/* Serialize XMSS secret key data into a byte string, return an allocated buffer. Users have to unallocated the buffer. */
OQS_STATUS OQS_SECRET_KEY_XMSS_serialize_key(uint8_t **sk_buf_ptr, size_t *sk_len, const OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL || sk_len == NULL || sk_buf_ptr == NULL) {
		return OQS_ERROR;
	}

	/* Lock the key if possible */
	if (OQS_SECRET_KEY_XMSS_acquire_lock(sk) != OQS_SUCCESS) {
		return OQS_ERROR;
	}

	uint8_t *sk_buf = OQS_MEM_malloc(sk->length_secret_key * sizeof(uint8_t));
	if (sk_buf == NULL) {
		return OQS_ERROR;
	}

	// Simply copy byte string of secret_key_data
	memcpy(sk_buf, sk->secret_key_data, sk->length_secret_key);

	*sk_buf_ptr = sk_buf;
	*sk_len = sk->length_secret_key;

	/* Unlock the key if possible */
	if (OQS_SECRET_KEY_XMSS_release_lock(sk) != OQS_SUCCESS) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

/* Only for internal use. Similar to OQS_SECRET_KEY_XMSS_serialize_key, but this function does not aquire and release lock. */
OQS_STATUS OQS_SECRET_KEY_XMSS_inner_serialize_key(uint8_t **sk_buf_ptr, size_t *sk_len, const OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL || sk_len == NULL || sk_buf_ptr == NULL) {
		return OQS_ERROR;
	}

	uint8_t *sk_buf = OQS_MEM_malloc(sk->length_secret_key * sizeof(uint8_t));
	if (sk_buf == NULL) {
		return OQS_ERROR;
	}

	// Simply copy byte string of secret_key_data
	memcpy(sk_buf, sk->secret_key_data, sk->length_secret_key);

	*sk_buf_ptr = sk_buf;
	*sk_len = sk->length_secret_key;

	return OQS_SUCCESS;
}

/* Deserialize XMSS byte string into an XMSS secret key data. */
OQS_STATUS OQS_SECRET_KEY_XMSS_deserialize_key(OQS_SIG_STFL_SECRET_KEY *sk, const uint8_t *sk_buf, const size_t sk_len, XMSS_UNUSED_ATT void *context) {
#ifndef OQS_ALLOW_XMSS_KEY_AND_SIG_GEN
	return OQS_ERROR;
#endif

	if (sk == NULL || sk_buf == NULL || (sk_len != sk->length_secret_key)) {
		return OQS_ERROR;
	}

	memcpy(sk->secret_key_data, sk_buf, sk->length_secret_key);
	sk->context = context;

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

OQS_STATUS OQS_SECRET_KEY_XMSS_acquire_lock(const OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return OQS_ERROR;
	}

	/* Lock the key if possible, otherwise return OQS_ERROR because the lock_key, unlock_key and mutex are not defined.*/
	if ((sk->lock_key != NULL) && (sk->mutex != NULL) && (sk->unlock_key != NULL)) {
		if (sk->lock_key(sk->mutex) != OQS_SUCCESS) {
			return OQS_ERROR;
		}
	}

	return OQS_SUCCESS;
}

OQS_STATUS OQS_SECRET_KEY_XMSS_release_lock(const OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return OQS_ERROR;
	}

	/* Unlock the key if possible, otherwise return OQS_ERROR because the lock_key, unlock_key and mutex are not defined. */
	if ((sk->unlock_key != NULL) && (sk->mutex != NULL) && (sk->lock_key != NULL)) {
		if (sk->unlock_key(sk->mutex) != OQS_SUCCESS) {
			return OQS_ERROR;
		}
	}

	return OQS_SUCCESS;
}
