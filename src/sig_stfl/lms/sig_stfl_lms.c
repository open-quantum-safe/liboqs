// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdio.h>
#include <oqs/oqs.h>
#include "./external/config.h"
#include "sig_stfl_lms_wrap.h"
#include "sig_stfl_lms.h"


// ======================== LMS-SHA256 H5/W1 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, uint8_t *secret_key) {
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

	if (sk->length_secret_key) {
		sk->secret_key_data = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
		if (sk->secret_key_data) {
			memset(sk->secret_key_data, 0, sk->length_secret_key);
		} else {
			OQS_SECRET_KEY_LMS_free(sk);
			OQS_MEM_insecure_free(sk);
			sk = NULL;
			return NULL;
		}
	}

	sk->free_key = OQS_SECRET_KEY_LMS_free;

	return sk;
}

void OQS_SECRET_KEY_LMS_free(OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return;
	}

	//TODO: cleanup lock_key

	if (sk->sig) {
		OQS_MEM_insecure_free(sk->sig);
		sk->sig = NULL;
	}
	OQS_MEM_secure_free(sk->secret_key_data, sk->length_secret_key);
	sk->secret_key_data = NULL;
}
