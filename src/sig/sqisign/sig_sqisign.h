// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_SQISIGN_H
#define OQS_SIG_SQISIGN_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_sqisign_p324_3)
#define OQS_SIG_sqisign_p324_3_length_public_key 83
#define OQS_SIG_sqisign_p324_3_length_secret_key 270
#define OQS_SIG_sqisign_p324_3_length_signature 200

OQS_SIG *OQS_SIG_sqisign_p324_3_new(void);
OQS_API OQS_STATUS OQS_SIG_sqisign_p324_3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p324_3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p324_3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p324_3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p324_3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sqisign_p500_27)
#define OQS_SIG_sqisign_p500_27_length_public_key 129
#define OQS_SIG_sqisign_p500_27_length_secret_key 417
#define OQS_SIG_sqisign_p500_27_length_signature 306

OQS_SIG *OQS_SIG_sqisign_p500_27_new(void);
OQS_API OQS_STATUS OQS_SIG_sqisign_p500_27_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p500_27_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p500_27_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p500_27_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p500_27_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sqisign_p664_17)
#define OQS_SIG_sqisign_p664_17_length_public_key 169
#define OQS_SIG_sqisign_p664_17_length_secret_key 549
#define OQS_SIG_sqisign_p664_17_length_signature 406

OQS_SIG *OQS_SIG_sqisign_p664_17_new(void);
OQS_API OQS_STATUS OQS_SIG_sqisign_p664_17_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p664_17_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p664_17_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p664_17_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sqisign_p664_17_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
