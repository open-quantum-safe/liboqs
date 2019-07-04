/*---------------------------------------------------------------------
This file has been adapted from the implementation
(available at, Public Domain https://github.com/pq-crystals/kyber)
of "CRYSTALS – Kyber: a CCA-secure module-lattice-based KEM"
by : Joppe Bos, Leo Ducas, Eike Kiltz, Tancrede Lepoint,
Vadim Lyubashevsky, John M. Schanck, Peter Schwabe & Damien stehle
----------------------------------------------------------------------*/
#include "SABER_params.h"
#include "cbd.h"
#include "fips202.h"
#include "poly.h"

void PQCLEAN_FIRESABER_CLEAN_GenSecret(uint16_t r[SABER_K][SABER_N], const unsigned char *seed) {
    uint8_t buf[SABER_MU * SABER_N * SABER_K / 8];

    shake128(buf, sizeof(buf), seed, SABER_NOISESEEDBYTES);

    for (size_t i = 0; i < SABER_K; i++) {
        PQCLEAN_FIRESABER_CLEAN_cbd(r[i], buf + i * SABER_MU * SABER_N / 8);
    }
}
