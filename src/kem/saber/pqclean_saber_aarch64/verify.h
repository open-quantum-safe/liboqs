#ifndef VERIFY_H
#define VERIFY_H
/*-------------------------------------------------
This file has been adapted from the implementation
(available at https://github.com/pq-crystals/kyber) of
"CRYSTALS – Kyber: a CCA-secure module-lattice-based KEM"
 by : Joppe Bos, Leo Ducas, Eike Kiltz, Tancrede Lepoint,
Vadim Lyubashevsky, John M. Schanck, Peter Schwabe & Damien stehle
----------------------------------------------------*/
#include "SABER_params.h"
#include <stdio.h>

/* returns 0 for equal strings, 1 for non-equal strings */
#define verify SABER_NAMESPACE(verify)
int verify(const unsigned char *a, const unsigned char *b, size_t len);

/* b = 1 means mov, b = 0 means don't mov*/
#define cmov SABER_NAMESPACE(cmov)
void cmov(unsigned char *r, const unsigned char *x, size_t len, unsigned char b);

#endif
