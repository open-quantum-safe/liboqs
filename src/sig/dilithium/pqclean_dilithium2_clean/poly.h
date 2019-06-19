#ifndef POLY_H
#define POLY_H

#include "params.h"
#include <stdint.h>

typedef struct {
    uint32_t coeffs[N];
} poly;

void PQCLEAN_DILITHIUM2_CLEAN_poly_reduce(poly *a);
void PQCLEAN_DILITHIUM2_CLEAN_poly_csubq(poly *a);
void PQCLEAN_DILITHIUM2_CLEAN_poly_freeze(poly *a);

void PQCLEAN_DILITHIUM2_CLEAN_poly_add(poly *c, const poly *a, const poly *b);
void PQCLEAN_DILITHIUM2_CLEAN_poly_sub(poly *c, const poly *a, const poly *b);
void PQCLEAN_DILITHIUM2_CLEAN_poly_shiftl(poly *a);

void PQCLEAN_DILITHIUM2_CLEAN_poly_ntt(poly *a);
void PQCLEAN_DILITHIUM2_CLEAN_poly_invntt_montgomery(poly *a);
void PQCLEAN_DILITHIUM2_CLEAN_poly_pointwise_invmontgomery(poly *c, const poly *a, const poly *b);

void PQCLEAN_DILITHIUM2_CLEAN_poly_power2round(poly *a1, poly *a0, const poly *a);
void PQCLEAN_DILITHIUM2_CLEAN_poly_decompose(poly *a1, poly *a0, const poly *a);
unsigned int PQCLEAN_DILITHIUM2_CLEAN_poly_make_hint(poly *h, const poly *a0, const poly *a1);
void PQCLEAN_DILITHIUM2_CLEAN_poly_use_hint(poly *a, const poly *b, const poly *h);

int  PQCLEAN_DILITHIUM2_CLEAN_poly_chknorm(const poly *a, uint32_t B);
void PQCLEAN_DILITHIUM2_CLEAN_poly_uniform(poly *a,
        const unsigned char seed[SEEDBYTES],
        uint16_t nonce);
void PQCLEAN_DILITHIUM2_CLEAN_poly_uniform_eta(poly *a,
        const unsigned char seed[SEEDBYTES],
        uint16_t nonce);
void PQCLEAN_DILITHIUM2_CLEAN_poly_uniform_gamma1m1(poly *a,
        const unsigned char seed[CRHBYTES],
        uint16_t nonce);

void PQCLEAN_DILITHIUM2_CLEAN_polyeta_pack(unsigned char *r, const poly *a);
void PQCLEAN_DILITHIUM2_CLEAN_polyeta_unpack(poly *r, const unsigned char *a);

void PQCLEAN_DILITHIUM2_CLEAN_polyt1_pack(unsigned char *r, const poly *a);
void PQCLEAN_DILITHIUM2_CLEAN_polyt1_unpack(poly *r, const unsigned char *a);

void PQCLEAN_DILITHIUM2_CLEAN_polyt0_pack(unsigned char *r, const poly *a);
void PQCLEAN_DILITHIUM2_CLEAN_polyt0_unpack(poly *r, const unsigned char *a);

void PQCLEAN_DILITHIUM2_CLEAN_polyz_pack(unsigned char *r, const poly *a);
void PQCLEAN_DILITHIUM2_CLEAN_polyz_unpack(poly *r, const unsigned char *a);

void PQCLEAN_DILITHIUM2_CLEAN_polyw1_pack(unsigned char *r, const poly *a);

#endif
