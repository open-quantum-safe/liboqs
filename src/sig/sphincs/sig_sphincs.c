#include <stdlib.h>

#include <oqs/sig_sphincs.h>

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_128s_simple

OQS_SIG_SPHINCS_haraka_128s_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_128s_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_128s_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_128s_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_128s_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_128s_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_128s_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_128s_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128s_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_128s_robust

OQS_SIG_SPHINCS_haraka_128s_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_128s_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_128s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_128s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_128s_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_128s_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_128s_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_128s_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA128SROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA128SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA128SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128SROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_128f_simple

OQS_SIG_SPHINCS_haraka_128f_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_128f_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_128f_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_128f_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_128f_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_128f_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_128f_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_128f_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128f_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_128f_robust

OQS_SIG_SPHINCS_haraka_128f_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_128f_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_128f_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_128f_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_128f_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_128f_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_128f_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_128f_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA128FROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA128FROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA128FROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128f_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128FROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128FROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_128f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA128FROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_192s_simple

OQS_SIG_SPHINCS_haraka_192s_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_192s_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_192s_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_192s_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_192s_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_192s_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_192s_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_192s_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192s_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_192s_robust

OQS_SIG_SPHINCS_haraka_192s_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_192s_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_192s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_192s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_192s_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_192s_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_192s_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_192s_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA192SROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA192SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA192SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192SROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_192f_simple

OQS_SIG_SPHINCS_haraka_192f_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_192f_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_192f_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_192f_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_192f_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_192f_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_192f_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_192f_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA192FSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA192FSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA192FSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192f_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192FSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192FSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192FSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_192f_robust

OQS_SIG_SPHINCS_haraka_192f_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_192f_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_192f_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_192f_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_192f_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_192f_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_192f_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_192f_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA192FROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA192FROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA192FROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192f_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192FROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192FROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_192f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA192FROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_256s_simple

OQS_SIG_SPHINCS_haraka_256s_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_256s_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_256s_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_256s_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_256s_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_256s_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_256s_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_256s_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA256SSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA256SSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA256SSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256s_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256SSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256SSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256SSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_256s_robust

OQS_SIG_SPHINCS_haraka_256s_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_256s_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_256s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_256s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_256s_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_256s_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_256s_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_256s_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_256f_simple

OQS_SIG_SPHINCS_haraka_256f_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_256f_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_256f_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_256f_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_256f_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_256f_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_256f_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_256f_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA256FSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA256FSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA256FSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256f_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256FSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256FSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256FSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_haraka_256f_robust

OQS_SIG_SPHINCS_haraka_256f_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_haraka_256f_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_haraka_256f_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_haraka_256f_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_haraka_256f_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_haraka_256f_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_haraka_256f_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_haraka_256f_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256f_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_haraka_256f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_128s_simple

OQS_SIG_SPHINCS_shake_128s_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_128s_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_128s_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_128s_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_128s_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_128s_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_128s_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_128s_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE128SSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE128SSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE128SSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128s_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128SSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128SSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128SSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_128s_robust

OQS_SIG_SPHINCS_shake_128s_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_128s_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_128s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_128s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_128s_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_128s_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_128s_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_128s_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE128SROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE128SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE128SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128SROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_128f_simple

OQS_SIG_SPHINCS_shake_128f_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_128f_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_128f_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_128f_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_128f_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_128f_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_128f_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_128f_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE128FSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE128FSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE128FSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128f_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128FSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128FSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128FSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_128f_robust

OQS_SIG_SPHINCS_shake_128f_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_128f_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_128f_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_128f_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_128f_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_128f_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_128f_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_128f_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE128FROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE128FROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE128FROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128f_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128FROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128FROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_128f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE128FROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_192s_simple

OQS_SIG_SPHINCS_shake_192s_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_192s_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_192s_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_192s_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_192s_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_192s_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_192s_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_192s_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE192SSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE192SSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE192SSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192s_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192SSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192SSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192SSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_192s_robust

OQS_SIG_SPHINCS_shake_192s_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_192s_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_192s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_192s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_192s_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_192s_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_192s_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_192s_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE192SROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE192SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE192SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192SROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_192f_simple

OQS_SIG_SPHINCS_shake_192f_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_192f_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_192f_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_192f_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_192f_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_192f_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_192f_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_192f_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE192FSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE192FSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE192FSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192f_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192FSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192FSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192FSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_192f_robust

OQS_SIG_SPHINCS_shake_192f_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_192f_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_192f_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_192f_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_192f_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_192f_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_192f_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_192f_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE192FROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE192FROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE192FROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192f_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192FROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192FROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_192f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE192FROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_256s_simple

