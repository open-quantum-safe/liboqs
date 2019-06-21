#ifndef NTT_H
#define NTT_H

#include "inttypes.h"

extern const uint16_t PQCLEAN_NEWHOPE1024CCA_CLEAN_omegas_inv_bitrev_montgomery[];
extern const uint16_t PQCLEAN_NEWHOPE1024CCA_CLEAN_gammas_bitrev_montgomery[];
extern const uint16_t PQCLEAN_NEWHOPE1024CCA_CLEAN_gammas_inv_montgomery[];

void PQCLEAN_NEWHOPE1024CCA_CLEAN_bitrev_vector(uint16_t *poly);
void PQCLEAN_NEWHOPE1024CCA_CLEAN_mul_coefficients(uint16_t *poly, const uint16_t *factors);
void PQCLEAN_NEWHOPE1024CCA_CLEAN_ntt(uint16_t *a, const uint16_t *omegas);

#endif
