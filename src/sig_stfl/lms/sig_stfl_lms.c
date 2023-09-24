// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdio.h>
#include <oqs/oqs.h>
#include "./external/config.h"
#include "sig_stfl_lms_wrap.h"
#include "sig_stfl_lms.h"

/* Convert LMS secret key object to byte string */
static OQS_STATUS OQS_SECRET_KEY_LMS_serialize_key(const OQS_SIG_STFL_SECRET_KEY *sk, size_t *sk_len, uint8_t **sk_buf_ptr);

/* Insert lms byte string in an LMS secret key object */
static OQS_STATUS OQS_SECRET_KEY_LMS_deserialize_key(OQS_SIG_STFL_SECRET_KEY *sk, const size_t sk_len, const uint8_t *sk_buf, void *context);

static void OQS_SECRET_KEY_LMS_set_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context);

// ======================== LMS-SHA256 H5/W1 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	if (oqs_sig_stfl_lms_keypair(public_key, secret_key, (const uint32_t)0x00000001) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w1_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->oid = 0x00000001;
	sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1;
	sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_signature;
	sig->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_sk;

	sig->keypair = OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair;
	sig->sign = OQS_SIG_STFL_alg_lms_sign;
	sig->verify = OQS_SIG_STFL_alg_lms_verify;

	sig->sigs_remaining = OQS_SIG_STFL_lms_sigs_left;
	sig->sigs_total = OQS_SIG_STFL_lms_sigs_total;

	return sig;
}

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W1_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SIG_STFL_SECRET_KEY *sk = malloc(sizeof(OQS_SIG_STFL_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SIG_STFL_SECRET_KEY));

	// Initialize the key with length_secret_key amount of bytes.
	sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_sk;

	/* Function that returns the total number of signatures for the secret key */
	sk->sigs_total = NULL;

	/* set Function to returns the number of signatures left for the secret key */
	sk->sigs_left = NULL;

	/*
	 * Secret Key retrieval Function
	 */
	sk->serialize_key = OQS_SECRET_KEY_LMS_serialize_key;

	/*
	 * set Secret Key to internal structure Function
	 */
	sk->deserialize_key = OQS_SECRET_KEY_LMS_deserialize_key;

	/*
	 * Set Secret Key Locking Function
	 */
	sk->lock_key = NULL;

	/*
	 * Set Secret Key Unlocking / Releasing Function
	 */
	sk->unlock_key = NULL;

	/*
	 * Set Secret Key Saving Function
	 */
	sk->secure_store_scrt_key = NULL;

	/*
	 * Set Secret Key free function
	 */
	sk->free_key = OQS_SECRET_KEY_LMS_free;

	sk->set_scrt_key_store_cb = OQS_SECRET_KEY_LMS_set_store_cb;

	return sk;
}

void OQS_SECRET_KEY_LMS_free(OQS_SIG_STFL_SECRET_KEY *sk) {
	oqs_secret_lms_key_free(sk);
}

/* Convert LMS secret key object to byte string */
static OQS_STATUS OQS_SECRET_KEY_LMS_serialize_key(const OQS_SIG_STFL_SECRET_KEY *sk, size_t *sk_len, uint8_t **sk_buf_ptr) {
    OQS_STATUS status;
    if (sk->lock_key && sk->mutex) {
        sk->lock_key(sk->mutex);
    }

    status = oqs_serialize_lms_key(sk, sk_len, sk_buf_ptr);

    if (sk->unlock_key && sk->mutex) {
        sk->unlock_key(sk->mutex);
    }
    return status;
}

/* Insert lms byte string in an LMS secret key object */
static OQS_STATUS OQS_SECRET_KEY_LMS_deserialize_key(OQS_SIG_STFL_SECRET_KEY *sk, const size_t sk_len, const uint8_t *sk_buf, void *context) {
	return oqs_deserialize_lms_key(sk, sk_len, sk_buf, context);
}

static void OQS_SECRET_KEY_LMS_set_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context) {
	if (sk && store_cb && context) {
		oqs_lms_key_set_store_cb(sk, store_cb, context);
	}
}
