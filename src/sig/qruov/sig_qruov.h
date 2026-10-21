// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_QRUOV_H
#define OQS_SIG_QRUOV_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_qruov_1q7L10aes)
#define OQS_SIG_qruov_1q7L10aes_length_public_key 20641
#define OQS_SIG_qruov_1q7L10aes_length_secret_key 32
#define OQS_SIG_qruov_1q7L10aes_length_signature 331

OQS_SIG *OQS_SIG_qruov_1q7L10aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_1q7L10shake)
#define OQS_SIG_qruov_1q7L10shake_length_public_key 20641
#define OQS_SIG_qruov_1q7L10shake_length_secret_key 32
#define OQS_SIG_qruov_1q7L10shake_length_signature 331

OQS_SIG *OQS_SIG_qruov_1q7L10shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q7L10shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_1q31L3aes)
#define OQS_SIG_qruov_1q31L3aes_length_public_key 23641
#define OQS_SIG_qruov_1q31L3aes_length_secret_key 32
#define OQS_SIG_qruov_1q31L3aes_length_signature 157

OQS_SIG *OQS_SIG_qruov_1q31L3aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_1q31L3shake)
#define OQS_SIG_qruov_1q31L3shake_length_public_key 23641
#define OQS_SIG_qruov_1q31L3shake_length_secret_key 32
#define OQS_SIG_qruov_1q31L3shake_length_signature 157

OQS_SIG *OQS_SIG_qruov_1q31L3shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L3shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_1q31L10aes)
#define OQS_SIG_qruov_1q31L10aes_length_public_key 12266
#define OQS_SIG_qruov_1q31L10aes_length_secret_key 32
#define OQS_SIG_qruov_1q31L10aes_length_signature 435

OQS_SIG *OQS_SIG_qruov_1q31L10aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_1q31L10shake)
#define OQS_SIG_qruov_1q31L10shake_length_public_key 12266
#define OQS_SIG_qruov_1q31L10shake_length_secret_key 32
#define OQS_SIG_qruov_1q31L10shake_length_signature 435

OQS_SIG *OQS_SIG_qruov_1q31L10shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q31L10shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_1q127L3aes)
#define OQS_SIG_qruov_1q127L3aes_length_public_key 24256
#define OQS_SIG_qruov_1q127L3aes_length_secret_key 32
#define OQS_SIG_qruov_1q127L3aes_length_signature 200

OQS_SIG *OQS_SIG_qruov_1q127L3aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_1q127L3shake)
#define OQS_SIG_qruov_1q127L3shake_length_public_key 24256
#define OQS_SIG_qruov_1q127L3shake_length_secret_key 32
#define OQS_SIG_qruov_1q127L3shake_length_signature 200

OQS_SIG *OQS_SIG_qruov_1q127L3shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L3shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_1q127L10aes)
#define OQS_SIG_qruov_1q127L10aes_length_public_key 11041
#define OQS_SIG_qruov_1q127L10aes_length_secret_key 32
#define OQS_SIG_qruov_1q127L10aes_length_signature 541

OQS_SIG *OQS_SIG_qruov_1q127L10aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_1q127L10shake)
#define OQS_SIG_qruov_1q127L10shake_length_public_key 11041
#define OQS_SIG_qruov_1q127L10shake_length_secret_key 32
#define OQS_SIG_qruov_1q127L10shake_length_signature 541

OQS_SIG *OQS_SIG_qruov_1q127L10shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_1q127L10shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q7L10aes)
#define OQS_SIG_qruov_3q7L10aes_length_public_key 55141
#define OQS_SIG_qruov_3q7L10aes_length_secret_key 32
#define OQS_SIG_qruov_3q7L10aes_length_signature 489

OQS_SIG *OQS_SIG_qruov_3q7L10aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q7L10shake)
#define OQS_SIG_qruov_3q7L10shake_length_public_key 55141
#define OQS_SIG_qruov_3q7L10shake_length_secret_key 32
#define OQS_SIG_qruov_3q7L10shake_length_signature 489

OQS_SIG *OQS_SIG_qruov_3q7L10shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q7L10shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q31L3aes)
#define OQS_SIG_qruov_3q31L3aes_length_public_key 70976
#define OQS_SIG_qruov_3q31L3aes_length_secret_key 32
#define OQS_SIG_qruov_3q31L3aes_length_signature 233

OQS_SIG *OQS_SIG_qruov_3q31L3aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q31L3shake)
#define OQS_SIG_qruov_3q31L3shake_length_public_key 70976
#define OQS_SIG_qruov_3q31L3shake_length_secret_key 32
#define OQS_SIG_qruov_3q31L3shake_length_signature 233

OQS_SIG *OQS_SIG_qruov_3q31L3shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L3shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q31L10aes)
#define OQS_SIG_qruov_3q31L10aes_length_public_key 34391
#define OQS_SIG_qruov_3q31L10aes_length_secret_key 32
#define OQS_SIG_qruov_3q31L10aes_length_signature 643

