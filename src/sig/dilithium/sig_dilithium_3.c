// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/sig_dilithium.h>

#if defined(OQS_ENABLE_SIG_dilithium_3)

OQS_SIG *OQS_SIG_dilithium_3_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_dilithium_3;
	sig->alg_version = "https://github.com/pq-crystals/dilithium/commit/40097237d92dbff3c5c869df9dfcaa32001776c2";

	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_dilithium_3_length_public_key;
	sig->length_secret_key = OQS_SIG_dilithium_3_length_secret_key;
	sig->length_signature = OQS_SIG_dilithium_3_length_signature;

	sig->keypair = OQS_SIG_dilithium_3_keypair;
	sig->sign = OQS_SIG_dilithium_3_sign;
	sig->verify = OQS_SIG_dilithium_3_verify;

	return sig;
}

extern int pqcrystals_dilithium3_ref_keypair(uint8_t *pk, uint8_t *sk);
extern int pqcrystals_dilithium3_ref_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int pqcrystals_dilithium3_ref_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

#if defined(OQS_ENABLE_SIG_dilithium_3_avx2)
extern int pqcrystals_dilithium3_avx2_keypair(uint8_t *pk, uint8_t *sk);
extern int pqcrystals_dilithium3_avx2_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int pqcrystals_dilithium3_avx2_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);
#endif

OQS_API OQS_STATUS OQS_SIG_dilithium_3_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_dilithium_3_avx2)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED && available_cpu_extensions.AES_ENABLED && available_cpu_extensions.BMI1_ENABLED && available_cpu_extensions.POPCNT_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) pqcrystals_dilithium3_avx2_keypair(public_key, secret_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_dilithium3_ref_keypair(public_key, secret_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) pqcrystals_dilithium3_ref_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_dilithium_3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_dilithium_3_avx2)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED && available_cpu_extensions.AES_ENABLED && available_cpu_extensions.BMI1_ENABLED && available_cpu_extensions.POPCNT_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) pqcrystals_dilithium3_avx2_signature(signature, signature_len, message, message_len, secret_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_dilithium3_ref_signature(signature, signature_len, message, message_len, secret_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) pqcrystals_dilithium3_ref_signature(signature, signature_len, message, message_len, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_dilithium_3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
#if defined(OQS_ENABLE_SIG_dilithium_3_avx2)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED && available_cpu_extensions.AES_ENABLED && available_cpu_extensions.BMI1_ENABLED && available_cpu_extensions.POPCNT_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) pqcrystals_dilithium3_avx2_verify(signature, signature_len, message, message_len, public_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_dilithium3_ref_verify(signature, signature_len, message, message_len, public_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) pqcrystals_dilithium3_ref_verify(signature, signature_len, message, message_len, public_key);
#endif
}

#endif
