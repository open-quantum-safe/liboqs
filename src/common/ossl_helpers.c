// SPDX-License-Identifier: MIT
#include <oqs/oqs.h>
#if defined(OQS_USE_OPENSSL)
#include <openssl/evp.h>
#include "ossl_helpers.h"
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
static EVP_MD *sha256_ptr, *sha384_ptr, *sha512_ptr,
       *sha3_256_ptr, *sha3_384_ptr, *sha3_512_ptr,
       *shake128_ptr, *shake256_ptr;

static EVP_CIPHER *aes128_ecb_ptr, *aes256_ecb_ptr, *aes256_ctr_ptr;
#endif

CRYPTO_ONCE OQS_ONCE_STATIC_INIT;

static void oqs_fetch_ossl_objects(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	sha256_ptr = EVP_MD_fetch(NULL, "SHA256", NULL);
	sha384_ptr = EVP_MD_fetch(NULL, "SHA384", NULL);
	sha512_ptr = EVP_MD_fetch(NULL, "SHA512", NULL);

	sha3_256_ptr = EVP_MD_fetch(NULL, "SHA3-256", NULL);
	sha3_384_ptr = EVP_MD_fetch(NULL, "SHA3-384", NULL);
	sha3_512_ptr = EVP_MD_fetch(NULL, "SHA3-512", NULL);
	shake128_ptr = EVP_MD_fetch(NULL, "SHAKE128", NULL);
	shake256_ptr = EVP_MD_fetch(NULL, "SHAKE256", NULL);

	aes128_ecb_ptr = EVP_CIPHER_fetch(NULL, "AES-128-ECB", NULL);
	aes256_ecb_ptr = EVP_CIPHER_fetch(NULL, "AES-256-ECB", NULL);
	aes256_ctr_ptr = EVP_CIPHER_fetch(NULL, "AES-256-CTR", NULL);

	if (!sha256_ptr || !sha384_ptr || !sha512_ptr || !sha3_256_ptr ||
	        !sha3_384_ptr || !sha3_512_ptr || !shake128_ptr || !shake256_ptr ||
	        !aes128_ecb_ptr || !aes256_ecb_ptr || !aes256_ctr_ptr) {
		fprintf(stderr, "liboqs warning: OpenSSL initialization failure. Is provider for SHA, SHAKE, AES enabled?\n");
	}
#endif
}

void oqs_free_ossl_objects(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	EVP_MD_free(sha256_ptr);
	EVP_MD_free(sha384_ptr);
	EVP_MD_free(sha512_ptr);
	EVP_MD_free(sha3_256_ptr);
	EVP_MD_free(sha3_384_ptr);
	EVP_MD_free(sha3_512_ptr);
	EVP_MD_free(shake128_ptr);
	EVP_MD_free(shake256_ptr);
	EVP_CIPHER_free(aes128_ecb_ptr);
	EVP_CIPHER_free(aes256_ecb_ptr);
	EVP_CIPHER_free(aes256_ctr_ptr);
#endif
}

const EVP_MD *oqs_sha256(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!sha256_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return sha256_ptr;
#else
	return EVP_sha256();
#endif
}

const EVP_MD *oqs_sha384(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!sha384_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return sha384_ptr;
#else
	return EVP_sha384();
#endif
}

const EVP_MD *oqs_sha512(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!sha512_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return sha512_ptr;
#else
	return EVP_sha512();
#endif
}

const EVP_MD *oqs_shake128(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!shake128_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return shake128_ptr;
#else
	return EVP_shake128();
#endif
}

const EVP_MD *oqs_shake256(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!shake256_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return shake256_ptr;
#else
	return EVP_shake256();
#endif
}

const EVP_MD *oqs_sha3_256(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!sha3_256_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return sha3_256_ptr;
#else
	return EVP_sha3_256();
#endif
}

const EVP_MD *oqs_sha3_384(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!sha3_384_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return sha3_384_ptr;
#else
	return EVP_sha3_384();
#endif
}

const EVP_MD *oqs_sha3_512(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!sha3_512_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return sha3_512_ptr;
#else
	return EVP_sha3_512();
#endif
}

const EVP_CIPHER *oqs_aes_128_ecb(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!aes128_ecb_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return aes128_ecb_ptr;
#else
	return EVP_aes_128_ecb();
#endif
}

const EVP_CIPHER *oqs_aes_256_ecb(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!aes256_ecb_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return aes256_ecb_ptr;
#else
	return EVP_aes_256_ecb();
#endif
}

const EVP_CIPHER *oqs_aes_256_ctr(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	if (!aes256_ctr_ptr) {
		if (!CRYPTO_THREAD_run_once(&OQS_ONCE_STATIC_INIT, oqs_fetch_ossl_objects)) {
			return NULL;
		}
	}
	return aes256_ctr_ptr;
#else
	return EVP_aes_256_ctr();
#endif
}
#endif
