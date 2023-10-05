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
#define OQS_LMS_ID_lms_sha256_n32_h10_w2 0x7 //"10/2"
#define OQS_LMS_ID_lms_sha256_n32_h10_w4 0x8 //"10/4"
#define OQS_LMS_ID_lms_sha256_n32_h10_w8 0x9 //"10/8"

#define OQS_LMS_ID_lms_sha256_n32_h15_w1 0xa //"15/1"
#define OQS_LMS_ID_lms_sha256_n32_h15_w2 0xb //"15/2"
#define OQS_LMS_ID_lms_sha256_n32_h15_w4 0xc//"15/4"
#define OQS_LMS_ID_lms_sha256_n32_h15_w8 0xd //"15/8"

#define OQS_LMS_ID_lms_sha256_n32_h20_w1 0xe //"20/1"
#define OQS_LMS_ID_lms_sha256_n32_h20_w2 0xf //"20/2"
#define OQS_LMS_ID_lms_sha256_n32_h20_w4 0x10 //"20/4"
#define OQS_LMS_ID_lms_sha256_n32_h20_w8 0x11 //"20/8"

#define OQS_LMS_ID_lms_sha256_n32_h25_w1 0x12 //"25/1"
#define OQS_LMS_ID_lms_sha256_n32_h25_w2 0x13 //"25/2"
#define OQS_LMS_ID_lms_sha256_n32_h25_w4 0x14 //"25/4"
#define OQS_LMS_ID_lms_sha256_n32_h25_w8 0x15 //"25/8"


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

OQS_STATUS oqs_serialize_lms_key(const OQS_SIG_STFL_SECRET_KEY *sk, size_t *sk_len, uint8_t **sk_key);
OQS_STATUS oqs_deserialize_lms_key(OQS_SIG_STFL_SECRET_KEY *sk, const size_t sk_len, const uint8_t *sk_buf, void *context);
void oqs_lms_key_set_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context);

// ---------------------------- FUNCTIONS INDEPENDENT OF VARIANT -----------------------------------------

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

// --------------------------------------------------------------------------------------------------------

#endif /* OQS_SIG_STFL_LMS_H */
