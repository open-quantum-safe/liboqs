// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0

#ifndef OQS_SIG_STFL_XMSS_H
#define OQS_SIG_STFL_XMSS_H

#include <oqs/oqs.h>
#if defined(__GNUC__) || defined(__clang__)
#define XMSS_UNUSED_ATT __attribute__((unused))
#else
#define XMSS_UNUSED_ATT
#endif

#define XMSS_OID_LEN 4

/*
 * | Algorithms                    | oid  | sk (b) | pk (b) | sig (b) | n  | RFC8391 | NIST SP 800-208 | CNSA 2.0 |
 * |-------------------------------|------|--------|--------|---------|----| ------- | --------------- | -------- |
 * | XMSS-SHA2_10_256              | 0x01 |  1373  |   64   |  2500   | 32 |    ✅   |       ✅        |    ✅    |
 * | XMSS-SHA2_16_256              | 0x02 |  2093  |   64   |  2692   | 32 |    ✅   |       ✅        |    ✅    |
 * | XMSS-SHA2_20_256              | 0x03 |  2573  |   64   |  2820   | 32 |    ✅   |       ✅        |    ✅    |
 *
 * | XMSS-SHAKE_10_256             | 0x07 |  1373  |   64   |  2500   | 32 |    ✅   |       ❌        |    ❌    |
 * | XMSS-SHAKE_16_256             | 0x08 |  2093  |   64   |  2692   | 32 |    ✅   |       ❌        |    ❌    |
 * | XMSS-SHAKE_20_256             | 0x09 |  2573  |   64   |  2820   | 32 |    ✅   |       ❌        |    ❌    |
 *
 * | XMSS-SHA2_10_512              | 0x04 |  2653  |  128   |  9092   | 64 |    ✅   |       ❌        |    ❌    |
 * | XMSS-SHA2_16_512              | 0x05 |  4045  |  128   |  9476   | 64 |    ✅   |       ❌        |    ❌    |
 * | XMSS-SHA2_20_512              | 0x06 |  4973  |  128   |  9732   | 64 |    ✅   |       ❌        |    ❌    |
 *
 * | XMSS-SHAKE_10_512             | 0x0a |  2653  |  128   |  9092   | 64 |    ✅   |       ❌        |    ❌    |
 * | XMSS-SHAKE_16_512             | 0x0b |  4045  |  128   |  9476   | 64 |    ✅   |       ❌        |    ❌    |
 * | XMSS-SHAKE_20_512             | 0x0c |  4973  |  128   |  9732   | 64 |    ✅   |       ❌        |    ❌    |
 *
 * | XMSS-SHA2_10_192              | 0x0d |  1053  |   48   |  1492   | 24 |    ❌   |       ✅        |    ✅    |
 * | XMSS-SHA2_16_192              | 0x0e |  1605  |   48   |  1636   | 24 |    ❌   |       ✅        |    ✅    |
 * | XMSS-SHA2_20_192              | 0x0f |  1973  |   48   |  1732   | 24 |    ❌   |       ✅        |    ✅    |
 *
 * | XMSS-SHAKE256_10_256          | 0x10 |  1373  |   64   |  2500   | 32 |    ❌   |       ✅        |    ✅    |
 * | XMSS-SHAKE256_16_256          | 0x11 |  2093  |   64   |  2692   | 32 |    ❌   |       ✅        |    ✅    |
 * | XMSS-SHAKE256_20_256          | 0x12 |  2573  |   64   |  2820   | 32 |    ❌   |       ✅        |    ✅    |
 *
 * | XMSS-SHAKE256_10_192          | 0x13 |  1053  |   48   |  1492   | 24 |    ❌   |       ✅        |    ✅    |
 * | XMSS-SHAKE256_16_192          | 0x14 |  1605  |   48   |  1636   | 24 |    ❌   |       ✅        |    ✅    |
 * | XMSS-SHAKE256_20_192          | 0x15 |  1973  |   48   |  1732   | 24 |    ❌   |       ✅        |    ✅    |
 *
 * | XMSSMT-SHA2_20/2_256          | 0x01 |  5998  |   64   |  4963   | 32 |    ✅   |       ✅        |    ❌    |
 * | XMSSMT-SHA2_20/4_256          | 0x02 | 10938  |   64   |  9251   | 32 |    ✅   |       ✅        |    ❌    |
 * | XMSSMT-SHA2_40/2_256          | 0x03 |  9600  |   64   |  5605   | 32 |    ✅   |       ✅        |    ❌    |
 * | XMSSMT-SHA2_40/4_256          | 0x04 | 15252  |   64   |  9893   | 32 |    ✅   |       ✅        |    ❌    |
 * | XMSSMT-SHA2_40/8_256          | 0x05 | 24516  |   64   | 18469   | 32 |    ✅   |       ✅        |    ❌    |
 * | XMSSMT-SHA2_60/3_256          | 0x06 | 16629  |   64   |  8392   | 32 |    ✅   |       ✅        |    ❌    |
 * | XMSSMT-SHA2_60/6_256          | 0x07 | 24507  |   64   | 14824   | 32 |    ✅   |       ✅        |    ❌    |
 * | XMSSMT-SHA2_60/12_256         | 0x08 | 38095  |   64   | 27688   | 32 |    ✅   |       ✅        |    ❌    |
 *
 * | XMSSMT-SHAKE_20/2_256         | 0x11 |  5998  |   64   |  4963   | 32 |    ✅   |       ❌        |    ❌    |
 * | XMSSMT-SHAKE_20/4_256         | 0x12 | 10938  |   64   |  9251   | 32 |    ✅   |       ❌        |    ❌    |
 * | XMSSMT-SHAKE_40/2_256         | 0x13 |  9600  |   64   |  5605   | 32 |    ✅   |       ❌        |    ❌    |
 * | XMSSMT-SHAKE_40/4_256         | 0x14 | 15252  |   64   |  9893   | 32 |    ✅   |       ❌        |    ❌    |
 * | XMSSMT-SHAKE_40/8_256         | 0x15 | 24516  |   64   | 18469   | 32 |    ✅   |       ❌        |    ❌    |
 * | XMSSMT-SHAKE_60/3_256         | 0x16 | 16629  |   64   |  8392   | 32 |    ✅   |       ❌        |    ❌    |
 * | XMSSMT-SHAKE_60/6_256         | 0x17 | 24507  |   64   | 14824   | 32 |    ✅   |       ❌        |    ❌    |
 * | XMSSMT-SHAKE_60/12_256        | 0x18 | 38095  |   64   | 27688   | 32 |    ✅   |       ❌        |    ❌    |
 */

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10

