#ifndef __OQS_SIG_QTESLA_H
#define __OQS_SIG_QTESLA_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_qTESLA_I

#define OQS_SIG_qTESLA_I_length_public_key 1504
#define OQS_SIG_qTESLA_I_length_secret_key 2112
#define OQS_SIG_qTESLA_I_length_sig_overhead 1376

OQS_SIG *OQS_SIG_qTESLA_I_new();

OQS_STATUS OQS_SIG_qTESLA_I_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_I_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_I_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_III_size

#define OQS_SIG_qTESLA_III_size_length_public_key 2976
#define OQS_SIG_qTESLA_III_size_length_secret_key 4160
#define OQS_SIG_qTESLA_III_size_length_sig_overhead 2720

OQS_SIG *OQS_SIG_qTESLA_III_size_new();

OQS_STATUS OQS_SIG_qTESLA_III_size_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_III_size_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_III_size_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_III_speed

#define OQS_SIG_qTESLA_III_speed_length_public_key 3104
#define OQS_SIG_qTESLA_III_speed_length_secret_key 4160
#define OQS_SIG_qTESLA_III_speed_length_sig_overhead 2848

OQS_SIG *OQS_SIG_qTESLA_III_speed_new();

OQS_STATUS OQS_SIG_qTESLA_III_speed_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_III_speed_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_III_speed_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_p_I

#define OQS_SIG_qTESLA_p_I_length_public_key 14880
#define OQS_SIG_qTESLA_p_I_length_secret_key 5184
#define OQS_SIG_qTESLA_p_I_length_sig_overhead 2848

OQS_SIG *OQS_SIG_qTESLA_p_I_new();

OQS_STATUS OQS_SIG_qTESLA_p_I_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_p_I_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_p_I_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_qTESLA_p_III

#define OQS_SIG_qTESLA_p_III_length_public_key 39712
#define OQS_SIG_qTESLA_p_III_length_secret_key 12352
#define OQS_SIG_qTESLA_p_III_length_sig_overhead 6176

OQS_SIG *OQS_SIG_qTESLA_p_III_new();

OQS_STATUS OQS_SIG_qTESLA_p_III_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_p_III_sign(uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_STATUS OQS_SIG_qTESLA_p_III_sign_open(uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key);

#endif

#endif
