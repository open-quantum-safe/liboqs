// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_SNOVA_H
#define OQS_SIG_SNOVA_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_snova_SNOVA_I_K)
#define OQS_SIG_snova_SNOVA_I_K_length_public_key 376
#define OQS_SIG_snova_SNOVA_I_K_length_secret_key 96
#define OQS_SIG_snova_SNOVA_I_K_length_signature 528

OQS_SIG *OQS_SIG_snova_SNOVA_I_K_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_I_K_AES)
#define OQS_SIG_snova_SNOVA_I_K_AES_length_public_key 376
#define OQS_SIG_snova_SNOVA_I_K_AES_length_secret_key 96
#define OQS_SIG_snova_SNOVA_I_K_AES_length_signature 528

OQS_SIG *OQS_SIG_snova_SNOVA_I_K_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_K_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_I_B)
#define OQS_SIG_snova_SNOVA_I_B_length_public_key 656
#define OQS_SIG_snova_SNOVA_I_B_length_secret_key 96
#define OQS_SIG_snova_SNOVA_I_B_length_signature 388

OQS_SIG *OQS_SIG_snova_SNOVA_I_B_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_I_B_AES)
#define OQS_SIG_snova_SNOVA_I_B_AES_length_public_key 656
#define OQS_SIG_snova_SNOVA_I_B_AES_length_secret_key 96
#define OQS_SIG_snova_SNOVA_I_B_AES_length_signature 388

OQS_SIG *OQS_SIG_snova_SNOVA_I_B_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_B_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_I_S)
#define OQS_SIG_snova_SNOVA_I_S_length_public_key 1016
#define OQS_SIG_snova_SNOVA_I_S_length_secret_key 96
#define OQS_SIG_snova_SNOVA_I_S_length_signature 272

OQS_SIG *OQS_SIG_snova_SNOVA_I_S_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_I_S_AES)
#define OQS_SIG_snova_SNOVA_I_S_AES_length_public_key 1016
#define OQS_SIG_snova_SNOVA_I_S_AES_length_secret_key 96
#define OQS_SIG_snova_SNOVA_I_S_AES_length_signature 272

OQS_SIG *OQS_SIG_snova_SNOVA_I_S_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_S_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_I_X_AES)
#define OQS_SIG_snova_SNOVA_I_X_AES_length_public_key 9842
#define OQS_SIG_snova_SNOVA_I_X_AES_length_secret_key 48
#define OQS_SIG_snova_SNOVA_I_X_AES_length_signature 150

OQS_SIG *OQS_SIG_snova_SNOVA_I_X_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_X_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_X_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_X_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_X_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_I_X_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_III_K)
#define OQS_SIG_snova_SNOVA_III_K_length_public_key 912
#define OQS_SIG_snova_SNOVA_III_K_length_secret_key 96
#define OQS_SIG_snova_SNOVA_III_K_length_signature 688

OQS_SIG *OQS_SIG_snova_SNOVA_III_K_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_III_K_AES)
#define OQS_SIG_snova_SNOVA_III_K_AES_length_public_key 912
#define OQS_SIG_snova_SNOVA_III_K_AES_length_secret_key 96
#define OQS_SIG_snova_SNOVA_III_K_AES_length_signature 688

OQS_SIG *OQS_SIG_snova_SNOVA_III_K_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_K_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_III_B)
#define OQS_SIG_snova_SNOVA_III_B_length_public_key 1416
#define OQS_SIG_snova_SNOVA_III_B_length_secret_key 96
#define OQS_SIG_snova_SNOVA_III_B_length_signature 532

OQS_SIG *OQS_SIG_snova_SNOVA_III_B_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_III_B_AES)
#define OQS_SIG_snova_SNOVA_III_B_AES_length_public_key 1416
#define OQS_SIG_snova_SNOVA_III_B_AES_length_secret_key 96
#define OQS_SIG_snova_SNOVA_III_B_AES_length_signature 532

OQS_SIG *OQS_SIG_snova_SNOVA_III_B_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_B_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_III_S)
#define OQS_SIG_snova_SNOVA_III_S_length_public_key 2032
#define OQS_SIG_snova_SNOVA_III_S_length_secret_key 96
#define OQS_SIG_snova_SNOVA_III_S_length_signature 456

OQS_SIG *OQS_SIG_snova_SNOVA_III_S_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_III_S_AES)
#define OQS_SIG_snova_SNOVA_III_S_AES_length_public_key 2032
#define OQS_SIG_snova_SNOVA_III_S_AES_length_secret_key 96
#define OQS_SIG_snova_SNOVA_III_S_AES_length_signature 456

OQS_SIG *OQS_SIG_snova_SNOVA_III_S_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_S_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_III_X_AES)
#define OQS_SIG_snova_SNOVA_III_X_AES_length_public_key 31266
#define OQS_SIG_snova_SNOVA_III_X_AES_length_secret_key 48
#define OQS_SIG_snova_SNOVA_III_X_AES_length_signature 218

OQS_SIG *OQS_SIG_snova_SNOVA_III_X_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_X_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_X_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_X_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_X_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_III_X_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_V_K)
#define OQS_SIG_snova_SNOVA_V_K_length_public_key 1216
#define OQS_SIG_snova_SNOVA_V_K_length_secret_key 96
#define OQS_SIG_snova_SNOVA_V_K_length_signature 896

OQS_SIG *OQS_SIG_snova_SNOVA_V_K_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_V_K_AES)
#define OQS_SIG_snova_SNOVA_V_K_AES_length_public_key 1216
#define OQS_SIG_snova_SNOVA_V_K_AES_length_secret_key 96
#define OQS_SIG_snova_SNOVA_V_K_AES_length_signature 896

OQS_SIG *OQS_SIG_snova_SNOVA_V_K_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_K_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_V_B)
#define OQS_SIG_snova_SNOVA_V_B_length_public_key 1891
#define OQS_SIG_snova_SNOVA_V_B_length_secret_key 96
#define OQS_SIG_snova_SNOVA_V_B_length_signature 691

OQS_SIG *OQS_SIG_snova_SNOVA_V_B_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_V_B_AES)
#define OQS_SIG_snova_SNOVA_V_B_AES_length_public_key 1891
#define OQS_SIG_snova_SNOVA_V_B_AES_length_secret_key 96
#define OQS_SIG_snova_SNOVA_V_B_AES_length_signature 691

OQS_SIG *OQS_SIG_snova_SNOVA_V_B_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_B_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_V_S)
#define OQS_SIG_snova_SNOVA_V_S_length_public_key 2716
#define OQS_SIG_snova_SNOVA_V_S_length_secret_key 96
#define OQS_SIG_snova_SNOVA_V_S_length_signature 591

OQS_SIG *OQS_SIG_snova_SNOVA_V_S_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#if defined(OQS_ENABLE_SIG_snova_SNOVA_V_S_AES)
#define OQS_SIG_snova_SNOVA_V_S_AES_length_public_key 2716
#define OQS_SIG_snova_SNOVA_V_S_AES_length_secret_key 96
#define OQS_SIG_snova_SNOVA_V_S_AES_length_signature 591

OQS_SIG *OQS_SIG_snova_SNOVA_V_S_AES_new(void);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_AES_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_AES_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_AES_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_AES_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_snova_SNOVA_V_S_AES_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
