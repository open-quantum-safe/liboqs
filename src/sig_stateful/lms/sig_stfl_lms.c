// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdio.h>
#include <oqs/oqs.h>
#include "./external/config.h"
#include "sig_stfl_lms_wrap.h"
#include "sig_stfl_lms.h"

static OQS_STATUS OQS_SIG_STFL_alg_lms_aux_data(OQS_SECRET_KEY *secret_key, uint8_t **aux_data, size_t *aux_len) {
    size_t aux_data_len = 0;
    uint8_t *buffer = NULL;
    unsigned char *lms_aux_data = NULL;

    if (secret_key == NULL || aux_data == NULL || aux_len == NULL) {
        return OQS_ERROR;
    }
    *aux_len = 0;
    *aux_data = NULL;

    if (secret_key->data) {
        aux_data_len = ((oqs_lms_key_data*)secret_key->data)->len_aux_data;
        if (aux_data_len == 0) {
            goto done;
        }
        lms_aux_data = ((oqs_lms_key_data*)secret_key->data)->aux_data;
        if(!lms_aux_data) {
            goto done;
        }

        buffer = malloc(aux_data_len);
        if (!buffer) {
            goto done;
        }

        memset(buffer, 0, aux_data_len);
        memcpy(buffer, lms_aux_data, aux_data_len);

        *aux_len = aux_data_len;
        *aux_data = (void*)buffer;
    }
done:
    return OQS_SUCCESS;
}

// ======================== LMS-SHA256 H5/W1 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
    if (secret_key == NULL || public_key == NULL) {
        return OQS_ERROR;
    }

    uint32_t oid = 0x00000001;
    if (oqs_sig_stfl_lms_keypair(public_key, secret_key, oid) != 0) {
        return OQS_ERROR;
    }
    return OQS_SUCCESS;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w1_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->method_name = "LMS-SHA2_H5_W1";
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W1_new(void) {

    char name[] = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1;
    char *method_name = NULL;


    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    /* set method name */
    method_name = malloc(sizeof(name));
    if (method_name == NULL) {
        OQS_SECRET_KEY_free(sk);
        sk = NULL;
        return NULL;
    }
    memset(method_name, 0, sizeof(name));
    snprintf(method_name, sizeof(name), "%s", name);
    sk->method_name = method_name;

    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data = OQS_SIG_STFL_alg_lms_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

// ================================================================ //


// ================================================================ //
