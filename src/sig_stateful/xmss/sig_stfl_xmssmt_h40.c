// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./external/xmss.h"
#include "./external/params.h"
#include "sig_stfl_xmss_xmssmt.h"
#include "./external/xmss_namespace.h"

// ======================== XMSSMT-SHA256_2_H40 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	uint32_t oid = 0x00000003;
	if (xmssmt_keypair(public_key, secret_key, oid) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->method_name = "XMSSMT-SHA2_40/2_256";
	sig->alg_version = "...";

	// Check how true this is
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmssmt_sign;
	sig->verify = OQS_SIG_STFL_alg_xmssmt_verify;
	sig->derive_subkey = OQS_SIG_STFL_alg_xmssmt_derive_subkey;

	return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_2_H40_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_length_sk;

	perform_key_allocation(sk);

	return sk;
}

// ================================================================ //



// ======================== XMSSMT-SHA256_4_H40 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	uint32_t oid = 0x00000004;
	if (xmssmt_keypair(public_key, secret_key, oid) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->method_name = "XMSSMT-SHA2_40/4_256";
	sig->alg_version = "...";

	// Check how true this is
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmssmt_sign;
	sig->verify = OQS_SIG_STFL_alg_xmssmt_verify;
	sig->derive_subkey = OQS_SIG_STFL_alg_xmssmt_derive_subkey;

	return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_4_H40_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_length_sk;

	perform_key_allocation(sk);

	return sk;
}

// ================================================================ //


// ======================== XMSSMT-SHA256_8_H40 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	uint32_t oid = 0x00000005;
	if (xmssmt_keypair(public_key, secret_key, oid) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->method_name = "XMSSMT-SHA2_40/8_256";
	sig->alg_version = "...";

	// Check how true this is
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmssmt_sign;
	sig->verify = OQS_SIG_STFL_alg_xmssmt_verify;
	sig->derive_subkey = OQS_SIG_STFL_alg_xmssmt_derive_subkey;

	return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_8_H40_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_length_sk;

	perform_key_allocation(sk);

	return sk;
}

// ================================================================ //
