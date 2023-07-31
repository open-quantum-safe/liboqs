// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_STFL_LMS_H
#define OQS_SIG_STFL_LMS_H

#include <oqs/oqs.h>

//H5
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_signature 8688
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_pk 60
#define OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_sk 64

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, uint8_t *secret_key);

OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W1_new(void);
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w1_new(void);

OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_left(unsigned long long *remain, const uint8_t *secret_key);
OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_total(uint64_t *totaln, const uint8_t *secret_key);

void OQS_SECRET_KEY_LMS_free(OQS_SIG_STFL_SECRET_KEY *sk);

// ----------------------------------- WRAPPER FUNCTIONS ------------------------------------------------
int oqs_sig_stfl_lms_keypair(uint8_t *pk, uint8_t *sk, const uint32_t oid);

int oqs_sig_stfl_lms_sign(uint8_t *sk, uint8_t *sm, size_t *smlen,
                          const uint8_t *m, size_t mlen);

int oqs_sig_stfl_lms_verify(const uint8_t *m, size_t mlen, const uint8_t *sm, size_t smlen,
                            const uint8_t *pk);

// ---------------------------- FUNCTIONS INDEPENDENT OF VARIANT -----------------------------------------

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, uint8_t *secret_key);

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key);


// --------------------------------------------------------------------------------------------------------

#endif /* OQS_SIG_STFL_LMS_H */
