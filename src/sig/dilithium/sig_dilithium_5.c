// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/sig_dilithium.h>

#if defined(OQS_ENABLE_SIG_dilithium_5)

OQS_SIG *OQS_SIG_dilithium_5_new(void) {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_dilithium_5;
	sig->alg_version = "https://github.com/pq-crystals/dilithium/commit/d9c885d3f2e11c05529eeeb7d70d808c972b8409";

	sig->claimed_nist_level = 5;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_dilithium_5_length_public_key;
	sig->length_secret_key = OQS_SIG_dilithium_5_length_secret_key;
	sig->length_signature = OQS_SIG_dilithium_5_length_signature;

	sig->keypair = OQS_SIG_dilithium_5_keypair;
	sig->sign = OQS_SIG_dilithium_5_sign;
	sig->verify = OQS_SIG_dilithium_5_verify;

	return sig;
}

extern int pqcrystals_dilithium5_ref_keypair(uint8_t *pk, uint8_t *sk);
extern int pqcrystals_dilithium5_ref_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int pqcrystals_dilithium5_ref_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

#if defined(OQS_ENABLE_SIG_dilithium_5_avx2)
extern int pqcrystals_dilithium5_avx2_keypair(uint8_t *pk, uint8_t *sk);
extern int pqcrystals_dilithium5_avx2_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int pqcrystals_dilithium5_avx2_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);
#endif

#if defined(OQS_ENABLE_SIG_dilithium_5_aarch64)
extern int PQCLEAN_DILITHIUM5_AARCH64_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_DILITHIUM5_AARCH64_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int PQCLEAN_DILITHIUM5_AARCH64_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);
#endif

OQS_API OQS_STATUS OQS_SIG_dilithium_5_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_dilithium_5_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) pqcrystals_dilithium5_avx2_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_dilithium5_ref_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_dilithium_5_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_DILITHIUM5_AARCH64_crypto_sign_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_dilithium5_ref_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) pqcrystals_dilithium5_ref_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_dilithium_5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_dilithium_5_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) pqcrystals_dilithium5_avx2_signature(signature, signature_len, message, message_len, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_dilithium5_ref_signature(signature, signature_len, message, message_len, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_dilithium_5_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_DILITHIUM5_AARCH64_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_dilithium5_ref_signature(signature, signature_len, message, message_len, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) pqcrystals_dilithium5_ref_signature(signature, signature_len, message, message_len, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_dilithium_5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
#if defined(OQS_ENABLE_SIG_dilithium_5_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) pqcrystals_dilithium5_avx2_verify(signature, signature_len, message, message_len, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_dilithium5_ref_verify(signature, signature_len, message, message_len, public_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_dilithium_5_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_DILITHIUM5_AARCH64_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_dilithium5_ref_verify(signature, signature_len, message, message_len, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) pqcrystals_dilithium5_ref_verify(signature, signature_len, message, message_len, public_key);
#endif
}

#endif
