// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_{{ family|upper }}_H
#define OQS_SIG_{{ family|upper }}_H

#include <oqs/oqs.h>

{% for scheme in schemes -%}
{%- set default_impl = scheme['metadata']['implementations'] | selectattr("name", "equalto", scheme['default_implementation']) | first -%}
#if defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['alias_scheme'] }}){%- endif %}
#define OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_public_key {{ scheme['metadata']['length-public-key'] }}
#define OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_secret_key {{ scheme['metadata']['length-secret-key'] }}
#define OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_signature {{ scheme['metadata']['length-signature'] }}

OQS_SIG *OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_new(void);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
{% if 'alias_scheme' in scheme %}
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_length_public_key OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_public_key
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_length_secret_key OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_secret_key
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_length_signature OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_signature
OQS_SIG *OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_new(void);
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_keypair OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_keypair
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_sign OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_sign
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_verify OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_verify
{%- if 'api-with-context-string' in default_impl and default_impl['api-with-context-string'] %}
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_sign_with_ctx_str OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_sign
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_verify_with_ctx_str OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_verify
{%- endif %}
{% endif -%}
#endif

{% endfor -%}

#endif