OQS_SIG_SPHINCS_shake_256s_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_256s_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_256s_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_256s_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_256s_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_256s_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_256s_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_256s_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE256SSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE256SSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE256SSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256s_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256SSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256SSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256SSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_256s_robust

OQS_SIG_SPHINCS_shake_256s_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_256s_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_256s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_256s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_256s_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_256s_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_256s_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_256s_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE256SROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE256SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE256SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256SROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_256f_simple

OQS_SIG_SPHINCS_shake_256f_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_256f_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_256f_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_256f_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_256f_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_256f_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_256f_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_256f_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE256FSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE256FSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE256FSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256f_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256FSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256FSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256FSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_shake_256f_robust

OQS_SIG_SPHINCS_shake_256f_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_shake_256f_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_shake_256f_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_shake_256f_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_shake_256f_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_shake_256f_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_shake_256f_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_shake_256f_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHAKE256FROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHAKE256FROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHAKE256FROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256f_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256FROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256FROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_shake_256f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHAKE256FROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_128s_simple

OQS_SIG_SPHINCS_sha256_128s_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_128s_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_128s_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_128s_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_128s_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_128s_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_128s_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_128s_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128s_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_128s_robust

OQS_SIG_SPHINCS_sha256_128s_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_128s_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_128s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_128s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_128s_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_128s_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_128s_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_128s_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256128SROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256128SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256128SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128SROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_128f_simple

OQS_SIG_SPHINCS_sha256_128f_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_128f_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_128f_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_128f_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_128f_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_128f_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_128f_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_128f_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128f_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_128f_robust

OQS_SIG_SPHINCS_sha256_128f_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_128f_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_128f_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_128f_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_128f_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_128f_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_128f_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_128f_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128f_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_128f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_192s_simple

OQS_SIG_SPHINCS_sha256_192s_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_192s_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_192s_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_192s_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_192s_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_192s_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_192s_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_192s_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192s_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_192s_robust

OQS_SIG_SPHINCS_sha256_192s_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_192s_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_192s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_192s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_192s_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_192s_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_192s_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_192s_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192SROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_192f_simple

OQS_SIG_SPHINCS_sha256_192f_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_192f_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_192f_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_192f_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_192f_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_192f_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_192f_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_192f_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256192FSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256192FSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256192FSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192f_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192FSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192FSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192FSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_192f_robust

OQS_SIG_SPHINCS_sha256_192f_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_192f_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_192f_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_192f_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_192f_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_192f_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_192f_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_192f_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256192FROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256192FROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256192FROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192f_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192FROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192FROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_192f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256192FROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_256s_simple

OQS_SIG_SPHINCS_sha256_256s_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_256s_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_256s_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_256s_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_256s_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_256s_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_256s_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_256s_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256s_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_256s_robust

OQS_SIG_SPHINCS_sha256_256s_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_256s_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_256s_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_256s_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_256s_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_256s_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_256s_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_256s_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256s_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_256f_simple

OQS_SIG_SPHINCS_sha256_256f_simple_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_256f_simple;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_256f_simple_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_256f_simple_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_256f_simple_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_256f_simple_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_256f_simple_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_256f_simple_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256f_simple_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif

#ifdef OQS_ENABLE_SIG_SPHINCS_sha256_256f_robust

OQS_SIG_SPHINCS_sha256_256f_robust_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));

	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_SPHINCS_sha256_256f_robust;
	sig->alg_version = "https://github.com/PQClean/PQClean/commit/3d7cbefd3b9f9f2b98745edd88118db95f575edf";
	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_SPHINCS_sha256_256f_robust_length_public_key;
	sig->length_secret_key = OQS_SIG_SPHINCS_sha256_256f_robust_length_secret_key;
	sig->length_signature = OQS_SIG_SPHINCS_sha256_256f_robust_length_signature;

	sig->keypair = OQS_SIG_SPHINCS_sha256_256f_robust_keypair;
	sig->sign = OQS_SIG_SPHINCS_sha256_256f_robust_sign;
	sig->verify = OQS_SIG_SPHINCS_sha256_256f_robust_verify;

	return sig;
}

int PQCLEAN_SPHINCSSHA256256FROBUST_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SPHINCSSHA256256FROBUST_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_SPHINCSSHA256256FROBUST_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256f_robust_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256FROBUST_CLEAN_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256FROBUST_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_SPHINCS_sha256_256f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_SPHINCSSHA256256FROBUST_CLEAN_crypto_sign_verify(*signature, signature_len, message, message_len, public_key);
}

#endif
