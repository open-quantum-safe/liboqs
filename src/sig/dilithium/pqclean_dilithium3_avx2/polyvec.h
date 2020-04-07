#ifndef PQCLEAN_DILITHIUM3_AVX2_POLYVEC_H
#define PQCLEAN_DILITHIUM3_AVX2_POLYVEC_H

#include <stdint.h>

#include "params.h"
#include "poly.h"

/* Vectors of polynomials of length L */
typedef struct {
    poly vec[L];
} polyvecl;

void PQCLEAN_DILITHIUM3_AVX2_polyvecl_freeze(polyvecl *v);

void PQCLEAN_DILITHIUM3_AVX2_polyvecl_add(polyvecl *w, const polyvecl *u, const polyvecl *v);

void PQCLEAN_DILITHIUM3_AVX2_polyvecl_ntt(polyvecl *v);
void PQCLEAN_DILITHIUM3_AVX2_polyvecl_pointwise_acc_invmontgomery(
    poly *w, const polyvecl *u, const polyvecl *v);

int PQCLEAN_DILITHIUM3_AVX2_polyvecl_chknorm(const polyvecl *v, uint32_t B);



/* Vectors of polynomials of length K */
typedef struct {
    poly vec[K];
} polyveck;

void PQCLEAN_DILITHIUM3_AVX2_polyveck_reduce(polyveck *v);
void PQCLEAN_DILITHIUM3_AVX2_polyveck_csubq(polyveck *v);
void PQCLEAN_DILITHIUM3_AVX2_polyveck_freeze(polyveck *v);

void PQCLEAN_DILITHIUM3_AVX2_polyveck_add(
    polyveck *w, const polyveck *u, const polyveck *v);
void PQCLEAN_DILITHIUM3_AVX2_polyveck_sub(
    polyveck *w, const polyveck *u, const polyveck *v);
void PQCLEAN_DILITHIUM3_AVX2_polyveck_shiftl(polyveck *v);

void PQCLEAN_DILITHIUM3_AVX2_polyveck_ntt(polyveck *v);
void PQCLEAN_DILITHIUM3_AVX2_polyveck_invntt_montgomery(polyveck *v);

int PQCLEAN_DILITHIUM3_AVX2_polyveck_chknorm(
    const polyveck *v, uint32_t B);

void PQCLEAN_DILITHIUM3_AVX2_polyveck_power2round(
    polyveck *v1, polyveck *v0, const polyveck *v);
void PQCLEAN_DILITHIUM3_AVX2_polyveck_decompose(
    polyveck *v1, polyveck *v0, const polyveck *v);
uint32_t PQCLEAN_DILITHIUM3_AVX2_polyveck_make_hint(
    polyveck *h,
    const polyveck *v0,
    const polyveck *v1);
void PQCLEAN_DILITHIUM3_AVX2_polyveck_use_hint(
    polyveck *w, const polyveck *v, const polyveck *h);

#endif
