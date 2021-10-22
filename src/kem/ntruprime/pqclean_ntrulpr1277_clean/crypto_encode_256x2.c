#include "crypto_encode_256x2.h"


void PQCLEAN_NTRULPR1277_CLEAN_crypto_encode_256x2(unsigned char *s, const void *v) {
    const unsigned char *r = v;
    int i;
    for (i = 0; i < 32; ++i) {
        s[i] = 0;
    }
    for (i = 0; i < 256; ++i) {
        s[i >> 3] |= (unsigned char) ((r[i] & 1) << (i & 7));
    }
}
