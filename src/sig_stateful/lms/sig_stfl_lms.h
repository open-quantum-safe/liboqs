// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_STFL_LMS_LMSMT_H
#define OQS_SIG_STFL_LMS_LMSMT_H

#include <oqs/oqs.h>

#define LMS_OID_LEN 4

//#ifdef OQS_ENABLE_SIG_STFL_LMS

//H5
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_signature 8688
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W1_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w1_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h5_w2_length_signature 4464
#define OQS_SIG_STFL_alg_lms_sha256_h5_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w2_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_w2_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w2_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h5_w4_length_signature 2352
#define OQS_SIG_STFL_alg_lms_sha256_h5_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w4_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_w4_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w4_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h5_w8_length_signature 1296
#define OQS_SIG_STFL_alg_lms_sha256_h5_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w8_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_w8_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w8_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w8_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

//H10
//  H10           W1            60           8848            64
//  H10           W2            60           4624            64
//  H10           W4            60           2512            64
//  H10           W8            60           1456            64
#define OQS_SIG_STFL_alg_lms_sha256_h10_w1_length_signature 8848
#define OQS_SIG_STFL_alg_lms_sha256_h10_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h10_w1_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_w1_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w1_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w1_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h10_w2_length_signature 4624
#define OQS_SIG_STFL_alg_lms_sha256_h10_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h10_w2_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_w2_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w2_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h10_w4_length_signature 2512
#define OQS_SIG_STFL_alg_lms_sha256_h10_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h10_w4_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_w4_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w4_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h10_w8_length_signature 1456
#define OQS_SIG_STFL_alg_lms_sha256_h10_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h10_w8_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_w8_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w8_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w8_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

//H15
//  H15           W1            60           9008            64
//  H15           W2            60           4784            64
//  H15           W4            60           2672            64
//  H15           W8            60           1616            64
#define OQS_SIG_STFL_alg_lms_sha256_h15_w1_length_signature 9008
#define OQS_SIG_STFL_alg_lms_sha256_h15_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h15_w1_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_w1_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w1_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w1_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h15_w2_length_signature 4784
#define OQS_SIG_STFL_alg_lms_sha256_h15_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h15_w2_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_w2_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w2_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h15_w4_length_signature 2672
#define OQS_SIG_STFL_alg_lms_sha256_h15_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h15_w4_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_w4_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w4_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h15_w8_length_signature 1616
#define OQS_SIG_STFL_alg_lms_sha256_h15_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h15_w8_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_w8_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w8_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w8_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

//H20
//  H20           W1            60           9168            64
//  H20           W2            60           4944            64
//  H20           W4            60           2832            64
//  H20           W8            60           1776            64
#define OQS_SIG_STFL_alg_lms_sha256_h20_w1_length_signature 9168
#define OQS_SIG_STFL_alg_lms_sha256_h20_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h20_w1_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_w1_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w1_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w1_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h20_w2_length_signature 4944
#define OQS_SIG_STFL_alg_lms_sha256_h20_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h20_w2_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_w2_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w2_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h20_w4_length_signature 2832
#define OQS_SIG_STFL_alg_lms_sha256_h20_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h20_w4_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_w4_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w4_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h20_w8_length_signature 1776
#define OQS_SIG_STFL_alg_lms_sha256_h20_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h20_w8_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_w8_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w8_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w8_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

//H25
//  H25           W1            60           9328            64
//  H25           W2            60           5104            64
//  H25           W4            60           2992            64
//  H25           W8            60           1936            64
#define OQS_SIG_STFL_alg_lms_sha256_h25_w1_length_signature 9328
#define OQS_SIG_STFL_alg_lms_sha256_h25_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h25_w1_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_w1_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w1_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h25_w1_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h25_w2_length_signature 5104
#define OQS_SIG_STFL_alg_lms_sha256_h25_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h25_w2_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_w2_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h25_w2_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h25_w4_length_signature 2992
#define OQS_SIG_STFL_alg_lms_sha256_h25_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h25_w4_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_w4_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h25_w4_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h25_w8_length_signature 1936
#define OQS_SIG_STFL_alg_lms_sha256_h25_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h25_w8_length_sk 64

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_w8_new(void);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w8_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h25_w8_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key);


// ----------------------------------- WRAPPER FUNCTIONS ------------------------------------------------
int oqs_sig_stfl_lms_keypair(uint8_t *pk, OQS_SECRET_KEY *sk, const uint32_t oid);


// ---------------------------- FUNCTIONS INDEPENDENT OF VARIANT -----------------------------------------

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key);

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

unsigned long long OQS_SECRET_KEY_lms_sigs_left(const OQS_SECRET_KEY *secret_key);

unsigned long long OQS_SECRET_KEY_lms_sigs_total(const OQS_SECRET_KEY *secret_key);


OQS_SECRET_KEY *OQS_SIG_STFL_alg_lms_derive_subkey(OQS_SECRET_KEY *master_key, const unsigned long long number_of_sigs);

// --------------------------------------------------------------------------------------------------------
//#endif //OQS_ENABLE_SIG_STFL_LMS
#endif /* OQS_SIG_STFL_LMS_H */
