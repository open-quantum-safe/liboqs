#include <stdlib.h>

#include <oqs/sig_dilithium.h>

// TODO: can't reliably provide a base version as code included in libOQS
//       doesn't match the NIST PQ submission zip or their github version.

#ifdef OQS_ENABLE_SIG_Dilithium_II_medium

OQS_SIG *OQS_SIG_Dilithium_II_medium_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_Dilithium_II_medium;
	sig->alg_version = "TODO";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_Dilithium_II_medium_length_public_key;
	sig->length_secret_key = OQS_SIG_Dilithium_II_medium_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_Dilithium_II_medium_length_sig_overhead;

	sig->keypair = OQS_SIG_Dilithium_II_medium_keypair;
	sig->sign = OQS_SIG_Dilithium_II_medium_sign;
	sig->sign_open = OQS_SIG_Dilithium_II_medium_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_Dilithium_III_recommended

OQS_SIG *OQS_SIG_Dilithium_III_recommended_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_Dilithium_III_recommended;
	sig->alg_version = "TODO";

	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_Dilithium_III_recommended_length_public_key;
	sig->length_secret_key = OQS_SIG_Dilithium_III_recommended_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_Dilithium_III_recommended_length_sig_overhead;

	sig->keypair = OQS_SIG_Dilithium_III_recommended_keypair;
	sig->sign = OQS_SIG_Dilithium_III_recommended_sign;
	sig->sign_open = OQS_SIG_Dilithium_III_recommended_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_Dilithium_IV_very_high

OQS_SIG *OQS_SIG_Dilithium_IV_very_high_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_Dilithium_IV_very_high;
	sig->alg_version = "TODO";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_Dilithium_IV_very_high_length_public_key;
	sig->length_secret_key = OQS_SIG_Dilithium_IV_very_high_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_Dilithium_IV_very_high_length_sig_overhead;

	sig->keypair = OQS_SIG_Dilithium_IV_very_high_keypair;
	sig->sign = OQS_SIG_Dilithium_IV_very_high_sign;
	sig->sign_open = OQS_SIG_Dilithium_IV_very_high_sign_open;

	return sig;
}

#endif
