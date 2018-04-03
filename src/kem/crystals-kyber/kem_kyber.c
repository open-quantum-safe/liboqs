#include <stdlib.h>

#include <oqs/kem_kyber.h>

#ifdef OQS_ENABLE_KEM_kyber512

OQS_KEM *OQS_KEM_kyber512_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_kyber512;

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_kyber512_length_public_key;
	kem->length_secret_key = OQS_KEM_kyber512_length_secret_key;
	kem->length_ciphertext = OQS_KEM_kyber512_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_kyber512_length_shared_secret;

	kem->keypair = OQS_KEM_kyber512_keypair;
	kem->encaps = OQS_KEM_kyber512_encaps;
	kem->decaps = OQS_KEM_kyber512_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_kyber768

OQS_KEM *OQS_KEM_kyber768_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_kyber768;

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_kyber768_length_public_key;
	kem->length_secret_key = OQS_KEM_kyber768_length_secret_key;
	kem->length_ciphertext = OQS_KEM_kyber768_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_kyber768_length_shared_secret;

	kem->keypair = OQS_KEM_kyber768_keypair;
	kem->encaps = OQS_KEM_kyber768_encaps;
	kem->decaps = OQS_KEM_kyber768_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_kyber1024

OQS_KEM *OQS_KEM_kyber1024_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_kyber1024;

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_kyber1024_length_public_key;
	kem->length_secret_key = OQS_KEM_kyber1024_length_secret_key;
	kem->length_ciphertext = OQS_KEM_kyber1024_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_kyber1024_length_shared_secret;

	kem->keypair = OQS_KEM_kyber1024_keypair;
	kem->encaps = OQS_KEM_kyber1024_encaps;
	kem->decaps = OQS_KEM_kyber1024_decaps;

	return kem;
}

#endif
