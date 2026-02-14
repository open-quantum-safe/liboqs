// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_ML_DSA_EXTMU_H
#define OQS_SIG_ML_DSA_EXTMU_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_ml_dsa_44_extmu)
#define OQS_SIG_ml_dsa_44_extmu_length_public_key 1312
#define OQS_SIG_ml_dsa_44_extmu_length_secret_key 2560
#define OQS_SIG_ml_dsa_44_extmu_length_signature 2420

OQS_SIG *OQS_SIG_ml_dsa_44_extmu_new(void);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_sign(uint8_t *signature, size_t *signature_len, const uint8_t *mu, size_t mu_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_verify(const uint8_t *mu, size_t mu_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_ml_dsa_65_extmu)
#define OQS_SIG_ml_dsa_65_extmu_length_public_key 1952
#define OQS_SIG_ml_dsa_65_extmu_length_secret_key 4032
#define OQS_SIG_ml_dsa_65_extmu_length_signature 3309

OQS_SIG *OQS_SIG_ml_dsa_65_extmu_new(void);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_extmu_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_extmu_sign(uint8_t *signature, size_t *signature_len, const uint8_t *mu, size_t mu_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_extmu_verify(const uint8_t *mu, size_t mu_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_extmu_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_65_extmu_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_ml_dsa_87_extmu)
#define OQS_SIG_ml_dsa_87_extmu_length_public_key 2592
#define OQS_SIG_ml_dsa_87_extmu_length_secret_key 4896
#define OQS_SIG_ml_dsa_87_extmu_length_signature 4627

OQS_SIG *OQS_SIG_ml_dsa_87_extmu_new(void);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_extmu_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_extmu_sign(uint8_t *signature, size_t *signature_len, const uint8_t *mu, size_t mu_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_extmu_verify(const uint8_t *mu, size_t mu_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_extmu_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_extmu_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
