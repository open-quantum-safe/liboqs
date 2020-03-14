#include "util.h"

void PQCLEAN_MCELIECE8192128F_VEC_store2(unsigned char *dest, uint16_t a) {
    dest[0] = a & 0xFF;
    dest[1] = a >> 8;
}

uint16_t PQCLEAN_MCELIECE8192128F_VEC_load2(const unsigned char *src) {
    uint16_t a;

    a = src[1];
    a <<= 8;
    a |= src[0];

    return a & GFMASK;
}

uint32_t PQCLEAN_MCELIECE8192128F_VEC_load4(const unsigned char *src) {
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

void PQCLEAN_MCELIECE8192128F_VEC_irr_load(vec out[][GFBITS], const unsigned char *in) {
    int i, j;
    uint64_t v0 = 0, v1 = 0;
    uint16_t irr[ SYS_T ];

    for (i = 0; i < SYS_T; i++) {
        irr[i] = PQCLEAN_MCELIECE8192128F_VEC_load2(in + i * 2);
        irr[i] &= GFMASK;
    }

    for (i = 0; i < GFBITS; i++) {
        for (j = 63; j >= 0; j--) {
            v0 <<= 1;
            v1 <<= 1;
            v0 |= (irr[j] >> i) & 1;
            v1 |= (irr[j + 64] >> i) & 1;
        }

        out[0][i] = v0;
        out[1][i] = v1;
    }
}

void PQCLEAN_MCELIECE8192128F_VEC_store8(unsigned char *out, uint64_t in) {
    out[0] = (in >> 0x00) & 0xFF;
    out[1] = (in >> 0x08) & 0xFF;
    out[2] = (in >> 0x10) & 0xFF;
    out[3] = (in >> 0x18) & 0xFF;
    out[4] = (in >> 0x20) & 0xFF;
    out[5] = (in >> 0x28) & 0xFF;
    out[6] = (in >> 0x30) & 0xFF;
    out[7] = (in >> 0x38) & 0xFF;
}

uint64_t PQCLEAN_MCELIECE8192128F_VEC_load8(const unsigned char *in) {
    int i;
    uint64_t ret = in[7];

    for (i = 6; i >= 0; i--) {
        ret <<= 8;
        ret |= in[i];
    }

    return ret;
}
