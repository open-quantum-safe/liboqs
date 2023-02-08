// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_bike.h>

#ifdef OQS_ENABLE_KEM_bike_l1
OQS_KEM *OQS_KEM_bike_l1_new(void) {
	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike_l1;
	kem->alg_version = "Additional - 01/27/2023";

	kem->claimed_nist_level = 1;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike_l1_length_public_key;
	kem->length_secret_key = OQS_KEM_bike_l1_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike_l1_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike_l1_length_shared_secret;

	kem->keypair = OQS_KEM_bike_l1_keypair;
	kem->encaps = OQS_KEM_bike_l1_encaps;
	kem->decaps = OQS_KEM_bike_l1_decaps;

	return kem;
}
#endif

#ifdef OQS_ENABLE_KEM_bike_l3
OQS_KEM *OQS_KEM_bike_l3_new(void) {
	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike_l3;
	kem->alg_version = "Additional - 01/27/2023";

	kem->claimed_nist_level = 3;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike_l3_length_public_key;
	kem->length_secret_key = OQS_KEM_bike_l3_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike_l3_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike_l3_length_shared_secret;

	kem->keypair = OQS_KEM_bike_l3_keypair;
	kem->encaps = OQS_KEM_bike_l3_encaps;
	kem->decaps = OQS_KEM_bike_l3_decaps;

	return kem;
}
#endif

#ifdef OQS_ENABLE_KEM_bike_l5
OQS_KEM *OQS_KEM_bike_l5_new(void) {
	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike_l5;
	kem->alg_version = "Additional - 01/07/2023";

	kem->claimed_nist_level = 5;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike_l5_length_public_key;
	kem->length_secret_key = OQS_KEM_bike_l5_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike_l5_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike_l5_length_shared_secret;

	kem->keypair = OQS_KEM_bike_l5_keypair;
	kem->encaps = OQS_KEM_bike_l5_encaps;
	kem->decaps = OQS_KEM_bike_l5_decaps;

	return kem;
}
#endif
