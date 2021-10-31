#include "crypto_decode_256x16.h"


void PQCLEAN_NTRULPR1277_AVX2_crypto_decode_256x16(void *v, const unsigned char *s) {
    unsigned char *T = v;
    int i;
    for (i = 0; i < 128; ++i) {
        T[2 * i] = s[i] & 15;
        T[2 * i + 1] = s[i] >> 4;
    }
}
