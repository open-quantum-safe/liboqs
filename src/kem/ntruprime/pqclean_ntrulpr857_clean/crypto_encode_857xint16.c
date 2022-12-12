#include "crypto_encode_857xint16.h"


void PQCLEAN_NTRULPR857_CLEAN_crypto_encode_857xint16(unsigned char *s, const void *v) {
    const uint16_t *x = v;
    int i;

    for (i = 0; i < 857; ++i) {
        uint16_t u = *x++;
        *s++ = (unsigned char) u;
        *s++ = (unsigned char) (u >> 8);
    }
}
