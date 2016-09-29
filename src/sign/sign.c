#include <oqs/rand.h>
#include <oqs/sign.h>

#if defined(OQS_SIGN_DEFAULT_BLISSB)
#include <oqs/kex_blissb.h>
#endif

OQS_SIGN *OQS_SIGN_new(OQS_RAND *rand, const uint8_t *seed, const size_t seed_len, const char *named_parameters) {
#if defined(OQS_SIGN_DEFAULT_BLISSB)
	return OQS_SIGN_blissb_new(rand, seed, seed_len, named_parameters);
#else
#error "No default SIGN method defined."
#endif
}

int OQS_SIGN_keygen(OQS_SIGN *s, void **priv, uint8_t **verif, size_t *verif_len) {
	if (s == NULL) {
		return 0;
	} else {
		return s->keygen(s, priv, verif, verif_len);
	}
}

int OQS_SIGN_sign(OQS_SIGN *s, void *priv, uint8_t *msg, const size_t msg_len, uint8_t **sig, size_t *sig_len) {
	if (s == NULL) {
		return 0;
	} else {
		return s->sign(s, priv, msg, msg_len, sig, sig_len);
	}
}

int OQS_SIGN_verify(OQS_SIGN *s, uint8_t *verif, const size_t verif_len, uint8_t *msg, const size_t msg_len, uint8_t *sig, const size_t sig_len) {
	if (s == NULL) {
		return 0;
	} else {
		return s->verify(s, verif, verif_len, msg, msg_len, sig, sig_len);
	}
}

void OQS_SIGN_priv_free(OQS_SIGN *s, void *priv) {
	if (s) {
		s->priv_free(s, priv);
	}
}

void OQS_SIGN_free(OQS_SIGN *s) {
	if (s) {
		s->free(s);
	}
}
