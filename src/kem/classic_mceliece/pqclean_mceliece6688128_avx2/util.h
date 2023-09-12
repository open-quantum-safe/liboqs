#ifndef UTIL_H
#define UTIL_H
/*
  This file is for loading/storing data in a little-endian fashion
*/

#include "namespace.h"

#include "vec128.h"

#include <stdint.h>

static inline void store_i(unsigned char *out, uint64_t in, int i) {
    int j;

    for (j = 0; j < i; j++) {
        out[j] = (in >> (j * 8)) & 0xFF;
    }
}

static inline void store_gf(unsigned char *dest, uint16_t a) {
    dest[0] = a & 0xFF;
    dest[1] = a >> 8;
}

static inline uint16_t load_gf(const unsigned char *src) {
    uint16_t a;

    a = src[1];
    a <<= 8;
    a |= src[0];

    return a & GFMASK;
}

static inline uint32_t load4(const unsigned char *src) {
    uint32_t a;

    a  = src[3];
    a <<= 8;
    a |= src[2];
    a <<= 8;
    a |= src[1];
    a <<= 8;
    a |= src[0];

    return a;
}

static inline void irr_load(vec128 *out, const unsigned char *in) {
    int i, j;
    uint64_t v[2];
    uint16_t irr[ SYS_T + 1 ];

    for (i = 0; i < SYS_T; i++) {
        irr[i] = load_gf(in + i * 2);
    }

    irr[ SYS_T ] = 1;

    for (i = 0; i < GFBITS; i++) {
        v[0] = v[1] = 0;

        for (j =    63; j >=  0; j--) {
            v[0] <<= 1;
            v[0] |= (irr[j] >> i) & 1;
        }
        for (j = SYS_T; j >= 64; j--) {
            v[1] <<= 1;
            v[1] |= (irr[j] >> i) & 1;
        }

        out[i] = vec128_set2x(v[0], v[1]);
    }
}

static inline void store8(unsigned char *out, uint64_t in) {
    out[0] = (in >> 0x00) & 0xFF;
    out[1] = (in >> 0x08) & 0xFF;
    out[2] = (in >> 0x10) & 0xFF;
    out[3] = (in >> 0x18) & 0xFF;
    out[4] = (in >> 0x20) & 0xFF;
    out[5] = (in >> 0x28) & 0xFF;
    out[6] = (in >> 0x30) & 0xFF;
    out[7] = (in >> 0x38) & 0xFF;
}

static inline uint64_t load8(const unsigned char *in) {
    int i;
    uint64_t ret = in[7];

    for (i = 6; i >= 0; i--) {
        ret <<= 8;
        ret |= in[i];
    }

    return ret;
}

static inline vec128 load16(const unsigned char *in) {
    return vec128_set2x( load8(in), load8(in + 8) );
}

static inline void store16(unsigned char *out, vec128 in) {
    store8(out + 0, vec128_extract(in, 0));
    store8(out + 8, vec128_extract(in, 1));
}

#endif
