#include "crypto_decode_int16.h"


void PQCLEAN_SNTRUP761_AVX2_crypto_decode_int16(void *x, const unsigned char *s) {
    uint16_t u0 = s[0];
    uint16_t u1 = s[1];
    u1 = (uint16_t) (u1 << 8);
    *(uint16_t *) x = u0 | u1;
}
