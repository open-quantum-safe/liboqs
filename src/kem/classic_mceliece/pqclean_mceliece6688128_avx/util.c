/*
  This file is for loading/storing data in a little-endian fashion
*/

#include "util.h"

#include "vec128.h"

#include <stdint.h>

void PQCLEAN_MCELIECE6688128_AVX_store_i(unsigned char *out, uint64_t in, int i) {
    int j;

    for (j = 0; j < i; j++) {
        out[j] = (in >> (j * 8)) & 0xFF;
    }
}

void PQCLEAN_MCELIECE6688128_AVX_store2(unsigned char *dest, uint16_t a) {
    dest[0] = a & 0xFF;
    dest[1] = a >> 8;
}

uint16_t PQCLEAN_MCELIECE6688128_AVX_load2(const unsigned char *src) {
    uint16_t a;

    a = src[1];
    a <<= 8;
    a |= src[0];

    return a & GFMASK;
}

uint32_t PQCLEAN_MCELIECE6688128_AVX_load4(const unsigned char *src) {
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

void PQCLEAN_MCELIECE6688128_AVX_irr_load(vec128 *out, const unsigned char *in) {
    int i, j;
    uint64_t v[2];
    uint16_t irr[ SYS_T + 1 ];

    for (i = 0; i < SYS_T; i++) {
        irr[i] = PQCLEAN_MCELIECE6688128_AVX_load2(in + i * 2);
        irr[i] &= GFMASK;
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

        out[i] = PQCLEAN_MCELIECE6688128_AVX_vec128_set2x(v[0], v[1]);
    }
}

void PQCLEAN_MCELIECE6688128_AVX_store8(unsigned char *out, uint64_t in) {
    out[0] = (in >> 0x00) & 0xFF;
    out[1] = (in >> 0x08) & 0xFF;
    out[2] = (in >> 0x10) & 0xFF;
    out[3] = (in >> 0x18) & 0xFF;
    out[4] = (in >> 0x20) & 0xFF;
    out[5] = (in >> 0x28) & 0xFF;
    out[6] = (in >> 0x30) & 0xFF;
    out[7] = (in >> 0x38) & 0xFF;
}

uint64_t PQCLEAN_MCELIECE6688128_AVX_load8(const unsigned char *in) {
    int i;
    uint64_t ret = in[7];

    for (i = 6; i >= 0; i--) {
        ret <<= 8;
        ret |= in[i];
    }

    return ret;
}

vec128 PQCLEAN_MCELIECE6688128_AVX_load16(const unsigned char *in) {
    return PQCLEAN_MCELIECE6688128_AVX_vec128_set2x( PQCLEAN_MCELIECE6688128_AVX_load8(in), PQCLEAN_MCELIECE6688128_AVX_load8(in + 8) );
}

void PQCLEAN_MCELIECE6688128_AVX_store16(unsigned char *out, vec128 in) {
    PQCLEAN_MCELIECE6688128_AVX_store8(out + 0, PQCLEAN_MCELIECE6688128_AVX_vec128_extract(in, 0));
    PQCLEAN_MCELIECE6688128_AVX_store8(out + 8, PQCLEAN_MCELIECE6688128_AVX_vec128_extract(in, 1));
}
