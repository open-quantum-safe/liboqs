#include <string.h>
#include <stdlib.h>

#include "./external/xmss.h"
#include "./external/params.h"
#include "sig_stfl_xmss.h"

// ======================== XMSS10-SHA256 ======================== //

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h10_new(void) {
    
    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->method_name = "XMSS-SHA2_10_256";
    sig->alg_version = "..."; 

    // Check how true this is
    sig->claimed_nist_level = 2;
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha256_h10_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_xmss_sha256_h10_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_xmss_keypair;
    sig->sign = OQS_SIG_STFL_alg_xmss_sign;
    sig->verify = OQS_SIG_STFL_alg_xmss_verify;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H10_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) return NULL;
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x00000001;
    sk->is_xmssmt = 0;

    sk->length_secret_key = OQS_SIG_STFL_alg_xmss_sha256_h10_length_sk;
    
    perform_key_allocation(sk);

    return sk;
}

// ================================================================ //

// ======================== XMSS16-SHA256 ======================== //

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h16_new(void) {
    
    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    sig->method_name = "XMSS-SHA2_16_256";
    sig->alg_version = "..."; 

    // Check how true this is
    sig->claimed_nist_level = 2;
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha256_h16_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_xmss_sha256_h16_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_xmss_keypair;
    sig->sign = OQS_SIG_STFL_alg_xmss_sign;
    sig->verify = OQS_SIG_STFL_alg_xmss_verify;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H16_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) return NULL;
    memset(sk, 0, sizeof(OQS_SECRET_KEY));
    sk->oid = 0x00000002;
    sk->is_xmssmt = 0;

    sk->length_secret_key = OQS_SIG_STFL_alg_xmss_sha256_h16_length_sk;

    perform_key_allocation(sk);

    return sk;
}

// ================================================================ //

// ======================== XMSS20-SHA256 ======================== //

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_sha256_h20_new(void) {
    
    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    sig->method_name = "XMSS-SHA2_20_256";
    sig->alg_version = "..."; 

    // Check how true this is
    sig->claimed_nist_level = 2;
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_xmss_sha256_h20_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_xmss_sha256_h20_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_xmss_keypair;
    sig->sign = OQS_SIG_STFL_alg_xmss_sign;
    sig->verify = OQS_SIG_STFL_alg_xmss_verify;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_XMSS_SHA256_H20_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) return NULL;
    memset(sk, 0, sizeof(OQS_SECRET_KEY));
    sk->oid = 0x00000003;
    sk->is_xmssmt = 0;

    sk->length_secret_key = OQS_SIG_STFL_alg_xmss_sha256_h20_length_sk ;

    perform_key_allocation(sk);

    return sk;
}

// ================================================================ //