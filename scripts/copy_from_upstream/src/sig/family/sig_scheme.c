// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/sig_{{ family }}.h>

{% for scheme in schemes -%}
#if defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }})

OQS_SIG *OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_new(void) {

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

    {%- for impl in scheme['metadata']['implementations'] if impl['name'] == scheme['default_implementation'] %}

        {%- if impl['signature_keypair'] %}
           {%- set cleankeypair = scheme['metadata'].update({'default_keypair_signature': impl['signature_keypair']}) -%}
        {%- else %}
           {%- set cleankeypair = scheme['metadata'].update({'default_keypair_signature': "PQCLEAN_"+scheme['pqclean_scheme_c']|upper+"_"+scheme['default_implementation']|upper+"_crypto_sign_keypair"}) -%}
        {%- endif %}

extern int {{ scheme['metadata']['default_keypair_signature'] }}(uint8_t *pk, uint8_t *sk);

        {%- if impl['signature_signature'] %}
           {%- set cleansignature = scheme['metadata'].update({'default_signature_signature': impl['signature_signature']}) -%}
        {%- else %}
           {%- set cleansignature = scheme['metadata'].update({'default_signature_signature': "PQCLEAN_"+scheme['pqclean_scheme_c']|upper+"_"+scheme['default_implementation']|upper+"_crypto_sign_signature"}) -%}
        {%- endif %}
extern int {{ scheme['metadata']['default_signature_signature'] }}(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);

        {%- if impl['signature_verify'] %}
           {%- set cleanverify = scheme['metadata'].update({'default_verify_signature': impl['signature_verify']}) -%}
        {%- else %}
           {%- set cleanverify = scheme['metadata'].update({'default_verify_signature': "PQCLEAN_"+scheme['pqclean_scheme_c']|upper+"_"+scheme['default_implementation']|upper+"_crypto_sign_verify"}) -%}
        {%- endif %}
extern int {{ scheme['metadata']['default_verify_signature']  }}(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

    {%- endfor %}

    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}

#if defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
        {%- if impl['signature_keypair'] %}
extern int {{ impl['signature_keypair'] }}(uint8_t *pk, uint8_t *sk);
        {%- else %}
extern int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
        {%- endif %}

        {%- if impl['signature_signature'] %}
extern int {{ impl['signature_signature'] }}(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
        {%- else %}
extern int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
        {%- endif %}

        {%- if impl['signature_verify'] %}
extern int {{ impl['signature_verify'] }}(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);
        {%- else %}
extern int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);
        {%- endif %}
#endif
    {%- endfor %}

OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_keypair(uint8_t *public_key, uint8_t *secret_key) {
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}
    {%- if loop.first %}
#if defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
    {%- else %}
#elif defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
    {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
    {%- endif %}
           {%- if impl['signature_keypair'] %}
		return (OQS_STATUS) {{ impl['signature_keypair'] }}(public_key, secret_key);
           {%- else %}
		return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_sign_keypair(public_key, secret_key);
           {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) {{ scheme['metadata']['default_keypair_signature'] }}(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
    {%- endif %}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) {{ scheme['metadata']['default_keypair_signature'] }}(public_key, secret_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
}

OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}
    {%- if loop.first %}
#if defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
    {%- else %}
#elif defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
    {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
    {%- endif %}
           {%- if impl['signature_signature'] %}
		return (OQS_STATUS) {{ impl['signature_signature'] }}(signature, signature_len, message, message_len, secret_key);
           {%- else %}
		return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
           {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) {{ scheme['metadata']['default_signature_signature'] }}(signature, signature_len, message, message_len, secret_key);
	}
#endif /* OQS_DIST_BUILD */
    {%- endif %}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) {{ scheme['metadata']['default_signature_signature'] }}(signature, signature_len, message, message_len, secret_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
}

OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}
    {%- if loop.first %}
#if defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
    {%- else %}
#elif defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
    {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
    {%- endif %}
           {%- if impl['signature_verify'] %}
		return (OQS_STATUS) {{ impl['signature_verify'] }}(signature, signature_len, message, message_len, public_key);
           {%- else %}
		return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
           {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) {{ scheme['metadata']['default_verify_signature'] }}(signature, signature_len, message, message_len, public_key);
	}
#endif /* OQS_DIST_BUILD */
    {%- endif %}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) {{ scheme['metadata']['default_verify_signature'] }}(signature, signature_len, message, message_len, public_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
}

#endif
{% endfor -%}
