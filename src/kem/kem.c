#include <assert.h>
#include <stdlib.h>

#include <oqs/oqs.h>

OQS_KEM *OQS_KEM_new(enum OQS_KEM_alg_name alg_name) {
	switch(alg_name) {
		case OQS_KEM_alg_default:
			return OQS_KEM_new(OQS_KEM_alg_dummy);
		case OQS_KEM_alg_dummy:
#ifdef OQS_ENABLE_KEM_dummy
			return OQS_KEM_dummy_new();
#else
			assert(0);
#endif
		default:
			assert(0);
	}
}

OQS_STATUS OQS_KEM_keypair(const OQS_KEM *kem, uint8_t *public_key, uint8_t *secret_key) {
	if (kem == NULL) {
		return OQS_ERROR;
	} else {
		return kem->keypair(public_key, secret_key);
	}
}
OQS_STATUS OQS_KEM_encaps(const OQS_KEM *kem, uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	if (kem == NULL) {
		return OQS_ERROR;
	} else {
		return kem->encaps(ciphertext, shared_secret, public_key);
	}
}
OQS_STATUS OQS_KEM_decaps(const OQS_KEM *kem, uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (kem == NULL) {
		return OQS_ERROR;
	} else {
		return kem->decaps(shared_secret, ciphertext, secret_key);
	}
}

void OQS_KEM_free(OQS_KEM *kem) {
	free(kem);
}
