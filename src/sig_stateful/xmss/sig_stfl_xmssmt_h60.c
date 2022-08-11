// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./external/xmss.h"
#include "./external/params.h"
#include "sig_stfl_xmss_xmssmt.h"
#include "./external/xmss_namespace.h"

// ======================== XMSSMT-SHA256_3_H60 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	uint32_t oid = 0x00000006;
	if (xmssmt_keypair(public_key, secret_key, oid) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->method_name = "XMSSMT-SHA2_60/3_256";
	sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8391";

	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmssmt_sign;
	sig->verify = OQS_SIG_STFL_alg_xmssmt_verify;
	sig->derive_subkey = OQS_SIG_STFL_alg_xmssmt_derive_subkey;

	return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_3_H60_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_length_sk;

	// Assign the sigs_left and sigs_max functions
	sk->sigs_left = OQS_SECRET_KEY_xmss_sigs_left;
	sk->sigs_total = OQS_SECRET_KEY_xmss_sigs_total;

	// Initialize the key with length_secret_key amount of bytes.
	sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
	memset(sk->secret_key, 0, sk->length_secret_key);
	return sk;
}

// ================================================================ //


// ======================== XMSSMT-SHA256_6_H60 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	uint32_t oid = 0x00000007;
	if (xmssmt_keypair(public_key, secret_key, oid) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->method_name = "XMSSMT-SHA2_60/6_256";
	sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8391";

	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmssmt_sign;
	sig->verify = OQS_SIG_STFL_alg_xmssmt_verify;
	sig->derive_subkey = OQS_SIG_STFL_alg_xmssmt_derive_subkey;

	return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_6_H60_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_length_sk;

	// Assign the sigs_left and sigs_max functions
	sk->sigs_left = OQS_SECRET_KEY_xmss_sigs_left;
	sk->sigs_total = OQS_SECRET_KEY_xmss_sigs_total;

	// Initialize the key with length_secret_key amount of bytes.
	sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
	memset(sk->secret_key, 0, sk->length_secret_key);
	return sk;
}

// ================================================================ //


// ======================== XMSSMT-SHA256_12_H60 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	uint32_t oid = 0x00000008;
	if (xmssmt_keypair(public_key, secret_key, oid) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->method_name = "XMSSMT-SHA2_60/12_256";
	sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8391";

	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmssmt_sign;
	sig->verify = OQS_SIG_STFL_alg_xmssmt_verify;
	sig->derive_subkey = OQS_SIG_STFL_alg_xmssmt_derive_subkey;

	return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_12_H60_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_length_sk;

	// Assign the sigs_left and sigs_max functions
	sk->sigs_left = OQS_SECRET_KEY_xmss_sigs_left;
	sk->sigs_total = OQS_SECRET_KEY_xmss_sigs_total;

	// Initialize the key with length_secret_key amount of bytes.
	sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
	memset(sk->secret_key, 0, sk->length_secret_key);

	return sk;
}

// ================================================================ //
