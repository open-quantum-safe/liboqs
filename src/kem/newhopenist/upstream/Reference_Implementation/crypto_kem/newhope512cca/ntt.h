#ifndef NTT_H
#define NTT_H

#include "inttypes.h"

extern uint16_t omegas_bitrev_montgomery[];
extern uint16_t omegas_inv_bitrev_montgomery[];

extern uint16_t psis_bitrev_montgomery[];
extern uint16_t psis_inv_montgomery[];

void bitrev_vector(uint16_t* poly);
void mul_coefficients(uint16_t* poly, const uint16_t* factors);
void ntt(uint16_t* poly, const uint16_t* omegas);

#endif
