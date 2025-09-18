// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_ntru.h>

#if defined(OQS_ENABLE_KEM_ntru_hps2048509)

OQS_KEM *OQS_KEM_ntru_hps2048509_new(void) {

	OQS_KEM *kem = OQS_MEM_malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_ntru_hps2048509;
	kem->alg_version = "https://github.com/jschanck/ntru/tree/6d96ed37 reference implementation";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_ntru_hps2048509_length_public_key;
	kem->length_secret_key = OQS_KEM_ntru_hps2048509_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ntru_hps2048509_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ntru_hps2048509_length_shared_secret;
	kem->length_keypair_seed = OQS_KEM_ntru_hps2048509_length_keypair_seed;
	kem->length_encaps_seed = OQS_KEM_ntru_hps2048509_length_encaps_seed;

	kem->keypair = OQS_KEM_ntru_hps2048509_keypair;
	kem->keypair_derand = OQS_KEM_ntru_hps2048509_keypair_derand;
	kem->encaps = OQS_KEM_ntru_hps2048509_encaps;
	kem->decaps = OQS_KEM_ntru_hps2048509_decaps;

	return kem;
}

extern int PQCLEAN_NTRUHPS2048509_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_NTRUHPS2048509_CLEAN_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCLEAN_NTRUHPS2048509_CLEAN_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#if defined(OQS_ENABLE_KEM_ntru_hps2048509_avx2)
extern int PQCLEAN_NTRUHPS2048509_AVX2_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_NTRUHPS2048509_AVX2_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCLEAN_NTRUHPS2048509_AVX2_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#endif

OQS_API OQS_STATUS OQS_KEM_ntru_hps2048509_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_ntru_hps2048509_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_NTRUHPS2048509_AVX2_crypto_kem_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_NTRUHPS2048509_CLEAN_crypto_kem_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_NTRUHPS2048509_CLEAN_crypto_kem_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_ntru_hps2048509_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed) {
	(void)public_key;
	(void)secret_key;
	(void)seed;
	return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_KEM_ntru_hps2048509_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
#if defined(OQS_ENABLE_KEM_ntru_hps2048509_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_NTRUHPS2048509_AVX2_crypto_kem_enc(ciphertext, shared_secret, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_NTRUHPS2048509_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_NTRUHPS2048509_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_ntru_hps2048509_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_ntru_hps2048509_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_NTRUHPS2048509_AVX2_crypto_kem_dec(shared_secret, ciphertext, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_NTRUHPS2048509_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_NTRUHPS2048509_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
#endif
}

#endif