#define OQS_SIG_STFL_alg_xmss_sha256_h10_oid 0x01
#define OQS_SIG_STFL_alg_xmss_sha256_h10_length_sk (1373 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h10_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h10_length_signature 2500

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h10_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H10_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16

#define OQS_SIG_STFL_alg_xmss_sha256_h16_oid 0x02
#define OQS_SIG_STFL_alg_xmss_sha256_h16_length_sk (2093 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h16_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h16_length_signature 2692

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h16_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H16_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20

#define OQS_SIG_STFL_alg_xmss_sha256_h20_oid 0x03
#define OQS_SIG_STFL_alg_xmss_sha256_h20_length_sk (2573 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h20_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h20_length_signature 2820

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h20_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h10

#define OQS_SIG_STFL_alg_xmss_shake128_h10_oid 0x07
#define OQS_SIG_STFL_alg_xmss_shake128_h10_length_sk (1373 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake128_h10_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake128_h10_length_signature 2500

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake128_h10_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE128_H10_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h10_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16

#define OQS_SIG_STFL_alg_xmss_shake128_h16_oid 0x08
#define OQS_SIG_STFL_alg_xmss_shake128_h16_length_sk (2093 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake128_h16_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake128_h16_length_signature 2692

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake128_h16_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE128_H16_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h16_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h16_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h16_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h16_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h16_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20

