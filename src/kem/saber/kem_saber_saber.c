// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_saber.h>

#if defined(OQS_ENABLE_KEM_saber_saber)

OQS_KEM *OQS_KEM_saber_saber_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_saber_saber;
	kem->alg_version = "https://github.com/KULeuven-COSIC/SABER/tree/509cc5ec3a7e12a751ccdd2ef5bd6e54e00bd350 via https://github.com/jschanck/package-pqclean/tree/1ae84c3c/saber";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_saber_saber_length_public_key;
	kem->length_secret_key = OQS_KEM_saber_saber_length_secret_key;
	kem->length_ciphertext = OQS_KEM_saber_saber_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_saber_saber_length_shared_secret;

	kem->keypair = OQS_KEM_saber_saber_keypair;
	kem->encaps = OQS_KEM_saber_saber_encaps;
	kem->decaps = OQS_KEM_saber_saber_decaps;

	return kem;
}

extern int PQCLEAN_SABER_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
extern int PQCLEAN_SABER_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
extern int PQCLEAN_SABER_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#if defined(OQS_ENABLE_KEM_saber_saber_avx2)
extern int PQCLEAN_SABER_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
extern int PQCLEAN_SABER_AVX2_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
extern int PQCLEAN_SABER_AVX2_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);
#endif

OQS_API OQS_STATUS OQS_KEM_saber_saber_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_saber_saber_avx2)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) PQCLEAN_SABER_AVX2_crypto_kem_keypair(public_key, secret_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_SABER_CLEAN_crypto_kem_keypair(public_key, secret_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_SABER_CLEAN_crypto_kem_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_saber_saber_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
#if defined(OQS_ENABLE_KEM_saber_saber_avx2)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) PQCLEAN_SABER_AVX2_crypto_kem_enc(ciphertext, shared_secret, public_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_SABER_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_SABER_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_saber_saber_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_saber_saber_avx2)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) PQCLEAN_SABER_AVX2_crypto_kem_dec(shared_secret, ciphertext, secret_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_SABER_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_SABER_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
#endif
}

#endif
