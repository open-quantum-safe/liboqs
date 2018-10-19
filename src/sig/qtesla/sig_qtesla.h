#ifndef __OQS_SIG_QTESLA_H
#define __OQS_SIG_QTESLA_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_qTESLA_I

#define OQS_SIG_qTESLA_I_length_public_key 1504
#define OQS_SIG_qTESLA_I_length_secret_key 2112
#define OQS_SIG_qTESLA_I_length_signature 1376

OQS_SIG *OQS_SIG_qTESLA_I_new();

OQS_API OQS_STATUS OQS_SIG_qTESLA_I_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTESLA_I_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTESLA_I_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_III_size

#define OQS_SIG_qTESLA_III_size_length_public_key 2976
#define OQS_SIG_qTESLA_III_size_length_secret_key 4160
#define OQS_SIG_qTESLA_III_size_length_signature 2720

OQS_SIG *OQS_SIG_qTESLA_III_size_new();

OQS_API OQS_STATUS OQS_SIG_qTESLA_III_size_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTESLA_III_size_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTESLA_III_size_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_III_speed

#define OQS_SIG_qTESLA_III_speed_length_public_key 3104
#define OQS_SIG_qTESLA_III_speed_length_secret_key 4160
#define OQS_SIG_qTESLA_III_speed_length_signature 2848

OQS_SIG *OQS_SIG_qTESLA_III_speed_new();

OQS_API OQS_STATUS OQS_SIG_qTESLA_III_speed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTESLA_III_speed_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTESLA_III_speed_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#endif
