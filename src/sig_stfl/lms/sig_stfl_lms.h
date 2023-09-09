// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_STFL_LMS_H
#define OQS_SIG_STFL_LMS_H

#include <oqs/oqs.h>

//H5
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_signature 8688
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_sk 64

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W1_new(void);

/* Convert LMS secret key object to byte string */
OQS_STATUS OQS_SECRET_KEY_LMS_serialize_key(const OQS_SIG_STFL_SECRET_KEY *sk, size_t *sk_len, uint8_t **sk_buf_ptr);

/* Insert lms byte string in an LMS secret key object */
OQS_STATUS OQS_SECRET_KEY_LMS_deserialize_key(OQS_SIG_STFL_SECRET_KEY *sk, const size_t key_len, const uint8_t *sk_buf);

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w1_new(void);

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
OQS_STATUS oqs_deserialize_lms_key(OQS_SIG_STFL_SECRET_KEY *sk, const size_t sk_len, const uint8_t *sk_buf);

// ---------------------------- FUNCTIONS INDEPENDENT OF VARIANT -----------------------------------------

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key);

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);

// --------------------------------------------------------------------------------------------------------

#endif /* OQS_SIG_STFL_LMS_H */