#define OQS_SIG_STFL_alg_xmss_shake128_h20_oid 0x09
#define OQS_SIG_STFL_alg_xmss_shake128_h20_length_sk (2573 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake128_h20_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake128_h20_length_signature 2820

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake128_h20_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE128_H20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h20_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h20_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h20_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h20_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake128_h20_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h10

#define OQS_SIG_STFL_alg_xmss_sha512_h10_oid 0x04
#define OQS_SIG_STFL_alg_xmss_sha512_h10_length_sk (2653 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha512_h10_length_pk (128 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha512_h10_length_signature 9092

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha512_h10_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA512_H10_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h10_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h10_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h10_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h10_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h10_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16

#define OQS_SIG_STFL_alg_xmss_sha512_h16_oid 0x05
#define OQS_SIG_STFL_alg_xmss_sha512_h16_length_sk (4045 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha512_h16_length_pk (128 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha512_h16_length_signature 9476

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha512_h16_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA512_H16_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h16_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h16_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h16_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h16_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h16_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20

#define OQS_SIG_STFL_alg_xmss_sha512_h20_oid 0x06
#define OQS_SIG_STFL_alg_xmss_sha512_h20_length_sk (4973 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha512_h20_length_pk (128 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha512_h20_length_signature 9732

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha512_h20_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA512_H20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h20_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h20_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h20_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h20_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h20_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10

#define OQS_SIG_STFL_alg_xmss_shake256_h10_oid 0x0a
#define OQS_SIG_STFL_alg_xmss_shake256_h10_length_sk (2653 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h10_length_pk (128 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h10_length_signature 9092

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h10_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H10_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16

#define OQS_SIG_STFL_alg_xmss_shake256_h16_oid 0x0b
#define OQS_SIG_STFL_alg_xmss_shake256_h16_length_sk (4045 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h16_length_pk (128 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h16_length_signature 9476

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h16_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H16_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20

#define OQS_SIG_STFL_alg_xmss_shake256_h20_oid 0x0c
#define OQS_SIG_STFL_alg_xmss_shake256_h20_length_sk (4973 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h20_length_pk (128 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h20_length_signature 9732

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h20_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10_192

#define OQS_SIG_STFL_alg_xmss_sha256_h10_192_oid 0x0d
#define OQS_SIG_STFL_alg_xmss_sha256_h10_192_length_sk (1053 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h10_192_length_pk (48 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h10_192_length_signature 1492

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h10_192_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H10_192_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_192_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_192_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_192_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_192_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_192_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16_192

#define OQS_SIG_STFL_alg_xmss_sha256_h16_192_oid 0x0e
#define OQS_SIG_STFL_alg_xmss_sha256_h16_192_length_sk (1605 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h16_192_length_pk (48 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h16_192_length_signature 1636

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h16_192_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H16_192_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_192_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_192_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_192_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_192_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_192_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20_192

#define OQS_SIG_STFL_alg_xmss_sha256_h20_192_oid 0x0f
#define OQS_SIG_STFL_alg_xmss_sha256_h20_192_length_sk (1973 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h20_192_length_pk (48 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_sha256_h20_192_length_signature 1732

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h20_192_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H20_192_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_192_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_192_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_192_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_192_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_192_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10_192

#define OQS_SIG_STFL_alg_xmss_shake256_h10_192_oid 0x13
#define OQS_SIG_STFL_alg_xmss_shake256_h10_192_length_sk (1053 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h10_192_length_pk (48 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h10_192_length_signature 1492

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h10_192_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H10_192_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_192_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_192_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_192_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_192_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_192_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_192

