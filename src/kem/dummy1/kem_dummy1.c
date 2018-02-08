#include <stdlib.h>

#include <oqs/kem_dummy1.h>

#ifdef OQS_ENABLE_KEM_dummy1

OQS_KEM *OQS_KEM_dummy1_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = "Dummy 1";

	kem->claimed_nist_level = 0;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_dummy1_length_public_key;
	kem->length_secret_key = OQS_KEM_dummy1_length_secret_key;
	kem->length_ciphertext = OQS_KEM_dummy1_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_dummy1_length_shared_secret;

	kem->keypair = OQS_KEM_dummy1_keypair;
	kem->encaps = OQS_KEM_dummy1_encaps;
	kem->decaps = OQS_KEM_dummy1_decaps;

	return kem;
}

#endif
