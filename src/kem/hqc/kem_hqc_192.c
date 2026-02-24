// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_hqc.h>

#include "oqs_adapter.h"

#if defined(OQS_ENABLE_KEM_hqc_192)

OQS_KEM *OQS_KEM_hqc_192_new(void) {
	OQS_KEM *kem = OQS_MEM_malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_hqc_192;
	kem->alg_version =
	    "HQC reference implementation via https://gitlab.com/pqc-hqc/hqc "
	    "commit f46e5422 (v5.0.0, Aug. 22, 2025)";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_hqc_192_length_public_key;
	kem->length_secret_key = OQS_KEM_hqc_192_length_secret_key;
	kem->length_ciphertext = OQS_KEM_hqc_192_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_hqc_192_length_shared_secret;
	kem->length_keypair_seed = OQS_KEM_hqc_192_length_keypair_seed;
	kem->length_encaps_seed = OQS_KEM_hqc_192_length_encaps_seed;

	kem->keypair = OQS_KEM_hqc_192_keypair;
	kem->keypair_derand = OQS_KEM_hqc_192_keypair_derand;
	kem->encaps = OQS_KEM_hqc_192_encaps;
	kem->encaps_derand = OQS_KEM_hqc_192_encaps_derand;
	kem->decaps = OQS_KEM_hqc_192_decaps;

	return kem;
}

OQS_API OQS_STATUS OQS_KEM_hqc_192_keypair_derand(uint8_t *public_key,
        uint8_t *secret_key,
        const uint8_t *seed) {
	(void)public_key;
	(void)secret_key;
	(void)seed;
	return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_KEM_hqc_192_keypair(uint8_t *public_key,
        uint8_t *secret_key) {
	return (OQS_STATUS)OQS_MAKE_FN(KEM_PREFIX, crypto_kem_keypair)(public_key,
	        secret_key);
}

OQS_API OQS_STATUS OQS_KEM_hqc_192_encaps_derand(uint8_t *ciphertext,
        uint8_t *shared_secret,
        const uint8_t *public_key,
        const uint8_t *seed) {
	(void)ciphertext;
	(void)shared_secret;
	(void)public_key;
	(void)seed;
	return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_KEM_hqc_192_encaps(uint8_t *ciphertext,
        uint8_t *shared_secret,
        const uint8_t *public_key) {
	return (OQS_STATUS)OQS_MAKE_FN(KEM_PREFIX, crypto_kem_enc)(
	           ciphertext, shared_secret, public_key);
}

OQS_API OQS_STATUS OQS_KEM_hqc_192_decaps(uint8_t *shared_secret,
        const uint8_t *ciphertext,
        const uint8_t *secret_key) {
	return (OQS_STATUS)OQS_MAKE_FN(KEM_PREFIX, crypto_kem_dec)(
	           shared_secret, ciphertext, secret_key);
}

#endif
