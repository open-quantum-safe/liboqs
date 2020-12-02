#ifndef PQCLEAN_KYBER512_CLEAN_KEM_H
#define PQCLEAN_KYBER512_CLEAN_KEM_H
#include "params.h"


int PQCLEAN_KYBER512_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_KYBER512_CLEAN_crypto_kem_enc(unsigned char *ct,
        unsigned char *ss,
        const unsigned char *pk);

int PQCLEAN_KYBER512_CLEAN_crypto_kem_dec(unsigned char *ss,
        const unsigned char *ct,
        const unsigned char *sk);

#endif
