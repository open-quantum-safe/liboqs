// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_{{ family|upper }}_H
#define OQS_KEM_{{ family|upper }}_H

#include <oqs/oqs.h>

{% for scheme in schemes -%}
#ifdef OQS_ENABLE_KEM_{{ family }}_{{ scheme['scheme'] }}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_public_key {{ scheme['metadata']['length-public-key'] }}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_secret_key {{ scheme['metadata']['length-secret-key'] }}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_ciphertext {{ scheme['metadata']['length-ciphertext'] }}
#define OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_length_shared_secret {{ scheme['metadata']['length-shared-secret'] }}
OQS_KEM *OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_new(void);
OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_{{ family }}_{{ scheme['scheme'] }}_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

{% endfor -%}

#endif


