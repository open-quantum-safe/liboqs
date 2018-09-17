#include <oqs/sig.h>

#ifdef ENABLE_SIG_QTESLA

#include <string.h>
#include <oqs/common.h>
#include "sig_qtesla.h"
#include "external/qTESLA_I.h"
#include "external/qTESLA_III_size.h"
#include "external/qTESLA_III_speed.h"

static const char *qTESLA_I_name = "qTESLA_I";
static const char *qTESLA_III_size_name = "qTESLA_III_size";
static const char *qTESLA_III_speed_name = "qTESLA_III_speed";

OQS_STATUS OQS_SIG_qTESLA_get(OQS_SIG *s, enum OQS_SIG_algid algid) {
	if (s == NULL) {
		return OQS_ERROR;
	}

	// set the scheme-specific alg values
	switch (algid) {
	case OQS_SIG_qTESLA_I:
		s->method_name = qTESLA_I_name;
		s->estimated_classical_security = 104;
		s->estimated_quantum_security = 97;
		s->priv_key_len = 2112;
		s->pub_key_len = 1504;
		s->max_sig_len = 1376;
		break;
	case OQS_SIG_qTESLA_III_size:
		s->method_name = qTESLA_III_size_name;
		s->estimated_classical_security = 188;
		s->estimated_quantum_security = 169;
		s->priv_key_len = 4160;
		s->pub_key_len = 2976;
		s->max_sig_len = 2720;
		break;
	case OQS_SIG_qTESLA_III_speed:
		s->method_name = qTESLA_III_speed_name;
		s->estimated_classical_security = 178;
		s->estimated_quantum_security = 164;
		s->priv_key_len = 4160;
		s->pub_key_len = 3104;
		s->max_sig_len = 2848;
		break;
	default:
		return OQS_ERROR;
	}
	s->keygen = &OQS_SIG_qTESLA_keygen;
	s->sign = &OQS_SIG_qTESLA_sign;
	s->verify = &OQS_SIG_qTESLA_verify;
	s->free = &OQS_SIG_qTESLA_free; // IGNORE free-check

	return OQS_SUCCESS;
}

OQS_STATUS OQS_SIG_qTESLA_keygen(const OQS_SIG *s, uint8_t *priv, uint8_t *pub) {
	if (s == NULL || priv == NULL || pub == NULL) {
		return OQS_ERROR;
	}

	if (strcmp(s->method_name, qTESLA_I_name) == 0) {
		if (oqs_qTESLA_I_crypto_sign_keypair(pub, priv) != 0) {
			return OQS_ERROR;
		}
	} else if (strcmp(s->method_name, qTESLA_III_size_name) == 0) {
		if (oqs_qTESLA_III_size_crypto_sign_keypair(pub, priv) != 0) {
			return OQS_ERROR;
		}
	} else if (strcmp(s->method_name, qTESLA_III_speed_name) == 0) {
		if (oqs_qTESLA_III_speed_crypto_sign_keypair(pub, priv) != 0) {
			return OQS_ERROR;
		}
	} else {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_STATUS OQS_SIG_qTESLA_sign(const OQS_SIG *s, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t *sig, size_t *sig_len) {
	if (s == NULL || priv == NULL || msg == NULL || sig == NULL || sig_len == NULL) {
		return OQS_ERROR;
	}

	if (strcmp(s->method_name, qTESLA_I_name) == 0) {
		if (oqs_qTESLA_I_crypto_sign(sig, (unsigned long long *) sig_len, msg, (unsigned long long) msg_len, priv) != 0) {
			return OQS_ERROR;
		}
	} else if (strcmp(s->method_name, qTESLA_III_size_name) == 0) {
		if (oqs_qTESLA_III_size_crypto_sign(sig, (unsigned long long *) sig_len, msg, (unsigned long long) msg_len, priv) != 0) {
			return OQS_ERROR;
		}
	} else if (strcmp(s->method_name, qTESLA_III_speed_name) == 0) {
		if (oqs_qTESLA_III_speed_crypto_sign(sig, (unsigned long long *) sig_len, msg, (unsigned long long) msg_len, priv) != 0) {
			return OQS_ERROR;
		}
	} else {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_STATUS OQS_SIG_qTESLA_verify(UNUSED const OQS_SIG *s, const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len) {
	if (pub == NULL || msg == NULL || sig == NULL) {
		return OQS_ERROR;
	}

	if (strcmp(s->method_name, qTESLA_I_name) == 0) {
		if (oqs_qTESLA_I_crypto_verify((unsigned char *) msg, (unsigned long long) msg_len, sig, (unsigned long long) sig_len, pub) != 0) {
			return OQS_ERROR;
		}
	} else if (strcmp(s->method_name, qTESLA_III_size_name) == 0) {
		if (oqs_qTESLA_III_size_crypto_verify((unsigned char *) msg, (unsigned long long) msg_len, sig, (unsigned long long) sig_len, pub) != 0) {
			return OQS_ERROR;
		}
	} else if (strcmp(s->method_name, qTESLA_III_speed_name) == 0) {
		if (oqs_qTESLA_III_speed_crypto_verify((unsigned char *) msg, (unsigned long long) msg_len, sig, (unsigned long long) sig_len, pub) != 0) {
			return OQS_ERROR;
		}
	} else {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

void OQS_SIG_qTESLA_free(OQS_SIG *s) {
	if (!s) {
		return;
	}
	OQS_MEM_insecure_free(s);
}

#endif
