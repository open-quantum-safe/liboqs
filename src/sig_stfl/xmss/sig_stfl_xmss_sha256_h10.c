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

// ======================== XMSS-SHA2_10_256 ======================== //

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h10_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->method_name = "XMSS-SHA2_10_256";
	sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8391";
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha256_h10_length_pk;
	sig->length_secret_key = OQS_SIG_STFL_alg_xmss_sha256_h10_length_sk;
	sig->length_signature = OQS_SIG_STFL_alg_xmss_sha256_h10_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmss_sha256_h10_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmss_sha256_h10_sign;
	sig->verify = OQS_SIG_STFL_alg_xmss_sha256_h10_verify;
	sig->sigs_remaining = OQS_SIG_STFL_alg_xmss_sha256_h10_sigs_remaining;
	sig->sigs_total = OQS_SIG_STFL_alg_xmss_sha256_h10_sigs_total;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_keypair(XMSS_UNUSED_ATT uint8_t *public_key, XMSS_UNUSED_ATT uint8_t *secret_key) {

	if (public_key == NULL || secret_key == NULL) {
		return OQS_ERROR;
	}

	const uint32_t xmss_sha256_h10_oid = 0x00000001;
	if (oqs_sig_stfl_xmss_xmss_keypair(public_key, secret_key, xmss_sha256_h10_oid)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sign(uint8_t *signature, size_t *signature_len, XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, XMSS_UNUSED_ATT uint8_t *secret_key) {

	if (signature == NULL || signature_len == NULL || message == NULL || secret_key == NULL) {
		return OQS_ERROR;
	}

	unsigned long long sig_length = 0;
	if (oqs_sig_stfl_xmss_xmss_sign(secret_key, signature, &sig_length, message, message_len)) {
		return OQS_ERROR;
	}
	*signature_len = (size_t) sig_length;

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_verify(XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, const uint8_t *signature, size_t signature_len, XMSS_UNUSED_ATT const uint8_t *public_key) {

	if (message == NULL || signature == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	if (oqs_sig_stfl_xmss_xmss_sign_open(message, (unsigned long long) message_len, signature, (unsigned long long) signature_len, public_key)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sigs_remaining(size_t *remain, const uint8_t *secret_key) {
	if (remain == NULL || secret_key == NULL) {
		return OQS_ERROR;
	}

	unsigned long long remaining_signatures = 0;
	if (oqs_sig_stfl_xmss_xmss_remaining_signatures(&remaining_signatures, secret_key)) {
		return OQS_ERROR;
	}
	*remain = (size_t) remaining_signatures;

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sigs_total(size_t *total, const uint8_t *secret_key) {
	if (total == NULL || secret_key == NULL) {
		return OQS_ERROR;
	}

	unsigned long long total_signatures = 0;
	if (oqs_sig_stfl_xmss_xmss_total_signatures(&total_signatures, secret_key)) {
		return OQS_ERROR;
	}
	*total = (size_t) total_signatures;

	return OQS_SUCCESS;
}