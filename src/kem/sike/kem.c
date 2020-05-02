#include <oqs/common.h>

#ifdef OQS_ENABLE_KEM_SIKE

#ifdef COMPRESS
#include "compression/sike_compressed.c"
#else
#include "sike.c"
#endif

#endif

#ifdef OQS_ENABLE_KEM_SIDH

OQS_API OQS_STATUS OQS_KEM_sidh_keypair(uint8_t *public_key, uint8_t *secret_key) {
	random_mod_order_A((unsigned char *) secret_key);
	if (EphemeralKeyGeneration_A((unsigned char *) secret_key, (unsigned char *) public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_KEM_sidh_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	OQS_STATUS ret = OQS_ERROR;
	unsigned char *secret_key = malloc(SECRETKEY_B_BYTES);
	random_mod_order_B(secret_key);
	if (EphemeralKeyGeneration_B(secret_key, (unsigned char *) ciphertext)) {
		goto err;
	}
	if (EphemeralSecretAgreement_B(secret_key, (unsigned char *) public_key, (unsigned char *) shared_secret) != 0) {
		goto err;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_MEM_secure_free(secret_key, SECRETKEY_B_BYTES);
	}

	return ret;
}

OQS_API OQS_STATUS OQS_KEM_sidh_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (EphemeralSecretAgreement_A((unsigned char *) secret_key, (unsigned char *) ciphertext, (unsigned char *) shared_secret) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif
