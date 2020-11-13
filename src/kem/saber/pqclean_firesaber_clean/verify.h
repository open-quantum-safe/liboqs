#ifndef VERIFY_H
#define VERIFY_H
/*-------------------------------------------------
This file has been adapted from the implementation
(available at https://github.com/pq-crystals/kyber) of
"CRYSTALS – Kyber: a CCA-secure module-lattice-based KEM"
 by : Joppe Bos, Leo Ducas, Eike Kiltz, Tancrede Lepoint,
Vadim Lyubashevsky, John M. Schanck, Peter Schwabe & Damien stehle
----------------------------------------------------*/

#include <stddef.h>
#include <stdint.h>

/* returns 0 for equal strings, 1 for non-equal strings */
uint8_t PQCLEAN_FIRESABER_CLEAN_verify(const uint8_t *a, const uint8_t *b, size_t len);


/* b = 1 means mov, b = 0 means don't mov*/
void PQCLEAN_FIRESABER_CLEAN_cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b);


#endif
