// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_SPHINCS_H
#define OQS_SIG_SPHINCS_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_SIG_sphincs_haraka_128f_robust
#define OQS_SIG_sphincs_haraka_128f_robust_length_public_key 32
#define OQS_SIG_sphincs_haraka_128f_robust_length_secret_key 64
#define OQS_SIG_sphincs_haraka_128f_robust_length_signature 17088

OQS_SIG *OQS_SIG_sphincs_haraka_128f_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128f_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_128f_simple
#define OQS_SIG_sphincs_haraka_128f_simple_length_public_key 32
#define OQS_SIG_sphincs_haraka_128f_simple_length_secret_key 64
#define OQS_SIG_sphincs_haraka_128f_simple_length_signature 17088

OQS_SIG *OQS_SIG_sphincs_haraka_128f_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128f_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_128s_robust
#define OQS_SIG_sphincs_haraka_128s_robust_length_public_key 32
#define OQS_SIG_sphincs_haraka_128s_robust_length_secret_key 64
#define OQS_SIG_sphincs_haraka_128s_robust_length_signature 7856

OQS_SIG *OQS_SIG_sphincs_haraka_128s_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128s_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_128s_simple
#define OQS_SIG_sphincs_haraka_128s_simple_length_public_key 32
#define OQS_SIG_sphincs_haraka_128s_simple_length_secret_key 64
#define OQS_SIG_sphincs_haraka_128s_simple_length_signature 7856

OQS_SIG *OQS_SIG_sphincs_haraka_128s_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128s_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_128s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_192f_robust
#define OQS_SIG_sphincs_haraka_192f_robust_length_public_key 48
#define OQS_SIG_sphincs_haraka_192f_robust_length_secret_key 96
#define OQS_SIG_sphincs_haraka_192f_robust_length_signature 35664

OQS_SIG *OQS_SIG_sphincs_haraka_192f_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192f_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_192f_simple
#define OQS_SIG_sphincs_haraka_192f_simple_length_public_key 48
#define OQS_SIG_sphincs_haraka_192f_simple_length_secret_key 96
#define OQS_SIG_sphincs_haraka_192f_simple_length_signature 35664

OQS_SIG *OQS_SIG_sphincs_haraka_192f_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192f_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_192s_robust
#define OQS_SIG_sphincs_haraka_192s_robust_length_public_key 48
#define OQS_SIG_sphincs_haraka_192s_robust_length_secret_key 96
#define OQS_SIG_sphincs_haraka_192s_robust_length_signature 16224

OQS_SIG *OQS_SIG_sphincs_haraka_192s_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192s_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_192s_simple
#define OQS_SIG_sphincs_haraka_192s_simple_length_public_key 48
#define OQS_SIG_sphincs_haraka_192s_simple_length_secret_key 96
#define OQS_SIG_sphincs_haraka_192s_simple_length_signature 16224

OQS_SIG *OQS_SIG_sphincs_haraka_192s_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192s_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_192s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_256f_robust
#define OQS_SIG_sphincs_haraka_256f_robust_length_public_key 64
#define OQS_SIG_sphincs_haraka_256f_robust_length_secret_key 128
#define OQS_SIG_sphincs_haraka_256f_robust_length_signature 49856

OQS_SIG *OQS_SIG_sphincs_haraka_256f_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256f_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_256f_simple
#define OQS_SIG_sphincs_haraka_256f_simple_length_public_key 64
#define OQS_SIG_sphincs_haraka_256f_simple_length_secret_key 128
#define OQS_SIG_sphincs_haraka_256f_simple_length_signature 49856

OQS_SIG *OQS_SIG_sphincs_haraka_256f_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256f_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_256s_robust
#define OQS_SIG_sphincs_haraka_256s_robust_length_public_key 64
#define OQS_SIG_sphincs_haraka_256s_robust_length_secret_key 128
#define OQS_SIG_sphincs_haraka_256s_robust_length_signature 29792

OQS_SIG *OQS_SIG_sphincs_haraka_256s_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256s_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_haraka_256s_simple
#define OQS_SIG_sphincs_haraka_256s_simple_length_public_key 64
#define OQS_SIG_sphincs_haraka_256s_simple_length_secret_key 128
#define OQS_SIG_sphincs_haraka_256s_simple_length_signature 29792

OQS_SIG *OQS_SIG_sphincs_haraka_256s_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256s_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_haraka_256s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_128f_robust
#define OQS_SIG_sphincs_sha256_128f_robust_length_public_key 32
#define OQS_SIG_sphincs_sha256_128f_robust_length_secret_key 64
#define OQS_SIG_sphincs_sha256_128f_robust_length_signature 17088

OQS_SIG *OQS_SIG_sphincs_sha256_128f_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128f_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_128f_simple
#define OQS_SIG_sphincs_sha256_128f_simple_length_public_key 32
#define OQS_SIG_sphincs_sha256_128f_simple_length_secret_key 64
#define OQS_SIG_sphincs_sha256_128f_simple_length_signature 17088

OQS_SIG *OQS_SIG_sphincs_sha256_128f_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128f_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_128s_robust
#define OQS_SIG_sphincs_sha256_128s_robust_length_public_key 32
#define OQS_SIG_sphincs_sha256_128s_robust_length_secret_key 64
#define OQS_SIG_sphincs_sha256_128s_robust_length_signature 7856

OQS_SIG *OQS_SIG_sphincs_sha256_128s_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128s_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_128s_simple
#define OQS_SIG_sphincs_sha256_128s_simple_length_public_key 32
#define OQS_SIG_sphincs_sha256_128s_simple_length_secret_key 64
#define OQS_SIG_sphincs_sha256_128s_simple_length_signature 7856

OQS_SIG *OQS_SIG_sphincs_sha256_128s_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128s_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_128s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_192f_robust
#define OQS_SIG_sphincs_sha256_192f_robust_length_public_key 48
#define OQS_SIG_sphincs_sha256_192f_robust_length_secret_key 96
#define OQS_SIG_sphincs_sha256_192f_robust_length_signature 35664

OQS_SIG *OQS_SIG_sphincs_sha256_192f_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192f_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_192f_simple
#define OQS_SIG_sphincs_sha256_192f_simple_length_public_key 48
#define OQS_SIG_sphincs_sha256_192f_simple_length_secret_key 96
#define OQS_SIG_sphincs_sha256_192f_simple_length_signature 35664

OQS_SIG *OQS_SIG_sphincs_sha256_192f_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192f_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_192s_robust
#define OQS_SIG_sphincs_sha256_192s_robust_length_public_key 48
#define OQS_SIG_sphincs_sha256_192s_robust_length_secret_key 96
#define OQS_SIG_sphincs_sha256_192s_robust_length_signature 16224

OQS_SIG *OQS_SIG_sphincs_sha256_192s_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192s_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_192s_simple
#define OQS_SIG_sphincs_sha256_192s_simple_length_public_key 48
#define OQS_SIG_sphincs_sha256_192s_simple_length_secret_key 96
#define OQS_SIG_sphincs_sha256_192s_simple_length_signature 16224

OQS_SIG *OQS_SIG_sphincs_sha256_192s_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192s_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_192s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_256f_robust
#define OQS_SIG_sphincs_sha256_256f_robust_length_public_key 64
#define OQS_SIG_sphincs_sha256_256f_robust_length_secret_key 128
#define OQS_SIG_sphincs_sha256_256f_robust_length_signature 49856

OQS_SIG *OQS_SIG_sphincs_sha256_256f_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256f_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_256f_simple
#define OQS_SIG_sphincs_sha256_256f_simple_length_public_key 64
#define OQS_SIG_sphincs_sha256_256f_simple_length_secret_key 128
#define OQS_SIG_sphincs_sha256_256f_simple_length_signature 49856

OQS_SIG *OQS_SIG_sphincs_sha256_256f_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256f_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_256s_robust
#define OQS_SIG_sphincs_sha256_256s_robust_length_public_key 64
#define OQS_SIG_sphincs_sha256_256s_robust_length_secret_key 128
#define OQS_SIG_sphincs_sha256_256s_robust_length_signature 29792

OQS_SIG *OQS_SIG_sphincs_sha256_256s_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256s_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_sha256_256s_simple
#define OQS_SIG_sphincs_sha256_256s_simple_length_public_key 64
#define OQS_SIG_sphincs_sha256_256s_simple_length_secret_key 128
#define OQS_SIG_sphincs_sha256_256s_simple_length_signature 29792

OQS_SIG *OQS_SIG_sphincs_sha256_256s_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256s_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_sha256_256s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_128f_robust
#define OQS_SIG_sphincs_shake256_128f_robust_length_public_key 32
#define OQS_SIG_sphincs_shake256_128f_robust_length_secret_key 64
#define OQS_SIG_sphincs_shake256_128f_robust_length_signature 17088

OQS_SIG *OQS_SIG_sphincs_shake256_128f_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128f_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_128f_simple
#define OQS_SIG_sphincs_shake256_128f_simple_length_public_key 32
#define OQS_SIG_sphincs_shake256_128f_simple_length_secret_key 64
#define OQS_SIG_sphincs_shake256_128f_simple_length_signature 17088

OQS_SIG *OQS_SIG_sphincs_shake256_128f_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128f_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_128s_robust
#define OQS_SIG_sphincs_shake256_128s_robust_length_public_key 32
#define OQS_SIG_sphincs_shake256_128s_robust_length_secret_key 64
#define OQS_SIG_sphincs_shake256_128s_robust_length_signature 7856

OQS_SIG *OQS_SIG_sphincs_shake256_128s_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128s_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_128s_simple
#define OQS_SIG_sphincs_shake256_128s_simple_length_public_key 32
#define OQS_SIG_sphincs_shake256_128s_simple_length_secret_key 64
#define OQS_SIG_sphincs_shake256_128s_simple_length_signature 7856

OQS_SIG *OQS_SIG_sphincs_shake256_128s_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128s_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_128s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_192f_robust
#define OQS_SIG_sphincs_shake256_192f_robust_length_public_key 48
#define OQS_SIG_sphincs_shake256_192f_robust_length_secret_key 96
#define OQS_SIG_sphincs_shake256_192f_robust_length_signature 35664

OQS_SIG *OQS_SIG_sphincs_shake256_192f_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192f_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_192f_simple
#define OQS_SIG_sphincs_shake256_192f_simple_length_public_key 48
#define OQS_SIG_sphincs_shake256_192f_simple_length_secret_key 96
#define OQS_SIG_sphincs_shake256_192f_simple_length_signature 35664

OQS_SIG *OQS_SIG_sphincs_shake256_192f_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192f_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_192s_robust
#define OQS_SIG_sphincs_shake256_192s_robust_length_public_key 48
#define OQS_SIG_sphincs_shake256_192s_robust_length_secret_key 96
#define OQS_SIG_sphincs_shake256_192s_robust_length_signature 16224

OQS_SIG *OQS_SIG_sphincs_shake256_192s_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192s_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_192s_simple
#define OQS_SIG_sphincs_shake256_192s_simple_length_public_key 48
#define OQS_SIG_sphincs_shake256_192s_simple_length_secret_key 96
#define OQS_SIG_sphincs_shake256_192s_simple_length_signature 16224

OQS_SIG *OQS_SIG_sphincs_shake256_192s_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192s_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_192s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_256f_robust
#define OQS_SIG_sphincs_shake256_256f_robust_length_public_key 64
#define OQS_SIG_sphincs_shake256_256f_robust_length_secret_key 128
#define OQS_SIG_sphincs_shake256_256f_robust_length_signature 49856

OQS_SIG *OQS_SIG_sphincs_shake256_256f_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256f_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256f_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256f_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_256f_simple
#define OQS_SIG_sphincs_shake256_256f_simple_length_public_key 64
#define OQS_SIG_sphincs_shake256_256f_simple_length_secret_key 128
#define OQS_SIG_sphincs_shake256_256f_simple_length_signature 49856

OQS_SIG *OQS_SIG_sphincs_shake256_256f_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256f_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256f_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256f_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_256s_robust
#define OQS_SIG_sphincs_shake256_256s_robust_length_public_key 64
#define OQS_SIG_sphincs_shake256_256s_robust_length_secret_key 128
#define OQS_SIG_sphincs_shake256_256s_robust_length_signature 29792

OQS_SIG *OQS_SIG_sphincs_shake256_256s_robust_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256s_robust_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256s_robust_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256s_robust_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#ifdef OQS_ENABLE_SIG_sphincs_shake256_256s_simple
#define OQS_SIG_sphincs_shake256_256s_simple_length_public_key 64
#define OQS_SIG_sphincs_shake256_256s_simple_length_secret_key 128
#define OQS_SIG_sphincs_shake256_256s_simple_length_signature 29792

OQS_SIG *OQS_SIG_sphincs_shake256_256s_simple_new(void);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256s_simple_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256s_simple_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_sphincs_shake256_256s_simple_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
#endif

#endif
