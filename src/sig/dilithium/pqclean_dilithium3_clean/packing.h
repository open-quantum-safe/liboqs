#ifndef PACKING_H
#define PACKING_H

#include "params.h"
#include "polyvec.h"

void PQCLEAN_DILITHIUM3_CLEAN_pack_pk(unsigned char pk[CRYPTO_PUBLICKEYBYTES],
                                      const unsigned char rho[SEEDBYTES], const polyveck *t1);
void PQCLEAN_DILITHIUM3_CLEAN_pack_sk(unsigned char sk[CRYPTO_SECRETKEYBYTES],
                                      const unsigned char rho[SEEDBYTES],
                                      const unsigned char key[SEEDBYTES],
                                      const unsigned char tr[CRHBYTES],
                                      const polyvecl *s1,
                                      const polyveck *s2,
                                      const polyveck *t0);
void PQCLEAN_DILITHIUM3_CLEAN_pack_sig(unsigned char sig[CRYPTO_BYTES],
                                       const polyvecl *z, const polyveck *h, const poly *c);

void PQCLEAN_DILITHIUM3_CLEAN_unpack_pk(unsigned char rho[SEEDBYTES], polyveck *t1,
                                        const unsigned char pk[CRYPTO_PUBLICKEYBYTES]);
void PQCLEAN_DILITHIUM3_CLEAN_unpack_sk(unsigned char rho[SEEDBYTES],
                                        unsigned char key[SEEDBYTES],
                                        unsigned char tr[CRHBYTES],
                                        polyvecl *s1,
                                        polyveck *s2,
                                        polyveck *t0,
                                        const unsigned char sk[CRYPTO_SECRETKEYBYTES]);
int PQCLEAN_DILITHIUM3_CLEAN_unpack_sig(polyvecl *z, polyveck *h, poly *c,
                                        const unsigned char sig[CRYPTO_BYTES]);

#endif
