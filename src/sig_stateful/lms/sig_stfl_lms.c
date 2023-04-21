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


static void OQS_SIG_STFL_alg_lms_set_aux_data(OQS_SECRET_KEY *sk, void *data) {
    sk->data = data;
}

static void OQS_SIG_STFL_alg_lms_free_aux_data(OQS_SECRET_KEY *sk) {
    if (!sk || !sk->data) {
        return;
    }
    oqs_lms_key_data *key_data = NULL;
    key_data = (oqs_lms_key_data*)sk->data;
    OQS_MEM_secure_free(key_data->aux_data, key_data->len_aux_data);
    key_data->aux_data = NULL;
    key_data->len_aux_data = 0;
    sk->data = NULL;
}

// ======================== LMS-SHA256 H5/W1 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
    if (secret_key == NULL || public_key == NULL) {
        return OQS_ERROR;
    }
    if (secret_key->oid == 0) {
        return OQS_ERROR;
    }

    if (oqs_sig_stfl_lms_keypair(public_key, secret_key, secret_key->oid) != 0) {
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

    sig->oid = 0x00000001;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W1_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000001;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w2_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000002;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w2;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h5_w2_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h5_w2_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W2_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000002;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w2;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h5_w2_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w4_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000003;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w4;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h5_w4_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h5_w4_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W4_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000003;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w4;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h5_w4_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;


    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h5_w8_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000004;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w8;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h5_w8_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h5_w8_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H5_W8_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000004;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h5_w8;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h5_w8_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w1_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000005;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h10_w1;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h10_w1_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h10_w1_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_W1_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000005;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h10_w1;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h10_w1_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w2_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000006;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h10_w2;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h10_w2_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h10_w2_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_W2_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000006;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h10_w2;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h10_w2_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w4_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000007;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h10_w4;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h10_w4_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h10_w4_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_W4_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000007;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h10_w4;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h10_w4_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h10_w8_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000008;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h10_w8;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h10_w8_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h10_w8_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H10_W8_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000008;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h10_w8;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h10_w8_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w1_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000009;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h15_w1;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h15_w1_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h15_w1_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_W1_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000009;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h15_w1;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h15_w1_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w2_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x0000000a;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h15_w2;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h15_w2_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h15_w2_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_W2_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x0000000a;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h15_w2;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h15_w2_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w4_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x0000000b;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h15_w4;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h15_w4_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h15_w4_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_W4_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x0000000b;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h15_w4;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h15_w4_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h15_w8_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x0000000c;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h15_w8;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h15_w8_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h15_w8_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H15_W8_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x0000000c;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h15_w8;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h15_w8_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w1_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x0000000d;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h20_w1;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h20_w1_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h20_w1_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_W1_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x0000000d;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h20_w1;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h20_w1_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w2_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x0000000e;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h20_w2;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h20_w2_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h20_w2_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_W2_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x0000000e;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h20_w2;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h20_w2_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w4_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x0000000f;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h20_w4;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h20_w4_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h20_w4_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_W4_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x0000000f;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h20_w4;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h20_w4_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h20_w8_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000010;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h20_w8;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h20_w8_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h20_w8_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H20_W8_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000010;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h20_w8;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h20_w8_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w1_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000011;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h25_w1;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h25_w1_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h25_w1_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_W1_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000011;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h25_w1;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h25_w1_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w2_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000012;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h25_w2;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h25_w2_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h25_w2_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_W2_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000012;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h25_w2;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h25_w2_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w4_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000013;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h25_w4;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h25_w4_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h25_w4_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_W4_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000013;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h25_w4;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h25_w4_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_sha256_h25_w8_new(void) {

    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->oid = 0x00000014;
    sig->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h25_w8;
    sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_lms_sha256_h25_w8_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_lms_sha256_h25_w8_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_lms_sha256_hx_wx_keypair;
    sig->sign = OQS_SIG_STFL_alg_lms_sign;
    sig->verify = OQS_SIG_STFL_alg_lms_verify;
    sig->derive_subkey = NULL;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_LMS_SHA256_H25_W8_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000014;
    /* set method name */
    sk->method_name = OQS_SIG_STFL_alg_lms_sha256_n32_h25_w8;
    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h25_w8_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left     = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total    = OQS_SECRET_KEY_lms_sigs_total;
    sk->get_key_data  = OQS_SIG_STFL_alg_lms_aux_data;
    sk->free_key_data = OQS_SIG_STFL_alg_lms_free_aux_data;
    sk->set_key_data  = OQS_SIG_STFL_alg_lms_set_aux_data;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

// ================================================================ //


// ================================================================ //
