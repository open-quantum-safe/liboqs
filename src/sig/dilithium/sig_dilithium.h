// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_DILITHIUM_H
#define OQS_SIG_DILITHIUM_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_dilithium_2
#define OQS_SIG_dilithium_2_length_public_key 1184
#define OQS_SIG_dilithium_2_length_secret_key 2800
#define OQS_SIG_dilithium_2_length_signature 2044

OQS_SIG *OQS_SIG_dilithium_2_new(void);
OQS_API OQS_STATUS OQS_SIG_dilithium_2_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_dilithium_3
#define OQS_SIG_dilithium_3_length_public_key 1472
#define OQS_SIG_dilithium_3_length_secret_key 3504
#define OQS_SIG_dilithium_3_length_signature 2701

OQS_SIG *OQS_SIG_dilithium_3_new(void);
OQS_API OQS_STATUS OQS_SIG_dilithium_3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_dilithium_4
#define OQS_SIG_dilithium_4_length_public_key 1760
#define OQS_SIG_dilithium_4_length_secret_key 3856
#define OQS_SIG_dilithium_4_length_signature 3366

OQS_SIG *OQS_SIG_dilithium_4_new(void);
OQS_API OQS_STATUS OQS_SIG_dilithium_4_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_4_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_4_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif
