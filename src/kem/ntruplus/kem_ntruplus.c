#include <stdlib.h>
#include <string.h>
#include <oqs/oqs.h>
#include "kem_ntruplus.h"
#include "ntruplus.h"

#ifdef OQS_ENABLE_KEM_ntruplus_701

OQS_KEM *OQS_KEM_ntruplus_701_new(void) {
    OQS_KEM *kem = malloc(sizeof(OQS_KEM));
    if (kem == NULL) {
        return NULL;
    }
    
    kem->method_name = "NTRU+701";
    kem->alg_version = "1.0";
    
    kem->claimed_nist_level = 1;
    kem->ind_cca = true;
    
    kem->length_public_key = OQS_KEM_ntruplus_701_length_public_key;
    kem->length_secret_key = OQS_KEM_ntruplus_701_length_secret_key;
    kem->length_ciphertext = OQS_KEM_ntruplus_701_length_ciphertext;
    kem->length_shared_secret = OQS_KEM_ntruplus_701_length_shared_secret;
    
    kem->keypair = OQS_KEM_ntruplus_701_keypair;
    kem->encaps = OQS_KEM_ntruplus_701_encaps;
    kem->decaps = OQS_KEM_ntruplus_701_decaps;
    
    return kem;
}

OQS_STATUS OQS_KEM_ntruplus_701_keypair(uint8_t *public_key, uint8_t *secret_key) {
    if (crypto_kem_keypair(public_key, secret_key) != 0) {
        return OQS_ERROR;
    }
    return OQS_SUCCESS;
}

OQS_STATUS OQS_KEM_ntruplus_701_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
    if (crypto_kem_enc(ciphertext, shared_secret, public_key) != 0) {
        return OQS_ERROR;
    }
    return OQS_SUCCESS;
}

OQS_STATUS OQS_KEM_ntruplus_701_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
    if (crypto_kem_dec(shared_secret, ciphertext, secret_key) != 0) {
        return OQS_ERROR;
    }
    return OQS_SUCCESS;
}

#endif