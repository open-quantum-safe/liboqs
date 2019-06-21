#ifndef INDCPA_H
#define INDCPA_H

void PQCLEAN_KYBER1024_CLEAN_indcpa_keypair(unsigned char *pk,
        unsigned char *sk);

void PQCLEAN_KYBER1024_CLEAN_indcpa_enc(unsigned char *c,
                                        const unsigned char *m,
                                        const unsigned char *pk,
                                        const unsigned char *coins);

void PQCLEAN_KYBER1024_CLEAN_indcpa_dec(unsigned char *m,
                                        const unsigned char *c,
                                        const unsigned char *sk);

#endif
