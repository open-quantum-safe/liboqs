// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_RAINBOW_H
#define OQS_SIG_RAINBOW_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_rainbow_Ia_classic
#define OQS_SIG_rainbow_Ia_classic_length_public_key 148992
#define OQS_SIG_rainbow_Ia_classic_length_secret_key 92960
#define OQS_SIG_rainbow_Ia_classic_length_signature 64

OQS_SIG *OQS_SIG_rainbow_Ia_classic_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_classic_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_classic_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_classic_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_Ia_cyclic
#define OQS_SIG_rainbow_Ia_cyclic_length_public_key 58144
#define OQS_SIG_rainbow_Ia_cyclic_length_secret_key 92960
#define OQS_SIG_rainbow_Ia_cyclic_length_signature 64

OQS_SIG *OQS_SIG_rainbow_Ia_cyclic_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_cyclic_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_cyclic_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_cyclic_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_Ia_cyclic_compressed
#define OQS_SIG_rainbow_Ia_cyclic_compressed_length_public_key 58144
#define OQS_SIG_rainbow_Ia_cyclic_compressed_length_secret_key 64
#define OQS_SIG_rainbow_Ia_cyclic_compressed_length_signature 64

OQS_SIG *OQS_SIG_rainbow_Ia_cyclic_compressed_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_cyclic_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_cyclic_compressed_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_cyclic_compressed_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_IIIc_classic
#define OQS_SIG_rainbow_IIIc_classic_length_public_key 710640
#define OQS_SIG_rainbow_IIIc_classic_length_secret_key 511448
#define OQS_SIG_rainbow_IIIc_classic_length_signature 156

OQS_SIG *OQS_SIG_rainbow_IIIc_classic_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_IIIc_classic_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_IIIc_classic_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_IIIc_classic_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_IIIc_cyclic
#define OQS_SIG_rainbow_IIIc_cyclic_length_public_key 206744
#define OQS_SIG_rainbow_IIIc_cyclic_length_secret_key 511448
#define OQS_SIG_rainbow_IIIc_cyclic_length_signature 156

OQS_SIG *OQS_SIG_rainbow_IIIc_cyclic_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_IIIc_cyclic_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_IIIc_cyclic_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_IIIc_cyclic_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_IIIc_cyclic_compressed
#define OQS_SIG_rainbow_IIIc_cyclic_compressed_length_public_key 206744
#define OQS_SIG_rainbow_IIIc_cyclic_compressed_length_secret_key 64
#define OQS_SIG_rainbow_IIIc_cyclic_compressed_length_signature 156

OQS_SIG *OQS_SIG_rainbow_IIIc_cyclic_compressed_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_IIIc_cyclic_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_IIIc_cyclic_compressed_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_IIIc_cyclic_compressed_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_Vc_classic
#define OQS_SIG_rainbow_Vc_classic_length_public_key 1705536
#define OQS_SIG_rainbow_Vc_classic_length_secret_key 1227104
#define OQS_SIG_rainbow_Vc_classic_length_signature 204

OQS_SIG *OQS_SIG_rainbow_Vc_classic_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_Vc_classic_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Vc_classic_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Vc_classic_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_Vc_cyclic
#define OQS_SIG_rainbow_Vc_cyclic_length_public_key 491936
#define OQS_SIG_rainbow_Vc_cyclic_length_secret_key 1227104
#define OQS_SIG_rainbow_Vc_cyclic_length_signature 204

OQS_SIG *OQS_SIG_rainbow_Vc_cyclic_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_Vc_cyclic_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Vc_cyclic_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Vc_cyclic_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_rainbow_Vc_cyclic_compressed
#define OQS_SIG_rainbow_Vc_cyclic_compressed_length_public_key 491936
#define OQS_SIG_rainbow_Vc_cyclic_compressed_length_secret_key 64
#define OQS_SIG_rainbow_Vc_cyclic_compressed_length_signature 204

OQS_SIG *OQS_SIG_rainbow_Vc_cyclic_compressed_new(void);
OQS_API OQS_STATUS OQS_SIG_rainbow_Vc_cyclic_compressed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Vc_cyclic_compressed_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Vc_cyclic_compressed_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif
