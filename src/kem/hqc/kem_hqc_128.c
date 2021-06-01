// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_hqc.h>

#if defined(OQS_ENABLE_KEM_hqc_128)

OQS_KEM *OQS_KEM_hqc_128_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_hqc_128;
	kem->alg_version = "hqc-submission_2020-10-01 via https://github.com/jschanck/package-pqclean/tree/29f79e72/hqc";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_hqc_128_length_public_key;
	kem->length_secret_key = OQS_KEM_hqc_128_length_secret_key;
	kem->length_ciphertext = OQS_KEM_hqc_128_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_hqc_128_length_shared_secret;

	kem->keypair = OQS_KEM_hqc_128_keypair;
	kem->encaps = OQS_KEM_hqc_128_encaps;
	kem->decaps = OQS_KEM_hqc_128_decaps;

	return kem;
}

extern int PQCLEAN_HQCRMRS128_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_HQCRMRS128_CLEAN_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCLEAN_HQCRMRS128_CLEAN_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#if defined(OQS_ENABLE_KEM_hqc_128_avx2)
extern int PQCLEAN_HQCRMRS128_AVX2_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_HQCRMRS128_AVX2_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCLEAN_HQCRMRS128_AVX2_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#endif

OQS_API OQS_STATUS OQS_KEM_hqc_128_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_hqc_128_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI1) && OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_HQCRMRS128_AVX2_crypto_kem_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_HQCRMRS128_CLEAN_crypto_kem_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_HQCRMRS128_CLEAN_crypto_kem_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_hqc_128_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
#if defined(OQS_ENABLE_KEM_hqc_128_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI1) && OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_HQCRMRS128_AVX2_crypto_kem_enc(ciphertext, shared_secret, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_HQCRMRS128_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_HQCRMRS128_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_hqc_128_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_hqc_128_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI1) && OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_HQCRMRS128_AVX2_crypto_kem_dec(shared_secret, ciphertext, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_HQCRMRS128_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_HQCRMRS128_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
#endif
}

#endif
