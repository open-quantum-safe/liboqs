// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_STFL_XMSS_XMSSMT_H
#define OQS_SIG_STFL_XMSS_XMSSMT_H

#include <oqs/oqs.h>

#define XMSS_OID_LEN 4

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H10

#define OQS_SIG_STFL_alg_xmss_sha256_h10_length_signature 2500
#define OQS_SIG_STFL_alg_xmss_sha256_h10_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_sha256_h10_length_sk 2047 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H10_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h10_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h10_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H16

#define OQS_SIG_STFL_alg_xmss_sha256_h16_length_signature 2692
#define OQS_SIG_STFL_alg_xmss_sha256_h16_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_sha256_h16_length_sk 3151 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H16_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h16_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H20

#define OQS_SIG_STFL_alg_xmss_sha256_h20_length_signature 2820
#define OQS_SIG_STFL_alg_xmss_sha256_h20_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_sha256_h20_length_sk 3889 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H20_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H10

#define OQS_SIG_STFL_alg_xmss_sha512_h10_length_signature 9092
#define OQS_SIG_STFL_alg_xmss_sha512_h10_length_pk 128 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_sha512_h10_length_sk 3999 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA512_H10_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha512_h10_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h10_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H16

#define OQS_SIG_STFL_alg_xmss_sha512_h16_length_signature 9476
#define OQS_SIG_STFL_alg_xmss_sha512_h16_length_pk 128 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_sha512_h16_length_sk 6159 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA512_H16_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha512_h16_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h16_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H20

#define OQS_SIG_STFL_alg_xmss_sha512_h20_length_signature 9732
#define OQS_SIG_STFL_alg_xmss_sha512_h20_length_pk 128 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_sha512_h20_length_sk 7601 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA512_H20_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha512_h20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha512_h20_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H10

#define OQS_SIG_STFL_alg_xmss_shake256_h10_length_signature 2500
#define OQS_SIG_STFL_alg_xmss_shake256_h10_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_shake256_h10_length_sk 2047 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H10_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h10_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h10_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H16

#define OQS_SIG_STFL_alg_xmss_shake256_h16_length_signature 2692
#define OQS_SIG_STFL_alg_xmss_shake256_h16_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_shake256_h16_length_sk 3151 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H16_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h16_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h16_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H20

#define OQS_SIG_STFL_alg_xmss_shake256_h20_length_signature 2820
#define OQS_SIG_STFL_alg_xmss_shake256_h20_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_shake256_h20_length_sk 3889 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE256_H20_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake256_h20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake256_h20_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H10

#define OQS_SIG_STFL_alg_xmss_shake512_h10_length_signature 9092
#define OQS_SIG_STFL_alg_xmss_shake512_h10_length_pk 128 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_shake512_h10_length_sk 3999 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE512_H10_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake512_h10_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake512_h10_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H16

#define OQS_SIG_STFL_alg_xmss_shake512_h16_length_signature 9476
#define OQS_SIG_STFL_alg_xmss_shake512_h16_length_pk 128 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_shake512_h16_length_sk 6159 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE512_H16_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake512_h16_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake512_h16_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif

#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H20

#define OQS_SIG_STFL_alg_xmss_shake512_h20_length_signature 9732
#define OQS_SIG_STFL_alg_xmss_shake512_h20_length_pk 128 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmss_shake512_h20_length_sk 7601 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHAKE512_H20_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake512_h20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_shake512_h20_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif


#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_2_H20

#define OQS_SIG_STFL_alg_xmssmt_sha256_2_h20_length_signature 4963
#define OQS_SIG_STFL_alg_xmssmt_sha256_2_h20_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmssmt_sha256_2_h20_length_sk 8082 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_2_H20_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_2_h20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_2_h20_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif


#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_4_H20

#define OQS_SIG_STFL_alg_xmssmt_sha256_4_h20_length_signature 9251
#define OQS_SIG_STFL_alg_xmssmt_sha256_4_h20_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmssmt_sha256_4_h20_length_sk 13598 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_4_H20_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_4_h20_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_4_h20_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif


#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_2_H40

#define OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_length_signature 5605
#define OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_length_sk 13608 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_2_H40_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif


#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_4_H40

#define OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_length_signature 9893
#define OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_length_sk 20156 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_4_H40_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif


#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_8_H40

#define OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_length_signature 18469
#define OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_length_sk 30252 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_8_H40_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif


#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_3_H60

#define OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_length_signature 8392
#define OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_length_sk 23325 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_3_H60_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif


#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_6_H60

#define OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_length_signature 14824
#define OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_length_sk 32227 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_6_H60_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif


#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_12_H60

#define OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_length_signature 27688
#define OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_length_pk 64 + XMSS_OID_LEN
#define OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_length_sk 46903 + XMSS_OID_LEN

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSSMT_SHA256_12_H60_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);
#endif


/* ===
    ADD OTHER XMSS^MT VARIANTS HERE
    ===
*/


// ---------------------------- FUNCTIONS INDEPENDENT OF VARIANT -----------------------------------------

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key);

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key);

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

unsigned long long OQS_SIG_STFL_alg_xmss_xmssmt_sigs_left(const OQS_SECRET_KEY *secret_key);

unsigned long long OQS_SIG_STFL_alg_xmss_xmssmt_sigs_total(const OQS_SECRET_KEY *secret_key);

void perform_key_allocation(OQS_SECRET_KEY *sk);

OQS_SECRET_KEY *OQS_SIG_STFL_alg_xmss_derive_subkey(OQS_SECRET_KEY *master_key, const unsigned long long number_of_sigs);

OQS_SECRET_KEY *OQS_SIG_STFL_alg_xmssmt_derive_subkey(OQS_SECRET_KEY *master_key, const unsigned long long number_of_sigs);

// --------------------------------------------------------------------------------------------------------

#endif /* OQS_SIG_STFL_XMSS_H */