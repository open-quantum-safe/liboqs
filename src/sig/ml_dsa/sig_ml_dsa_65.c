// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/sig_ml_dsa.h>

#if defined(OQS_ENABLE_SIG_ml_dsa_65)
OQS_SIG *OQS_SIG_ml_dsa_65_new(void) {

	OQS_SIG *sig = OQS_MEM_malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_ml_dsa_65;
	sig->alg_version = "FIPS204";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;
	sig->suf_cma = true;
	sig->sig_with_ctx_support = true;

	sig->length_public_key = OQS_SIG_ml_dsa_65_length_public_key;
	sig->length_secret_key = OQS_SIG_ml_dsa_65_length_secret_key;
	sig->length_signature = OQS_SIG_ml_dsa_65_length_signature;

	sig->keypair = OQS_SIG_ml_dsa_65_keypair;
	sig->sign = OQS_SIG_ml_dsa_65_sign;
	sig->verify = OQS_SIG_ml_dsa_65_verify;
	sig->sign_with_ctx_str = OQS_SIG_ml_dsa_65_sign_with_ctx_str;
	sig->verify_with_ctx_str = OQS_SIG_ml_dsa_65_verify_with_ctx_str;

	return sig;
}

extern int PQCP_MLDSA_NATIVE_MLDSA65_C_keypair(uint8_t *pk, uint8_t *sk);

extern int PQCP_MLDSA_NATIVE_MLDSA65_C_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *ctx, size_t ctxlen, const uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA65_C_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *ctx, size_t ctxlen, const uint8_t *pk);




OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA65_C_keypair(public_key, secret_key);
}

OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {




	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA65_C_signature(signature, signature_len, message, message_len, NULL, 0, secret_key);


}



#endif
