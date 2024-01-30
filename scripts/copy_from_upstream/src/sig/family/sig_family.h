// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_{{ family|upper }}_H
#define OQS_SIG_{{ family|upper }}_H

#include <oqs/oqs.h>

{% for scheme in schemes -%}
#if defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}) {%- if 'alias_scheme' in scheme %} || defined(OQS_ENABLE_SIG_{{ family }}_{{ scheme['alias_scheme'] }}){%- endif %}
#define OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_public_key {{ scheme['metadata']['length-public-key'] }}
#define OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_secret_key {{ scheme['metadata']['length-secret-key'] }}
#define OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_signature {{ scheme['metadata']['length-signature'] }}

OQS_SIG *OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_new(void);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
{% if 'alias_scheme' in scheme %}
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_length_public_key OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_public_key
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_length_secret_key OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_secret_key
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_length_ciphertext OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_ciphertext
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_length_shared_secret OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_shared_secret
OQS_SIG *OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_new(void);
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_keypair OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_keypair
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_encaps OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_encaps
#define OQS_SIG_{{ family }}_{{ scheme['alias_scheme'] }}_decaps OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_decaps
{% endif -%}
#endif

{% endfor -%}

#endif

