// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_MQOM_H
#define OQS_SIG_MQOM_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat1_gf16_fast_r3)
#define OQS_SIG_mqom_mqom2_cat1_gf16_fast_r3_length_public_key 60
#define OQS_SIG_mqom_mqom2_cat1_gf16_fast_r3_length_secret_key 88
#define OQS_SIG_mqom_mqom2_cat1_gf16_fast_r3_length_signature 3484

OQS_SIG *OQS_SIG_mqom_mqom2_cat1_gf16_fast_r3_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat1_gf16_fast_r5)
#define OQS_SIG_mqom_mqom2_cat1_gf16_fast_r5_length_public_key 60
#define OQS_SIG_mqom_mqom2_cat1_gf16_fast_r5_length_secret_key 88
#define OQS_SIG_mqom_mqom2_cat1_gf16_fast_r5_length_signature 3280

OQS_SIG *OQS_SIG_mqom_mqom2_cat1_gf16_fast_r5_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r5_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_fast_r5_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat1_gf16_short_r3)
#define OQS_SIG_mqom_mqom2_cat1_gf16_short_r3_length_public_key 60
#define OQS_SIG_mqom_mqom2_cat1_gf16_short_r3_length_secret_key 88
#define OQS_SIG_mqom_mqom2_cat1_gf16_short_r3_length_signature 3060

OQS_SIG *OQS_SIG_mqom_mqom2_cat1_gf16_short_r3_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat1_gf16_short_r5)
#define OQS_SIG_mqom_mqom2_cat1_gf16_short_r5_length_public_key 60
#define OQS_SIG_mqom_mqom2_cat1_gf16_short_r5_length_secret_key 88
#define OQS_SIG_mqom_mqom2_cat1_gf16_short_r5_length_signature 2916

OQS_SIG *OQS_SIG_mqom_mqom2_cat1_gf16_short_r5_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r5_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat1_gf16_short_r5_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat3_gf16_fast_r3)
#define OQS_SIG_mqom_mqom2_cat3_gf16_fast_r3_length_public_key 90
#define OQS_SIG_mqom_mqom2_cat3_gf16_fast_r3_length_secret_key 132
#define OQS_SIG_mqom_mqom2_cat3_gf16_fast_r3_length_signature 8224

OQS_SIG *OQS_SIG_mqom_mqom2_cat3_gf16_fast_r3_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat3_gf16_fast_r5)
#define OQS_SIG_mqom_mqom2_cat3_gf16_fast_r5_length_public_key 90
#define OQS_SIG_mqom_mqom2_cat3_gf16_fast_r5_length_secret_key 132
#define OQS_SIG_mqom_mqom2_cat3_gf16_fast_r5_length_signature 7738

OQS_SIG *OQS_SIG_mqom_mqom2_cat3_gf16_fast_r5_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r5_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_fast_r5_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat3_gf16_short_r3)
#define OQS_SIG_mqom_mqom2_cat3_gf16_short_r3_length_public_key 90
#define OQS_SIG_mqom_mqom2_cat3_gf16_short_r3_length_secret_key 132
#define OQS_SIG_mqom_mqom2_cat3_gf16_short_r3_length_signature 6820

OQS_SIG *OQS_SIG_mqom_mqom2_cat3_gf16_short_r3_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat3_gf16_short_r5)
#define OQS_SIG_mqom_mqom2_cat3_gf16_short_r5_length_public_key 90
#define OQS_SIG_mqom_mqom2_cat3_gf16_short_r5_length_secret_key 132
#define OQS_SIG_mqom_mqom2_cat3_gf16_short_r5_length_signature 6496

OQS_SIG *OQS_SIG_mqom_mqom2_cat3_gf16_short_r5_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r5_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat3_gf16_short_r5_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat5_gf16_fast_r3)
#define OQS_SIG_mqom_mqom2_cat5_gf16_fast_r3_length_public_key 122
#define OQS_SIG_mqom_mqom2_cat5_gf16_fast_r3_length_secret_key 180
#define OQS_SIG_mqom_mqom2_cat5_gf16_fast_r3_length_signature 14708

OQS_SIG *OQS_SIG_mqom_mqom2_cat5_gf16_fast_r3_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat5_gf16_fast_r5)
#define OQS_SIG_mqom_mqom2_cat5_gf16_fast_r5_length_public_key 122
#define OQS_SIG_mqom_mqom2_cat5_gf16_fast_r5_length_secret_key 180
#define OQS_SIG_mqom_mqom2_cat5_gf16_fast_r5_length_signature 13772

OQS_SIG *OQS_SIG_mqom_mqom2_cat5_gf16_fast_r5_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r5_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_fast_r5_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat5_gf16_short_r3)
#define OQS_SIG_mqom_mqom2_cat5_gf16_short_r3_length_public_key 122
#define OQS_SIG_mqom_mqom2_cat5_gf16_short_r3_length_secret_key 180
#define OQS_SIG_mqom_mqom2_cat5_gf16_short_r3_length_signature 12664

OQS_SIG *OQS_SIG_mqom_mqom2_cat5_gf16_short_r3_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom2_cat5_gf16_short_r5)
#define OQS_SIG_mqom_mqom2_cat5_gf16_short_r5_length_public_key 122
#define OQS_SIG_mqom_mqom2_cat5_gf16_short_r5_length_secret_key 180
#define OQS_SIG_mqom_mqom2_cat5_gf16_short_r5_length_signature 12014

OQS_SIG *OQS_SIG_mqom_mqom2_cat5_gf16_short_r5_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r5_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom2_cat5_gf16_short_r5_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
