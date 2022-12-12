#include "crypto_decode_256x2.h"


void PQCLEAN_NTRULPR1277_CLEAN_crypto_decode_256x2(void *v, const unsigned char *s) {
    unsigned char *r = v;
    int i;
    for (i = 0; i < 256; ++i) {
        r[i] = 1 & (s[i >> 3] >> (i & 7));
    }
}
