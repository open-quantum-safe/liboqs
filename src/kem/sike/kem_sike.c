#include <stdlib.h>

#include <oqs/kem_sike.h>

#ifdef OQS_ENABLE_KEM_sike_p503

#include "P503/P503_api.h"

OQS_KEM *OQS_KEM_sike_p503_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p503;

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p503_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p503_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p503_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p503_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p503_keypair;
	kem->encaps = OQS_KEM_sike_p503_enc;
	kem->decaps = OQS_KEM_sike_p503_dec;

	return kem;
}

#endif

// Preventing redefinition of symbols
// Todo: remove
#undef OQS_SIDH_MSR_CRYPTO_SECRETKEYBYTES
#undef OQS_SIDH_MSR_CRYPTO_PUBLICKEYBYTES
#undef OQS_SIDH_MSR_CRYPTO_BYTES
#undef OQS_SIDH_MSR_CRYPTO_CIPHERTEXTBYTES
#undef OQS_SIDH_MSR_CRYPTO_ALGNAME
#undef SIDH_SECRETKEYBYTES
#undef SIDH_PUBLICKEYBYTES
#undef SIDH_BYTES

#ifdef OQS_ENABLE_KEM_sike_p751

#include "P751/P751_api.h"

OQS_KEM *OQS_KEM_sike_p751_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p751;

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_sike_p751_length_public_key;
	kem->length_secret_key = OQS_KEM_sike_p751_length_secret_key;
	kem->length_ciphertext = OQS_KEM_sike_p751_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_sike_p751_length_shared_secret;

	kem->keypair = OQS_KEM_sike_p751_keypair;
	kem->encaps = OQS_KEM_sike_p751_enc;
	kem->decaps = OQS_KEM_sike_p751_dec;

	return kem;
}

#endif
