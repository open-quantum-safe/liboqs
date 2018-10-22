#include <stdlib.h>

#include <oqs/kem_sike.h>

#ifdef OQS_ENABLE_KEM_sike_p503

OQS_KEM *OQS_KEM_sike_p503_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p503;
	kem->alg_version = "https://github.com/Microsoft/PQCrypto-SIDH/tree/77044b76181eb61c744ac8eb7ddc7a8fe72f6919";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p503_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p503_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p503_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p503_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p503_keypair;
	kem->encaps = OQS_KEM_sike_p503_encaps;
	kem->decaps = OQS_KEM_sike_p503_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_sike_p751

OQS_KEM *OQS_KEM_sike_p751_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p751;
	kem->alg_version = "https://github.com/Microsoft/PQCrypto-SIDH/tree/77044b76181eb61c744ac8eb7ddc7a8fe72f6919";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p751_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p751_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p751_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p751_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p751_keypair;
	kem->encaps = OQS_KEM_sike_p751_encaps;
	kem->decaps = OQS_KEM_sike_p751_decaps;

	return kem;
}

#endif
