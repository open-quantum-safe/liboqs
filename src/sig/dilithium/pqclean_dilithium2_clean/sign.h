#ifndef PQCLEAN_DILITHIUM2_CLEAN_SIGN_H
#define PQCLEAN_DILITHIUM2_CLEAN_SIGN_H

#include "api.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"

void PQCLEAN_DILITHIUM2_CLEAN_expand_mat(polyvecl mat[K], const uint8_t rho[SEEDBYTES]);
void PQCLEAN_DILITHIUM2_CLEAN_challenge(poly *c, const uint8_t mu[CRHBYTES],
                                        const polyveck *w1);
#endif
