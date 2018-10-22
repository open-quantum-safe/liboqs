#include <stdlib.h>

#include <oqs/sig_qtesla.h>

#ifdef OQS_ENABLE_SIG_qTESLA_I

OQS_SIG *OQS_SIG_qTESLA_I_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_qTESLA_I;
	sig->alg_version = "https://github.com/qtesla/qTesla/commit/5e921da989b9b44aba95f63d9c28927d518f630c";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_qTESLA_I_length_public_key;
	sig->length_secret_key = OQS_SIG_qTESLA_I_length_secret_key;
	sig->length_signature = OQS_SIG_qTESLA_I_length_signature;

	sig->keypair = OQS_SIG_qTESLA_I_keypair;
	sig->sign = OQS_SIG_qTESLA_I_sign;
	sig->verify = OQS_SIG_qTESLA_I_verify;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_III_size

OQS_SIG *OQS_SIG_qTESLA_III_size_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_qTESLA_III_size;
	sig->alg_version = "https://github.com/qtesla/qTesla/commit/5e921da989b9b44aba95f63d9c28927d518f630c";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_qTESLA_III_size_length_public_key;
	sig->length_secret_key = OQS_SIG_qTESLA_III_size_length_secret_key;
	sig->length_signature = OQS_SIG_qTESLA_III_size_length_signature;

	sig->keypair = OQS_SIG_qTESLA_III_size_keypair;
	sig->sign = OQS_SIG_qTESLA_III_size_sign;
	sig->verify = OQS_SIG_qTESLA_III_size_verify;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_III_speed

OQS_SIG *OQS_SIG_qTESLA_III_speed_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_qTESLA_III_speed;
	sig->alg_version = "https://github.com/qtesla/qTesla/commit/5e921da989b9b44aba95f63d9c28927d518f630c";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_qTESLA_III_speed_length_public_key;
	sig->length_secret_key = OQS_SIG_qTESLA_III_speed_length_secret_key;
	sig->length_signature = OQS_SIG_qTESLA_III_speed_length_signature;

	sig->keypair = OQS_SIG_qTESLA_III_speed_keypair;
	sig->sign = OQS_SIG_qTESLA_III_speed_sign;
	sig->verify = OQS_SIG_qTESLA_III_speed_verify;

	return sig;
}

#endif
