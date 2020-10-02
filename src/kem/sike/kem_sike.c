// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_sike.h>

#ifdef OQS_ENABLE_KEM_sike_p434

#include "external/P434/P434_api.h"

OQS_KEM *OQS_KEM_sike_p434_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p434;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p434_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p434_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p434_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p434_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p434_keypair;
	kem->encaps = OQS_KEM_sike_p434_encaps;
	kem->decaps = OQS_KEM_sike_p434_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_sike_p434_compressed

#include "external/P434/P434_compressed_api.h"

OQS_KEM *OQS_KEM_sike_p434_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p434_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p434_compressed_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p434_compressed_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p434_compressed_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p434_compressed_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p434_compressed_keypair;
	kem->encaps = OQS_KEM_sike_p434_compressed_encaps;
	kem->decaps = OQS_KEM_sike_p434_compressed_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_sike_p503

#include "external/P503/P503_api.h"

OQS_KEM *OQS_KEM_sike_p503_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p503;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 2;
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

#ifdef OQS_ENABLE_KEM_sike_p503_compressed

#include "external/P503/P503_compressed_api.h"

OQS_KEM *OQS_KEM_sike_p503_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p503_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 2;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p503_compressed_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p503_compressed_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p503_compressed_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p503_compressed_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p503_compressed_keypair;
	kem->encaps = OQS_KEM_sike_p503_compressed_encaps;
	kem->decaps = OQS_KEM_sike_p503_compressed_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_sike_p610

#include "external/P610/P610_api.h"

OQS_KEM *OQS_KEM_sike_p610_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p610;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p610_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p610_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p610_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p610_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p610_keypair;
	kem->encaps = OQS_KEM_sike_p610_encaps;
	kem->decaps = OQS_KEM_sike_p610_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_sike_p610_compressed

#include "external/P610/P610_compressed_api.h"

OQS_KEM *OQS_KEM_sike_p610_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p610_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p610_compressed_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p610_compressed_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p610_compressed_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p610_compressed_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p610_compressed_keypair;
	kem->encaps = OQS_KEM_sike_p610_compressed_encaps;
	kem->decaps = OQS_KEM_sike_p610_compressed_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_sike_p751

#include "external/P751/P751_api.h"

OQS_KEM *OQS_KEM_sike_p751_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p751;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 5;
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

#ifdef OQS_ENABLE_KEM_sike_p751_compressed

#include "external/P751/P751_compressed_api.h"

OQS_KEM *OQS_KEM_sike_p751_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p751_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 5;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p751_compressed_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p751_compressed_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p751_compressed_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p751_compressed_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p751_compressed_keypair;
	kem->encaps = OQS_KEM_sike_p751_compressed_encaps;
	kem->decaps = OQS_KEM_sike_p751_compressed_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_sidh_p434

#include "external/P434/P434_api.h"

OQS_KEM *OQS_KEM_sidh_p434_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sidh_p434;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 1;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_sidh_p434_length_public_key;
	kem->length_secret_key = OQS_KEM_sidh_p434_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sidh_p434_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sidh_p434_length_shared_secret;

	kem->keypair = OQS_KEM_sidh_p434_keypair;
	kem->encaps = OQS_KEM_sidh_p434_encaps;
	kem->decaps = OQS_KEM_sidh_p434_decaps;

	return kem;
}

