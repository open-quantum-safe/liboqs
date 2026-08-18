
// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_SLH_DSA_H
#define OQS_SIG_SLH_DSA_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_SIG_slh_dsa_pure_sha2_128s)
#define OQS_SIG_slh_dsa_pure_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_pure_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_pure_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_pure_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_shake_128s)
#define OQS_SIG_slh_dsa_pure_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_pure_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_pure_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_pure_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_sha2_128f)
#define OQS_SIG_slh_dsa_pure_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_pure_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_pure_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_pure_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_shake_128f)
#define OQS_SIG_slh_dsa_pure_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_pure_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_pure_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_pure_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_sha2_192s)
#define OQS_SIG_slh_dsa_pure_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_pure_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_pure_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_pure_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_shake_192s)
#define OQS_SIG_slh_dsa_pure_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_pure_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_pure_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_pure_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_sha2_192f)
#define OQS_SIG_slh_dsa_pure_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_pure_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_pure_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_pure_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_shake_192f)
#define OQS_SIG_slh_dsa_pure_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_pure_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_pure_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_pure_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_sha2_256s)
#define OQS_SIG_slh_dsa_pure_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_pure_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_pure_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_pure_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_shake_256s)
#define OQS_SIG_slh_dsa_pure_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_pure_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_pure_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_pure_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_sha2_256f)
#define OQS_SIG_slh_dsa_pure_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_pure_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_pure_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_pure_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_pure_shake_256f)
#define OQS_SIG_slh_dsa_pure_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_pure_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_pure_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_pure_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_pure_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_128s)
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_length_public_key 32
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_128s)
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_128s)
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_128s)
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_length_public_key 32
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_length_secret_key 64
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_length_signature 7856

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_128f)
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_length_public_key 32
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_128f)
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_128f)
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_128f)
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_length_public_key 32
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_length_secret_key 64
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_length_signature 17088

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_192s)
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_length_public_key 48
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_192s)
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_192s)
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_192s)
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_length_public_key 48
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_length_secret_key 96
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_length_signature 16224

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_192f)
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_length_public_key 48
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_192f)
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_192f)
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_192f)
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_length_public_key 48
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_length_secret_key 96
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_length_signature 35664

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_256s)
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_length_public_key 64
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_256s)
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_256s)
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_256s)
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_length_public_key 64
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_length_secret_key 128
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_length_signature 29792

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_256f)
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_length_public_key 64
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_256f)
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_256f)
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif


#if defined(OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_256f)
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_length_public_key 64
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_length_secret_key 128
#define OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_length_signature 49856

OQS_SIG *OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_new(void);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx, size_t ctxlen, const uint8_t *public_key);
#endif

#endif
