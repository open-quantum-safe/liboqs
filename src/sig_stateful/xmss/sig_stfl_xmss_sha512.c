// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdlib.h>

#include <oqs/oqs.h>
#include "./external/xmss.h"
#include "./external/params.h"
#include "sig_stfl_xmss_xmssmt.h"
#include "./external/xmss_namespace.h"

// ======================== XMSS10-SHa512 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h10_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	uint32_t oid = 0x00000004;
	if (xmss_keypair(public_key, secret_key, oid) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha512_h10_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->method_name = "XMSS-SHA2_10_512";
	sig->alg_version = "...";

	// Check how true this is
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha512_h10_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_xmss_sha512_h10_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmss_sha512_h10_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmss_sign;
	sig->verify = OQS_SIG_STFL_alg_xmss_verify;
	sig->derive_subkey = OQS_SIG_STFL_alg_xmss_derive_subkey;

	return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA512_H10_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmss_sha512_h10_length_sk;

	perform_key_allocation(sk);

	return sk;
}

// ================================================================ //

// ======================== XMSS16-SHA512 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h16_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	uint32_t oid = 0x00000005;
	if (xmss_keypair(public_key, secret_key, oid) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha512_h16_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = "XMSS-SHA2_16_512";
	sig->alg_version = "...";

	// Check how true this is
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha512_h16_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_xmss_sha512_h16_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmss_sha512_h16_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmss_sign;
	sig->verify = OQS_SIG_STFL_alg_xmss_verify;
	sig->derive_subkey = OQS_SIG_STFL_alg_xmss_derive_subkey;

	return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA512_H16_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmss_sha512_h16_length_sk;

	perform_key_allocation(sk);

	return sk;
}

// ================================================================ //

// ======================== XMSS20-SHA512 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h20_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	uint32_t oid = 0x00000006;
	if (xmss_keypair(public_key, secret_key, oid) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha512_h20_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = "XMSS-SHA2_20_512";
	sig->alg_version = "...";

	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha512_h20_length_pk;
	sig->length_signature = OQS_SIG_STFL_alg_xmss_sha512_h20_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmss_sha512_h20_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmss_sign;
	sig->verify = OQS_SIG_STFL_alg_xmss_verify;
	sig->derive_subkey = OQS_SIG_STFL_alg_xmss_derive_subkey;

	return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA512_H20_new(void) {

	// Initialize the secret key in the heap with adequate memory
	OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
	if (sk == NULL) {
		return NULL;
	}
	memset(sk, 0, sizeof(OQS_SECRET_KEY));

	sk->length_secret_key = OQS_SIG_STFL_alg_xmss_sha512_h20_length_sk ;

	perform_key_allocation(sk);

	return sk;
}

// ================================================================ //
