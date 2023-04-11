// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <oqs/oqs.h>
#include "./external/config.h"
//#include "./external/xmss.h"
//#include "./external/params.h"
#include "sig_stfl_lms.h"

//#include "./external/xmss_namespace.h"


// ======================== LMS5-SHA2 ======================== //

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
    if (secret_key == NULL || public_key == NULL) {
        return OQS_ERROR;
    }

    uint32_t oid = 0x00000001;
    LMS_UNUSED(oid);
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

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) {
        return NULL;
    }
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->length_secret_key = OQS_SIG_STFL_alg_lms_sha256_h5_w1_length_sk;

    // Assign the sigs_left and sigs_max functions
    sk->sigs_left = OQS_SECRET_KEY_lms_sigs_left;
    sk->sigs_total = OQS_SECRET_KEY_lms_sigs_total;

    // Initialize the key with length_secret_key amount of bytes.
    sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
    memset(sk->secret_key, 0, sk->length_secret_key);

    return sk;
}

// ================================================================ //

// ======================== LMS-SHA256 H10======================== //
//
//OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
// if (secret_key == NULL || public_key == NULL) {
//     return OQS_ERROR;
// }
//
// uint32_t oid = 0x00000001;
// if (xmss_keypair(public_key, secret_key, oid) != 0) {
//     return OQS_ERROR;
// }
// return OQS_SUCCESS;
//}
//
//OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
//    if (secret_key == NULL || public_key == NULL) {
//        return OQS_ERROR;
//    }
//
//    uint32_t oid = 0x00000001;
//    if (xmss_keypair(public_key, secret_key, oid) != 0) {
//        return OQS_ERROR;
//    }
//    return OQS_SUCCESS;
//}
//
//OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h10_new(void) {
//
// OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
// if (sig == NULL) {
//     return NULL;
// }
// memset(sig, 0, sizeof(OQS_SIG_STFL));
//
// sig->method_name = "XMSS-SHA2_10_256";
// sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
// sig->euf_cma = true;
//
// sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha256_h10_length_pk;
// sig->length_signature = OQS_SIG_STFL_alg_xmss_sha256_h10_length_signature;
//
// sig->keypair = OQS_SIG_STFL_alg_xmss_sha256_h10_keypair;
// sig->sign = OQS_SIG_STFL_alg_xmss_sign;
// sig->verify = OQS_SIG_STFL_alg_xmss_verify;
// sig->derive_subkey = OQS_SIG_STFL_alg_xmss_derive_subkey;
//
// return sig;
//}
//
//OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H10_new(void) {
//
// // Initialize the secret key in the heap with adequate memory
// OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
// if (sk == NULL) {
//     return NULL;
// }
// memset(sk, 0, sizeof(OQS_SECRET_KEY));
//
// sk->length_secret_key = OQS_SIG_STFL_alg_xmss_sha256_h10_length_sk;
//
// // Assign the sigs_left and sigs_max functions
// sk->sigs_left = OQS_SECRET_KEY_xmss_sigs_left;
// sk->sigs_total = OQS_SECRET_KEY_xmss_sigs_total;
//
// // Initialize the key with length_secret_key amount of bytes.
// sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
// memset(sk->secret_key, 0, sk->length_secret_key);
//
// return sk;
//}
//
//// ================================================================ //
//
//// ======================== XMSS16-SHA256 ======================== //
//
//OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h16_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
// if (secret_key == NULL || public_key == NULL) {
//     return OQS_ERROR;
// }
//
// uint32_t oid = 0x00000002;
// if (xmss_keypair(public_key, secret_key, oid) != 0) {
//     return OQS_ERROR;
// }
// return OQS_SUCCESS;
//}
//
//
//OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h16_new(void) {
//
// OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
// if (sig == NULL) {
//     return NULL;
// }
// memset(sig, 0, sizeof(OQS_SIG_STFL));
//
// sig->method_name = "XMSS-SHA2_16_256";
// sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
// sig->euf_cma = true;
//
// sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha256_h16_length_pk;
// sig->length_signature = OQS_SIG_STFL_alg_xmss_sha256_h16_length_signature;
//
// sig->keypair = OQS_SIG_STFL_alg_xmss_sha256_h16_keypair;
// sig->sign = OQS_SIG_STFL_alg_xmss_sign;
// sig->verify = OQS_SIG_STFL_alg_xmss_verify;
// sig->derive_subkey = OQS_SIG_STFL_alg_xmss_derive_subkey;
//
// return sig;
//}
//
//OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H16_new(void) {
//
// // Initialize the secret key in the heap with adequate memory
// OQS_SECRET_KEY *sk = (OQS_SECRET_KEY *)malloc(sizeof(OQS_SECRET_KEY));
// if (sk == NULL) {
//     return NULL;
// }
// memset(sk, 0, sizeof(OQS_SECRET_KEY));
//
// sk->length_secret_key = OQS_SIG_STFL_alg_xmss_sha256_h16_length_sk;
//
// // Assign the sigs_left and sigs_max functions
// sk->sigs_left = OQS_SECRET_KEY_xmss_sigs_left;
// sk->sigs_total = OQS_SECRET_KEY_xmss_sigs_total;
//
// // Initialize the key with length_secret_key amount of bytes.
// sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
// memset(sk->secret_key, 0, sk->length_secret_key);
// return sk;
//}
//
//// ================================================================ //
//
//
//// ======================== XMSS20-SHA256 ======================== //
//
//OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sha256_h20_keypair(uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
// if (secret_key == NULL || public_key == NULL) {
//     return OQS_ERROR;
// }
//
// uint32_t oid = 0x00000003;
// if (xmss_keypair(public_key, secret_key, oid) != 0) {
//     return OQS_ERROR;
// }
// return OQS_SUCCESS;
//}
//
//OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h20_new(void) {
//
// OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
// if (sig == NULL) {
//     return NULL;
// }
// memset(sig, 0, sizeof(OQS_SIG_STFL));
//
// sig->method_name = "XMSS-SHA2_20_256";
// sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554";
// sig->euf_cma = true;
//
// sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha256_h20_length_pk;
// sig->length_signature = OQS_SIG_STFL_alg_xmss_sha256_h20_length_signature;
//
// sig->keypair = OQS_SIG_STFL_alg_xmss_sha256_h20_keypair;
// sig->sign = OQS_SIG_STFL_alg_xmss_sign;
// sig->verify = OQS_SIG_STFL_alg_xmss_verify;
// sig->derive_subkey = OQS_SIG_STFL_alg_xmss_derive_subkey;
//
// return sig;
//}
//
//OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H20_new(void) {
//
// // Initialize the secret key in the heap with adequate memory
// OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
// if (sk == NULL) {
//     return NULL;
// }
// memset(sk, 0, sizeof(OQS_SECRET_KEY));
//
// sk->length_secret_key = OQS_SIG_STFL_alg_xmss_sha256_h20_length_sk ;
// // Assign the sigs_left and sigs_max functions
// sk->sigs_left = OQS_SECRET_KEY_xmss_sigs_left;
// sk->sigs_total = OQS_SECRET_KEY_xmss_sigs_total;
//
// // Initialize the key with length_secret_key amount of bytes.
// sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
// memset(sk->secret_key, 0, sk->length_secret_key);
//
// return sk;
//}

// ================================================================ //
