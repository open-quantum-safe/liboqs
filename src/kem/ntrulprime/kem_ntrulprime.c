#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/kem_ntrulprime.h>
#include "ref/crypto_kem_ntrulpr4591761.h"

static OQS_STATUS OQS_KEM_ntrulprime_keypair(uint8_t *public_key, uint8_t *secret_key);
static OQS_STATUS OQS_KEM_ntrulprime_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
static OQS_STATUS OQS_KEM_ntrulprime_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

OQS_KEM *OQS_KEM_ntrulprime_kem_new() {
	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}

	kem->method_name = OQS_KEM_alg_ntrulprime;
	kem->alg_version = "0.1";

	kem->claimed_nist_level = 5;
	kem->ind_cca = true;

	kem->length_public_key = crypto_kem_ntrulpr4591761_ref_PUBLICKEYBYTES;
	kem->length_secret_key = crypto_kem_ntrulpr4591761_ref_SECRETKEYBYTES;
	kem->length_ciphertext = crypto_kem_ntrulpr4591761_ref_CIPHERTEXTBYTES;
	kem->length_shared_secret = crypto_kem_ntrulpr4591761_ref_BYTES;

	kem->keypair = OQS_KEM_ntrulprime_keypair;
	kem->encaps = OQS_KEM_ntrulprime_encaps;
	kem->decaps = OQS_KEM_ntrulprime_decaps;

	return kem;
}

static OQS_STATUS OQS_KEM_ntrulprime_keypair(uint8_t *public_key, uint8_t *secret_key) {
	int res = crypto_kem_ntrulpr4591761_ref_keypair(public_key, secret_key);
	if (res) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

static OQS_STATUS OQS_KEM_ntrulprime_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	int res = crypto_kem_ntrulpr4591761_ref_enc(ciphertext, shared_secret, public_key);
	if (res) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

static OQS_STATUS OQS_KEM_ntrulprime_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	int res = crypto_kem_ntrulpr4591761_ref_dec(shared_secret, ciphertext, secret_key);
	if (res) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}
