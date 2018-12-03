#include <stdlib.h>

#include <oqs/kem_threebears.h>

#ifdef OQS_ENABLE_KEM_babybear_cca

OQS_KEM *OQS_KEM_babybear_cca_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_babybear_cca;
	kem->alg_version = "https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/ThreeBears.zip";

	kem->claimed_nist_level = 2;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_babybear_cca_length_public_key;
	kem->length_secret_key = OQS_KEM_babybear_cca_length_secret_key;
	kem->length_ciphertext = OQS_KEM_babybear_cca_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_babybear_cca_length_shared_secret;

	kem->keypair = OQS_KEM_babybear_cca_keypair;
	kem->encaps = OQS_KEM_babybear_cca_encaps;
	kem->decaps = OQS_KEM_babybear_cca_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_mamabear_cca

OQS_KEM *OQS_KEM_mamabear_cca_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_mamabear_cca;
	kem->alg_version = "https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/ThreeBears.zip";

	kem->claimed_nist_level = 4;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_mamabear_cca_length_public_key;
	kem->length_secret_key = OQS_KEM_mamabear_cca_length_secret_key;
	kem->length_ciphertext = OQS_KEM_mamabear_cca_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_mamabear_cca_length_shared_secret;

	kem->keypair = OQS_KEM_mamabear_cca_keypair;
	kem->encaps = OQS_KEM_mamabear_cca_encaps;
	kem->decaps = OQS_KEM_mamabear_cca_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_papabear_cca

OQS_KEM *OQS_KEM_papabear_cca_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_papabear_cca;
	kem->alg_version = "https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/ThreeBears.zip";

	kem->claimed_nist_level = 4;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_papabear_cca_length_public_key;
	kem->length_secret_key = OQS_KEM_papabear_cca_length_secret_key;
	kem->length_ciphertext = OQS_KEM_papabear_cca_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_papabear_cca_length_shared_secret;

	kem->keypair = OQS_KEM_papabear_cca_keypair;
	kem->encaps = OQS_KEM_papabear_cca_encaps;
	kem->decaps = OQS_KEM_papabear_cca_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_babybear_ephem_cpa

OQS_KEM *OQS_KEM_babybear_ephem_cpa_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_babybear_ephem_cpa;
	kem->alg_version = "https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/ThreeBears.zip";

	kem->claimed_nist_level = 2;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_babybear_ephem_cpa_length_public_key;
	kem->length_secret_key = OQS_KEM_babybear_ephem_cpa_length_secret_key;
	kem->length_ciphertext = OQS_KEM_babybear_ephem_cpa_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_babybear_ephem_cpa_length_shared_secret;

	kem->keypair = OQS_KEM_babybear_ephem_cpa_keypair;
	kem->encaps = OQS_KEM_babybear_ephem_cpa_encaps;
	kem->decaps = OQS_KEM_babybear_ephem_cpa_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_mamabear_ephem_cpa

OQS_KEM *OQS_KEM_mamabear_ephem_cpa_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_mamabear_ephem_cpa;
	kem->alg_version = "https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/ThreeBears.zip";

	kem->claimed_nist_level = 5;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_mamabear_ephem_cpa_length_public_key;
	kem->length_secret_key = OQS_KEM_mamabear_ephem_cpa_length_secret_key;
	kem->length_ciphertext = OQS_KEM_mamabear_ephem_cpa_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_mamabear_ephem_cpa_length_shared_secret;

	kem->keypair = OQS_KEM_mamabear_ephem_cpa_keypair;
	kem->encaps = OQS_KEM_mamabear_ephem_cpa_encaps;
	kem->decaps = OQS_KEM_mamabear_ephem_cpa_decaps;

	return kem;
}

#endif

#ifdef OQS_ENABLE_KEM_papabear_ephem_cpa

OQS_KEM *OQS_KEM_papabear_ephem_cpa_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_papabear_ephem_cpa;
	kem->alg_version = "https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/ThreeBears.zip";

	kem->claimed_nist_level = 5;
	kem->ind_cca = false;

	kem->length_public_key = OQS_KEM_papabear_ephem_cpa_length_public_key;
	kem->length_secret_key = OQS_KEM_papabear_ephem_cpa_length_secret_key;
	kem->length_ciphertext = OQS_KEM_papabear_ephem_cpa_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_papabear_ephem_cpa_length_shared_secret;

	kem->keypair = OQS_KEM_papabear_ephem_cpa_keypair;
	kem->encaps = OQS_KEM_papabear_ephem_cpa_encaps;
	kem->decaps = OQS_KEM_papabear_ephem_cpa_decaps;

	return kem;
}

#endif
