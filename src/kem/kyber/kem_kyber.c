#include <stdio.h>
#include <stdlib.h>

#include <oqs/kem_kyber.h>

#ifdef OQS_ENABLE_KEM_kyber_512_cca_kem

OQS_KEM *OQS_KEM_kyber_512_cca_kem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_kyber_512_cca_kem;
	kem->alg_version = "https://github.com/pq-crystals/kyber/commit/ab996e7460e5356b0e23aa034e7c2fe6922e60e6";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_kyber_512_cca_kem_length_public_key;
	kem->length_secret_key = OQS_KEM_kyber_512_cca_kem_length_secret_key;
	kem->length_ciphertext = OQS_KEM_kyber_512_cca_kem_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_kyber_512_cca_kem_length_shared_secret;

	kem->keypair = OQS_KEM_kyber_512_cca_kem_keypair;
	kem->encaps = OQS_KEM_kyber_512_cca_kem_encaps;
	kem->decaps = OQS_KEM_kyber_512_cca_kem_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_kyber_768_cca_kem

OQS_KEM *OQS_KEM_kyber_768_cca_kem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_kyber_768_cca_kem;
	kem->alg_version = "https://github.com/pq-crystals/kyber/commit/ab996e7460e5356b0e23aa034e7c2fe6922e60e6";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_kyber_768_cca_kem_length_public_key;
	kem->length_secret_key = OQS_KEM_kyber_768_cca_kem_length_secret_key;
	kem->length_ciphertext = OQS_KEM_kyber_768_cca_kem_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_kyber_768_cca_kem_length_shared_secret;

	kem->keypair = OQS_KEM_kyber_768_cca_kem_keypair;
	kem->encaps = OQS_KEM_kyber_768_cca_kem_encaps;
	kem->decaps = OQS_KEM_kyber_768_cca_kem_decaps;

	return kem;
}

int PQCLEAN_KYBER768_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_KYBER768_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int PQCLEAN_KYBER768_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

OQS_API OQS_STATUS OQS_KEM_kyber_768_cca_kem_keypair(uint8_t *public_key,
                                                     uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_KYBER768_CLEAN_crypto_kem_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_KEM_kyber_768_cca_kem_encaps(uint8_t *ciphertext,
                                                    uint8_t *shared_secret,
                                                    const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_KYBER768_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
}
OQS_API OQS_STATUS OQS_KEM_kyber_768_cca_kem_decaps(uint8_t *shared_secret,
                                                    const unsigned char *ciphertext,
                                                    const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_KYBER768_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
}

#endif

#ifdef OQS_ENABLE_KEM_kyber_1024_cca_kem

OQS_KEM *OQS_KEM_kyber_1024_cca_kem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_kyber_1024_cca_kem;
	kem->alg_version = "https://github.com/pq-crystals/kyber/commit/ab996e7460e5356b0e23aa034e7c2fe6922e60e6";

	kem->claimed_nist_level = 5;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_kyber_1024_cca_kem_length_public_key;
	kem->length_secret_key = OQS_KEM_kyber_1024_cca_kem_length_secret_key;
	kem->length_ciphertext = OQS_KEM_kyber_1024_cca_kem_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_kyber_1024_cca_kem_length_shared_secret;

	kem->keypair = OQS_KEM_kyber_1024_cca_kem_keypair;
	kem->encaps = OQS_KEM_kyber_1024_cca_kem_encaps;
	kem->decaps = OQS_KEM_kyber_1024_cca_kem_decaps;

	return kem;
}

#endif
