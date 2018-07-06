#include <stdlib.h>

#include <oqs/sig_qtesla.h>

#ifdef OQS_ENABLE_SIG_qTESLA_I

OQS_SIG *OQS_SIG_qTESLA_I_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_qTESLA_I;

	sig->claimed_nist_level = 1;
	sig->euf_cma = false;

	sig->length_public_key = OQS_SIG_qTESLA_I_length_public_key;
	sig->length_secret_key = OQS_SIG_qTESLA_I_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_qTESLA_I_length_sig_overhead;

	sig->keypair = OQS_SIG_qTESLA_I_keypair;
	sig->sign = OQS_SIG_qTESLA_I_sign;
	sig->sign_open = OQS_SIG_qTESLA_I_sign_open;

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

	sig->claimed_nist_level = 3;
	sig->euf_cma = false;

	sig->length_public_key = OQS_SIG_qTESLA_III_size_length_public_key;
	sig->length_secret_key = OQS_SIG_qTESLA_III_size_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_qTESLA_III_size_length_sig_overhead;

	sig->keypair = OQS_SIG_qTESLA_III_size_keypair;
	sig->sign = OQS_SIG_qTESLA_III_size_sign;
	sig->sign_open = OQS_SIG_qTESLA_III_size_sign_open;

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

	sig->claimed_nist_level = 3;
	sig->euf_cma = false;

	sig->length_public_key = OQS_SIG_qTESLA_III_speed_length_public_key;
	sig->length_secret_key = OQS_SIG_qTESLA_III_speed_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_qTESLA_III_speed_length_sig_overhead;

	sig->keypair = OQS_SIG_qTESLA_III_speed_keypair;
	sig->sign = OQS_SIG_qTESLA_III_speed_sign;
	sig->sign_open = OQS_SIG_qTESLA_III_speed_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_p_I

OQS_SIG *OQS_SIG_qTESLA_p_I_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_qTESLA_p_I;

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_qTESLA_p_I_length_public_key;
	sig->length_secret_key = OQS_SIG_qTESLA_p_I_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_qTESLA_p_I_length_sig_overhead;

	sig->keypair = OQS_SIG_qTESLA_p_I_keypair;
	sig->sign = OQS_SIG_qTESLA_p_I_sign;
	sig->sign_open = OQS_SIG_qTESLA_p_I_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_p_III

OQS_SIG *OQS_SIG_qTESLA_p_III_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_qTESLA_p_III;

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_qTESLA_p_III_length_public_key;
	sig->length_secret_key = OQS_SIG_qTESLA_p_III_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_qTESLA_p_III_length_sig_overhead;

	sig->keypair = OQS_SIG_qTESLA_p_III_keypair;
	sig->sign = OQS_SIG_qTESLA_p_III_sign;
	sig->sign_open = OQS_SIG_qTESLA_p_III_sign_open;

	return sig;
}

#endif
