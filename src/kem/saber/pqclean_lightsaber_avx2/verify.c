#include "verify.h"

/*-------------------------------------------------
This file has been adapted from the implementation
(available at https://github.com/pq-crystals/kyber) of
"CRYSTALS – Kyber: a CCA-secure module-lattice-based KEM"
 by : Joppe Bos, Leo Ducas, Eike Kiltz, Tancrede Lepoint,
Vadim Lyubashevsky, John M. Schanck, Peter Schwabe & Damien stehle
----------------------------------------------------*/


/* returns 0 for equal strings, 1 for non-equal strings */
uint8_t PQCLEAN_LIGHTSABER_AVX2_verify(const uint8_t *a, const uint8_t *b, size_t len) {
    uint64_t r;
    size_t i;
    r = 0;

    for (i = 0; i < len; i++) {
        r |= a[i] ^ b[i];
    }

    r = (~r + 1); // Two's complement
    r >>= 63;
    return (uint8_t) r;
}

/* b = 1 means mov, b = 0 means don't mov*/
void PQCLEAN_LIGHTSABER_AVX2_cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b) {
    size_t i;

    b = -b;
    for (i = 0; i < len; i++) {
        r[i] ^= b & (x[i] ^ r[i]);
    }
}
