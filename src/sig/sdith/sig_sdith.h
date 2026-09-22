// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_SDITH_H
#define OQS_SIG_SDITH_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l1_gf2_short)
#define OQS_SIG_sdith_sdith3_l1_gf2_short_length_public_key 70
#define OQS_SIG_sdith_sdith3_l1_gf2_short_length_secret_key 147
#define OQS_SIG_sdith_sdith3_l1_gf2_short_length_signature 3721

OQS_SIG *OQS_SIG_sdith_sdith3_l1_gf2_short_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l1_gf2_short_cipherpow)
#define OQS_SIG_sdith_sdith3_l1_gf2_short_cipherpow_length_public_key 70
#define OQS_SIG_sdith_sdith3_l1_gf2_short_cipherpow_length_secret_key 147
#define OQS_SIG_sdith_sdith3_l1_gf2_short_cipherpow_length_signature 3721

OQS_SIG *OQS_SIG_sdith_sdith3_l1_gf2_short_cipherpow_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_cipherpow_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_cipherpow_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_cipherpow_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_cipherpow_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_short_cipherpow_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l1_gf2_fast)
#define OQS_SIG_sdith_sdith3_l1_gf2_fast_length_public_key 70
#define OQS_SIG_sdith_sdith3_l1_gf2_fast_length_secret_key 147
#define OQS_SIG_sdith_sdith3_l1_gf2_fast_length_signature 4914

OQS_SIG *OQS_SIG_sdith_sdith3_l1_gf2_fast_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l1_gf2_fast_cipherpow)
#define OQS_SIG_sdith_sdith3_l1_gf2_fast_cipherpow_length_public_key 70
#define OQS_SIG_sdith_sdith3_l1_gf2_fast_cipherpow_length_secret_key 147
#define OQS_SIG_sdith_sdith3_l1_gf2_fast_cipherpow_length_signature 4643

OQS_SIG *OQS_SIG_sdith_sdith3_l1_gf2_fast_cipherpow_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_cipherpow_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_cipherpow_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_cipherpow_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_cipherpow_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l1_gf2_fast_cipherpow_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l3_gf2_short)
#define OQS_SIG_sdith_sdith3_l3_gf2_short_length_public_key 98
#define OQS_SIG_sdith_sdith3_l3_gf2_short_length_secret_key 208
#define OQS_SIG_sdith_sdith3_l3_gf2_short_length_signature 8484

OQS_SIG *OQS_SIG_sdith_sdith3_l3_gf2_short_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l3_gf2_short_cipherpow)
#define OQS_SIG_sdith_sdith3_l3_gf2_short_cipherpow_length_public_key 98
#define OQS_SIG_sdith_sdith3_l3_gf2_short_cipherpow_length_secret_key 208
#define OQS_SIG_sdith_sdith3_l3_gf2_short_cipherpow_length_signature 8484

OQS_SIG *OQS_SIG_sdith_sdith3_l3_gf2_short_cipherpow_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_cipherpow_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_cipherpow_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_cipherpow_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_cipherpow_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_short_cipherpow_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l3_gf2_fast)
#define OQS_SIG_sdith_sdith3_l3_gf2_fast_length_public_key 98
#define OQS_SIG_sdith_sdith3_l3_gf2_fast_length_secret_key 208
#define OQS_SIG_sdith_sdith3_l3_gf2_fast_length_signature 10852

OQS_SIG *OQS_SIG_sdith_sdith3_l3_gf2_fast_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l3_gf2_fast_cipherpow)
#define OQS_SIG_sdith_sdith3_l3_gf2_fast_cipherpow_length_public_key 98
#define OQS_SIG_sdith_sdith3_l3_gf2_fast_cipherpow_length_secret_key 208
#define OQS_SIG_sdith_sdith3_l3_gf2_fast_cipherpow_length_signature 10452

OQS_SIG *OQS_SIG_sdith_sdith3_l3_gf2_fast_cipherpow_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_cipherpow_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_cipherpow_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_cipherpow_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_cipherpow_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l3_gf2_fast_cipherpow_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l5_gf2_short)
#define OQS_SIG_sdith_sdith3_l5_gf2_short_length_public_key 132
#define OQS_SIG_sdith_sdith3_l5_gf2_short_length_secret_key 275
#define OQS_SIG_sdith_sdith3_l5_gf2_short_length_signature 15147

OQS_SIG *OQS_SIG_sdith_sdith3_l5_gf2_short_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l5_gf2_short_cipherpow)
#define OQS_SIG_sdith_sdith3_l5_gf2_short_cipherpow_length_public_key 132
#define OQS_SIG_sdith_sdith3_l5_gf2_short_cipherpow_length_secret_key 275
#define OQS_SIG_sdith_sdith3_l5_gf2_short_cipherpow_length_signature 15147

OQS_SIG *OQS_SIG_sdith_sdith3_l5_gf2_short_cipherpow_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_cipherpow_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_cipherpow_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_cipherpow_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_cipherpow_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_short_cipherpow_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l5_gf2_fast)
#define OQS_SIG_sdith_sdith3_l5_gf2_fast_length_public_key 132
#define OQS_SIG_sdith_sdith3_l5_gf2_fast_length_secret_key 275
#define OQS_SIG_sdith_sdith3_l5_gf2_fast_length_signature 19144

OQS_SIG *OQS_SIG_sdith_sdith3_l5_gf2_fast_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_sdith_sdith3_l5_gf2_fast_cipherpow)
#define OQS_SIG_sdith_sdith3_l5_gf2_fast_cipherpow_length_public_key 132
#define OQS_SIG_sdith_sdith3_l5_gf2_fast_cipherpow_length_secret_key 275
#define OQS_SIG_sdith_sdith3_l5_gf2_fast_cipherpow_length_signature 19144

OQS_SIG *OQS_SIG_sdith_sdith3_l5_gf2_fast_cipherpow_new(void);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_cipherpow_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_cipherpow_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_cipherpow_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_cipherpow_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sdith_sdith3_l5_gf2_fast_cipherpow_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
