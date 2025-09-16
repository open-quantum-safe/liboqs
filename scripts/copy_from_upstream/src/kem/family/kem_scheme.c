// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_{{ family }}.h>

{% for scheme in schemes -%}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}){%- endif %}
{% if 'alias_scheme' in scheme %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }})
{% endif %}
OQS_KEM *OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_new(void) {

	OQS_KEM *kem = OQS_MEM_malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_{{ family }}_{{ scheme['scheme'] }};
	kem->alg_version = "{{ scheme['metadata']['implementations'][0]['version'] }}";

	kem->claimed_nist_level = {{ scheme['metadata']['claimed-nist-level'] }};
	kem->ind_cca = {{ scheme['metadata']['ind_cca'] }};

	kem->length_public_key = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_public_key;
	kem->length_secret_key = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_secret_key;
	kem->length_ciphertext = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_shared_secret;
	kem->length_keypair_seed = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_keypair_seed;
	kem->length_encaps_seed = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_encaps_seed;

	kem->keypair = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair;
	kem->keypair_derand = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair_derand;
	kem->encaps = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps;
	kem->encaps_derand = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps_derand;
	kem->decaps = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_decaps;

	return kem;
}
{%- if 'alias_scheme' in scheme %}
#endif
{%- endif -%}

{%- if 'alias_scheme' in scheme %}

#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }})
/** Alias */
OQS_KEM *OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_new(void) {

	OQS_KEM *kem = OQS_MEM_malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_{{ family }}_{{ scheme['alias_scheme'] }};
	kem->alg_version = "{{ scheme['metadata']['implementations'][0]['version'] }}";

	kem->claimed_nist_level = {{ scheme['metadata']['claimed-nist-level'] }};
	kem->ind_cca = {{ scheme['metadata']['ind_cca'] }};

	kem->length_public_key = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_public_key;
	kem->length_secret_key = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_secret_key;
	kem->length_ciphertext = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_shared_secret;
	kem->length_keypair_seed = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_keypair_seed;
	kem->length_encaps_seed = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_encaps_seed;

	kem->keypair = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_keypair;
	kem->keypair_derand = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_keypair_derand;
	kem->encaps = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_encaps;
    kem->encaps_derand = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_encaps_derand;
	kem->decaps = OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_decaps;

	return kem;
}
#endif
{%- endif -%}

    {%- for impl in scheme['metadata']['implementations'] if impl['name'] == scheme['default_implementation'] %}

        {%- if impl['signature_keypair'] %}
           {%- set cleankeypair = scheme['metadata'].update({'default_keypair_signature': impl['signature_keypair']}) -%}
        {%- else %}
           {%- set cleankeypair = scheme['metadata'].update({'default_keypair_signature': "PQCLEAN_"+scheme['pqclean_scheme_c']|upper+"_"+scheme['default_implementation']|upper+"_crypto_kem_keypair"}) -%}
        {%- endif %}

extern int {{ scheme['metadata']['default_keypair_signature'] }}(uint8_t *pk, uint8_t *sk);

        {%- if impl['signature_keypair_derand'] %}
           {%- set cleankeypairderand = scheme['metadata'].update({'default_keypair_derand_signature': impl['signature_keypair_derand']}) %}
extern int {{ scheme['metadata']['default_keypair_derand_signature'] }}(uint8_t *pk, uint8_t *sk, const uint8_t *seed);
        {%- endif %}

        {%- if impl['signature_enc'] %}
           {%- set cleanenc = scheme['metadata'].update({'default_enc_signature': impl['signature_enc']}) -%}
        {%- else %}
           {%- set cleanenc = scheme['metadata'].update({'default_enc_signature': "PQCLEAN_"+scheme['pqclean_scheme_c']|upper+"_"+scheme['default_implementation']|upper+"_crypto_kem_enc"}) -%}
        {%- endif %}
