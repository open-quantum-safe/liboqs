// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_hqc.h>

#if defined(OQS_ENABLE_KEM_hqc_5)

OQS_KEM *OQS_KEM_hqc_5_new(void) {

	OQS_KEM *kem = OQS_MEM_calloc(1, sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_hqc_5;
	kem->alg_version = "2026-10-12";

	kem->claimed_nist_level = 5;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_hqc_5_length_public_key;
	kem->length_secret_key = OQS_KEM_hqc_5_length_secret_key;
	kem->length_ciphertext = OQS_KEM_hqc_5_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_hqc_5_length_shared_secret;
	kem->length_keypair_seed = OQS_KEM_hqc_5_length_keypair_seed;
	kem->length_encaps_seed = OQS_KEM_hqc_5_length_encaps_seed;

	kem->keypair = OQS_KEM_hqc_5_keypair;
	kem->keypair_derand = OQS_KEM_hqc_5_keypair_derand;
	kem->encaps = OQS_KEM_hqc_5_encaps;
	kem->encaps_derand = OQS_KEM_hqc_5_encaps_derand;
	kem->decaps = OQS_KEM_hqc_5_decaps;

	return kem;
}

extern int PQCHQC_HQC5_C_hqc_kem_keygen_oqs(uint8_t *pk, uint8_t *sk);
extern int PQCHQC_HQC5_C_hqc_kem_encaps_oqs(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCHQC_HQC5_C_hqc_kem_decaps_oqs(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#if defined(OQS_ENABLE_KEM_hqc_5_x86_64)
extern int PQCHQC_HQC5_AVX256_hqc_kem_keygen_oqs(uint8_t *pk, uint8_t *sk);
extern int PQCHQC_HQC5_AVX256_hqc_kem_encaps_oqs(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCHQC_HQC5_AVX256_hqc_kem_decaps_oqs(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#endif

OQS_API OQS_STATUS OQS_KEM_hqc_5_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed) {
	(void)public_key;
	(void)secret_key;
	(void)seed;
	return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_KEM_hqc_5_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_hqc_5_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX) && OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI1) && OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCHQC_HQC5_AVX256_hqc_kem_keygen_oqs(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCHQC_HQC5_C_hqc_kem_keygen_oqs(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCHQC_HQC5_C_hqc_kem_keygen_oqs(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_hqc_5_encaps_derand(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *seed) {
	(void)ciphertext;
	(void)shared_secret;
	(void)public_key;
	(void)seed;
	return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_KEM_hqc_5_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
#if defined(OQS_ENABLE_KEM_hqc_5_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX) && OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI1) && OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCHQC_HQC5_AVX256_hqc_kem_encaps_oqs(ciphertext, shared_secret, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCHQC_HQC5_C_hqc_kem_encaps_oqs(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCHQC_HQC5_C_hqc_kem_encaps_oqs(ciphertext, shared_secret, public_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_hqc_5_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_hqc_5_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX) && OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI1) && OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCHQC_HQC5_AVX256_hqc_kem_decaps_oqs(shared_secret, ciphertext, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCHQC_HQC5_C_hqc_kem_decaps_oqs(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCHQC_HQC5_C_hqc_kem_decaps_oqs(shared_secret, ciphertext, secret_key);
#endif
}

#endif
