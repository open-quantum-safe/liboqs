// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_hqc.h>

#if defined(OQS_ENABLE_KEM_hqc_192_1_cca2)

OQS_KEM *OQS_KEM_hqc_192_1_cca2_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_hqc_192_1_cca2;
	kem->alg_version = "https://pqc-hqc.org/doc/hqc-reference-implementation_2019-08-24.zip";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_hqc_192_1_cca2_length_public_key;
	kem->length_secret_key = OQS_KEM_hqc_192_1_cca2_length_secret_key;
	kem->length_ciphertext = OQS_KEM_hqc_192_1_cca2_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_hqc_192_1_cca2_length_shared_secret;

	kem->keypair = OQS_KEM_hqc_192_1_cca2_keypair;
	kem->encaps = OQS_KEM_hqc_192_1_cca2_encaps;
	kem->decaps = OQS_KEM_hqc_192_1_cca2_decaps;

	return kem;
}

extern int PQCLEAN_HQC1921CCA2_LEAKTIME_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
extern int PQCLEAN_HQC1921CCA2_LEAKTIME_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
extern int PQCLEAN_HQC1921CCA2_LEAKTIME_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

OQS_API OQS_STATUS OQS_KEM_hqc_192_1_cca2_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_HQC1921CCA2_LEAKTIME_crypto_kem_keypair(public_key, secret_key);
}

OQS_API OQS_STATUS OQS_KEM_hqc_192_1_cca2_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_HQC1921CCA2_LEAKTIME_crypto_kem_enc(ciphertext, shared_secret, public_key);
}

OQS_API OQS_STATUS OQS_KEM_hqc_192_1_cca2_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_HQC1921CCA2_LEAKTIME_crypto_kem_dec(shared_secret, ciphertext, secret_key);
}

#endif