#define OQS_SIG_STFL_alg_xmss_shake256_h16_192_oid 0x14
#define OQS_SIG_STFL_alg_xmss_shake256_h16_192_length_sk (1605 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h16_192_length_pk (48 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h16_192_length_signature 1636

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h16_192_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H16_192_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_192_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_192_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_192_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_192_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_192_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_192

#define OQS_SIG_STFL_alg_xmss_shake256_h20_192_oid 0x15
#define OQS_SIG_STFL_alg_xmss_shake256_h20_192_length_sk (1973 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h20_192_length_pk (48 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h20_192_length_signature 1732

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h20_192_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H20_192_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_192_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_192_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_192_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_192_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_192_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10_256

#define OQS_SIG_STFL_alg_xmss_shake256_h10_256_oid 0x10
#define OQS_SIG_STFL_alg_xmss_shake256_h10_256_length_sk (1373 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h10_256_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h10_256_length_signature 2500

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h10_256_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H10_256_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_256_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_256_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_256_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_256_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_256_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_256

#define OQS_SIG_STFL_alg_xmss_shake256_h16_256_oid 0x11
#define OQS_SIG_STFL_alg_xmss_shake256_h16_256_length_sk (2093 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h16_256_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h16_256_length_signature 2692

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h16_256_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H16_256_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_256_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_256_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_256_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_256_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_256_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_256

#define OQS_SIG_STFL_alg_xmss_shake256_h20_256_oid 0x12
#define OQS_SIG_STFL_alg_xmss_shake256_h20_256_length_sk (2573 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h20_256_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmss_shake256_h20_256_length_signature 2820

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h20_256_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H20_256_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_256_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_256_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_256_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_256_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_256_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_2

#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_oid 0x01
#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_length_sk (5998 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_length_signature 4963

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_H20_2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_4

#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_oid 0x02
#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_length_sk (10938 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_length_signature 9251

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_H20_4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2

