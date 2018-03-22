#include <assert.h>
#include <stdlib.h>

#include <oqs/oqs.h>

OQS_KEM *OQS_KEM_new(enum OQS_KEM_alg_name alg_name) {
	switch (alg_name) {
	case OQS_KEM_alg_default:
		return OQS_KEM_new(OQS_KEM_DEFAULT);
	case OQS_KEM_alg_dummy1:
#ifdef OQS_ENABLE_KEM_dummy1
		return OQS_KEM_dummy1_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_dummy2:
#ifdef OQS_ENABLE_KEM_dummy2
		return OQS_KEM_dummy2_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_frodokem_640_aes:
#ifdef OQS_ENABLE_KEM_frodokem_640_aes
		return OQS_KEM_frodokem_640_aes_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_frodokem_976_aes:
#ifdef OQS_ENABLE_KEM_frodokem_976_aes
		return OQS_KEM_frodokem_976_aes_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_frodokem_640_cshake:
#ifdef OQS_ENABLE_KEM_frodokem_640_cshake
		return OQS_KEM_frodokem_640_cshake_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_frodokem_976_cshake:
#ifdef OQS_ENABLE_KEM_frodokem_976_cshake
		return OQS_KEM_frodokem_976_cshake_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_newhope_512_cca_kem:
#ifdef OQS_ENABLE_KEM_newhope_512_cca_kem
		return OQS_KEM_newhope_512_cca_kem_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_newhope_1024_cca_kem:
#ifdef OQS_ENABLE_KEM_newhope_1024_cca_kem
		return OQS_KEM_newhope_1024_cca_kem_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_kyber512:
#ifdef OQS_ENABLE_KEM_kyber512
		return OQS_KEM_kyber512_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_kyber768:
#ifdef OQS_ENABLE_KEM_kyber768
		return OQS_KEM_kyber768_new();
#else
		return NULL;
#endif
	case OQS_KEM_alg_kyber1024:
#ifdef OQS_ENABLE_KEM_kyber1024
		return OQS_KEM_kyber1024_new();
#else
		return NULL;
#endif
		// EDIT-WHEN-ADDING-KEM
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
