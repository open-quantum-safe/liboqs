// SPDX-License-Identifier: MIT

#include <string.h>
#include "sig_stfl_lms.h"
#include "external/config.h"



OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key) {
   if (secret_key == NULL || message == NULL || signature == NULL) {
       return OQS_ERROR;
   }

   LMS_UNUSED(signature_length);
   LMS_UNUSED(message_len);

// return OQS_SUCCESS;
   return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
   if (message == NULL || signature == NULL || public_key == NULL) {
       return OQS_ERROR;
   }

     LMS_UNUSED(signature_len);
     LMS_UNUSED(message_len);

// return OQS_SUCCESS;
    return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_hss_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key) {
   if (secret_key == NULL || message == NULL || signature == NULL) {
       return OQS_ERROR;
   }

    LMS_UNUSED(signature_length);
    LMS_UNUSED(message_len);


// return OQS_SUCCESS;
    return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_hss_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
   if (message == NULL || signature == NULL || public_key == NULL) {
       return OQS_ERROR;
   }
    LMS_UNUSED(signature_len);
    LMS_UNUSED(message_len);

// return OQS_SUCCESS;
    return OQS_ERROR;
}

unsigned long long OQS_SECRET_KEY_lms_sigs_left(const OQS_SECRET_KEY *secret_key) {
   if (secret_key == NULL) {
       return -1;
   }

   LMS_UNUSED(secret_key);

   return 0;
}

unsigned long long OQS_SECRET_KEY_lms_sigs_total(const OQS_SECRET_KEY *secret_key) {
   if (secret_key == NULL) {
       return -1;
   }

   return 0;
}

unsigned long long OQS_SECRET_KEY_hss_sigs_left(const OQS_SECRET_KEY *secret_key) {
   if (secret_key == NULL) {
       return -1;
   }

   return 0;
}

unsigned long long OQS_SECRET_KEY_hss_sigs_total(const OQS_SECRET_KEY *secret_key) {
   if (secret_key == NULL) {
       return -1;
   }
   return 0;
}

OQS_SECRET_KEY *OQS_SIG_STFL_alg_lms_derive_subkey(OQS_SECRET_KEY *master_key, const unsigned long long number_of_sigs) {

    LMS_UNUSED(master_key);
    LMS_UNUSED(number_of_sigs);

   return NULL;
}

OQS_SECRET_KEY *OQS_SIG_STFL_alg_hss_derive_subkey(OQS_SECRET_KEY *master_key, const unsigned long long number_of_sigs) {
    LMS_UNUSED(master_key);
    LMS_UNUSED(number_of_sigs);

   return NULL;
}


/* LMS wrapper functions use internal OIDs to
 * identify the parameter set to be used
 */

int oqs_sig_stfl_lms_keypair(uint8_t *pk, OQS_SECRET_KEY *sk, const uint32_t oid)
{
    int ret = -1;
    LMS_UNUSED(sk);
    LMS_UNUSED(oid);
    LMS_UNUSED(pk);

    // Set lms param set
    // gen key pair
    // store key pair

    return ret;
}

int oqs_sig_stfl_lms_sign(OQS_SECRET_KEY *sk,
              uint8_t *sm, unsigned long long *smlen,
              const uint8_t *m, unsigned long long mlen)
{
    LMS_UNUSED(sk);
    LMS_UNUSED(sm);
    LMS_UNUSED(smlen);
    LMS_UNUSED(m);
    LMS_UNUSED(mlen);

    return -1;
}

int oqs_sig_stfl_lms_verify(uint8_t *m, unsigned long long *mlen,
                   const uint8_t *sm, unsigned long long smlen,
                   const uint8_t *pk)
{
    LMS_UNUSED(m);
    LMS_UNUSED(mlen);
    LMS_UNUSED(sm);
    LMS_UNUSED(smlen);
    LMS_UNUSED(pk);

    return -1;
}

int oqs_sig_stfl_hss_keypair(uint8_t *pk, OQS_SECRET_KEY *sk, const uint32_t oid)
{
    LMS_UNUSED(sk);
    LMS_UNUSED(oid);
    LMS_UNUSED(pk);

    return -1;
}

int oqs_sig_stfl_hss_sign(OQS_SECRET_KEY *sk,
        uint8_t *sm, unsigned long long *smlen,
        const uint8_t *m, unsigned long long mlen)
{
    LMS_UNUSED(sk);
    LMS_UNUSED(sm);
    LMS_UNUSED(smlen);
    LMS_UNUSED(m);
    LMS_UNUSED(mlen);

    return -1;
}

int oqs_sig_stfl_hss_verify(uint8_t *m, unsigned long long *mlen,
                     const uint8_t *sm, unsigned long long smlen,
                     const uint8_t *pk)
{
    LMS_UNUSED(m);
    LMS_UNUSED(mlen);
    LMS_UNUSED(sm);
    LMS_UNUSED(smlen);
    LMS_UNUSED(pk);

    return -1;
}
