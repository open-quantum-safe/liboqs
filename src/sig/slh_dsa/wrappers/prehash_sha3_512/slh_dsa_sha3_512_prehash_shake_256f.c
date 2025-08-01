// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include "../../sig_slh_dsa.h"
#include "../../slh_dsa_c/slh_dsa.h"
#include "../../slh_dsa_c/slh_prehash.h"
#include <oqs/oqs.h>


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_256f)
OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_new(void) {

	OQS_SIG *sig = OQS_MEM_malloc(sizeof(OQS_SIG));
	if(sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_256f;
	sig->alg_version = "FIPS205";

	sig->claimed_nist_level = 5;
	sig->euf_cma = true;
	sig->suf_cma = false;
	sig->sig_with_ctx_support = true;

	sig->length_public_key = OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_length_public_key;
	sig->length_secret_key = OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_length_secret_key;
	sig->length_signature = OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_length_signature;

	sig->keypair = OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_keypair;
	sig->sign = OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_sign;
	sig->verify = OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_verify;
	sig->sign_with_ctx_str = OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_sign_with_ctx_str;
	sig->verify_with_ctx_str = OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_verify_with_ctx_str;

	return sig;
}

static int slh_randombytes(uint8_t *x, size_t xlen) {
	
	OQS_randombytes(x, xlen);
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key) {
	
	const slh_param_t *prm = &slh_dsa_shake_256f;
	int(*rbg)(uint8_t *x, size_t xlen) = slh_randombytes;
	return slh_keygen(secret_key, public_key, rbg, prm);
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_sign(uint8_t *signature, 
	size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	
	const slh_param_t *prm = &slh_dsa_shake_256f;
	
	const char *ph = "SHA3-512";
	const uint8_t *ctx_str = NULL;
	const size_t ctx_str_len = 0;
	uint8_t addrnd[32];
	OQS_randombytes(addrnd, 32);

	*signature_len = hash_slh_sign(signature, message, message_len, ctx_str,
		ctx_str_len, ph, secret_key, addrnd, prm);

	if(*signature_len == 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_verify(const uint8_t *message, 
	size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	
	const slh_param_t *prm = &slh_dsa_shake_256f;
	const char *ph = "SHA3-512";
	const uint8_t *ctx_str = NULL;
	const size_t ctx_str_len = 0;

	int res = hash_slh_verify(message, message_len, signature, signature_len,
		ctx_str, ctx_str_len, ph, public_key, prm);

	if(res == 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, 
	size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx_str, 
	size_t ctx_str_len, const uint8_t *secret_key) {
	
	const slh_param_t *prm = &slh_dsa_shake_256f;
	const char *ph = "SHA3-512";
	uint8_t addrnd[32];
	OQS_randombytes(addrnd, 32);

	*signature_len = hash_slh_sign(signature, message, message_len, ctx_str,
		ctx_str_len, ph, secret_key, addrnd, prm);

	if(*signature_len == 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, 
	size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx_str, 
	size_t ctx_str_len, const uint8_t *public_key) {
	
	const slh_param_t *prm = &slh_dsa_shake_256f;
	const char *ph = "SHA3-512";
	int res = hash_slh_verify(message, message_len, signature, signature_len,
		ctx_str, ctx_str_len, ph, public_key, prm);

   if(res == 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}


#endif

