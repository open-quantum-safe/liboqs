// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_{{ family|upper }}_H
#define OQS_KEM_{{ family|upper }}_H

#include <oqs/oqs.h>

{% for scheme in schemes -%}
#if defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_KEM_{{ family }}_{{ scheme['alias_scheme'] }}){%- endif %}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_public_key {{ scheme['metadata']['length-public-key'] }}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_secret_key {{ scheme['metadata']['length-secret-key'] }}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_ciphertext {{ scheme['metadata']['length-ciphertext'] }}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_shared_secret {{ scheme['metadata']['length-shared-secret'] }}
{%- if scheme['metadata']['length-keypair-coins'] is defined %}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_keypair_coins {{ scheme['metadata']['length-keypair-coins'] }}
{%- else %}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_keypair_coins 0
{%- endif %}
{%- if scheme['metadata']['length-encaps-coins'] is defined %}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_encaps_coins {{ scheme['metadata']['length-encaps-coins'] }}
{%- else %}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_encaps_coins 0
{%- endif %}
OQS_KEM *OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_new(void);
OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *coins);
OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps_derand(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *coins);
OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
{% if 'alias_scheme' in scheme %}
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_public_key OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_public_key
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_secret_key OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_secret_key
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_ciphertext OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_ciphertext
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_shared_secret OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_shared_secret
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_keypair_coins OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_keypair_coins
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_length_encaps_coins OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_encaps_coins
OQS_KEM *OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_new(void);
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_keypair_derand OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair_derand
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_keypair OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_encaps_derand OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps_derand
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_encaps OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps
#define OQS_KEM_{{ family }}_{{ scheme['alias_scheme'] }}_decaps OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_decaps
{% endif -%}
#endif

{% endfor -%}

#endif


