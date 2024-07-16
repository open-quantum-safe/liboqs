// SPDX-License-Identifier: MIT
#include "endian.h"

void put_bigendian( void *target, unsigned long long value, size_t bytes ) {
    unsigned char *b = target;
    int i;

    for (i = (int)(bytes-1); i >= 0; i--) {
        b[i] = value & 0xff;
        value >>= 8;
    }
}

unsigned long long get_bigendian( const void *target, size_t bytes ) {
    const unsigned char *b = target;
    unsigned long long result = 0;
    size_t i;

    for (i=0; i<bytes; i++) {
        result = 256 * result + (b[i] & 0xff);
    }

    return result;
}
