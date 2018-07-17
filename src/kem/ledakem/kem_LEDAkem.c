#include <stdlib.h>

#include <oqs/kem_ledakem.h>

#if defined(OQS_ENABLE_KEM_ledakem_C1_N02)

OQS_KEM *OQS_KEM_ledakem_C1_N02_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->method_name = "LEDAKEM_C1_N02";
	kem->length_public_key = OQS_KEM_ledakem_C1_N02_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C1_N02_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C1_N02_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C1_N02_length_shared_secret;

	kem->keypair = OQS_KEM_ledakem_C1_N02_keypair;
	kem->encaps = OQS_KEM_ledakem_C1_N02_encaps;
	kem->decaps = OQS_KEM_ledakem_C1_N02_decaps;
	return kem;
}

#endif

#if defined(OQS_ENABLE_KEM_ledakem_C1_N03)

OQS_KEM *OQS_KEM_ledakem_C1_N03_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->method_name = "LEDAKEM_C1_N03";
	kem->length_public_key = OQS_KEM_ledakem_C1_N03_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C1_N03_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C1_N03_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C1_N03_length_shared_secret;

	kem->keypair = OQS_KEM_ledakem_C1_N03_keypair;
	kem->encaps = OQS_KEM_ledakem_C1_N03_encaps;
	kem->decaps = OQS_KEM_ledakem_C1_N03_decaps;
	return kem;
}

#endif

#if defined(OQS_ENABLE_KEM_ledakem_C1_N04)

OQS_KEM *OQS_KEM_ledakem_C1_N04_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->method_name = "LEDAKEM_C1_N04";
	kem->length_public_key = OQS_KEM_ledakem_C1_N04_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C1_N04_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C1_N04_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C1_N04_length_shared_secret;

	kem->keypair = OQS_KEM_ledakem_C1_N04_keypair;
	kem->encaps = OQS_KEM_ledakem_C1_N04_encaps;
	kem->decaps = OQS_KEM_ledakem_C1_N04_decaps;
	return kem;
}

#endif

#if defined(OQS_ENABLE_KEM_ledakem_C3_N02)

OQS_KEM *OQS_KEM_ledakem_C3_N02_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->method_name = "LEDAKEM_C3_N02";
	kem->length_public_key = OQS_KEM_ledakem_C3_N02_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C3_N02_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C3_N02_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C3_N02_length_shared_secret;

	kem->keypair = OQS_KEM_ledakem_C3_N02_keypair;
	kem->encaps = OQS_KEM_ledakem_C3_N02_encaps;
	kem->decaps = OQS_KEM_ledakem_C3_N02_decaps;
	return kem;
}

#endif

#if defined(OQS_ENABLE_KEM_ledakem_C3_N03)

OQS_KEM *OQS_KEM_ledakem_C3_N03_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->method_name = "LEDAKEM_C3_N03";
	kem->length_public_key = OQS_KEM_ledakem_C3_N03_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C3_N03_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C3_N03_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C3_N03_length_shared_secret;

	kem->keypair = OQS_KEM_ledakem_C3_N03_keypair;
	kem->encaps = OQS_KEM_ledakem_C3_N03_encaps;
	kem->decaps = OQS_KEM_ledakem_C3_N03_decaps;
	return kem;
}

#endif

#if defined(OQS_ENABLE_KEM_ledakem_C3_N04)

OQS_KEM *OQS_KEM_ledakem_C3_N04_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->method_name = "LEDAKEM_C3_N04";
	kem->length_public_key = OQS_KEM_ledakem_C3_N04_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C3_N04_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C3_N04_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C3_N04_length_shared_secret;

	kem->keypair = OQS_KEM_ledakem_C3_N04_keypair;
	kem->encaps = OQS_KEM_ledakem_C3_N04_encaps;
	kem->decaps = OQS_KEM_ledakem_C3_N04_decaps;
	return kem;
}

#endif

#if defined(OQS_ENABLE_KEM_ledakem_C5_N02)

OQS_KEM *OQS_KEM_ledakem_C5_N02_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->method_name = "LEDAKEM_C5_N02";
	kem->length_public_key = OQS_KEM_ledakem_C5_N02_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C5_N02_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C5_N02_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C5_N02_length_shared_secret;

	kem->keypair = OQS_KEM_ledakem_C5_N02_keypair;
	kem->encaps = OQS_KEM_ledakem_C5_N02_encaps;
	kem->decaps = OQS_KEM_ledakem_C5_N02_decaps;
	return kem;
}

#endif

#if defined(OQS_ENABLE_KEM_ledakem_C5_N03)

OQS_KEM *OQS_KEM_ledakem_C5_N03_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->method_name = "LEDAKEM_C5_N03";
	kem->length_public_key = OQS_KEM_ledakem_C5_N03_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C5_N03_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C5_N03_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C5_N03_length_shared_secret;

	kem->keypair = OQS_KEM_ledakem_C5_N03_keypair;
	kem->encaps = OQS_KEM_ledakem_C5_N03_encaps;
	kem->decaps = OQS_KEM_ledakem_C5_N03_decaps;
	return kem;
}

#endif

#if defined(OQS_ENABLE_KEM_ledakem_C5_N04)

OQS_KEM *OQS_KEM_ledakem_C5_N04_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->method_name = "LEDAKEM_C5_N04";
	kem->length_public_key = OQS_KEM_ledakem_C5_N04_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C5_N04_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C5_N04_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C5_N04_length_shared_secret;

	kem->keypair = OQS_KEM_ledakem_C5_N04_keypair;
	kem->encaps = OQS_KEM_ledakem_C5_N04_encaps;
	kem->decaps = OQS_KEM_ledakem_C5_N04_decaps;

	return kem;
}

#endif
