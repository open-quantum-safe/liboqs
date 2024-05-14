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

// ======================== XMSSMT-SHAKE_40/8_256 ======================== //

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_new(void) {

	OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
	if (sig == NULL) {
		return NULL;
	}
	memset(sig, 0, sizeof(OQS_SIG_STFL));

	sig->oid = OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_oid;
	sig->method_name = "XMSSMT-SHAKE_40/8_256";
	sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8391";
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_length_pk;
	sig->length_secret_key = OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_length_sk;
	sig->length_signature = OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_length_signature;

	sig->keypair = OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_keypair;
	sig->sign = OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_sign;
	sig->verify = OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_verify;
	sig->sigs_remaining = OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_sigs_remaining;
	sig->sigs_total = OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_sigs_total;

	return sig;
}

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_8_new(void) {
	return OQS_SECRET_KEY_XMSS_new(OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_length_sk);
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_keypair(XMSS_UNUSED_ATT uint8_t *public_key, XMSS_UNUSED_ATT OQS_SIG_STFL_SECRET_KEY *secret_key) {

	if (public_key == NULL || secret_key == NULL || secret_key->secret_key_data == NULL) {
		return OQS_ERROR;
	}

	if (xmssmt_keypair(public_key, secret_key->secret_key_data, OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_oid)) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key) {
	return OQS_SIG_STFL_alg_xmssmt_sign(signature, signature_len, message, message_len, secret_key);
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return OQS_SIG_STFL_alg_xmssmt_verify(message, message_len, signature, signature_len, public_key);
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	return OQS_SIG_STFL_alg_xmssmt_sigs_remaining(remain, secret_key);
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	return OQS_SIG_STFL_alg_xmssmt_sigs_total(total, secret_key);
}