/* Forward KEM API calls to SIDH's API */
OQS_API OQS_STATUS OQS_KEM_sidh_p434_keypair(uint8_t *public_key, uint8_t *secret_key) {
	oqs_kem_sidh_p434_random_mod_order_A((unsigned char *) secret_key);
	if (oqs_kem_sidh_p434_EphemeralKeyGeneration_A((unsigned char *) secret_key, (unsigned char *) public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p434_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	OQS_STATUS ret = OQS_ERROR;
	unsigned char *secret_key = malloc(OQS_KEM_sidh_p434_length_secret_key);
	oqs_kem_sidh_p434_random_mod_order_B(secret_key);
	if (oqs_kem_sidh_p434_EphemeralKeyGeneration_B(secret_key, (unsigned char *) ciphertext)) {
		goto err;
	}
	if (oqs_kem_sidh_p434_EphemeralSecretAgreement_B(secret_key, (unsigned char *) public_key, (unsigned char *) shared_secret) != 0) {
		goto err;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_MEM_secure_free(secret_key, OQS_KEM_sidh_p434_length_secret_key);
	}

	return ret;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p434_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (oqs_kem_sidh_p434_EphemeralSecretAgreement_A((unsigned char *) secret_key, (unsigned char *) ciphertext, (unsigned char *) shared_secret) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif

#ifdef OQS_ENABLE_KEM_sidh_p434_compressed

#include "external/P434/P434_compressed_api.h"

OQS_KEM *OQS_KEM_sidh_p434_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sidh_p434_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 1;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_sidh_p434_compressed_length_public_key;
	kem->length_secret_key = OQS_KEM_sidh_p434_compressed_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sidh_p434_compressed_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sidh_p434_compressed_length_shared_secret;

	kem->keypair = OQS_KEM_sidh_p434_compressed_keypair;
	kem->encaps = OQS_KEM_sidh_p434_compressed_encaps;
	kem->decaps = OQS_KEM_sidh_p434_compressed_decaps;

	return kem;
}

/* Forward KEM API calls to SIDH's API */
OQS_API OQS_STATUS OQS_KEM_sidh_p434_compressed_keypair(uint8_t *public_key, uint8_t *secret_key) {
	oqs_kem_sidh_p434_compressed_random_mod_order_A((unsigned char *) secret_key);
	if (oqs_kem_sidh_p434_compressed_EphemeralKeyGeneration_A((unsigned char *) secret_key, (unsigned char *) public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p434_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	OQS_STATUS ret = OQS_ERROR;
	unsigned char *secret_key = malloc(OQS_KEM_sidh_p434_compressed_length_secret_key);
	oqs_kem_sidh_p434_compressed_random_mod_order_B(secret_key);
	if (oqs_kem_sidh_p434_compressed_EphemeralKeyGeneration_B(secret_key, (unsigned char *) ciphertext)) {
		goto err;
	}
	if (oqs_kem_sidh_p434_compressed_EphemeralSecretAgreement_B(secret_key, (unsigned char *) public_key, (unsigned char *) shared_secret) != 0) {
		goto err;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_MEM_secure_free(secret_key, OQS_KEM_sidh_p434_compressed_length_secret_key);
	}

	return ret;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p434_compressed_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (oqs_kem_sidh_p434_compressed_EphemeralSecretAgreement_A((unsigned char *) secret_key, (unsigned char *) ciphertext, (unsigned char *) shared_secret) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif

#ifdef OQS_ENABLE_KEM_sidh_p503

#include "external/P503/P503_api.h"

OQS_KEM *OQS_KEM_sidh_p503_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sidh_p503;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 2;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_sidh_p503_length_public_key;
	kem->length_secret_key = OQS_KEM_sidh_p503_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sidh_p503_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sidh_p503_length_shared_secret;

	kem->keypair = OQS_KEM_sidh_p503_keypair;
	kem->encaps = OQS_KEM_sidh_p503_encaps;
	kem->decaps = OQS_KEM_sidh_p503_decaps;

	return kem;
}

/* Forward KEM API calls to SIDH's API */
OQS_API OQS_STATUS OQS_KEM_sidh_p503_keypair(uint8_t *public_key, uint8_t *secret_key) {
	oqs_kem_sidh_p503_random_mod_order_A((unsigned char *) secret_key);
	if (oqs_kem_sidh_p503_EphemeralKeyGeneration_A((unsigned char *) secret_key, (unsigned char *) public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p503_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	OQS_STATUS ret = OQS_ERROR;
	unsigned char *secret_key = malloc(OQS_KEM_sidh_p503_length_secret_key);
	oqs_kem_sidh_p503_random_mod_order_B(secret_key);
	if (oqs_kem_sidh_p503_EphemeralKeyGeneration_B(secret_key, (unsigned char *) ciphertext)) {
		goto err;
	}
	if (oqs_kem_sidh_p503_EphemeralSecretAgreement_B(secret_key, (unsigned char *) public_key, (unsigned char *) shared_secret) != 0) {
		goto err;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_MEM_secure_free(secret_key, OQS_KEM_sidh_p503_length_secret_key);
	}

	return ret;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p503_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (oqs_kem_sidh_p503_EphemeralSecretAgreement_A((unsigned char *) secret_key, (unsigned char *) ciphertext, (unsigned char *) shared_secret) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif

#ifdef OQS_ENABLE_KEM_sidh_p503_compressed

#include "external/P503/P503_compressed_api.h"

OQS_KEM *OQS_KEM_sidh_p503_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sidh_p503_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 2;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_sidh_p503_compressed_length_public_key;
	kem->length_secret_key = OQS_KEM_sidh_p503_compressed_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sidh_p503_compressed_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sidh_p503_compressed_length_shared_secret;

	kem->keypair = OQS_KEM_sidh_p503_compressed_keypair;
	kem->encaps = OQS_KEM_sidh_p503_compressed_encaps;
	kem->decaps = OQS_KEM_sidh_p503_compressed_decaps;

	return kem;
}

/* Forward KEM API calls to SIDH's API */
OQS_API OQS_STATUS OQS_KEM_sidh_p503_compressed_keypair(uint8_t *public_key, uint8_t *secret_key) {
	oqs_kem_sidh_p503_compressed_random_mod_order_A((unsigned char *) secret_key);
	if (oqs_kem_sidh_p503_compressed_EphemeralKeyGeneration_A((unsigned char *) secret_key, (unsigned char *) public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p503_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	OQS_STATUS ret = OQS_ERROR;
	unsigned char *secret_key = malloc(OQS_KEM_sidh_p503_compressed_length_secret_key);
	oqs_kem_sidh_p503_compressed_random_mod_order_B(secret_key);
	if (oqs_kem_sidh_p503_compressed_EphemeralKeyGeneration_B(secret_key, (unsigned char *) ciphertext)) {
		goto err;
	}
	if (oqs_kem_sidh_p503_compressed_EphemeralSecretAgreement_B(secret_key, (unsigned char *) public_key, (unsigned char *) shared_secret) != 0) {
		goto err;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_MEM_secure_free(secret_key, OQS_KEM_sidh_p503_compressed_length_secret_key);
	}

	return ret;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p503_compressed_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (oqs_kem_sidh_p503_compressed_EphemeralSecretAgreement_A((unsigned char *) secret_key, (unsigned char *) ciphertext, (unsigned char *) shared_secret) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif

#ifdef OQS_ENABLE_KEM_sidh_p610

#include "external/P610/P610_api.h"

OQS_KEM *OQS_KEM_sidh_p610_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sidh_p610;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 3;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_sidh_p610_length_public_key;
	kem->length_secret_key = OQS_KEM_sidh_p610_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sidh_p610_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sidh_p610_length_shared_secret;

	kem->keypair = OQS_KEM_sidh_p610_keypair;
	kem->encaps = OQS_KEM_sidh_p610_encaps;
	kem->decaps = OQS_KEM_sidh_p610_decaps;

	return kem;
}

/* Forward KEM API calls to SIDH's API */
OQS_API OQS_STATUS OQS_KEM_sidh_p610_keypair(uint8_t *public_key, uint8_t *secret_key) {
	oqs_kem_sidh_p610_random_mod_order_A((unsigned char *) secret_key);
	if (oqs_kem_sidh_p610_EphemeralKeyGeneration_A((unsigned char *) secret_key, (unsigned char *) public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p610_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	OQS_STATUS ret = OQS_ERROR;
	unsigned char *secret_key = malloc(OQS_KEM_sidh_p610_length_secret_key);
	oqs_kem_sidh_p610_random_mod_order_B(secret_key);
	if (oqs_kem_sidh_p610_EphemeralKeyGeneration_B(secret_key, (unsigned char *) ciphertext)) {
		goto err;
	}
	if (oqs_kem_sidh_p610_EphemeralSecretAgreement_B(secret_key, (unsigned char *) public_key, (unsigned char *) shared_secret) != 0) {
		goto err;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_MEM_secure_free(secret_key, OQS_KEM_sidh_p610_length_secret_key);
	}

	return ret;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p610_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (oqs_kem_sidh_p610_EphemeralSecretAgreement_A((unsigned char *) secret_key, (unsigned char *) ciphertext, (unsigned char *) shared_secret) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif

#ifdef OQS_ENABLE_KEM_sidh_p610_compressed

#include "external/P610/P610_compressed_api.h"

OQS_KEM *OQS_KEM_sidh_p610_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sidh_p610_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 3;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_sidh_p610_compressed_length_public_key;
	kem->length_secret_key = OQS_KEM_sidh_p610_compressed_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sidh_p610_compressed_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sidh_p610_compressed_length_shared_secret;

	kem->keypair = OQS_KEM_sidh_p610_compressed_keypair;
	kem->encaps = OQS_KEM_sidh_p610_compressed_encaps;
	kem->decaps = OQS_KEM_sidh_p610_compressed_decaps;

	return kem;
}

/* Forward KEM API calls to SIDH's API */
OQS_API OQS_STATUS OQS_KEM_sidh_p610_compressed_keypair(uint8_t *public_key, uint8_t *secret_key) {
	oqs_kem_sidh_p610_compressed_random_mod_order_A((unsigned char *) secret_key);
	if (oqs_kem_sidh_p610_compressed_EphemeralKeyGeneration_A((unsigned char *) secret_key, (unsigned char *) public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p610_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	OQS_STATUS ret = OQS_ERROR;
	unsigned char *secret_key = malloc(OQS_KEM_sidh_p610_compressed_length_secret_key);
	oqs_kem_sidh_p610_compressed_random_mod_order_B(secret_key);
	if (oqs_kem_sidh_p610_compressed_EphemeralKeyGeneration_B(secret_key, (unsigned char *) ciphertext)) {
		goto err;
	}
	if (oqs_kem_sidh_p610_compressed_EphemeralSecretAgreement_B(secret_key, (unsigned char *) public_key, (unsigned char *) shared_secret) != 0) {
		goto err;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_MEM_secure_free(secret_key, OQS_KEM_sidh_p610_compressed_length_secret_key);
	}

	return ret;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p610_compressed_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (oqs_kem_sidh_p610_compressed_EphemeralSecretAgreement_A((unsigned char *) secret_key, (unsigned char *) ciphertext, (unsigned char *) shared_secret) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif

#ifdef OQS_ENABLE_KEM_sidh_p751

#include "external/P751/P751_api.h"

OQS_KEM *OQS_KEM_sidh_p751_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sidh_p751;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 5;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_sidh_p751_length_public_key;
	kem->length_secret_key = OQS_KEM_sidh_p751_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sidh_p751_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sidh_p751_length_shared_secret;

	kem->keypair = OQS_KEM_sidh_p751_keypair;
	kem->encaps = OQS_KEM_sidh_p751_encaps;
	kem->decaps = OQS_KEM_sidh_p751_decaps;

	return kem;
}

/* Forward KEM API calls to SIDH's API */
OQS_API OQS_STATUS OQS_KEM_sidh_p751_keypair(uint8_t *public_key, uint8_t *secret_key) {
	oqs_kem_sidh_p751_random_mod_order_A((unsigned char *) secret_key);
	if (oqs_kem_sidh_p751_EphemeralKeyGeneration_A((unsigned char *) secret_key, (unsigned char *) public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p751_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	OQS_STATUS ret = OQS_ERROR;
	unsigned char *secret_key = malloc(OQS_KEM_sidh_p751_length_secret_key);
	oqs_kem_sidh_p751_random_mod_order_B(secret_key);
	if (oqs_kem_sidh_p751_EphemeralKeyGeneration_B(secret_key, (unsigned char *) ciphertext)) {
		goto err;
	}
	if (oqs_kem_sidh_p751_EphemeralSecretAgreement_B(secret_key, (unsigned char *) public_key, (unsigned char *) shared_secret) != 0) {
		goto err;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_MEM_secure_free(secret_key, OQS_KEM_sidh_p751_length_secret_key);
	}

	return ret;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p751_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (oqs_kem_sidh_p751_EphemeralSecretAgreement_A((unsigned char *) secret_key, (unsigned char *) ciphertext, (unsigned char *) shared_secret) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif

#ifdef OQS_ENABLE_KEM_sidh_p751_compressed

#include "external/P751/P751_compressed_api.h"

OQS_KEM *OQS_KEM_sidh_p751_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sidh_p751_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/f43c9f7463071570f61bc576d5a50e5cb05d3b44";

	kem->claimed_nist_level = 5;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_sidh_p751_compressed_length_public_key;
	kem->length_secret_key = OQS_KEM_sidh_p751_compressed_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sidh_p751_compressed_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sidh_p751_compressed_length_shared_secret;

	kem->keypair = OQS_KEM_sidh_p751_compressed_keypair;
	kem->encaps = OQS_KEM_sidh_p751_compressed_encaps;
	kem->decaps = OQS_KEM_sidh_p751_compressed_decaps;

	return kem;
}

/* Forward KEM API calls to SIDH's API */
OQS_API OQS_STATUS OQS_KEM_sidh_p751_compressed_keypair(uint8_t *public_key, uint8_t *secret_key) {
	oqs_kem_sidh_p751_compressed_random_mod_order_A((unsigned char *) secret_key);
	if (oqs_kem_sidh_p751_compressed_EphemeralKeyGeneration_A((unsigned char *) secret_key, (unsigned char *) public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p751_compressed_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	OQS_STATUS ret = OQS_ERROR;
	unsigned char *secret_key = malloc(OQS_KEM_sidh_p751_compressed_length_secret_key);
	oqs_kem_sidh_p751_compressed_random_mod_order_B(secret_key);
	if (oqs_kem_sidh_p751_compressed_EphemeralKeyGeneration_B(secret_key, (unsigned char *) ciphertext)) {
		goto err;
	}
	if (oqs_kem_sidh_p751_compressed_EphemeralSecretAgreement_B(secret_key, (unsigned char *) public_key, (unsigned char *) shared_secret) != 0) {
		goto err;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_MEM_secure_free(secret_key, OQS_KEM_sidh_p751_compressed_length_secret_key);
	}

	return ret;
}

OQS_API OQS_STATUS OQS_KEM_sidh_p751_compressed_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (oqs_kem_sidh_p751_compressed_EphemeralSecretAgreement_A((unsigned char *) secret_key, (unsigned char *) ciphertext, (unsigned char *) shared_secret) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

#endif
