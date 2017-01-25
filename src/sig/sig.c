#include <assert.h>

#include <oqs/sig.h>

OQS_SIG *OQS_SIG_new(OQS_RAND *rand, enum OQS_SIG_alg_name alg_name, const char *named_parameters) {
  if (rand == 0 || named_parameters == 0) {} // here to silence unused params warning. TODO: remove
  switch (alg_name) {
  case OQS_SIG_alg_default:
  default:
    // FIXME: TODO
    assert(0);
    return NULL;
  }
}

int OQS_SIG_keygen(const OQS_SIG *s, uint8_t **priv, uint8_t **pub) {
  if (s == NULL) {
    return 0;
  } else {
    return s->keygen(s, priv, pub);
  }
}

int OQS_SIG_sign(const OQS_SIG *s, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t **sig, size_t *sig_len) {
  if (s == NULL) {
    return 0;
  } else {
    return s->sign(s, priv, msg, msg_len, sig, sig_len);
  }
}

int OQS_SIG_verify(const OQS_SIG *s, const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len) {
  if (s == NULL) {
    return 0;
  } else {
    return s->verify(s, pub, msg, msg_len, sig, sig_len);
  }
}

void OQS_SIG_free(OQS_SIG *s) {
  if (s) {
    s->free(s);
  }
}
