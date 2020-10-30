#include "crypto_encode_int16.h"

#define uint16 uint16_t

void PQCLEAN_SNTRUP761_CLEAN_crypto_encode_int16(unsigned char *s, const void *x) {
    uint16 u = *(const uint16 *) x;
    s[0] = (unsigned char) u;
    s[1] = (unsigned char) (u >> 8);
}
