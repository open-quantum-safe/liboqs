#include <stdlib.h>

#include <oqs/sig_picnic.h>

#ifdef OQS_ENABLE_SIG_picnic_L1_FS

// TODO: can't reliably provide a Picnic base version as code included in libOQS
//       doesn't match the NIST PQ submission zip or their website.

OQS_SIG *OQS_SIG_picnic_L1_FS_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L1_FS;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L1_FS_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L1_FS_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_picnic_L1_FS_length_sig_overhead;

	sig->keypair = OQS_SIG_picnic_L1_FS_keypair;
	sig->sign = OQS_SIG_picnic_L1_FS_sign;
	sig->sign_open = OQS_SIG_picnic_L1_FS_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L1_UR

OQS_SIG *OQS_SIG_picnic_L1_UR_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L1_UR;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L1_UR_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L1_UR_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_picnic_L1_UR_length_sig_overhead;

	sig->keypair = OQS_SIG_picnic_L1_UR_keypair;
	sig->sign = OQS_SIG_picnic_L1_UR_sign;
	sig->sign_open = OQS_SIG_picnic_L1_UR_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_FS

OQS_SIG *OQS_SIG_picnic_L3_FS_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L3_FS;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L3_FS_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L3_FS_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_picnic_L3_FS_length_sig_overhead;

	sig->keypair = OQS_SIG_picnic_L3_FS_keypair;
	sig->sign = OQS_SIG_picnic_L3_FS_sign;
	sig->sign_open = OQS_SIG_picnic_L3_FS_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_UR

OQS_SIG *OQS_SIG_picnic_L3_UR_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L3_UR;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L3_UR_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L3_UR_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_picnic_L3_UR_length_sig_overhead;

	sig->keypair = OQS_SIG_picnic_L3_UR_keypair;
	sig->sign = OQS_SIG_picnic_L3_UR_sign;
	sig->sign_open = OQS_SIG_picnic_L3_UR_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_FS

OQS_SIG *OQS_SIG_picnic_L5_FS_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L5_FS;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 5;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L5_FS_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L5_FS_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_picnic_L5_FS_length_sig_overhead;

	sig->keypair = OQS_SIG_picnic_L5_FS_keypair;
	sig->sign = OQS_SIG_picnic_L5_FS_sign;
	sig->sign_open = OQS_SIG_picnic_L5_FS_sign_open;

	return sig;
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_UR

OQS_SIG *OQS_SIG_picnic_L5_UR_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L5_UR;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 5;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L5_UR_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L5_UR_length_secret_key;
	sig->length_sig_overhead = OQS_SIG_picnic_L5_UR_length_sig_overhead;

	sig->keypair = OQS_SIG_picnic_L5_UR_keypair;
	sig->sign = OQS_SIG_picnic_L5_UR_sign;
	sig->sign_open = OQS_SIG_picnic_L5_UR_sign_open;

	return sig;
}

#endif
