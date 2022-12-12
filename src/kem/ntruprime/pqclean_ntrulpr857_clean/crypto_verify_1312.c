#include "crypto_verify_1312.h"


int PQCLEAN_NTRULPR857_CLEAN_crypto_verify_1312(const unsigned char *x, const unsigned char *y) {
    unsigned int differentbits = 0;
    int i;

    for (i = 0; i < PQCLEAN_NTRULPR857_CLEAN_crypto_verify_1312_BYTES; ++i) {
        differentbits |= x[i] ^ y[i];
    }

    return (int) (1 & ((differentbits - 1) >> 8)) - 1;
}
