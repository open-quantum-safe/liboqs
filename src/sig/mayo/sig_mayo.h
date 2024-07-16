// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_MAYO_H
#define OQS_SIG_MAYO_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_mayo_1)
#define OQS_SIG_mayo_1_length_public_key 1168
#define OQS_SIG_mayo_1_length_secret_key 24
#define OQS_SIG_mayo_1_length_signature 321

OQS_SIG *OQS_SIG_mayo_1_new(void);
OQS_API OQS_STATUS OQS_SIG_mayo_1_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mayo_1_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mayo_1_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mayo_2)
#define OQS_SIG_mayo_2_length_public_key 5488
#define OQS_SIG_mayo_2_length_secret_key 24
#define OQS_SIG_mayo_2_length_signature 180

OQS_SIG *OQS_SIG_mayo_2_new(void);
OQS_API OQS_STATUS OQS_SIG_mayo_2_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mayo_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mayo_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mayo_3)
#define OQS_SIG_mayo_3_length_public_key 2656
#define OQS_SIG_mayo_3_length_secret_key 32
#define OQS_SIG_mayo_3_length_signature 577

OQS_SIG *OQS_SIG_mayo_3_new(void);
OQS_API OQS_STATUS OQS_SIG_mayo_3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mayo_3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mayo_3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mayo_5)
#define OQS_SIG_mayo_5_length_public_key 5008
#define OQS_SIG_mayo_5_length_secret_key 40
#define OQS_SIG_mayo_5_length_signature 838

OQS_SIG *OQS_SIG_mayo_5_new(void);
OQS_API OQS_STATUS OQS_SIG_mayo_5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mayo_5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mayo_5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif
