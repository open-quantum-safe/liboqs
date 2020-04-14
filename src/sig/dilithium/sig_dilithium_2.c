#include <stdlib.h>

#include <oqs/sig_dilithium.h>

#if defined(OQS_ENABLE_SIG_dilithium_2)

OQS_SIG *OQS_SIG_dilithium_2_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_dilithium_2;
	sig->alg_version = "https://github.com/pq-crystals/dilithium/commit/c1b40fd599e71f65aa18be64dd6c3fc8e84b0c08";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_dilithium_2_length_public_key;
	sig->length_secret_key = OQS_SIG_dilithium_2_length_secret_key;
	sig->length_signature = OQS_SIG_dilithium_2_length_signature;

	sig->keypair = OQS_SIG_dilithium_2_keypair;
	sig->sign = OQS_SIG_dilithium_2_sign;
	sig->verify = OQS_SIG_dilithium_2_verify;

	return sig;
}

int PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
int PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

#if defined(OQS_ENABLE_SIG_dilithium_2_avx2)
int PQCLEAN_DILITHIUM2_AVX2_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
int PQCLEAN_DILITHIUM2_AVX2_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_DILITHIUM2_AVX2_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);
#endif

OQS_API OQS_STATUS OQS_SIG_dilithium_2_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_dilithium_2_avx2)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED && available_cpu_extensions.BMI2_ENABLED) {
		return (OQS_STATUS) PQCLEAN_DILITHIUM2_AVX2_crypto_sign_keypair(public_key, secret_key);
	} else {
		return (OQS_STATUS) PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_keypair(public_key, secret_key);
	}
#else
	return (OQS_STATUS) PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_dilithium_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_dilithium_2_avx2)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED && available_cpu_extensions.BMI2_ENABLED) {
		return (OQS_STATUS) PQCLEAN_DILITHIUM2_AVX2_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
	} else {
		return (OQS_STATUS) PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
	}
#else
	return (OQS_STATUS) PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_dilithium_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
#if defined(OQS_ENABLE_SIG_dilithium_2_avx2)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED && available_cpu_extensions.BMI2_ENABLED) {
		return (OQS_STATUS) PQCLEAN_DILITHIUM2_AVX2_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
	} else {
		return (OQS_STATUS) PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
	}
#else
	return (OQS_STATUS) PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
#endif
}

#endif
