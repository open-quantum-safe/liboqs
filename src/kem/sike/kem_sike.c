#include <stdlib.h>

#include <oqs/kem_sike.h>

#ifdef OQS_ENABLE_KEM_sike_p434

#include "P434/P434_api.h"

OQS_KEM *OQS_KEM_sike_p434_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p434;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/fde210a7cf03e835cceb1d5ff34ccf58625d0311";

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

#include "P434/P434_compressed_api.h"

OQS_KEM *OQS_KEM_sike_p434_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p434_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/fde210a7cf03e835cceb1d5ff34ccf58625d0311";

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

#include "P503/P503_api.h"

OQS_KEM *OQS_KEM_sike_p503_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p503;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/fde210a7cf03e835cceb1d5ff34ccf58625d0311";

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

#include "P503/P503_compressed_api.h"

OQS_KEM *OQS_KEM_sike_p503_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p503_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/fde210a7cf03e835cceb1d5ff34ccf58625d0311";

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

#include "P610/P610_api.h"

OQS_KEM *OQS_KEM_sike_p610_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p610;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/fde210a7cf03e835cceb1d5ff34ccf58625d0311";

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

#include "P610/P610_compressed_api.h"

OQS_KEM *OQS_KEM_sike_p610_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p610_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/fde210a7cf03e835cceb1d5ff34ccf58625d0311";

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

#include "P751/P751_api.h"

OQS_KEM *OQS_KEM_sike_p751_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p751;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/fde210a7cf03e835cceb1d5ff34ccf58625d0311";

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

#include "P751/P751_compressed_api.h"

OQS_KEM *OQS_KEM_sike_p751_compressed_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_sike_p751_compressed;
	kem->alg_version = "https://github.com/microsoft/PQCrypto-SIDH/commit/fde210a7cf03e835cceb1d5ff34ccf58625d0311";

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
