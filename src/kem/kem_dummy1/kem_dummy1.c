#include <stdlib.h>

#include <oqs/kem_dummy1.h>

#include "upstream/api.h"

#ifdef OQS_ENABLE_KEM_dummy1

OQS_KEM *OQS_KEM_dummy1_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = CRYPTO_ALGNAME;

	kem->claimed_classical_security = 0;
	kem->claimed_quantum_security = 0;
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

#else

OQS_KEM *OQS_KEM_dummy1_new() {
	return NULL;
}

OQS_STATUS OQS_KEM_dummy1_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return OQS_ERROR;
}

OQS_STATUS OQS_KEM_dummy1_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	return OQS_ERROR;
}

OQS_STATUS OQS_KEM_dummy1_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	return OQS_ERROR;
}

#endif
