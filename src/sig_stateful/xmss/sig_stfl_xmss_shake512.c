#include <string.h>
#include <stdlib.h>
#include "./external/xmss.h"
#include "./external/params.h"
#include "sig_stfl_xmss.h"

// ======================== XMSS10-SHAKE512 ======================== //

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake512_h10_new(void) {
    
    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    memset(sig, 0, sizeof(OQS_SIG_STFL));

    sig->method_name = "XMSS-SHAKE_10_512";
    sig->alg_version = "..."; 

    // Check how true this is
    sig->claimed_nist_level = 2;
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_xmss_shake512_h10_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_xmss_shake512_h10_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_xmss_keypair;
    sig->sign = OQS_SIG_STFL_alg_xmss_sign;
    sig->verify = OQS_SIG_STFL_alg_xmss_verify;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_xmss_shake512_H10_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) return NULL;
    memset(sk, 0, sizeof(OQS_SECRET_KEY));

    sk->oid = 0x0000000a;
    sk->is_xmssmt = 0;

    sk->length_secret_key = OQS_SIG_STFL_alg_xmss_shake512_h10_length_sk;
    
    perform_key_allocation(sk);

    return sk;
}

// ================================================================ //

// ======================== XMSS16-SHAKE256 ======================== //

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake512_h16_new(void) {
    
    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    sig->method_name = "XMSS-SHAKE_16_512";
    sig->alg_version = "..."; 

    // Check how true this is
    sig->claimed_nist_level = 2;
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_xmss_shake512_h16_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_xmss_shake512_h16_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_xmss_keypair;
    sig->sign = OQS_SIG_STFL_alg_xmss_sign;
    sig->verify = OQS_SIG_STFL_alg_xmss_verify;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_xmss_shake512_H16_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) return NULL;
    memset(sk, 0, sizeof(OQS_SECRET_KEY));
    sk->oid = 0x0000000b;
    sk->is_xmssmt = 0;

    sk->length_secret_key = OQS_SIG_STFL_alg_xmss_shake512_h16_length_sk;

    perform_key_allocation(sk);

    return sk;
}

// ================================================================ //

// ======================== XMSS20-SHAKE256 ======================== //

OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss_shake512_h20_new(void) {
    
    OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL));
    if (sig == NULL) {
        return NULL;
    }
    sig->method_name = "XMSS-SHAKE_16_512";;
    sig->alg_version = "..."; 

    // Check how true this is
    sig->claimed_nist_level = 2;
    sig->euf_cma = true;

    sig->length_public_key = OQS_SIG_STFL_alg_xmss_shake512_h20_length_pk;
    sig->length_signature = OQS_SIG_STFL_alg_xmss_shake512_h20_length_signature;

    sig->keypair = OQS_SIG_STFL_alg_xmss_keypair;
    sig->sign = OQS_SIG_STFL_alg_xmss_sign;
    sig->verify = OQS_SIG_STFL_alg_xmss_verify;

    return sig;
}

OQS_SECRET_KEY *OQS_SECRET_KEY_xmss_shake512_H20_new(void) {

    // Initialize the secret key in the heap with adequate memory
    OQS_SECRET_KEY *sk = malloc(sizeof(OQS_SECRET_KEY));
    if (sk == NULL) return NULL;
    memset(sk, 0, sizeof(OQS_SECRET_KEY));
    sk->oid = 0x0000000c;
    sk->is_xmssmt = 0;

    sk->length_secret_key = OQS_SIG_STFL_alg_xmss_shake512_h20_length_sk ;

    perform_key_allocation(sk);

    return sk;
}

// ================================================================ //