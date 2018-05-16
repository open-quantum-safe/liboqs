/*---------------------------------------------------------------------
This file has been adapted from the implementation 
(available at, Public Domain https://github.com/pq-crystals/kyber) 
of "CRYSTALS – Kyber: a CCA-secure module-lattice-based KEM"
by : Joppe Bos, Leo Ducas, Eike Kiltz, Tancrede Lepoint, 
Vadim Lyubashevsky, John M. Schanck, Peter Schwabe & Damien stehle
----------------------------------------------------------------------*/

#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "SABER_params.h"

typedef struct
{
  uint16_t coeffs[SABER_N];
} poly;

typedef struct{
  poly vec[SABER_K];
} polyvec;

void GenSecret(uint16_t r[SABER_K][SABER_N],const unsigned char *seed);

#endif
