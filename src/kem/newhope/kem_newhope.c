#include <stdlib.h>

#include <oqs/kem_newhope.h>

#ifdef OQS_ENABLE_KEM_newhope_512cca

OQS_KEM *OQS_KEM_newhope_512cca_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_newhope_512cca;

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_newhope_512cca_length_public_key;
	kem->length_secret_key = OQS_KEM_newhope_512cca_length_secret_key;
	kem->length_ciphertext = OQS_KEM_newhope_512cca_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_newhope_512cca_length_shared_secret;

	kem->keypair = OQS_KEM_newhope_512cca_keypair;
	kem->encaps = OQS_KEM_newhope_512cca_encaps;
	kem->decaps = OQS_KEM_newhope_512cca_decaps;

	return kem;
}

#endif


#ifdef OQS_ENABLE_KEM_newhope_1024cca

OQS_KEM *OQS_KEM_newhope_1024cca_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_newhope_1024cca;

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_newhope_1024cca_length_public_key;
	kem->length_secret_key = OQS_KEM_newhope_1024cca_length_secret_key;
	kem->length_ciphertext = OQS_KEM_newhope_1024cca_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_newhope_1024cca_length_shared_secret;

	kem->keypair = OQS_KEM_newhope_1024cca_keypair;
	kem->encaps = OQS_KEM_newhope_1024cca_encaps;
	kem->decaps = OQS_KEM_newhope_1024cca_decaps;

	return kem;
}

#endif


