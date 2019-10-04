#include <stdlib.h>

#include <oqs/sig_qtesla.h>

#ifdef OQS_ENABLE_SIG_qTesla_p_I

OQS_SIG *OQS_SIG_qTesla_p_I_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_qTesla_p_I;
	sig->alg_version = "https://github.com/microsoft/qTESLA-Library/commit/a5e60927e08d9f2fefea74138f208d14514ba5d1";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_qTesla_p_I_length_public_key;
	sig->length_secret_key = OQS_SIG_qTesla_p_I_length_secret_key;
	sig->length_signature = OQS_SIG_qTesla_p_I_length_signature;

	sig->keypair = OQS_SIG_qTesla_p_I_keypair;
	sig->sign = OQS_SIG_qTesla_p_I_sign;
	sig->verify = OQS_SIG_qTesla_p_I_verify;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_qTesla_p_III

OQS_SIG *OQS_SIG_qTesla_p_III_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_qTesla_p_III;
	sig->alg_version = "https://github.com/microsoft/qTESLA-Library/commit/a5e60927e08d9f2fefea74138f208d14514ba5d1";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_qTesla_p_III_length_public_key;
	sig->length_secret_key = OQS_SIG_qTesla_p_III_length_secret_key;
	sig->length_signature = OQS_SIG_qTesla_p_III_length_signature;

	sig->keypair = OQS_SIG_qTesla_p_III_keypair;
	sig->sign = OQS_SIG_qTesla_p_III_sign;
	sig->verify = OQS_SIG_qTesla_p_III_verify;

	return sig;
}

#endif
