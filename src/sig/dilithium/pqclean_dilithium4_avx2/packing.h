#ifndef PQCLEAN_DILITHIUM4_AVX2_PACKING_H
#define PQCLEAN_DILITHIUM4_AVX2_PACKING_H

#include "api.h"
#include "params.h"
#include "polyvec.h"

void PQCLEAN_DILITHIUM4_AVX2_pack_pk(
    uint8_t pk[PQCLEAN_DILITHIUM4_AVX2_CRYPTO_PUBLICKEYBYTES],
    const uint8_t rho[SEEDBYTES],
    const polyveck *t1);
void PQCLEAN_DILITHIUM4_AVX2_pack_sk(
    uint8_t sk[PQCLEAN_DILITHIUM4_AVX2_CRYPTO_SECRETKEYBYTES],
    const uint8_t rho[SEEDBYTES],
    const uint8_t key[SEEDBYTES],
    const uint8_t tr[SEEDBYTES],
    const polyvecl *s1,
    const polyveck *s2,
    const polyveck *t0);
void PQCLEAN_DILITHIUM4_AVX2_pack_sig(
    uint8_t sig[PQCLEAN_DILITHIUM4_AVX2_CRYPTO_SECRETKEYBYTES],
    const polyvecl *z, const polyveck *h, const poly *c);

void PQCLEAN_DILITHIUM4_AVX2_unpack_pk(
    uint8_t rho[SEEDBYTES],
    polyveck *t1,
    const uint8_t pk[PQCLEAN_DILITHIUM4_AVX2_CRYPTO_PUBLICKEYBYTES]);
void PQCLEAN_DILITHIUM4_AVX2_unpack_sk(
    uint8_t rho[SEEDBYTES],
    uint8_t key[SEEDBYTES],
    uint8_t tr[CRHBYTES],
    polyvecl *s1,
    polyveck *s2,
    polyveck *t0,
    const uint8_t *sk);
int PQCLEAN_DILITHIUM4_AVX2_unpack_sig(
    polyvecl *z,
    polyveck *h,
    poly *c,
    const uint8_t sig[PQCLEAN_DILITHIUM4_AVX2_CRYPTO_BYTES]);

#endif
