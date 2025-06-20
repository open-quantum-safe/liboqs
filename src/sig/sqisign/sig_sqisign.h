// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_SQISIGN_H
#define OQS_SIG_SQISIGN_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_sqisign_lvl1)
#define OQS_SIG_sqisign_lvl1_length_public_key 65
#define OQS_SIG_sqisign_lvl1_length_secret_key 353
#define OQS_SIG_sqisign_lvl1_length_signature 148

OQS_SIG *OQS_SIG_sqisign_lvl1_new(void);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl1_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl1_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl1_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl1_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl1_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sqisign_lvl3)
#define OQS_SIG_sqisign_lvl3_length_public_key 97
#define OQS_SIG_sqisign_lvl3_length_secret_key 529
#define OQS_SIG_sqisign_lvl3_length_signature 224

OQS_SIG *OQS_SIG_sqisign_lvl3_new(void);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sqisign_lvl5)
#define OQS_SIG_sqisign_lvl5_length_public_key 129
#define OQS_SIG_sqisign_lvl5_length_secret_key 701
#define OQS_SIG_sqisign_lvl5_length_signature 292

OQS_SIG *OQS_SIG_sqisign_lvl5_new(void);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl5_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_lvl5_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
