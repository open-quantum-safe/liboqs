// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_ML_DSA_H
#define OQS_SIG_ML_DSA_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_ml_dsa_44_ipd
#define OQS_SIG_ml_dsa_44_ipd_length_public_key 1312
#define OQS_SIG_ml_dsa_44_ipd_length_secret_key 2560
#define OQS_SIG_ml_dsa_44_ipd_length_signature 2420

OQS_SIG *OQS_SIG_ml_dsa_44_ipd_new(void);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_ipd_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_ipd_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_ipd_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_ml_dsa_65_ipd
#define OQS_SIG_ml_dsa_65_ipd_length_public_key 1952
#define OQS_SIG_ml_dsa_65_ipd_length_secret_key 4032
#define OQS_SIG_ml_dsa_65_ipd_length_signature 3309

OQS_SIG *OQS_SIG_ml_dsa_65_ipd_new(void);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_ipd_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_ipd_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_ipd_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_ml_dsa_87_ipd
#define OQS_SIG_ml_dsa_87_ipd_length_public_key 2592
#define OQS_SIG_ml_dsa_87_ipd_length_secret_key 4896
#define OQS_SIG_ml_dsa_87_ipd_length_signature 4627

OQS_SIG *OQS_SIG_ml_dsa_87_ipd_new(void);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_ipd_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_ipd_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_ipd_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif
