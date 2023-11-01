// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_STFL_LMS_H
#define OQS_SIG_STFL_LMS_H

#include <oqs/oqs.h>

//OQS LMS parameter identifiers
/* Defined LM parameter sets */
#define OQS_LMS_ID_sha256_n32_h5_w1 0x1 //"5/1"
#define OQS_LMS_ID_sha256_n32_h5_w2 0x2 //"5/2"
#define OQS_LMS_ID_sha256_n32_h5_w4 0x3 //"5/4"
#define OQS_LMS_ID_sha256_n32_h5_w8 0x4 //"5/8"

#define OQS_LMS_ID_sha256_n32_h10_w1 0x5 //"10/1"
#define OQS_LMS_ID_sha256_n32_h10_w2 0x7 //"10/2"
#define OQS_LMS_ID_sha256_n32_h10_w4 0x8 //"10/4"
#define OQS_LMS_ID_sha256_n32_h10_w8 0x9 //"10/8"

#define OQS_LMS_ID_sha256_n32_h15_w1 0xa //"15/1"
#define OQS_LMS_ID_sha256_n32_h15_w2 0xb //"15/2"
#define OQS_LMS_ID_sha256_n32_h15_w4 0xc//"15/4"
#define OQS_LMS_ID_sha256_n32_h15_w8 0xd //"15/8"

#define OQS_LMS_ID_sha256_n32_h20_w1 0xe //"20/1"
#define OQS_LMS_ID_sha256_n32_h20_w2 0xf //"20/2"
#define OQS_LMS_ID_sha256_n32_h20_w4 0x10 //"20/4"
#define OQS_LMS_ID_sha256_n32_h20_w8 0x11 //"20/8"

#define OQS_LMS_ID_sha256_n32_h25_w1 0x12 //"25/1"
#define OQS_LMS_ID_sha256_n32_h25_w2 0x13 //"25/2"
#define OQS_LMS_ID_sha256_n32_h25_w4 0x14 //"25/4"
#define OQS_LMS_ID_sha256_n32_h25_w8 0x15 //"25/8"

//H5
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_signature 8688
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_sk 64
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w1_new(void);
OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W1_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h5_w2_length_signature 4464
#define OQS_SIG_STFL_alg_lms_sha256_h5_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w2_length_sk 64
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w2_new(void);
OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W2_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h5_w4_length_signature 2352
#define OQS_SIG_STFL_alg_lms_sha256_h5_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w4_length_sk 64
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w4_new(void);
OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W4_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

#define OQS_SIG_STFL_alg_lms_sha256_h5_w8_length_signature 1296
#define OQS_SIG_STFL_alg_lms_sha256_h5_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w8_length_sk 64
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w8_new(void);
OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W8_new(void);
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

//H10
//  H10           W1            60           8848            64
//  H10           W2            60           4624            64
//  H10           W4            60           2512            64
//  H10           W8            60           1456            64
#define OQS_SIG_STFL_alg_lms_sha256_h10_w1_length_signature 8848
#define OQS_SIG_STFL_alg_lms_sha256_h10_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h10_w1_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_W1_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w1_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h10_w2_length_signature 4624
#define OQS_SIG_STFL_alg_lms_sha256_h10_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h10_w2_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_W2_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w2_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h10_w4_length_signature 2512
#define OQS_SIG_STFL_alg_lms_sha256_h10_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h10_w4_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_W4_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w4_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h10_w8_length_signature 1456
#define OQS_SIG_STFL_alg_lms_sha256_h10_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h10_w8_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_W8_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w8_new(void);

//H15
//  H15           W1            60           9008            64
//  H15           W2            60           4784            64
//  H15           W4            60           2672            64
//  H15           W8            60           1616            64
#define OQS_SIG_STFL_alg_lms_sha256_h15_w1_length_signature 9008
#define OQS_SIG_STFL_alg_lms_sha256_h15_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h15_w1_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_W1_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w1_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h15_w2_length_signature 4784
#define OQS_SIG_STFL_alg_lms_sha256_h15_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h15_w2_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_W2_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w2_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h15_w4_length_signature 2672
#define OQS_SIG_STFL_alg_lms_sha256_h15_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h15_w4_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_W4_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w4_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h15_w8_length_signature 1616
#define OQS_SIG_STFL_alg_lms_sha256_h15_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h15_w8_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_W8_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w8_new(void);

//H20
//  H20           W1            60           9168            64
//  H20           W2            60           4944            64
//  H20           W4            60           2832            64
//  H20           W8            60           1776            64
#define OQS_SIG_STFL_alg_lms_sha256_h20_w1_length_signature 9168
#define OQS_SIG_STFL_alg_lms_sha256_h20_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h20_w1_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_W1_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w1_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h20_w2_length_signature 4944
#define OQS_SIG_STFL_alg_lms_sha256_h20_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h20_w2_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_W2_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w2_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h20_w4_length_signature 2832
#define OQS_SIG_STFL_alg_lms_sha256_h20_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h20_w4_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_W4_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w4_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h20_w8_length_signature 1776
#define OQS_SIG_STFL_alg_lms_sha256_h20_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h20_w8_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_W8_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w8_new(void);

//H25
//  H25           W1            60           9328            64
//  H25           W2            60           5104            64
//  H25           W4            60           2992            64
//  H25           W8            60           1936            64
#define OQS_SIG_STFL_alg_lms_sha256_h25_w1_length_signature 9328
#define OQS_SIG_STFL_alg_lms_sha256_h25_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h25_w1_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_W1_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w1_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h25_w2_length_signature 5104
#define OQS_SIG_STFL_alg_lms_sha256_h25_w2_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h25_w2_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_W2_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w2_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h25_w4_length_signature 2992
#define OQS_SIG_STFL_alg_lms_sha256_h25_w4_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h25_w4_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_W4_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w4_new(void);

#define OQS_SIG_STFL_alg_lms_sha256_h25_w8_length_signature 1936
#define OQS_SIG_STFL_alg_lms_sha256_h25_w8_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h25_w8_length_sk 64

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_W8_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w8_new(void);

OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_left(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_total(unsigned long long *totaln, const OQS_SIG_STFL_SECRET_KEY *secret_key);

void OQS_SECRET_KEY_LMS_free(OQS_SIG_STFL_SECRET_KEY *sk);

// ----------------------------------- WRAPPER FUNCTIONS ------------------------------------------------
int oqs_sig_stfl_lms_keypair(uint8_t *pk, OQS_SIG_STFL_SECRET_KEY *sk, const uint32_t oid);

int oqs_sig_stfl_lms_sign(OQS_SIG_STFL_SECRET_KEY *sk, uint8_t *sm, size_t *smlen,
                          const uint8_t *m, size_t mlen);

int oqs_sig_stfl_lms_verify(const uint8_t *m, size_t mlen, const uint8_t *sm, size_t smlen,
                            const uint8_t *pk);

void oqs_secret_lms_key_free(OQS_SIG_STFL_SECRET_KEY *sk);

OQS_STATUS oqs_serialize_lms_key(uint8_t **sk_key, size_t *sk_len, const OQS_SIG_STFL_SECRET_KEY *sk);
OQS_STATUS oqs_deserialize_lms_key(OQS_SIG_STFL_SECRET_KEY *sk, const size_t sk_len, const uint8_t *sk_buf, void *context);
void oqs_lms_key_set_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context);

// ---------------------------- FUNCTIONS INDEPENDENT OF VARIANT -----------------------------------------

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

// --------------------------------------------------------------------------------------------------------

#endif /* OQS_SIG_STFL_LMS_H */
