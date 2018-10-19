#include <stdlib.h>

#include <oqs/kem_frodokem.h>

#ifdef OQS_ENABLE_KEM_frodokem_640_aes

OQS_KEM *OQS_KEM_frodokem_640_aes_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_frodokem_640_aes;
	kem->alg_version = "https://github.com/Microsoft/PQCrypto-LWEKE/commit/47da00a91270b6f103232314eef0b891b83bfd3b";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_frodokem_640_aes_length_public_key;
	kem->length_secret_key = OQS_KEM_frodokem_640_aes_length_secret_key;
	kem->length_ciphertext = OQS_KEM_frodokem_640_aes_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_frodokem_640_aes_length_shared_secret;

	kem->keypair = OQS_KEM_frodokem_640_aes_keypair;
	kem->encaps = OQS_KEM_frodokem_640_aes_encaps;
	kem->decaps = OQS_KEM_frodokem_640_aes_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_frodokem_976_aes

OQS_KEM *OQS_KEM_frodokem_976_aes_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_frodokem_976_aes;
	kem->alg_version = "https://github.com/Microsoft/PQCrypto-LWEKE/commit/47da00a91270b6f103232314eef0b891b83bfd3b";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_frodokem_976_aes_length_public_key;
	kem->length_secret_key = OQS_KEM_frodokem_976_aes_length_secret_key;
	kem->length_ciphertext = OQS_KEM_frodokem_976_aes_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_frodokem_976_aes_length_shared_secret;

	kem->keypair = OQS_KEM_frodokem_976_aes_keypair;
	kem->encaps = OQS_KEM_frodokem_976_aes_encaps;
	kem->decaps = OQS_KEM_frodokem_976_aes_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_frodokem_640_cshake

OQS_KEM *OQS_KEM_frodokem_640_cshake_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_frodokem_640_cshake;
	kem->alg_version = "https://github.com/Microsoft/PQCrypto-LWEKE/commit/47da00a91270b6f103232314eef0b891b83bfd3b";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_frodokem_640_cshake_length_public_key;
	kem->length_secret_key = OQS_KEM_frodokem_640_cshake_length_secret_key;
	kem->length_ciphertext = OQS_KEM_frodokem_640_cshake_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_frodokem_640_cshake_length_shared_secret;

	kem->keypair = OQS_KEM_frodokem_640_cshake_keypair;
	kem->encaps = OQS_KEM_frodokem_640_cshake_encaps;
	kem->decaps = OQS_KEM_frodokem_640_cshake_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_frodokem_976_cshake

OQS_KEM *OQS_KEM_frodokem_976_cshake_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_frodokem_976_cshake;
	kem->alg_version = "https://github.com/Microsoft/PQCrypto-LWEKE/commit/47da00a91270b6f103232314eef0b891b83bfd3b";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_frodokem_976_cshake_length_public_key;
	kem->length_secret_key = OQS_KEM_frodokem_976_cshake_length_secret_key;
	kem->length_ciphertext = OQS_KEM_frodokem_976_cshake_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_frodokem_976_cshake_length_shared_secret;

	kem->keypair = OQS_KEM_frodokem_976_cshake_keypair;
	kem->encaps = OQS_KEM_frodokem_976_cshake_encaps;
	kem->decaps = OQS_KEM_frodokem_976_cshake_decaps;

	return kem;
}

#endif
