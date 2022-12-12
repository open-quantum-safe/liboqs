#include "crypto_encode_256x16.h"


void PQCLEAN_NTRULPR761_AVX2_crypto_encode_256x16(unsigned char *s, const void *v) {
    const unsigned char *T = v;
    int i;
    for (i = 0; i < 128; ++i) {
        s[i] = (unsigned char) (T[2 * i] + (T[2 * i + 1] << 4));
    }
}
