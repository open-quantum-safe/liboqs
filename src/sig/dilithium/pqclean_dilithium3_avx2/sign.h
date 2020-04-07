#ifndef SIGN_H
#define SIGN_H

#include "api.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"

void PQCLEAN_DILITHIUM3_AVX2_expand_mat(polyvecl mat[K], const uint8_t rho[SEEDBYTES]);
void PQCLEAN_DILITHIUM3_AVX2_challenge(poly *c, const uint8_t mu[CRHBYTES],
                                       const polyveck *w1);


#endif

