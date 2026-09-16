// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_MQOM_H
#define OQS_SIG_MQOM_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat1_gf16_fast_ct)
#define OQS_SIG_mqom_mqom3_cat1_gf16_fast_ct_length_public_key 64
#define OQS_SIG_mqom_mqom3_cat1_gf16_fast_ct_length_secret_key 96
#define OQS_SIG_mqom_mqom3_cat1_gf16_fast_ct_length_signature 3316

OQS_SIG *OQS_SIG_mqom_mqom3_cat1_gf16_fast_ct_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ct_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ct_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ct_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ct_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ct_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat1_gf16_fast_ot)
#define OQS_SIG_mqom_mqom3_cat1_gf16_fast_ot_length_public_key 64
#define OQS_SIG_mqom_mqom3_cat1_gf16_fast_ot_length_secret_key 96
#define OQS_SIG_mqom_mqom3_cat1_gf16_fast_ot_length_signature 3316

OQS_SIG *OQS_SIG_mqom_mqom3_cat1_gf16_fast_ot_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ot_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ot_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ot_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ot_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_fast_ot_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat1_gf16_short_ct)
#define OQS_SIG_mqom_mqom3_cat1_gf16_short_ct_length_public_key 64
#define OQS_SIG_mqom_mqom3_cat1_gf16_short_ct_length_secret_key 96
#define OQS_SIG_mqom_mqom3_cat1_gf16_short_ct_length_signature 2932

OQS_SIG *OQS_SIG_mqom_mqom3_cat1_gf16_short_ct_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ct_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ct_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ct_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ct_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ct_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat1_gf16_short_ot)
#define OQS_SIG_mqom_mqom3_cat1_gf16_short_ot_length_public_key 64
#define OQS_SIG_mqom_mqom3_cat1_gf16_short_ot_length_secret_key 96
#define OQS_SIG_mqom_mqom3_cat1_gf16_short_ot_length_signature 2932

OQS_SIG *OQS_SIG_mqom_mqom3_cat1_gf16_short_ot_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ot_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ot_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ot_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ot_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf16_short_ot_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat1_gf2_shorter_ct)
#define OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ct_length_public_key 52
#define OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ct_length_secret_key 72
#define OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ct_length_signature 2492

OQS_SIG *OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ct_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ct_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ct_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ct_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ct_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ct_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat1_gf2_shorter_ot)
#define OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ot_length_public_key 52
#define OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ot_length_secret_key 72
#define OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ot_length_signature 2492

OQS_SIG *OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ot_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ot_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ot_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ot_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ot_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat1_gf2_shorter_ot_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat3_gf16_fast_ct)
#define OQS_SIG_mqom_mqom3_cat3_gf16_fast_ct_length_public_key 96
#define OQS_SIG_mqom_mqom3_cat3_gf16_fast_ct_length_secret_key 144
#define OQS_SIG_mqom_mqom3_cat3_gf16_fast_ct_length_signature 7564

OQS_SIG *OQS_SIG_mqom_mqom3_cat3_gf16_fast_ct_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ct_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ct_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ct_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ct_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ct_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat3_gf16_fast_ot)
#define OQS_SIG_mqom_mqom3_cat3_gf16_fast_ot_length_public_key 96
#define OQS_SIG_mqom_mqom3_cat3_gf16_fast_ot_length_secret_key 144
#define OQS_SIG_mqom_mqom3_cat3_gf16_fast_ot_length_signature 7660

OQS_SIG *OQS_SIG_mqom_mqom3_cat3_gf16_fast_ot_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ot_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ot_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ot_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ot_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_fast_ot_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat3_gf16_short_ct)
#define OQS_SIG_mqom_mqom3_cat3_gf16_short_ct_length_public_key 96
#define OQS_SIG_mqom_mqom3_cat3_gf16_short_ct_length_secret_key 144
#define OQS_SIG_mqom_mqom3_cat3_gf16_short_ct_length_signature 6556

OQS_SIG *OQS_SIG_mqom_mqom3_cat3_gf16_short_ct_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ct_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ct_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ct_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ct_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ct_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat3_gf16_short_ot)
#define OQS_SIG_mqom_mqom3_cat3_gf16_short_ot_length_public_key 96
#define OQS_SIG_mqom_mqom3_cat3_gf16_short_ot_length_secret_key 144
#define OQS_SIG_mqom_mqom3_cat3_gf16_short_ot_length_signature 6556

OQS_SIG *OQS_SIG_mqom_mqom3_cat3_gf16_short_ot_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ot_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ot_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ot_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ot_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf16_short_ot_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat3_gf2_shorter_ct)
#define OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ct_length_public_key 78
#define OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ct_length_secret_key 108
#define OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ct_length_signature 5932

OQS_SIG *OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ct_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ct_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ct_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ct_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ct_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ct_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat3_gf2_shorter_ot)
#define OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ot_length_public_key 78
#define OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ot_length_secret_key 108
#define OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ot_length_signature 5890

OQS_SIG *OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ot_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ot_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ot_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ot_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ot_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat3_gf2_shorter_ot_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat5_gf16_fast_ct)
#define OQS_SIG_mqom_mqom3_cat5_gf16_fast_ct_length_public_key 128
#define OQS_SIG_mqom_mqom3_cat5_gf16_fast_ct_length_secret_key 192
#define OQS_SIG_mqom_mqom3_cat5_gf16_fast_ct_length_signature 13540

OQS_SIG *OQS_SIG_mqom_mqom3_cat5_gf16_fast_ct_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ct_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ct_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ct_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ct_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ct_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat5_gf16_fast_ot)
#define OQS_SIG_mqom_mqom3_cat5_gf16_fast_ot_length_public_key 128
#define OQS_SIG_mqom_mqom3_cat5_gf16_fast_ot_length_secret_key 192
#define OQS_SIG_mqom_mqom3_cat5_gf16_fast_ot_length_signature 13380

OQS_SIG *OQS_SIG_mqom_mqom3_cat5_gf16_fast_ot_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ot_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ot_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ot_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ot_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_fast_ot_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat5_gf16_short_ct)
#define OQS_SIG_mqom_mqom3_cat5_gf16_short_ct_length_public_key 128
#define OQS_SIG_mqom_mqom3_cat5_gf16_short_ct_length_secret_key 192
#define OQS_SIG_mqom_mqom3_cat5_gf16_short_ct_length_signature 12100

OQS_SIG *OQS_SIG_mqom_mqom3_cat5_gf16_short_ct_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ct_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ct_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ct_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ct_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ct_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat5_gf16_short_ot)
#define OQS_SIG_mqom_mqom3_cat5_gf16_short_ot_length_public_key 128
#define OQS_SIG_mqom_mqom3_cat5_gf16_short_ot_length_secret_key 192
#define OQS_SIG_mqom_mqom3_cat5_gf16_short_ot_length_signature 11716

OQS_SIG *OQS_SIG_mqom_mqom3_cat5_gf16_short_ot_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ot_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ot_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ot_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ot_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf16_short_ot_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat5_gf2_shorter_ct)
#define OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ct_length_public_key 104
#define OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ct_length_secret_key 144
#define OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ct_length_signature 10836

OQS_SIG *OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ct_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ct_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ct_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ct_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ct_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ct_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_mqom_mqom3_cat5_gf2_shorter_ot)
#define OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ot_length_public_key 104
#define OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ot_length_secret_key 144
#define OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ot_length_signature 10804

OQS_SIG *OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ot_new(void);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ot_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ot_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ot_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ot_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_mqom_mqom3_cat5_gf2_shorter_ot_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
