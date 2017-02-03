#include <assert.h>
#include <oqs/sig.h>

OQS_SIG *OQS_SIG_get(enum OQS_SIG_scheme_id scheme_id) {
  switch (scheme_id) {
  case OQS_SIG_scheme_id_default:
  default:
    // FIXME: TODO
    assert(0);
    return NULL;
  }
}

int OQS_SIG_keygen(const OQS_SIG *s, const OQS_RAND *rand, uint8_t *priv, uint8_t *pub) {
  if (s == NULL) {
    return 0;
  } else {
    return s->keygen(rand, priv, pub);
  }
}

int OQS_SIG_sign(const OQS_SIG *s, const OQS_RAND *rand, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t *sig, size_t *sig_len) {
  if (s == NULL) {
    return 0;
  } else {
    return s->sign(rand, priv, msg, msg_len, sig, sig_len);
  }
}

int OQS_SIG_verify(const OQS_SIG *s, const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len) {
  if (s == NULL) {
    return 0;
  } else {
    return s->verify(pub, msg, msg_len, sig, sig_len);
  }
}