OQS_SIG *OQS_SIG_qruov_3q31L10aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q31L10shake)
#define OQS_SIG_qruov_3q31L10shake_length_public_key 34391
#define OQS_SIG_qruov_3q31L10shake_length_secret_key 32
#define OQS_SIG_qruov_3q31L10shake_length_signature 643

OQS_SIG *OQS_SIG_qruov_3q31L10shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q31L10shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q127L3aes)
#define OQS_SIG_qruov_3q127L3aes_length_public_key 71884
#define OQS_SIG_qruov_3q127L3aes_length_secret_key 32
#define OQS_SIG_qruov_3q127L3aes_length_signature 292

OQS_SIG *OQS_SIG_qruov_3q127L3aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q127L3shake)
#define OQS_SIG_qruov_3q127L3shake_length_public_key 71884
#define OQS_SIG_qruov_3q127L3shake_length_secret_key 32
#define OQS_SIG_qruov_3q127L3shake_length_signature 292

OQS_SIG *OQS_SIG_qruov_3q127L3shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L3shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q127L10aes)
#define OQS_SIG_qruov_3q127L10aes_length_public_key 35454
#define OQS_SIG_qruov_3q127L10aes_length_secret_key 32
#define OQS_SIG_qruov_3q127L10aes_length_signature 821

OQS_SIG *OQS_SIG_qruov_3q127L10aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_3q127L10shake)
#define OQS_SIG_qruov_3q127L10shake_length_public_key 35454
#define OQS_SIG_qruov_3q127L10shake_length_secret_key 32
#define OQS_SIG_qruov_3q127L10shake_length_signature 821

OQS_SIG *OQS_SIG_qruov_3q127L10shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_3q127L10shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q7L10aes)
#define OQS_SIG_qruov_5q7L10aes_length_public_key 135391
#define OQS_SIG_qruov_5q7L10aes_length_secret_key 32
#define OQS_SIG_qruov_5q7L10aes_length_signature 662

OQS_SIG *OQS_SIG_qruov_5q7L10aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q7L10shake)
#define OQS_SIG_qruov_5q7L10shake_length_public_key 135391
#define OQS_SIG_qruov_5q7L10shake_length_secret_key 32
#define OQS_SIG_qruov_5q7L10shake_length_signature 662

OQS_SIG *OQS_SIG_qruov_5q7L10shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q7L10shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q31L3aes)
#define OQS_SIG_qruov_5q31L3aes_length_public_key 158405
#define OQS_SIG_qruov_5q31L3aes_length_secret_key 32
#define OQS_SIG_qruov_5q31L3aes_length_signature 306

OQS_SIG *OQS_SIG_qruov_5q31L3aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q31L3shake)
#define OQS_SIG_qruov_5q31L3shake_length_public_key 158405
#define OQS_SIG_qruov_5q31L3shake_length_secret_key 32
#define OQS_SIG_qruov_5q31L3shake_length_signature 306

OQS_SIG *OQS_SIG_qruov_5q31L3shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L3shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q31L10aes)
#define OQS_SIG_qruov_5q31L10aes_length_public_key 58516
#define OQS_SIG_qruov_5q31L10aes_length_secret_key 32
#define OQS_SIG_qruov_5q31L10aes_length_signature 807

OQS_SIG *OQS_SIG_qruov_5q31L10aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q31L10shake)
#define OQS_SIG_qruov_5q31L10shake_length_public_key 58516
#define OQS_SIG_qruov_5q31L10shake_length_secret_key 32
#define OQS_SIG_qruov_5q31L10shake_length_signature 807

OQS_SIG *OQS_SIG_qruov_5q31L10shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q31L10shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q127L3aes)
#define OQS_SIG_qruov_5q127L3aes_length_public_key 173660
#define OQS_SIG_qruov_5q127L3aes_length_secret_key 32
#define OQS_SIG_qruov_5q127L3aes_length_signature 392

OQS_SIG *OQS_SIG_qruov_5q127L3aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q127L3shake)
#define OQS_SIG_qruov_5q127L3shake_length_public_key 173660
#define OQS_SIG_qruov_5q127L3shake_length_secret_key 32
#define OQS_SIG_qruov_5q127L3shake_length_signature 392

OQS_SIG *OQS_SIG_qruov_5q127L3shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L3shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q127L10aes)
#define OQS_SIG_qruov_5q127L10aes_length_public_key 63541
#define OQS_SIG_qruov_5q127L10aes_length_secret_key 32
#define OQS_SIG_qruov_5q127L10aes_length_signature 1039

OQS_SIG *OQS_SIG_qruov_5q127L10aes_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10aes_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10aes_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10aes_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10aes_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_qruov_5q127L10shake)
#define OQS_SIG_qruov_5q127L10shake_length_public_key 63541
#define OQS_SIG_qruov_5q127L10shake_length_secret_key 32
#define OQS_SIG_qruov_5q127L10shake_length_signature 1039

OQS_SIG *OQS_SIG_qruov_5q127L10shake_new(void);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10shake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10shake_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10shake_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10shake_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_qruov_5q127L10shake_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
