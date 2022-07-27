// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_RAINBOW_H
#define OQS_SIG_RAINBOW_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_rainbow_III_classic
#define OQS_SIG_rainbow_III_classic_length_public_key 882080
#define OQS_SIG_rainbow_III_classic_length_secret_key 626048
#define OQS_SIG_rainbow_III_classic_length_signature 164

OQS_SIG *OQS_SIG_rainbow_III_classic_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_III_classic_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_III_classic_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_III_classic_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_III_circumzenithal
#define OQS_SIG_rainbow_III_circumzenithal_length_public_key 264608
#define OQS_SIG_rainbow_III_circumzenithal_length_secret_key 626048
#define OQS_SIG_rainbow_III_circumzenithal_length_signature 164

OQS_SIG *OQS_SIG_rainbow_III_circumzenithal_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_III_circumzenithal_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_III_circumzenithal_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_III_circumzenithal_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_III_compressed
#define OQS_SIG_rainbow_III_compressed_length_public_key 264608
#define OQS_SIG_rainbow_III_compressed_length_secret_key 64
#define OQS_SIG_rainbow_III_compressed_length_signature 164

OQS_SIG *OQS_SIG_rainbow_III_compressed_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_III_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_III_compressed_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_III_compressed_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_V_classic
#define OQS_SIG_rainbow_V_classic_length_public_key 1930600
#define OQS_SIG_rainbow_V_classic_length_secret_key 1408736
#define OQS_SIG_rainbow_V_classic_length_signature 212

OQS_SIG *OQS_SIG_rainbow_V_classic_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_V_classic_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_V_classic_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_V_classic_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_V_circumzenithal
#define OQS_SIG_rainbow_V_circumzenithal_length_public_key 536136
#define OQS_SIG_rainbow_V_circumzenithal_length_secret_key 1408736
#define OQS_SIG_rainbow_V_circumzenithal_length_signature 212

OQS_SIG *OQS_SIG_rainbow_V_circumzenithal_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_V_circumzenithal_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_V_circumzenithal_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_V_circumzenithal_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_V_compressed
#define OQS_SIG_rainbow_V_compressed_length_public_key 536136
#define OQS_SIG_rainbow_V_compressed_length_secret_key 64
#define OQS_SIG_rainbow_V_compressed_length_signature 212

OQS_SIG *OQS_SIG_rainbow_V_compressed_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_V_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_V_compressed_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_V_compressed_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif
