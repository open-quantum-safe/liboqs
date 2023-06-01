// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_STFL_XMSS_H
#define OQS_SIG_STFL_XMSS_H

#include <oqs/oqs.h>

#define XMSS_OID_LEN 4

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10

#define OQS_SIG_STFL_alg_xmss_sha256_h10_length_signature 2500
#define OQS_SIG_STFL_alg_xmss_sha256_h10_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h10_length_sk (132 + XMSS_OID_LEN)

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h10_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sigs_remaining(size_t *remain, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sigs_total(size_t *total, const uint8_t *secret_key);

#endif

#endif /* OQS_SIG_STFL_XMSS_H */
