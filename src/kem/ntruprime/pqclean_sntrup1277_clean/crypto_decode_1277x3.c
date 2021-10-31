#include "crypto_decode_1277x3.h"

#define uint8 uint8_t

#define p 1277

void PQCLEAN_SNTRUP1277_CLEAN_crypto_decode_1277x3(void *v, const unsigned char *s) {
    uint8 *f = v;
    uint8 x;
    int i;

    for (i = 0; i < p / 4; ++i) {
        x = *s++;
        *f++ = ((uint8)(x & 3)) - 1;
        x >>= 2;
        *f++ = ((uint8)(x & 3)) - 1;
        x >>= 2;
        *f++ = ((uint8)(x & 3)) - 1;
        x >>= 2;
        *f++ = ((uint8)(x & 3)) - 1;
    }
    x = *s++;
    *f++ = ((uint8)(x & 3)) - 1;
}
