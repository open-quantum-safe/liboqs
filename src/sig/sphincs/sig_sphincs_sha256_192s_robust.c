#include <stdlib.h>

#include <oqs/sig_sphincs.h>

#if defined(OQS_ENABLE_SIG_sphincs_sha256_192s_robust)

OQS_SIG *OQS_SIG_sphincs_sha256_192s_robust_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_sphincs_sha256_192s_robust;
	sig->alg_version = "https://github.com/sphincs/sphincsplus/commit/77755c94d0bc744478044d6efbb888dc13156441";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_sphincs_sha256_192s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_sphincs_sha256_192s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_sphincs_sha256_192s_robust_length_signature;

	sig->keypair = OQS_SIG_sphincs_sha256_192s_robust_keypair;
	sig->sign = OQS_SIG_sphincs_sha256_192s_robust_sign;
	sig->verify = OQS_SIG_sphincs_sha256_192s_robust_verify;

	return sig;
}

extern int PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

#if defined(OQS_ENABLE_SIG_sphincs_sha256_192s_robust_avx2)
extern int PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);
#endif

OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_sphincs_sha256_192s_robust_avx2)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_crypto_sign_keypair(public_key, secret_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_sphincs_sha256_192s_robust_avx2)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
#if defined(OQS_ENABLE_SIG_sphincs_sha256_192s_robust_avx2)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
#endif
}

#endif
