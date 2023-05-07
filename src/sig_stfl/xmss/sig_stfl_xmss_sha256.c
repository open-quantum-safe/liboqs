// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdlib.h>

#include <oqs/oqs.h>
#include "sig_stfl_xmss.h"

// ======================== XMSS10-SHA256 ======================== //

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
	sig->length_signature = OQS_SIG_STFL_alg_xmss_sha256_h10_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmss_sha256_h10_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmss_sha256_h10_sign;
	sig->verify = OQS_SIG_STFL_alg_xmss_sha256_h10_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, uint8_t *secret_key) {
	return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return OQS_ERROR;
}
