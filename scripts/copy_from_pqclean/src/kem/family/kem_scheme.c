#include <stdlib.h>

#include <oqs/kem_{{ family }}.h>

{% for scheme in schemes -%}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }})

OQS_KEM *OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
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

	kem->keypair = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair;
	kem->encaps = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps;
	kem->decaps = OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_decaps;

	return kem;
}

int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['default_implementation']|upper }}_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['default_implementation']|upper }}_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['default_implementation']|upper }}_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}

#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);
#endif
    {%- endfor %}

OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair(uint8_t *public_key, uint8_t *secret_key) {
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if ({%- for flag in impl['required_flags'] -%}available_cpu_extensions.{{ flag|upper }}_ENABLED{%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
		return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_keypair(public_key, secret_key);
	} else {
		return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['default_implementation']|upper }}_crypto_kem_keypair(public_key, secret_key);
	}
#else
    {%- endfor %}
	return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['default_implementation']|upper }}_crypto_kem_keypair(public_key, secret_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
}

OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if ({%- for flag in impl['required_flags'] -%}available_cpu_extensions.{{ flag|upper }}_ENABLED{%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
		return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_enc(ciphertext, shared_secret, public_key);
	} else {
		return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['default_implementation']|upper }}_crypto_kem_enc(ciphertext, shared_secret, public_key);
	}
#else
    {%- endfor %}
	return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['default_implementation']|upper }}_crypto_kem_enc(ciphertext, shared_secret, public_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
}

OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
    {%- for impl in scheme['metadata']['implementations'] if impl['name'] != scheme['default_implementation'] %}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}_{{ impl['name'] }})
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if ({%- for flag in impl['required_flags'] -%}available_cpu_extensions.{{ flag|upper }}_ENABLED{%- if not loop.last %} && {% endif -%}{%- endfor -%}) {
		return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ impl['name']|upper }}_crypto_kem_dec(shared_secret, ciphertext, secret_key);
	} else {
		return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['default_implementation']|upper }}_crypto_kem_dec(shared_secret, ciphertext, secret_key);
	}
#else
    {%- endfor %}
	return (OQS_STATUS) PQCLEAN_{{ scheme['pqclean_scheme_c']|upper }}_{{ scheme['default_implementation']|upper }}_crypto_kem_dec(shared_secret, ciphertext, secret_key);
    {%- if scheme['metadata']['implementations']|rejectattr('name', 'equalto', scheme['default_implementation'])|list %}
#endif
    {%- endif %}
}

#endif
{% endfor -%}
