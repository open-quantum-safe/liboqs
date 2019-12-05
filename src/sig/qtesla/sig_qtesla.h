#ifndef OQS_SIG_QTESLA_H
#define OQS_SIG_QTESLA_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_qTesla_p_I

#define OQS_SIG_qTesla_p_I_length_public_key 14880
#define OQS_SIG_qTesla_p_I_length_secret_key 5184
#define OQS_SIG_qTesla_p_I_length_signature 2592

OQS_SIG *OQS_SIG_qTesla_p_I_new(void);

OQS_API OQS_STATUS OQS_SIG_qTesla_p_I_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTesla_p_I_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTesla_p_I_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#ifdef OQS_ENABLE_SIG_qTesla_p_III

#define OQS_SIG_qTesla_p_III_length_public_key 38432
#define OQS_SIG_qTesla_p_III_length_secret_key 12352
#define OQS_SIG_qTesla_p_III_length_signature 5664

OQS_SIG *OQS_SIG_qTesla_p_III_new(void);

OQS_API OQS_STATUS OQS_SIG_qTesla_p_III_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTesla_p_III_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qTesla_p_III_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

#endif

#endif // OQS_SIG_QTESLA_H