extern int {{ scheme['metadata']['default_enc_signature'] }}(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

        {%- if impl['signature_enc_derand'] %}
           {%- set cleanencderand = scheme['metadata'].update({'default_enc_derand_signature': impl['signature_enc_derand']}) %}
extern int {{ scheme['metadata']['default_enc_derand_signature'] }}(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *seed);
        {%- endif %}

        {%- if impl['signature_dec'] %}
           {%- set cleandec = scheme['metadata'].update({'default_dec_signature': impl['signature_dec']}) -%}
        {%- else %}
           {%- set cleandec = scheme['metadata'].update({'default_dec_signature': "PQCLEAN_"+scheme['pqclean_scheme_c']|upper+"_"+scheme['default_implementation']|upper+"_crypto_kem_dec"}) -%}
        {%- endif %}
extern int {{ scheme['metadata']['default_dec_signature']  }}(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

    {%- endfor %}

    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}
{% if impl['name'] == 'cuda'%}
#if defined(OQS_USE_CUPQC)
        {%- endif %}
{%- if impl['name'] == 'icicle_cuda'%}
#if defined(OQS_USE_ICICLE)
        {%- endif %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
        {%- if impl['signature_keypair'] %}
extern int {{ impl['signature_keypair'] }}(uint8_t *pk, uint8_t *sk);
        {%- else %}
extern int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
        {%- endif %}

        {%- if impl['signature_keypair_derand'] %}
extern int {{ impl['signature_keypair_derand'] }}(uint8_t *pk, uint8_t *sk, const uint8_t *seed);
        {%- endif %}

        {%- if impl['signature_enc'] %}
extern int {{ impl['signature_enc'] }}(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
        {%- else %}
extern int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
        {%- endif %}

        {%- if impl['signature_enc_derand'] %}
extern int {{ impl['signature_enc_derand'] }}(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *seed);
        {%- endif %}

        {%- if impl['signature_dec'] %}
extern int {{ impl['signature_dec'] }}(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
        {%- else %}
extern int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
        {%- endif %}
#endif
        {%- if impl['name'] == 'cuda'%}
#endif /* OQS_USE_CUPQC */
        {%- endif %}
        {%- if impl['name'] == 'icicle_cuda'%}
#endif /* OQS_USE_ICICLE */
        {%- endif %}
    {%- endfor %}

{%- if libjade_implementation is defined and scheme['libjade_implementation'] %}
{% for scheme in schemes -%}

{%- for impl in scheme['metadata']['implementations'] if impl['name'] in scheme['libjade_implementations'] %}
#if defined(OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['scheme'] }}{%- if impl['name'] != scheme['default_implementation'] %}_{{ impl['name'] }}{%- endif %}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
extern int libjade_{{ scheme['pqclean_scheme_c'] }}_{{ impl['name'] }}_keypair(uint8_t *pk, uint8_t *sk);
extern int libjade_{{ scheme['pqclean_scheme_c'] }}_{{ impl['name'] }}_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int libjade_{{ scheme['pqclean_scheme_c'] }}_{{ impl['name'] }}_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#endif
{% endfor -%}
{% endfor -%}
{% endif %}

OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed) {
{%- if scheme['derandomized_keypair'] %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}
    {%- if loop.first %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- else %}
#elif defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
        {%- if impl['signature_keypair_derand'] %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) {{ impl['signature_keypair_derand'] }}(public_key, secret_key, seed);
        {%- else %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_keypair_derand(public_key, secret_key, seed);
        {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) {{ scheme['metadata']['default_keypair_derand_signature'] }}(public_key, secret_key, seed);
	}
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) {{ scheme['metadata']['default_keypair_derand_signature'] }}(public_key, secret_key, seed);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
    {%- else %}
	(void)public_key;
	(void)secret_key;
	(void)seed;
	return OQS_ERROR;
    {%- endif %}
}

OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair(uint8_t *public_key, uint8_t *secret_key) {
{%- if libjade_implementation is defined and scheme['libjade_implementation'] %}
#if defined(OQS_LIBJADE_BUILD) && (defined(OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['scheme'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}){%- endif %})
{%- for scheme in schemes %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] and impl['name'] in scheme['libjade_implementations'] %}
        {%- if loop.first %}
#if defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
        {%- else %}
#elif defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
        {%- endif %}
        {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
        {%- endif -%}
        {%- if impl['name'] in scheme['libjade_implementations'] %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) libjade_{{ scheme['pqclean_scheme_c'] }}_{{ impl['name'] }}_keypair(public_key, secret_key);
        {%- else %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_keypair(public_key, secret_key);
        {%- endif %}
        {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) libjade_{{ scheme['pqclean_scheme_c'] }}_ref_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
        {%- endif -%}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) libjade_{{ scheme['pqclean_scheme_c'] }}_ref_keypair(public_key, secret_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
{% endfor -%}
#else /*OQS_LIBJADE_BUILD && (OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['scheme'] }} {%- if 'alias_scheme' in scheme %} || OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}{%- endif %})*/
{%- endif %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] == 'cuda' %}
#if defined(OQS_USE_CUPQC) && defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
	return (OQS_STATUS) {{ impl['signature_keypair'] }}(public_key, secret_key);
#endif /* OQS_USE_CUPQC && OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }} */
    {%- endfor %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] == 'icicle_cuda' %}
#if defined(OQS_USE_ICICLE) && defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
	return (OQS_STATUS) {{ impl['signature_keypair'] }}(public_key, secret_key);
#endif /* OQS_USE_ICICLE && OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }} */
    {%- endfor %}
    {%- for impl in scheme['metadata']['implementations'] if (impl['name'] != scheme['default_implementation'] and impl['name'] != 'cuda' and impl['name'] != 'icicle_cuda') %}
    {%- if loop.first %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- else %}
#elif defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
           {%- if impl['signature_keypair'] %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) {{ impl['signature_keypair'] }}(public_key, secret_key);
           {%- else %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_keypair(public_key, secret_key);
           {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) {{ scheme['metadata']['default_keypair_signature'] }}(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) {{ scheme['metadata']['default_keypair_signature'] }}(public_key, secret_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
{%- if libjade_implementation is defined and scheme['libjade_implementation'] %}
#endif /* OQS_LIBJADE_BUILD */
{%- endif %}
}

OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps_derand(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *seed) {
{%- if scheme['derandomized_encaps'] %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}
    {%- if loop.first %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- else %}
#elif defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
        {%- if impl['signature_enc_derand'] %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) {{ impl['signature_enc_derand'] }}(ciphertext, shared_secret, public_key, seed);
        {%- else %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_enc_derand(ciphertext, shared_secret, public_key, seed);
        {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) {{ scheme['metadata']['default_enc_derand_signature'] }}(ciphertext, shared_secret, public_key, seed);
	}
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) {{ scheme['metadata']['default_enc_derand_signature'] }}(ciphertext, shared_secret, public_key, seed);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
    {%- else %}
	(void)ciphertext;
	(void)shared_secret;
	(void)public_key;
	(void)seed;
	return OQS_ERROR;
    {%- endif %}
}

OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
{%- if libjade_implementation is defined and scheme['libjade_implementation'] %}
#if defined(OQS_LIBJADE_BUILD) && (defined(OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['scheme'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}){%- endif %})
{%- for scheme in schemes %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] and impl['name'] in scheme['libjade_implementations'] %}
        {%- if loop.first %}
#if defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
        {%- else %}
#elif defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
        {%- endif %}
        {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
        {%- endif -%}
        {%- if impl['name'] in scheme['libjade_implementations'] %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) libjade_{{ scheme['pqclean_scheme_c'] }}_{{ impl['name'] }}_enc(ciphertext, shared_secret, public_key);
        {%- else %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_enc(ciphertext, shared_secret, public_key);
        {%- endif %}
        {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) libjade_{{ scheme['pqclean_scheme_c'] }}_ref_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
        {%- endif -%}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) libjade_{{ scheme['pqclean_scheme_c'] }}_ref_enc(ciphertext, shared_secret, public_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif 
    {%- endif %}
{% endfor -%}
#else /*OQS_LIBJADE_BUILD && (OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['scheme'] }} {%- if 'alias_scheme' in scheme %} || OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}{%- endif %})*/
{%- endif %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] == 'cuda' %}
#if defined(OQS_USE_CUPQC) && defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
	return (OQS_STATUS) {{ impl['signature_enc'] }}(ciphertext, shared_secret, public_key);
#endif /* OQS_USE_CUPQC && OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }} */
    {%- endfor %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] == 'icicle_cuda' %}
#if defined(OQS_USE_ICICLE) && defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
	return (OQS_STATUS) {{ impl['signature_enc'] }}(ciphertext, shared_secret, public_key);
#endif /* OQS_USE_ICICLE && OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }} */
    {%- endfor %}
    {%- for impl in scheme['metadata']['implementations'] if (impl['name'] != scheme['default_implementation'] and impl['name'] != 'cuda' and impl['name'] != 'icicle_cuda') %}
    {%- if loop.first %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- else %}
#elif defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
           {%- if impl['signature_enc'] %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) {{ impl['signature_enc'] }}(ciphertext, shared_secret, public_key);
           {%- else %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_enc(ciphertext, shared_secret, public_key);
           {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) {{ scheme['metadata']['default_enc_signature'] }}(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) {{ scheme['metadata']['default_enc_signature'] }}(ciphertext, shared_secret, public_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
{%- if libjade_implementation is defined and scheme['libjade_implementation'] %}
#endif /* OQS_LIBJADE_BUILD */
{%- endif %}
}

OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
{%- if libjade_implementation is defined and scheme['libjade_implementation'] %}
#if defined(OQS_LIBJADE_BUILD) && (defined(OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['scheme'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}){%- endif %})
{%- for scheme in schemes %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] and impl['name'] in scheme['libjade_implementations'] %}
        {%- if loop.first %}
#if defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
        {%- else %}
#elif defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_{%- if impl['name'] in scheme['libjade_implementations'] %}LIBJADE_{%- endif %}KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
        {%- endif %}
        {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
        {%- endif -%}
        {%- if impl['name'] in scheme['libjade_implementations'] %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) libjade_{{ scheme['pqclean_scheme_c'] }}_{{ impl['name'] }}_dec(shared_secret, ciphertext, secret_key);
        {%- else %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_dec(shared_secret, ciphertext, secret_key);
        {%- endif %}
        {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) libjade_{{ scheme['pqclean_scheme_c'] }}_ref_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
        {%- endif -%}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) libjade_{{ scheme['pqclean_scheme_c'] }}_ref_dec(shared_secret, ciphertext, secret_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
{% endfor -%}
#else /*OQS_LIBJADE_BUILD && (OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['scheme'] }} {%- if 'alias_scheme' in scheme %} || OQS_ENABLE_LIBJADE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}{%- endif %})*/
{%- endif %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] == 'cuda' %}
#if defined(OQS_USE_CUPQC) && defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
	return (OQS_STATUS) {{ impl['signature_dec'] }}(shared_secret, ciphertext, secret_key);
#endif /* OQS_USE_CUPQC && OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }} */
    {%- endfor %}
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] == 'icicle_cuda' %}
#if defined(OQS_USE_ICICLE) && defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
	return (OQS_STATUS) {{ impl['signature_dec'] }}(shared_secret, ciphertext, secret_key);
#endif /* OQS_USE_ICICLE && OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }} */
    {%- endfor %}
    {%- for impl in scheme['metadata']['implementations'] if (impl['name'] != scheme['default_implementation'] and impl['name'] != 'cuda' and impl['name'] != 'icicle_cuda') %}
    {%- if loop.first %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- else %}
#elif defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_{{ impl['name'] }}){%- endif %}
    {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	if ({%- for flag in impl['required_flags'] -%}OQS_CPU_has_extension(OQS_CPU_EXT_{{ flag|upper }}){%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
           {%- if impl['signature_dec'] %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) {{ impl['signature_dec'] }}(shared_secret, ciphertext, secret_key);
           {%- else %}
	{% if 'required_flags' in impl and impl['required_flags'] %}	{% endif -%}return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_dec(shared_secret, ciphertext, secret_key);
           {%- endif %}
    {%- if 'required_flags' in impl and impl['required_flags'] %}
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) {{ scheme['metadata']['default_dec_signature'] }}(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
    {%- endif -%}
    {%- endfor %}
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#else
    {%- endif %}
	return (OQS_STATUS) {{ scheme['metadata']['default_dec_signature'] }}(shared_secret, ciphertext, secret_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
{%- if libjade_implementation is defined and scheme['libjade_implementation'] %}
#endif /* OQS_LIBJADE_BUILD */
{%- endif %}
}

#endif
{% endfor -%}