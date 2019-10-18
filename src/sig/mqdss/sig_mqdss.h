#ifndef OQS_SIG_MQDSS_H
#define OQS_SIG_MQDSS_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_mqdss_31_48
#define OQS_SIG_mqdss_31_48_length_public_key 46
#define OQS_SIG_mqdss_31_48_length_secret_key 16
#define OQS_SIG_mqdss_31_48_length_signature 20854

OQS_SIG *OQS_SIG_mqdss_31_48_new(void);
OQS_API OQS_STATUS OQS_SIG_mqdss_31_48_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqdss_31_48_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqdss_31_48_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_mqdss_31_64
#define OQS_SIG_mqdss_31_64_length_public_key 64
#define OQS_SIG_mqdss_31_64_length_secret_key 24
#define OQS_SIG_mqdss_31_64_length_signature 43728

OQS_SIG *OQS_SIG_mqdss_31_64_new(void);
OQS_API OQS_STATUS OQS_SIG_mqdss_31_64_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqdss_31_64_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqdss_31_64_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif // OQS_SIG_MQDSS_H
