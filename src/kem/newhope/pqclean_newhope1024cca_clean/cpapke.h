#ifndef INDCPA_H
#define INDCPA_H

void PQCLEAN_NEWHOPE1024CCA_CLEAN_cpapke_keypair(unsigned char *pk,
        unsigned char *sk);

void PQCLEAN_NEWHOPE1024CCA_CLEAN_cpapke_enc(unsigned char *c,
        const unsigned char *m,
        const unsigned char *pk,
        const unsigned char *coins);

void PQCLEAN_NEWHOPE1024CCA_CLEAN_cpapke_dec(unsigned char *m,
        const unsigned char *c,
        const unsigned char *sk);

#endif
