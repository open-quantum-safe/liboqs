// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/sig_cross.h>

#if defined(OQS_ENABLE_SIG_cross_rsdp_192_fast)
OQS_SIG *OQS_SIG_cross_rsdp_192_fast_new(void) {

	OQS_SIG *sig = OQS_MEM_malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_cross_rsdp_192_fast;
	sig->alg_version = "2.2 + PQClean and OQS patches";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;
	sig->suf_cma = false;
	sig->sig_with_ctx_support = false;

	sig->length_public_key = OQS_SIG_cross_rsdp_192_fast_length_public_key;
	sig->length_secret_key = OQS_SIG_cross_rsdp_192_fast_length_secret_key;
	sig->length_signature = OQS_SIG_cross_rsdp_192_fast_length_signature;

	sig->keypair = OQS_SIG_cross_rsdp_192_fast_keypair;
	sig->sign = OQS_SIG_cross_rsdp_192_fast_sign;
	sig->verify = OQS_SIG_cross_rsdp_192_fast_verify;
	sig->sign_with_ctx_str = OQS_SIG_cross_rsdp_192_fast_sign_with_ctx_str;
	sig->verify_with_ctx_str = OQS_SIG_cross_rsdp_192_fast_verify_with_ctx_str;

	return sig;
}

extern int PQCLEAN_CROSSRSDP192FAST_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_CROSSRSDP192FAST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int PQCLEAN_CROSSRSDP192FAST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

#if defined(OQS_ENABLE_SIG_cross_rsdp_192_fast_avx2)
extern int PQCLEAN_CROSSRSDP192FAST_AVX2_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_CROSSRSDP192FAST_AVX2_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int PQCLEAN_CROSSRSDP192FAST_AVX2_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);
#endif

OQS_API OQS_STATUS OQS_SIG_cross_rsdp_192_fast_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_cross_rsdp_192_fast_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_CROSSRSDP192FAST_AVX2_crypto_sign_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_CROSSRSDP192FAST_CLEAN_crypto_sign_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_CROSSRSDP192FAST_CLEAN_crypto_sign_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_cross_rsdp_192_fast_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_cross_rsdp_192_fast_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_CROSSRSDP192FAST_AVX2_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_CROSSRSDP192FAST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_CROSSRSDP192FAST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_cross_rsdp_192_fast_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
#if defined(OQS_ENABLE_SIG_cross_rsdp_192_fast_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_CROSSRSDP192FAST_AVX2_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_CROSSRSDP192FAST_CLEAN_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_CROSSRSDP192FAST_CLEAN_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_cross_rsdp_192_fast_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx_str, size_t ctx_str_len, const uint8_t *secret_key) {
	if (ctx_str == NULL && ctx_str_len == 0) {
		return OQS_SIG_cross_rsdp_192_fast_sign(signature, signature_len, message, message_len, secret_key);
	} else {
		return OQS_ERROR;
	}
}

OQS_API OQS_STATUS OQS_SIG_cross_rsdp_192_fast_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx_str, size_t ctx_str_len, const uint8_t *public_key) {
	if (ctx_str == NULL && ctx_str_len == 0) {
		return OQS_SIG_cross_rsdp_192_fast_verify(message, message_len, signature, signature_len, public_key);
	} else {
		return OQS_ERROR;
	}
}
#endif
