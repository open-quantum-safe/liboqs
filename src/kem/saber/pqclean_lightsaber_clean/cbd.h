#ifndef CBD_H
#define CBD_H

/*---------------------------------------------------------------------
This file has been adapted from the implementation
(available at, Public Domain https://github.com/pq-crystals/kyber)
of "CRYSTALS – Kyber: a CCA-secure module-lattice-based KEM"
by : Joppe Bos, Leo Ducas, Eike Kiltz, Tancrede Lepoint,
Vadim Lyubashevsky, John M. Schanck, Peter Schwabe & Damien stehle
----------------------------------------------------------------------*/

#include "poly.h"
#include <stdint.h>

void PQCLEAN_LIGHTSABER_CLEAN_cbd(uint16_t *r, const unsigned char *buf);

#endif
