// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_frodokem.h>

#ifdef OQS_ENABLE_KEM_frodokem_976_shake

OQS_KEM *OQS_KEM_frodokem_976_shake_new(void) {

	OQS_KEM *kem = OQS_MEM_malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_frodokem_976_shake;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-LWEKE/commit/a2f9dec8917ccc3464b3378d46b140fa7353320d";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_frodokem_976_shake_length_public_key;
	kem->length_secret_key = OQS_KEM_frodokem_976_shake_length_secret_key;
	kem->length_ciphertext = OQS_KEM_frodokem_976_shake_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_frodokem_976_shake_length_shared_secret;
	kem->length_keypair_seed = OQS_KEM_frodokem_976_shake_length_keypair_seed;
	kem->length_encaps_seed = OQS_KEM_frodokem_976_shake_length_encaps_seed;

	kem->keypair = OQS_KEM_frodokem_976_shake_keypair;
	kem->keypair_derand = OQS_KEM_frodokem_976_shake_keypair_derand;
	kem->encaps = OQS_KEM_frodokem_976_shake_encaps;
	kem->encaps_derand = OQS_KEM_frodokem_976_shake_encaps_derand;
	kem->decaps = OQS_KEM_frodokem_976_shake_decaps;

	return kem;
}

#endif
