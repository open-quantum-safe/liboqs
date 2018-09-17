#include <assert.h>

#include <oqs/common.h>
#include <oqs/sig.h>
#ifdef ENABLE_SIG_PICNIC
#include <oqs/sig_picnic.h>
#endif
#ifdef ENABLE_SIG_QTESLA
#include <oqs/sig_qtesla.h>
#endif

OQS_API OQS_SIG *OQS_SIG_new(enum OQS_SIG_algid algid) {
	OQS_SIG *s = malloc(sizeof(OQS_SIG));
	if (s == NULL) {
		return NULL;
	}

	switch (algid) {
#ifdef ENABLE_SIG_PICNIC
	case OQS_SIG_picnic_L1_FS:
	case OQS_SIG_picnic_L1_UR:
	case OQS_SIG_picnic_L3_FS:
	case OQS_SIG_picnic_L3_UR:
	case OQS_SIG_picnic_L5_FS:
	case OQS_SIG_picnic_L5_UR:
	case OQS_SIG_default:
		if (OQS_SIG_picnic_get(s, algid) != OQS_SUCCESS) {
			OQS_MEM_insecure_free(s);
			return NULL;
		}
		break;
#endif
#ifdef ENABLE_SIG_QTESLA
	case OQS_SIG_qTESLA_I:
	case OQS_SIG_qTESLA_III_speed:
	case OQS_SIG_qTESLA_III_size:
		if (OQS_SIG_qTESLA_get(s, algid) != OQS_SUCCESS) {
			OQS_MEM_insecure_free(s);
			return NULL;
		}
		break;
#endif
	default:
		OQS_MEM_insecure_free(s);
		return NULL;
	}

	return s;
}

OQS_API OQS_STATUS OQS_SIG_keygen(const OQS_SIG *s, uint8_t *priv, uint8_t *pub) {
	if (s == NULL) {
		return OQS_ERROR;
	} else {
		return s->keygen(s, priv, pub);
	}
}

OQS_API OQS_STATUS OQS_SIG_sign(const OQS_SIG *s, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t *sig, size_t *sig_len) {
	if (s == NULL) {
		return OQS_ERROR;
	} else {
		return s->sign(s, priv, msg, msg_len, sig, sig_len);
	}
}

OQS_API OQS_STATUS OQS_SIG_verify(const OQS_SIG *s, const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len) {
	if (s == NULL) {
		return OQS_ERROR;
	} else {
		return s->verify(s, pub, msg, msg_len, sig, sig_len);
	}
}

OQS_API void OQS_SIG_free(OQS_SIG *s) {
	if (s) {
		s->free(s); // IGNORE free-check
	}
}
