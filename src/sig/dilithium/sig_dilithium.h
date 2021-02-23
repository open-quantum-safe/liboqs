// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_DILITHIUM_H
#define OQS_SIG_DILITHIUM_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_dilithium_2
#define OQS_SIG_dilithium_2_length_public_key 1312
#define OQS_SIG_dilithium_2_length_secret_key 2528
#define OQS_SIG_dilithium_2_length_signature 2420

OQS_SIG *OQS_SIG_dilithium_2_new(void);
OQS_API OQS_STATUS OQS_SIG_dilithium_2_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_dilithium_3
#define OQS_SIG_dilithium_3_length_public_key 1952
#define OQS_SIG_dilithium_3_length_secret_key 4000
#define OQS_SIG_dilithium_3_length_signature 3293

OQS_SIG *OQS_SIG_dilithium_3_new(void);
OQS_API OQS_STATUS OQS_SIG_dilithium_3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_dilithium_5
#define OQS_SIG_dilithium_5_length_public_key 2592
#define OQS_SIG_dilithium_5_length_secret_key 4864
#define OQS_SIG_dilithium_5_length_signature 4595

OQS_SIG *OQS_SIG_dilithium_5_new(void);
OQS_API OQS_STATUS OQS_SIG_dilithium_5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_dilithium_2_aes
#define OQS_SIG_dilithium_2_aes_length_public_key 1312
#define OQS_SIG_dilithium_2_aes_length_secret_key 2528
#define OQS_SIG_dilithium_2_aes_length_signature 2420

OQS_SIG *OQS_SIG_dilithium_2_aes_new(void);
OQS_API OQS_STATUS OQS_SIG_dilithium_2_aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_2_aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_2_aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_dilithium_3_aes
#define OQS_SIG_dilithium_3_aes_length_public_key 1952
#define OQS_SIG_dilithium_3_aes_length_secret_key 4000
#define OQS_SIG_dilithium_3_aes_length_signature 3293

OQS_SIG *OQS_SIG_dilithium_3_aes_new(void);
OQS_API OQS_STATUS OQS_SIG_dilithium_3_aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_3_aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_3_aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_dilithium_5_aes
#define OQS_SIG_dilithium_5_aes_length_public_key 2592
#define OQS_SIG_dilithium_5_aes_length_secret_key 4864
#define OQS_SIG_dilithium_5_aes_length_signature 4595

OQS_SIG *OQS_SIG_dilithium_5_aes_new(void);
OQS_API OQS_STATUS OQS_SIG_dilithium_5_aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_5_aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_dilithium_5_aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif
