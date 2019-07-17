/**
 *  kem.c
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#include <string.h>
#include "api.h"
#include "nts_kem.h"
#include "nts_kem_errors.h"

int PQCLEAN_NTSKEM1264_CLEAN_crypto_kem_keypair(unsigned char* pk,
                       unsigned char* sk)
{
    int status;
    NTSKEM *nts_kem = NULL;

    status = PQCLEAN_NTSKEM1264_CLEAN_nts_kem_create(&nts_kem);
    if (status != NTS_KEM_SUCCESS)
        return status;
    
    memcpy(pk, nts_kem->public_key, PQCLEAN_NTSKEM1264_CLEAN_CRYPTO_PUBLICKEYBYTES);
    memcpy(sk, nts_kem->private_key, PQCLEAN_NTSKEM1264_CLEAN_CRYPTO_SECRETKEYBYTES);
    
    PQCLEAN_NTSKEM1264_CLEAN_nts_kem_release(nts_kem);
    
    return status;
}

int PQCLEAN_NTSKEM1264_CLEAN_crypto_kem_enc(unsigned char *ct,
                   unsigned char *ss,
                   const unsigned char *pk)
{
    return PQCLEAN_NTSKEM1264_CLEAN_nts_kem_encapsulate(pk, PQCLEAN_NTSKEM1264_CLEAN_CRYPTO_PUBLICKEYBYTES, ct, ss);
}

int PQCLEAN_NTSKEM1264_CLEAN_crypto_kem_dec(unsigned char *ss,
                   const unsigned char *ct,
                   const unsigned char *sk)
{
    return PQCLEAN_NTSKEM1264_CLEAN_nts_kem_decapsulate(sk, PQCLEAN_NTSKEM1264_CLEAN_CRYPTO_SECRETKEYBYTES, ct, ss);
}
