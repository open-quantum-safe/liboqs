#include <stdlib.h>

#include <oqs/kem_BIGQUAKE.h>

#ifdef OQS_ENABLE_KEM_BIG_QUAKE_1

OQS_KEM *OQS_KEM_BIG_QUAKE_1_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = "BIG_QUAKE_1";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_BIG_QUAKE_1_length_public_key;
	kem->length_secret_key = OQS_KEM_BIG_QUAKE_1_length_secret_key;
	kem->length_ciphertext = OQS_KEM_BIG_QUAKE_1_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_BIG_QUAKE_1_length_shared_secret;

	kem->keypair = OQS_KEM_BIG_QUAKE_1_keypair;
	kem->encaps = OQS_KEM_BIG_QUAKE_1_encaps;
	kem->decaps = OQS_KEM_BIG_QUAKE_1_decaps;

	return kem;
}

#endif
