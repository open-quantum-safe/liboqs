#include "crypto_encode_1277xint16.h"


void PQCLEAN_NTRULPR1277_CLEAN_crypto_encode_1277xint16(unsigned char *s, const void *v) {
    const uint16_t *x = v;
    int i;

    for (i = 0; i < 1277; ++i) {
        uint16_t u = *x++;
        *s++ = (unsigned char) u;
        *s++ = (unsigned char) (u >> 8);
    }
}
