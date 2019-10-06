#include <stdlib.h>

#include <oqs/sig_{{ family }}.h>

{% for scheme in schemes -%}
#ifdef OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}

OQS_SIG *OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_{{ family }}_{{ scheme['scheme'] }};
	sig->alg_version = "{{ scheme['metadata']['implementations'][0]['version'] }}";

	sig->claimed_nist_level = {{ scheme['metadata']['claimed-nist-level'] }};
	sig->euf_cma = {{ scheme['metadata']['euf_cma'] }};

	sig->length_public_key = OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_public_key;
	sig->length_secret_key = OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_secret_key;
	sig->length_signature = OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_signature;

	sig->keypair = OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_keypair;
	sig->sign = OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_sign;
	sig->verify = OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_verify;

	return sig;
}

int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['implementation']|upper }}_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['implementation']|upper }}_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['implementation']|upper }}_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['implementation']|upper }}_crypto_sign_keypair(public_key, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['implementation']|upper }}_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['implementation']|upper }}_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
}

#endif
{% endfor -%}
