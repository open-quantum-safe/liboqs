// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0

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

/* -------------- XMSSMT -------------- */
#ifndef OQS_ALLOW_STFL_KEY_AND_SIG_GEN
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sign(XMSS_UNUSED_ATT uint8_t *signature, XMSS_UNUSED_ATT size_t *signature_len, XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len,
        XMSS_UNUSED_ATT OQS_SIG_STFL_SECRET_KEY *secret_key) {
	return OQS_ERROR;
}
#else
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sign(uint8_t *signature, size_t *signature_len, XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, XMSS_UNUSED_ATT OQS_SIG_STFL_SECRET_KEY *secret_key) {

	OQS_STATUS status = OQS_SUCCESS;
	uint8_t *sk_key_buf_ptr = NULL;
	unsigned long long sig_length = 0;
	size_t sk_key_buf_len = 0;

	if (signature == NULL || signature_len == NULL || message == NULL || secret_key == NULL || secret_key->secret_key_data == NULL) {
		return OQS_ERROR;
	}

	/* Don't even attempt signing without a way to safe the updated private key */
	if (secret_key->secure_store_scrt_key == NULL) {
		return OQS_ERROR;
	}

	/* Lock secret to ensure OTS use */
	if (OQS_SECRET_KEY_XMSS_acquire_lock(secret_key) != OQS_SUCCESS) {
		return OQS_ERROR;
	}

	if (xmssmt_sign(secret_key->secret_key_data, signature, &sig_length, message, message_len)) {
		status = OQS_ERROR;
		goto err;
	}
	*signature_len = (size_t)sig_length;
	/*
	 * serialize and securely store the updated private key
	 * regardless, delete signature and the serialized key other wise
	 */

	status = OQS_SECRET_KEY_XMSS_inner_serialize_key(&sk_key_buf_ptr, &sk_key_buf_len, secret_key);
	if (status != OQS_SUCCESS) {
		goto err;
	}

	// Store updated private key securely
	status = secret_key->secure_store_scrt_key(sk_key_buf_ptr, sk_key_buf_len, secret_key->context);
	OQS_MEM_secure_free(sk_key_buf_ptr, sk_key_buf_len);

err:
	/* Unlock the key if possible */
	if (OQS_SECRET_KEY_XMSS_release_lock(secret_key) != OQS_SUCCESS) {
		return OQS_ERROR;
	}

	return status;
}
#endif

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_verify(XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, const uint8_t *signature, size_t signature_len, XMSS_UNUSED_ATT const uint8_t *public_key) {

	if (message == NULL || signature == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	if (xmssmt_sign_open(message, (unsigned long long)message_len, signature, (unsigned long long)signature_len, public_key)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	if (remain == NULL || secret_key == NULL || secret_key->secret_key_data == NULL) {
		return OQS_ERROR;
	}

	if (xmssmt_remaining_signatures(remain, secret_key->secret_key_data)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	if (total == NULL || secret_key == NULL || secret_key->secret_key_data == NULL) {
		return OQS_ERROR;
	}

	if (xmssmt_total_signatures(total, secret_key->secret_key_data)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}
