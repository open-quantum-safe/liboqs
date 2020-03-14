/*
  This file is for Niederreiter encryption
*/

#include "encrypt.h"

#include "params.h"
#include "randombytes.h"
#include "util.h"

#include <stdint.h>

/* output: e, an error vector of weight t */
static void gen_e(unsigned char *e) {
    size_t i, j;
    int eq;

    uint16_t ind[ SYS_T ];
    uint8_t *ind8 = (uint8_t *)ind;
    uint64_t e_int[ SYS_N / 64 ];
    uint64_t one = 1;
    uint64_t mask;
    uint64_t val[ SYS_T ];

    while (1) {
        randombytes(ind8, sizeof(ind));
        for (i = 0; i < sizeof(ind); i += 2) {
            ind[i / 2] = (uint16_t)ind8[i + 1] << 8 | ind8[i];
        }

        for (i = 0; i < SYS_T; i++) {
            ind[i] &= GFMASK;
        }

        // check for repetition

        eq = 0;

        for (i = 1; i < SYS_T; i++) {
            for (j = 0; j < i; j++) {
                if (ind[i] == ind[j]) {
                    eq = 1;
                }
            }
        }

        if (eq == 0) {
            break;
        }
    }

    for (j = 0; j < SYS_T; j++) {
        val[j] = one << (ind[j] & 63);
    }

    for (i = 0; i < SYS_N / 64; i++) {
        e_int[i] = 0;

        for (j = 0; j < SYS_T; j++) {
            mask = i ^ (ind[j] >> 6);
            mask -= 1;
            mask >>= 63;
            mask = -mask;

            e_int[i] |= val[j] & mask;
        }
    }

    for (i = 0; i < SYS_N / 64; i++) {
        PQCLEAN_MCELIECE8192128_VEC_store8(e + i * 8, e_int[i]);
    }
}

/* input: public key pk, error vector e */
/* output: syndrome s */
static void syndrome(unsigned char *s, const unsigned char *pk, const unsigned char *e) {
    uint64_t b;

    const uint8_t *e_ptr8 = e + SYND_BYTES;
    const uint8_t *pk_ptr8;

    int i, j;

    //

    for (i = 0; i < SYND_BYTES; i++) {
        s[i] = e[i];
    }

    for (i = 0; i < PK_NROWS; i++) {
        pk_ptr8 = pk + PK_ROW_BYTES * i;

        b = 0;
        for (j = 0; j < PK_NCOLS / 64; j++) {
            b ^= PQCLEAN_MCELIECE8192128_VEC_load8(pk_ptr8 + 8 * j) & PQCLEAN_MCELIECE8192128_VEC_load8(e_ptr8 + 8 * j);
        }

        b ^= b >> 32;
        b ^= b >> 16;
        b ^= b >> 8;
        b ^= b >> 4;
        b ^= b >> 2;
        b ^= b >> 1;
        b &= 1;

        s[ i / 8 ] ^= (b << (i % 8));
    }
}

/* input: public key pk */
/* output: error vector e, syndrome s */
void PQCLEAN_MCELIECE8192128_VEC_encrypt(unsigned char *s, unsigned char *e, const unsigned char *pk) {
    gen_e(e);
    syndrome(s, pk, e);
}

