// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_hqc.h>

#if defined(OQS_ENABLE_KEM_hqc_192)

OQS_KEM *OQS_KEM_hqc_192_new(void) {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_hqc_192;
	kem->alg_version = "hqc-submission_2023-04-30 via https://github.com/SWilson4/package-pqclean/tree/8db1b24b/hqc";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_hqc_192_length_public_key;
	kem->length_secret_key = OQS_KEM_hqc_192_length_secret_key;
	kem->length_ciphertext = OQS_KEM_hqc_192_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_hqc_192_length_shared_secret;

	kem->keypair = OQS_KEM_hqc_192_keypair;
	kem->encaps = OQS_KEM_hqc_192_encaps;
	kem->decaps = OQS_KEM_hqc_192_decaps;

	return kem;
}

extern int PQCLEAN_HQC192_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_HQC192_CLEAN_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCLEAN_HQC192_CLEAN_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

OQS_API OQS_STATUS OQS_KEM_hqc_192_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_HQC192_CLEAN_crypto_kem_keypair(public_key, secret_key);
}

OQS_API OQS_STATUS OQS_KEM_hqc_192_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_HQC192_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
}

OQS_API OQS_STATUS OQS_KEM_hqc_192_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_HQC192_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
}

#endif
