#ifndef __OQS_SIG_RAINBOW_H
#define __OQS_SIG_RAINBOW_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_rainbow_Ia_classic
#define OQS_SIG_rainbow_Ia_classic_length_public_key 148992
#define OQS_SIG_rainbow_Ia_classic_length_secret_key 92960
#define OQS_SIG_rainbow_Ia_classic_length_signature 64

OQS_SIG *OQS_SIG_rainbow_Ia_classic_new();
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_classic_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_classic_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_rainbow_Ia_classic_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif