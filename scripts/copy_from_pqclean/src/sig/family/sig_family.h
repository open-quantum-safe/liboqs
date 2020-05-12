// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_{{ family|upper }}_H
#define OQS_SIG_{{ family|upper }}_H

#include <oqs/oqs.h>

{% for scheme in schemes -%}
#ifdef OQS_ENABLE_SIG_{{ family }}_{{ scheme['scheme'] }}
#define OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_public_key {{ scheme['metadata']['length-public-key'] }}
#define OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_secret_key {{ scheme['metadata']['length-secret-key'] }}
#define OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_length_signature {{ scheme['metadata']['length-signature'] }}

OQS_SIG *OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_new(void);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_{{ family }}_{{ scheme['scheme'] }}_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

{% endfor -%}

#endif

