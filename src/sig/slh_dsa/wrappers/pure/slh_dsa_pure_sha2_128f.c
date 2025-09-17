// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include "../../sig_slh_dsa.h"
#include "../../slh_dsa_c/slh_dsa.h"
#include "../../slh_dsa_c/slh_prehash.h"
#include <oqs/oqs.h>


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_sha2_128f)
OQS_SIG *OQS_SIG_slh_dsa_pure_sha2_128f_new(void) {

	OQS_SIG *sig = OQS_MEM_malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_slh_dsa_pure_sha2_128f;
	sig->alg_version = "FIPS205";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;
	sig->suf_cma = false;
	sig->sig_with_ctx_support = true;

	sig->length_public_key = OQS_SIG_slh_dsa_pure_sha2_128f_length_public_key;
	sig->length_secret_key = OQS_SIG_slh_dsa_pure_sha2_128f_length_secret_key;
	sig->length_signature = OQS_SIG_slh_dsa_pure_sha2_128f_length_signature;

	sig->keypair = OQS_SIG_slh_dsa_pure_sha2_128f_keypair;
	sig->sign = OQS_SIG_slh_dsa_pure_sha2_128f_sign;
	sig->verify = OQS_SIG_slh_dsa_pure_sha2_128f_verify;
	sig->sign_with_ctx_str = OQS_SIG_slh_dsa_pure_sha2_128f_sign_with_ctx_str;
	sig->verify_with_ctx_str = OQS_SIG_slh_dsa_pure_sha2_128f_verify_with_ctx_str;

	return sig;
}

static int slh_randombytes(uint8_t *x, size_t xlen) {

	OQS_randombytes(x, xlen);
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key) {

	const slh_param_t *prm = &slh_dsa_sha2_128f;
	int(*rbg)(uint8_t *x, size_t xlen) = slh_randombytes;
	return slh_keygen(secret_key, public_key, rbg, prm);
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_sign(uint8_t *signature, size_t *signature_len,
        const uint8_t *message, size_t message_len, const uint8_t *secret_key) {

	const slh_param_t *prm = &slh_dsa_sha2_128f;
	const uint8_t *ctx = NULL;
	const size_t ctxlen = 0;
	uint8_t addrnd[32];
	OQS_randombytes(addrnd, 32);

	*signature_len = slh_sign(signature, message, message_len, ctx, ctxlen,
	                          secret_key, addrnd, prm);

	if (*signature_len == 0) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_verify(const uint8_t *message, size_t message_len,
        const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {

	const slh_param_t *prm = &slh_dsa_sha2_128f;
	const uint8_t *ctx = NULL;
	const size_t ctxlen = 0;

	int res = slh_verify(message, message_len, signature, signature_len, ctx,
	                     ctxlen, public_key, prm);

	if (res == 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_sign_with_ctx_str(uint8_t *signature,
        size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx_str,
        size_t ctx_str_len, const uint8_t *secret_key) {

	const slh_param_t *prm = &slh_dsa_sha2_128f;
	uint8_t addrnd[32];
	OQS_randombytes(addrnd, 32);

	*signature_len = slh_sign(signature, message, message_len, ctx_str, ctx_str_len,
	                          secret_key, addrnd, prm);

	if (*signature_len == 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_verify_with_ctx_str(const uint8_t *message,
        size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx_str, size_t ctx_str_len, const uint8_t *public_key) {

	const slh_param_t *prm = &slh_dsa_sha2_128f;

	int res = slh_verify(message, message_len, signature, signature_len, ctx_str,
	                     ctx_str_len, public_key, prm);

	if (res == 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif

