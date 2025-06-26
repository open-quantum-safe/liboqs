#include "cmov.h"

/* b = 1 means mov, b = 0 means don't mov*/
void PQCLEAN_NTRUHPS2048677_AVX2_cmov(unsigned char *r, const unsigned char *x, size_t len, unsigned char b) {
    size_t i;

    b = (~b + 1);
    for (i = 0; i < len; i++) {
        r[i] ^= b & (x[i] ^ r[i]);
    }
}
