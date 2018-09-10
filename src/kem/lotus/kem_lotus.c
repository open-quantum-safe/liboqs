#include <stdlib.h>

#include <oqs/kem_lotus.h>

#ifdef OQS_ENABLE_KEM_LOTUS192KEM

OQS_KEM *OQS_KEM_LOTUS192KEM_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_LOTUS192KEM;

	kem->claimed_nist_level = -1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_LOTUS192KEM_length_public_key;
	kem->length_secret_key = OQS_KEM_LOTUS192KEM_length_secret_key;
	kem->length_ciphertext = OQS_KEM_LOTUS192KEM_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_LOTUS192KEM_length_shared_secret;

	kem->keypair = OQS_KEM_LOTUS192KEM_keypair;
	kem->encaps = OQS_KEM_LOTUS192KEM_encaps;
	kem->decaps = OQS_KEM_LOTUS192KEM_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_LOTUS256KEM

OQS_KEM *OQS_KEM_LOTUS256KEM_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_LOTUS256KEM;

	kem->claimed_nist_level = -1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_LOTUS256KEM_length_public_key;
	kem->length_secret_key = OQS_KEM_LOTUS256KEM_length_secret_key;
	kem->length_ciphertext = OQS_KEM_LOTUS256KEM_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_LOTUS256KEM_length_shared_secret;

	kem->keypair = OQS_KEM_LOTUS256KEM_keypair;
	kem->encaps = OQS_KEM_LOTUS256KEM_encaps;
	kem->decaps = OQS_KEM_LOTUS256KEM_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_LOTUS128KEM

OQS_KEM *OQS_KEM_LOTUS128KEM_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_LOTUS128KEM;

	kem->claimed_nist_level = -1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_LOTUS128KEM_length_public_key;
	kem->length_secret_key = OQS_KEM_LOTUS128KEM_length_secret_key;
	kem->length_ciphertext = OQS_KEM_LOTUS128KEM_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_LOTUS128KEM_length_shared_secret;

	kem->keypair = OQS_KEM_LOTUS128KEM_keypair;
	kem->encaps = OQS_KEM_LOTUS128KEM_encaps;
	kem->decaps = OQS_KEM_LOTUS128KEM_decaps;

	return kem;
}

#endif
