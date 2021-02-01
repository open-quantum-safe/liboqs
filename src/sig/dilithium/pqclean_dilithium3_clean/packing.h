#ifndef PQCLEAN_DILITHIUM3_CLEAN_PACKING_H
#define PQCLEAN_DILITHIUM3_CLEAN_PACKING_H
#include "params.h"
#include "polyvec.h"
#include <stdint.h>

void PQCLEAN_DILITHIUM3_CLEAN_pack_pk(uint8_t pk[PQCLEAN_DILITHIUM3_CLEAN_CRYPTO_PUBLICKEYBYTES], const uint8_t rho[SEEDBYTES], const polyveck *t1);

void PQCLEAN_DILITHIUM3_CLEAN_pack_sk(uint8_t sk[PQCLEAN_DILITHIUM3_CLEAN_CRYPTO_SECRETKEYBYTES],
                                      const uint8_t rho[SEEDBYTES],
                                      const uint8_t tr[CRHBYTES],
                                      const uint8_t key[SEEDBYTES],
                                      const polyveck *t0,
                                      const polyvecl *s1,
                                      const polyveck *s2);

void PQCLEAN_DILITHIUM3_CLEAN_pack_sig(uint8_t sig[PQCLEAN_DILITHIUM3_CLEAN_CRYPTO_BYTES], const uint8_t c[SEEDBYTES], const polyvecl *z, const polyveck *h);

void PQCLEAN_DILITHIUM3_CLEAN_unpack_pk(uint8_t rho[SEEDBYTES], polyveck *t1, const uint8_t pk[PQCLEAN_DILITHIUM3_CLEAN_CRYPTO_PUBLICKEYBYTES]);

void PQCLEAN_DILITHIUM3_CLEAN_unpack_sk(uint8_t rho[SEEDBYTES],
                                        uint8_t tr[CRHBYTES],
                                        uint8_t key[SEEDBYTES],
                                        polyveck *t0,
                                        polyvecl *s1,
                                        polyveck *s2,
                                        const uint8_t sk[PQCLEAN_DILITHIUM3_CLEAN_CRYPTO_SECRETKEYBYTES]);

int PQCLEAN_DILITHIUM3_CLEAN_unpack_sig(uint8_t c[SEEDBYTES], polyvecl *z, polyveck *h, const uint8_t sig[PQCLEAN_DILITHIUM3_CLEAN_CRYPTO_BYTES]);

#endif
