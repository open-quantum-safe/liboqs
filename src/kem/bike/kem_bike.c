#include <stdlib.h>

#include <oqs/kem_bike.h>

#ifdef OQS_ENABLE_KEM_bike1_l1

OQS_KEM *OQS_KEM_bike1_l1_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike1_l1;
#if defined(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION)
	kem->alg_version = "Additional - 05/23/2018";
#else
	kem->alg_version = "Reference - 06/29/2018";
#endif

	kem->claimed_nist_level = 1;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike1_l1_length_public_key;
	kem->length_secret_key = OQS_KEM_bike1_l1_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike1_l1_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike1_l1_length_shared_secret;

	kem->keypair = OQS_KEM_bike1_l1_keypair;
	kem->encaps = OQS_KEM_bike1_l1_encaps;
	kem->decaps = OQS_KEM_bike1_l1_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_bike1_l3

OQS_KEM *OQS_KEM_bike1_l3_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike1_l3;
#if defined(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION)
	kem->alg_version = "Additional - 05/23/2018";
#else
	kem->alg_version = "Reference - 06/29/2018";
#endif

	kem->claimed_nist_level = 3;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike1_l3_length_public_key;
	kem->length_secret_key = OQS_KEM_bike1_l3_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike1_l3_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike1_l3_length_shared_secret;

	kem->keypair = OQS_KEM_bike1_l3_keypair;
	kem->encaps = OQS_KEM_bike1_l3_encaps;
	kem->decaps = OQS_KEM_bike1_l3_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_bike1_l5

OQS_KEM *OQS_KEM_bike1_l5_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike1_l5;
#if defined(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION)
	kem->alg_version = "Additional - 05/23/2018";
#else
	kem->alg_version = "Reference - 06/29/2018";
#endif

	kem->claimed_nist_level = 5;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike1_l5_length_public_key;
	kem->length_secret_key = OQS_KEM_bike1_l5_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike1_l5_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike1_l5_length_shared_secret;

	kem->keypair = OQS_KEM_bike1_l5_keypair;
	kem->encaps = OQS_KEM_bike1_l5_encaps;
	kem->decaps = OQS_KEM_bike1_l5_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_bike2_l1

OQS_KEM *OQS_KEM_bike2_l1_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike2_l1;
#if defined(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION)
	kem->alg_version = "Additional - 05/23/2018";
#else
	kem->alg_version = "Reference - 06/29/2018";
#endif

	kem->claimed_nist_level = 1;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike2_l1_length_public_key;
	kem->length_secret_key = OQS_KEM_bike2_l1_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike2_l1_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike2_l1_length_shared_secret;

	kem->keypair = OQS_KEM_bike2_l1_keypair;
	kem->encaps = OQS_KEM_bike2_l1_encaps;
	kem->decaps = OQS_KEM_bike2_l1_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_bike2_l3

OQS_KEM *OQS_KEM_bike2_l3_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike2_l3;
#if defined(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION)
	kem->alg_version = "Additional - 05/23/2018";
#else
	kem->alg_version = "Reference - 06/29/2018";
#endif

	kem->claimed_nist_level = 3;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike2_l3_length_public_key;
	kem->length_secret_key = OQS_KEM_bike2_l3_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike2_l3_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike2_l3_length_shared_secret;

	kem->keypair = OQS_KEM_bike2_l3_keypair;
	kem->encaps = OQS_KEM_bike2_l3_encaps;
	kem->decaps = OQS_KEM_bike2_l3_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_bike2_l5

OQS_KEM *OQS_KEM_bike2_l5_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike2_l5;
#if defined(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION)
	kem->alg_version = "Additional - 05/23/2018";
#else
	kem->alg_version = "Reference - 06/29/2018";
#endif

	kem->claimed_nist_level = 5;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike2_l5_length_public_key;
	kem->length_secret_key = OQS_KEM_bike2_l5_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike2_l5_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike2_l5_length_shared_secret;

	kem->keypair = OQS_KEM_bike2_l5_keypair;
	kem->encaps = OQS_KEM_bike2_l5_encaps;
	kem->decaps = OQS_KEM_bike2_l5_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_bike3_l1

OQS_KEM *OQS_KEM_bike3_l1_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike3_l1;
#if defined(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION)
	kem->alg_version = "Additional - 05/23/2018";
#else
	kem->alg_version = "Reference - 06/29/2018";
#endif

	kem->claimed_nist_level = 1;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike3_l1_length_public_key;
	kem->length_secret_key = OQS_KEM_bike3_l1_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike3_l1_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike3_l1_length_shared_secret;

	kem->keypair = OQS_KEM_bike3_l1_keypair;
	kem->encaps = OQS_KEM_bike3_l1_encaps;
	kem->decaps = OQS_KEM_bike3_l1_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_bike3_l3

OQS_KEM *OQS_KEM_bike3_l3_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike3_l3;
#if defined(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION)
	kem->alg_version = "Additional - 05/23/2018";
#else
	kem->alg_version = "Reference - 06/29/2018";
#endif

	kem->claimed_nist_level = 3;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike3_l3_length_public_key;
	kem->length_secret_key = OQS_KEM_bike3_l3_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike3_l3_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike3_l3_length_shared_secret;

	kem->keypair = OQS_KEM_bike3_l3_keypair;
	kem->encaps = OQS_KEM_bike3_l3_encaps;
	kem->decaps = OQS_KEM_bike3_l3_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_bike3_l5

OQS_KEM *OQS_KEM_bike3_l5_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_bike3_l5;
#if defined(OQS_KEM_BIKE_ADDITIONAL_IMPLEMENTATION)
	kem->alg_version = "Additional - 05/23/2018";
#else
	kem->alg_version = "Reference - 06/29/2018";
#endif

	kem->claimed_nist_level = 5;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_bike3_l5_length_public_key;
	kem->length_secret_key = OQS_KEM_bike3_l5_length_secret_key;
	kem->length_ciphertext = OQS_KEM_bike3_l5_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_bike3_l5_length_shared_secret;

	kem->keypair = OQS_KEM_bike3_l5_keypair;
	kem->encaps = OQS_KEM_bike3_l5_encaps;
	kem->decaps = OQS_KEM_bike3_l5_decaps;

	return kem;
}

#endif
