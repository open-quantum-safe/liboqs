// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_FALCON_H
#define OQS_SIG_FALCON_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_falcon_512
#define OQS_SIG_falcon_512_length_public_key 897
#define OQS_SIG_falcon_512_length_secret_key 1281
#define OQS_SIG_falcon_512_length_signature 666

OQS_SIG *OQS_SIG_falcon_512_new(void);
OQS_API OQS_STATUS OQS_SIG_falcon_512_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_falcon_512_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_falcon_512_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_falcon_1024
#define OQS_SIG_falcon_1024_length_public_key 1793
#define OQS_SIG_falcon_1024_length_secret_key 2305
#define OQS_SIG_falcon_1024_length_signature 1280

OQS_SIG *OQS_SIG_falcon_1024_new(void);
OQS_API OQS_STATUS OQS_SIG_falcon_1024_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_falcon_1024_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_falcon_1024_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif
