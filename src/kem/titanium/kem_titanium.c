#include <stdlib.h>

#include <oqs/kem_titanium.h>

#ifdef OQS_ENABLE_KEM_titanium_cca_std_kem

OQS_KEM *OQS_KEM_titanium_cca_std_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_titanium_cca_std_kem;
	kem->alg_version = "https://github.com/raykzhao/Titanium/tree/a7547ad486e7220e1d9ffac7a76fe260dfa36cc3";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_titanium_cca_std_length_public_key;
	kem->length_secret_key = OQS_KEM_titanium_cca_std_length_secret_key;
	kem->length_ciphertext = OQS_KEM_titanium_cca_std_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_titanium_cca_std_length_shared_secret;

	// redirection: link up externally defined API functions to KEM struct
	kem->keypair = OQS_KEM_titanium_cca_std_keypair;
	kem->encaps = OQS_KEM_titanium_cca_std_encaps;
	kem->decaps = OQS_KEM_titanium_cca_std_decaps;

	return kem;
}

#endif /* OQS_ENABLE_KEM_titanium_cca_std_kem */

#ifdef OQS_ENABLE_KEM_titanium_cca_hi_kem

OQS_KEM *OQS_KEM_titanium_cca_hi_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_titanium_cca_hi_kem;
	kem->alg_version = "https://github.com/raykzhao/Titanium/tree/a7547ad486e7220e1d9ffac7a76fe260dfa36cc3";

	kem->claimed_nist_level = 3;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_titanium_cca_hi_length_public_key;
	kem->length_secret_key = OQS_KEM_titanium_cca_hi_length_secret_key;
	kem->length_ciphertext = OQS_KEM_titanium_cca_hi_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_titanium_cca_hi_length_shared_secret;

	kem->keypair = OQS_KEM_titanium_cca_hi_keypair;
	kem->encaps = OQS_KEM_titanium_cca_hi_encaps;
	kem->decaps = OQS_KEM_titanium_cca_hi_decaps;

	return kem;
}

#endif /* OQS_ENABLE_KEM_titanium_cca_med_kem */

#ifdef OQS_ENABLE_KEM_titanium_cca_med_kem

OQS_KEM *OQS_KEM_titanium_cca_med_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_titanium_cca_med_kem;
	kem->alg_version = "https://github.com/raykzhao/Titanium/tree/a7547ad486e7220e1d9ffac7a76fe260dfa36cc3";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_titanium_cca_med_length_public_key;
	kem->length_secret_key = OQS_KEM_titanium_cca_med_length_secret_key;
	kem->length_ciphertext = OQS_KEM_titanium_cca_med_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_titanium_cca_med_length_shared_secret;

	kem->keypair = OQS_KEM_titanium_cca_med_keypair;
	kem->encaps = OQS_KEM_titanium_cca_med_encaps;
	kem->decaps = OQS_KEM_titanium_cca_med_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_titanium_cca_super_kem

OQS_KEM *OQS_KEM_titanium_cca_super_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_titanium_cca_super_kem;
	kem->alg_version = "https://github.com/raykzhao/Titanium/tree/a7547ad486e7220e1d9ffac7a76fe260dfa36cc3";

	kem->claimed_nist_level = 5;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_titanium_cca_super_length_public_key;
	kem->length_secret_key = OQS_KEM_titanium_cca_super_length_secret_key;
	kem->length_ciphertext = OQS_KEM_titanium_cca_super_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_titanium_cca_super_length_shared_secret;

	kem->keypair = OQS_KEM_titanium_cca_super_keypair;
	kem->encaps = OQS_KEM_titanium_cca_super_encaps;
	kem->decaps = OQS_KEM_titanium_cca_super_decaps;

	return kem;
}

#endif /* OQS_ENABLE_KEM_titanium_cca_super_kem */
