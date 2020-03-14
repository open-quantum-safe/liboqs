#ifndef PQCLEAN_MCELIECE6688128F_VEC_CONTROLBITS_H
#define PQCLEAN_MCELIECE6688128F_VEC_CONTROLBITS_H
/*
  This file is for functions required for generating the control bits of the Benes network w.r.t. a random permutation
  see the Lev-Pippenger-Valiant paper https://www.computer.org/csdl/trans/tc/1981/02/06312171.pdf
*/


#include <stdint.h>

void PQCLEAN_MCELIECE6688128F_VEC_sort_63b(int n, uint64_t *x);
void PQCLEAN_MCELIECE6688128F_VEC_controlbits(unsigned char *out, const uint32_t *pi);

#endif

