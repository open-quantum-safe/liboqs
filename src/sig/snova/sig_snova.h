// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_SNOVA_H
#define OQS_SIG_SNOVA_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_snova_SNOVA_24_5_4)
#define OQS_SIG_snova_SNOVA_24_5_4_length_public_key 1016
#define OQS_SIG_snova_SNOVA_24_5_4_length_secret_key 48
#define OQS_SIG_snova_SNOVA_24_5_4_length_signature 248

OQS_SIG *OQS_SIG_snova_SNOVA_24_5_4_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_24_5_4_SHAKE)
#define OQS_SIG_snova_SNOVA_24_5_4_SHAKE_length_public_key 1016
#define OQS_SIG_snova_SNOVA_24_5_4_SHAKE_length_secret_key 48
#define OQS_SIG_snova_SNOVA_24_5_4_SHAKE_length_signature 248

OQS_SIG *OQS_SIG_snova_SNOVA_24_5_4_SHAKE_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_24_5_4_esk)
#define OQS_SIG_snova_SNOVA_24_5_4_esk_length_public_key 1016
#define OQS_SIG_snova_SNOVA_24_5_4_esk_length_secret_key 36848
#define OQS_SIG_snova_SNOVA_24_5_4_esk_length_signature 248

OQS_SIG *OQS_SIG_snova_SNOVA_24_5_4_esk_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_esk_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_esk_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_esk_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_esk_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_esk_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_24_5_4_SHAKE_esk)
#define OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_length_public_key 1016
#define OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_length_secret_key 36848
#define OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_length_signature 248

OQS_SIG *OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_37_17_2)
#define OQS_SIG_snova_SNOVA_37_17_2_length_public_key 9842
#define OQS_SIG_snova_SNOVA_37_17_2_length_secret_key 48
#define OQS_SIG_snova_SNOVA_37_17_2_length_signature 124

OQS_SIG *OQS_SIG_snova_SNOVA_37_17_2_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_17_2_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_17_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_17_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_17_2_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_17_2_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_25_8_3)
#define OQS_SIG_snova_SNOVA_25_8_3_length_public_key 2320
#define OQS_SIG_snova_SNOVA_25_8_3_length_secret_key 48
#define OQS_SIG_snova_SNOVA_25_8_3_length_signature 165

OQS_SIG *OQS_SIG_snova_SNOVA_25_8_3_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_25_8_3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_25_8_3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_25_8_3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_25_8_3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_25_8_3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_56_25_2)
#define OQS_SIG_snova_SNOVA_56_25_2_length_public_key 31266
#define OQS_SIG_snova_SNOVA_56_25_2_length_secret_key 48
#define OQS_SIG_snova_SNOVA_56_25_2_length_signature 178

OQS_SIG *OQS_SIG_snova_SNOVA_56_25_2_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_56_25_2_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_56_25_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_56_25_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_56_25_2_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_56_25_2_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_49_11_3)
#define OQS_SIG_snova_SNOVA_49_11_3_length_public_key 6006
#define OQS_SIG_snova_SNOVA_49_11_3_length_secret_key 48
#define OQS_SIG_snova_SNOVA_49_11_3_length_signature 286

OQS_SIG *OQS_SIG_snova_SNOVA_49_11_3_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_49_11_3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_49_11_3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_49_11_3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_49_11_3_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_49_11_3_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_37_8_4)
#define OQS_SIG_snova_SNOVA_37_8_4_length_public_key 4112
#define OQS_SIG_snova_SNOVA_37_8_4_length_secret_key 48
#define OQS_SIG_snova_SNOVA_37_8_4_length_signature 376

OQS_SIG *OQS_SIG_snova_SNOVA_37_8_4_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_8_4_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_8_4_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_8_4_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_8_4_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_37_8_4_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_24_5_5)
#define OQS_SIG_snova_SNOVA_24_5_5_length_public_key 1579
#define OQS_SIG_snova_SNOVA_24_5_5_length_secret_key 48
#define OQS_SIG_snova_SNOVA_24_5_5_length_signature 379

OQS_SIG *OQS_SIG_snova_SNOVA_24_5_5_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_5_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_24_5_5_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_60_10_4)
#define OQS_SIG_snova_SNOVA_60_10_4_length_public_key 8016
#define OQS_SIG_snova_SNOVA_60_10_4_length_secret_key 48
#define OQS_SIG_snova_SNOVA_60_10_4_length_signature 576

OQS_SIG *OQS_SIG_snova_SNOVA_60_10_4_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_60_10_4_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_60_10_4_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_60_10_4_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_60_10_4_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_60_10_4_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_29_6_5)
#define OQS_SIG_snova_SNOVA_29_6_5_length_public_key 2716
#define OQS_SIG_snova_SNOVA_29_6_5_length_secret_key 48
#define OQS_SIG_snova_SNOVA_29_6_5_length_signature 454

OQS_SIG *OQS_SIG_snova_SNOVA_29_6_5_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_29_6_5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_29_6_5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_29_6_5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_29_6_5_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_29_6_5_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
