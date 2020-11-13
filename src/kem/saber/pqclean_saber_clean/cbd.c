#include "SABER_params.h"
#include "api.h"
#include "cbd.h"
#include <stdint.h>
/*---------------------------------------------------------------------
This file has been adapted from the implementation
(available at, Public Domain https://github.com/pq-crystals/kyber)
of "CRYSTALS – Kyber: a CCA-secure module-lattice-based KEM"
by : Joppe Bos, Leo Ducas, Eike Kiltz, Tancrede Lepoint,
Vadim Lyubashevsky, John M. Schanck, Peter Schwabe & Damien stehle
----------------------------------------------------------------------*/


static uint64_t load_littleendian(const uint8_t *x, int bytes) {
    int i;
    uint64_t r = x[0];
    for (i = 1; i < bytes; i++) {
        r |= (uint64_t)x[i] << (8 * i);
    }
    return r;
}

void PQCLEAN_SABER_CLEAN_cbd(uint16_t s[SABER_N], const uint8_t buf[SABER_POLYCOINBYTES]) {
    uint32_t t, d, a[4], b[4];
    int i, j;

    for (i = 0; i < SABER_N / 4; i++) {
        t = (uint32_t) load_littleendian(buf + 4 * i, 4);
        d = 0;
        for (j = 0; j < 4; j++) {
            d += (t >> j) & 0x11111111;
        }

        a[0] = d & 0xf;
        b[0] = (d >> 4) & 0xf;
        a[1] = (d >> 8) & 0xf;
        b[1] = (d >> 12) & 0xf;
        a[2] = (d >> 16) & 0xf;
        b[2] = (d >> 20) & 0xf;
        a[3] = (d >> 24) & 0xf;
        b[3] = (d >> 28);

        s[4 * i + 0] = (uint16_t)(a[0] - b[0]);
        s[4 * i + 1] = (uint16_t)(a[1] - b[1]);
        s[4 * i + 2] = (uint16_t)(a[2] - b[2]);
        s[4 * i + 3] = (uint16_t)(a[3] - b[3]);
    }
}
