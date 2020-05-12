// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_bike.h>

#ifdef OQS_ENABLE_KEM_bike1_l1_cpa
OQS_KEM *OQS_KEM_bike1_l1_cpa_new() {
	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike1_l1_cpa;
	kem->alg_version = "Additional - 05/23/2018";

	kem->claimed_nist_level = 1;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike1_l1_cpa_length_public_key;
	kem->length_secret_key = OQS_KEM_bike1_l1_cpa_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike1_l1_cpa_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike1_l1_cpa_length_shared_secret;

	kem->keypair = OQS_KEM_bike1_l1_cpa_keypair;
	kem->encaps = OQS_KEM_bike1_l1_cpa_encaps;
	kem->decaps = OQS_KEM_bike1_l1_cpa_decaps;

	return kem;
}
#endif

#ifdef OQS_ENABLE_KEM_bike1_l3_cpa
OQS_KEM *OQS_KEM_bike1_l3_cpa_new() {
	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike1_l3_cpa;
	kem->alg_version = "Additional - 05/23/2018";

	kem->claimed_nist_level = 3;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike1_l3_cpa_length_public_key;
	kem->length_secret_key = OQS_KEM_bike1_l3_cpa_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike1_l3_cpa_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike1_l3_cpa_length_shared_secret;

	kem->keypair = OQS_KEM_bike1_l3_cpa_keypair;
	kem->encaps = OQS_KEM_bike1_l3_cpa_encaps;
	kem->decaps = OQS_KEM_bike1_l3_cpa_decaps;

	return kem;
}
#endif

#ifdef OQS_ENABLE_KEM_bike1_l1_fo
OQS_KEM *OQS_KEM_bike1_l1_fo_new() {
	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike1_l1_fo;
	kem->alg_version = "Additional - 05/23/2018";

	kem->claimed_nist_level = 1;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike1_l1_fo_length_public_key;
	kem->length_secret_key = OQS_KEM_bike1_l1_fo_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike1_l1_fo_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike1_l1_fo_length_shared_secret;

	kem->keypair = OQS_KEM_bike1_l1_fo_keypair;
	kem->encaps = OQS_KEM_bike1_l1_fo_encaps;
	kem->decaps = OQS_KEM_bike1_l1_fo_decaps;

	return kem;
}
#endif

#ifdef OQS_ENABLE_KEM_bike1_l3_fo
OQS_KEM *OQS_KEM_bike1_l3_fo_new() {
	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike1_l3_fo;
	kem->alg_version = "Additional - 05/23/2018";

	kem->claimed_nist_level = 3;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike1_l3_fo_length_public_key;
	kem->length_secret_key = OQS_KEM_bike1_l3_fo_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike1_l3_fo_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike1_l3_fo_length_shared_secret;

	kem->keypair = OQS_KEM_bike1_l3_fo_keypair;
	kem->encaps = OQS_KEM_bike1_l3_fo_encaps;
	kem->decaps = OQS_KEM_bike1_l3_fo_decaps;

	return kem;
}
#endif
