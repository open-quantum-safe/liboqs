#include "crypto_encode_653xint16.h"


void PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653xint16(unsigned char *s, const void *v) {
    const uint16_t *x = v;
    int i;

    for (i = 0; i < 653; ++i) {
        uint16_t u = *x++;
        *s++ = (unsigned char) u;
        *s++ = (unsigned char) (u >> 8);
    }
}
