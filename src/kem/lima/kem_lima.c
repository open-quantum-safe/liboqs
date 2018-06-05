#include <stdlib.h>

#include <oqs/kem_lima.h>

#ifdef OQS_ENABLE_KEM_lima_2p_1024_cca_kem

OQS_KEM *OQS_KEM_lima_2p_1024_cca_kem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_lima_2p_1024_cca_kem;

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_lima_2p_1024_cca_kem_length_public_key;
	kem->length_secret_key = OQS_KEM_lima_2p_1024_cca_kem_length_secret_key;
	kem->length_ciphertext = OQS_KEM_lima_2p_1024_cca_kem_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_lima_2p_1024_cca_kem_length_shared_secret;

	kem->keypair = OQS_KEM_lima_2p_1024_cca_kem_keypair;
	kem->encaps = OQS_KEM_lima_2p_1024_cca_kem_encaps;
	kem->decaps = OQS_KEM_lima_2p_1024_cca_kem_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_lima_2p_2048_cca_kem

OQS_KEM *OQS_KEM_lima_2p_2048_cca_kem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_lima_2p_2048_cca_kem;

	kem->claimed_nist_level = 4;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_lima_2p_2048_cca_kem_length_public_key;
	kem->length_secret_key = OQS_KEM_lima_2p_2048_cca_kem_length_secret_key;
	kem->length_ciphertext = OQS_KEM_lima_2p_2048_cca_kem_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_lima_2p_2048_cca_kem_length_shared_secret;

	kem->keypair = OQS_KEM_lima_2p_2048_cca_kem_keypair;
	kem->encaps = OQS_KEM_lima_2p_2048_cca_kem_encaps;
	kem->decaps = OQS_KEM_lima_2p_2048_cca_kem_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_lima_sp_1018_cca_kem

OQS_KEM *OQS_KEM_lima_sp_1018_cca_kem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_lima_sp_1018_cca_kem;

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_lima_sp_1018_cca_kem_length_public_key;
	kem->length_secret_key = OQS_KEM_lima_sp_1018_cca_kem_length_secret_key;
	kem->length_ciphertext = OQS_KEM_lima_sp_1018_cca_kem_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_lima_sp_1018_cca_kem_length_shared_secret;

	kem->keypair = OQS_KEM_lima_sp_1018_cca_kem_keypair;
	kem->encaps = OQS_KEM_lima_sp_1018_cca_kem_encaps;
	kem->decaps = OQS_KEM_lima_sp_1018_cca_kem_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_lima_sp_1306_cca_kem

OQS_KEM *OQS_KEM_lima_sp_1306_cca_kem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_lima_sp_1306_cca_kem;

	kem->claimed_nist_level = 2;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_lima_sp_1306_cca_kem_length_public_key;
	kem->length_secret_key = OQS_KEM_lima_sp_1306_cca_kem_length_secret_key;
	kem->length_ciphertext = OQS_KEM_lima_sp_1306_cca_kem_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_lima_sp_1306_cca_kem_length_shared_secret;

	kem->keypair = OQS_KEM_lima_sp_1306_cca_kem_keypair;
	kem->encaps = OQS_KEM_lima_sp_1306_cca_kem_encaps;
	kem->decaps = OQS_KEM_lima_sp_1306_cca_kem_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_lima_sp_1822_cca_kem

OQS_KEM *OQS_KEM_lima_sp_1822_cca_kem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_lima_sp_1822_cca_kem;

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_lima_sp_1822_cca_kem_length_public_key;
	kem->length_secret_key = OQS_KEM_lima_sp_1822_cca_kem_length_secret_key;
	kem->length_ciphertext = OQS_KEM_lima_sp_1822_cca_kem_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_lima_sp_1822_cca_kem_length_shared_secret;

	kem->keypair = OQS_KEM_lima_sp_1822_cca_kem_keypair;
	kem->encaps = OQS_KEM_lima_sp_1822_cca_kem_encaps;
	kem->decaps = OQS_KEM_lima_sp_1822_cca_kem_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_lima_sp_2062_cca_kem

OQS_KEM *OQS_KEM_lima_sp_2062_cca_kem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_lima_sp_2062_cca_kem;

	kem->claimed_nist_level = 4;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_lima_sp_2062_cca_kem_length_public_key;
	kem->length_secret_key = OQS_KEM_lima_sp_2062_cca_kem_length_secret_key;
	kem->length_ciphertext = OQS_KEM_lima_sp_2062_cca_kem_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_lima_sp_2062_cca_kem_length_shared_secret;

	kem->keypair = OQS_KEM_lima_sp_2062_cca_kem_keypair;
	kem->encaps = OQS_KEM_lima_sp_2062_cca_kem_encaps;
	kem->decaps = OQS_KEM_lima_sp_2062_cca_kem_decaps;

	return kem;
}

#endif
