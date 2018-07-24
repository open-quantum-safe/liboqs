#include <stdlib.h>

#include <oqs/sig_dilithium.h>

#ifdef OQS_ENABLE_SIG_DILITHIUM_II

OQS_SIG *OQS_SIG_Dilithium_II_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_Dilithium_II;

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_Dilithium_II_length_public_key;
	sig->length_secret_key = OQS_SIG_Dilithium_II_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_Dilithium_II_length_sig_overhead;

	sig->keypair = OQS_SIG_Dilithium_II_keypair;
	sig->sign = OQS_SIG_Dilithium_II_sign;
	sig->sign_open = OQS_SIG_Dilithium_II_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_DILITHIUM_III

OQS_SIG *OQS_SIG_Dilithium_III_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_Dilithium_III;

	sig->claimed_nist_level = 2;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_Dilithium_III_length_public_key;
	sig->length_secret_key = OQS_SIG_Dilithium_III_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_Dilithium_III_length_sig_overhead;

	sig->keypair = OQS_SIG_Dilithium_III_keypair;
	sig->sign = OQS_SIG_Dilithium_III_sign;
	sig->sign_open = OQS_SIG_Dilithium_III_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_DILITHIUM_IV

OQS_SIG *OQS_SIG_Dilithium_IV_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_Dilithium_IV;

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_Dilithium_IV_length_public_key;
	sig->length_secret_key = OQS_SIG_Dilithium_IV_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_Dilithium_IV_length_sig_overhead;

	sig->keypair = OQS_SIG_Dilithium_IV_keypair;
	sig->sign = OQS_SIG_Dilithium_IV_sign;
	sig->sign_open = OQS_SIG_Dilithium_IV_sign_open;

	return sig;
}

#endif
