#include <stdlib.h>

#include <oqs/kem_BIGQUAKE.h>

#ifdef OQS_ENABLE_KEM_BIG_QUAKE_1

OQS_KEM *OQS_KEM_BIG_QUAKE_1_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = "BIG_QUAKE_1";
	kem->alg_version = "https://bigquake.inria.fr/files/2018/03/BIGQUAKE-source.tar.gz";

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

#ifdef OQS_ENABLE_KEM_BIG_QUAKE_3

OQS_KEM *OQS_KEM_BIG_QUAKE_3_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = "BIG_QUAKE_3";
	kem->alg_version = "https://bigquake.inria.fr/files/2018/03/BIGQUAKE-source.tar.gz";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_BIG_QUAKE_3_length_public_key;
	kem->length_secret_key = OQS_KEM_BIG_QUAKE_3_length_secret_key;
	kem->length_ciphertext = OQS_KEM_BIG_QUAKE_3_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_BIG_QUAKE_3_length_shared_secret;

	kem->keypair = OQS_KEM_BIG_QUAKE_3_keypair;
	kem->encaps = OQS_KEM_BIG_QUAKE_3_encaps;
	kem->decaps = OQS_KEM_BIG_QUAKE_3_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_BIG_QUAKE_5

OQS_KEM *OQS_KEM_BIG_QUAKE_5_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = "BIG_QUAKE_5";
	kem->alg_version = "https://bigquake.inria.fr/files/2018/03/BIGQUAKE-source.tar.gz";

	kem->claimed_nist_level = 5;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_BIG_QUAKE_5_length_public_key;
	kem->length_secret_key = OQS_KEM_BIG_QUAKE_5_length_secret_key;
	kem->length_ciphertext = OQS_KEM_BIG_QUAKE_5_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_BIG_QUAKE_5_length_shared_secret;

	kem->keypair = OQS_KEM_BIG_QUAKE_5_keypair;
	kem->encaps = OQS_KEM_BIG_QUAKE_5_encaps;
	kem->decaps = OQS_KEM_BIG_QUAKE_5_decaps;

	return kem;
}

#endif