#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_oid 0x03
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_length_sk (9600 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_length_signature 5605

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_H40_2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_4

#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_oid 0x04
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_length_sk (15252 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_length_signature 9893

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_H40_4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_8

#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_oid 0x05
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_length_sk (24516 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_length_signature 18469

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_H40_8_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3

#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_oid 0x06
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_length_sk (16629 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_length_signature 8392

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_H60_3_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_6

#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_oid 0x07
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_length_sk (24507 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_length_signature 14824

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_H60_6_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_12

#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_oid 0x08
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_length_sk (38095 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_length_signature 27688

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_H60_12_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_2

#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_oid 0x11
#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_length_sk (5998 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_length_signature 4963

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHAKE128_H20_2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_4

#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_oid 0x12
#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_length_sk (10938 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_length_signature 9251

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHAKE128_H20_4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2

#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_oid 0x13
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_length_sk (9600 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_length_signature 5605

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_4

#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_oid 0x14
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_length_sk (15252 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_length_signature 9893

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_8

#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_oid 0x15
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_length_sk (24516 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_length_signature 18469

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_8_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3

#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_oid 0x16
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_length_sk (16629 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_length_signature 8392

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHAKE128_H60_3_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_6

#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_oid 0x17
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_length_sk (24507 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_length_signature 14824

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHAKE128_H60_6_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_12

#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_oid 0x18
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_length_sk (38095 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_length_pk (64 + XMSS_OID_LEN)
#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_length_signature 27688

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_new(void);
OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHAKE128_H60_12_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#endif

#define __alg_xmss_XMSS(funcname, postfix) funcname##_##postfix
#define _alg_xmss_XMSS(funcname, postfix) __alg_xmss_XMSS(funcname, postfix)
#define OQS_SIG_STFL_alg_xmss_NAMESPACE(funcname) _alg_xmss_XMSS(funcname, XMSS_PARAMS_NAMESPACE)

/*
 * Generic XMSS APIs
 */
#define OQS_SIG_STFL_alg_xmss_sign OQS_SIG_STFL_alg_xmss_NAMESPACE(OQS_SIG_STFL_alg_xmss_sign)
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sign(uint8_t *signature, size_t *signature_len, XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, XMSS_UNUSED_ATT OQS_SIG_STFL_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_xmss_verify OQS_SIG_STFL_alg_xmss_NAMESPACE(OQS_SIG_STFL_alg_xmss_verify)
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_verify(XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, const uint8_t *signature, size_t signature_len, XMSS_UNUSED_ATT const uint8_t *public_key);

#define OQS_SIG_STFL_alg_xmss_sigs_remaining OQS_SIG_STFL_alg_xmss_NAMESPACE(OQS_SIG_STFL_alg_xmss_sigs_remaining)
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_xmss_sigs_total OQS_SIG_STFL_alg_xmss_NAMESPACE(OQS_SIG_STFL_alg_xmss_sigs_total)
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

/*
 * Generic XMSS^MT APIs
 */
#define OQS_SIG_STFL_alg_xmssmt_sign OQS_SIG_STFL_alg_xmss_NAMESPACE(OQS_SIG_STFL_alg_xmssmt_sign)
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sign(uint8_t *signature, size_t *signature_len, XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, XMSS_UNUSED_ATT OQS_SIG_STFL_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_xmssmt_verify OQS_SIG_STFL_alg_xmss_NAMESPACE(OQS_SIG_STFL_alg_xmssmt_verify)
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_verify(XMSS_UNUSED_ATT const uint8_t *message, XMSS_UNUSED_ATT size_t message_len, const uint8_t *signature, size_t signature_len, XMSS_UNUSED_ATT const uint8_t *public_key);

#define OQS_SIG_STFL_alg_xmssmt_sigs_remaining OQS_SIG_STFL_alg_xmss_NAMESPACE(OQS_SIG_STFL_alg_xmssmt_sigs_remaining)
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_xmssmt_sigs_total OQS_SIG_STFL_alg_xmss_NAMESPACE(OQS_SIG_STFL_alg_xmssmt_sigs_total)
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key);

/*
 * Secret key functions
 */
/* Generic XMSS SECRET_KEY object initialization */
OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS_new(size_t length_secret_key);

/* Serialize XMSS secret key data into a byte string, and return an allocated buffer. Users must deallocate the buffer. */
OQS_STATUS OQS_SECRET_KEY_XMSS_serialize_key(uint8_t **sk_buf_ptr, size_t *sk_len, const OQS_SIG_STFL_SECRET_KEY *sk);

/* Only for internal use. Similar to OQS_SECRET_KEY_XMSS_serialize_key, this function does not acquire and release a lock. */
OQS_STATUS OQS_SECRET_KEY_XMSS_inner_serialize_key(uint8_t **sk_buf_ptr, size_t *sk_len, const OQS_SIG_STFL_SECRET_KEY *sk);

/* Deserialize XMSS byte string into an XMSS secret key data */
OQS_STATUS OQS_SECRET_KEY_XMSS_deserialize_key(OQS_SIG_STFL_SECRET_KEY *sk, const uint8_t *sk_buf, const size_t sk_len, void *context);

/* Store Secret Key Function, ideally written to secure device */
void OQS_SECRET_KEY_XMSS_set_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context);

/* Free Secret key object */
void OQS_SECRET_KEY_XMSS_free(OQS_SIG_STFL_SECRET_KEY *sk);

/* Lock the key if possible */
OQS_STATUS OQS_SECRET_KEY_XMSS_acquire_lock(const OQS_SIG_STFL_SECRET_KEY *sk);

/* Unlock the key if possible */
OQS_STATUS OQS_SECRET_KEY_XMSS_release_lock(const OQS_SIG_STFL_SECRET_KEY *sk);

#endif /* OQS_SIG_STFL_XMSS_H */
