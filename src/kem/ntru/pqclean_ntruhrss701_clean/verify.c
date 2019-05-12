#include <stdint.h>
#include <stdlib.h>

#include "verify.h"

/* returns 0 for equal strings, 1 for non-equal strings */
unsigned char PQCLEAN_NTRUHRSS701_CLEAN_verify(const unsigned char *a, const unsigned char *b, size_t len) {
    uint64_t r;
    size_t i;

    r = 0;
    for (i = 0; i < len; i++) {
        r |= a[i] ^ b[i];
    }

    r = (~r + 1); // Two's complement
    r >>= 63;
    return (unsigned char)r;
}

/* b = 1 means mov, b = 0 means don't mov*/
void PQCLEAN_NTRUHRSS701_CLEAN_cmov(unsigned char *r, const unsigned char *x, size_t len, unsigned char b) {
    size_t i;

    b = (~b + 1); // Two's complement
    for (i = 0; i < len; i++) {
        r[i] ^= b & (x[i] ^ r[i]);
    }
}
