#ifndef OWCPA_H
#define OWCPA_H

#include "params.h"
#include "poly.h"

void PQCLEAN_NTRUHPS2048509_AVX2_owcpa_samplemsg(unsigned char msg[NTRU_OWCPA_MSGBYTES],
        const unsigned char seed[NTRU_SEEDBYTES]);

void PQCLEAN_NTRUHPS2048509_AVX2_owcpa_keypair(unsigned char *pk,
        unsigned char *sk,
        const unsigned char seed[NTRU_SEEDBYTES]);

void PQCLEAN_NTRUHPS2048509_AVX2_owcpa_enc(unsigned char *c,
        const poly *r,
        const poly *m,
        const unsigned char *pk);

int PQCLEAN_NTRUHPS2048509_AVX2_owcpa_dec(unsigned char *rm,
        const unsigned char *ciphertext,
        const unsigned char *secretkey);
#endif
