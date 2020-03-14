/*
  This file is for Niederreiter encryption
*/

#include "encrypt.h"

#include "params.h"
#include "randombytes.h"
#include "util.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "gf.h"

/* output: e, an error vector of weight t */
static void gen_e(unsigned char *e) {
    size_t i, j;
    int eq, count;

    uint16_t ind[ SYS_T * 2 ];
    uint8_t *ind8 = (uint8_t *)ind;
    uint32_t ind32[ SYS_T * 2 ];
    uint64_t e_int[ (SYS_N + 63) / 64 ];
    uint64_t one = 1;
    uint64_t mask;
    uint64_t val[ SYS_T ];

    while (1) {
        randombytes(ind8, sizeof(ind));
        for (i = 0; i < sizeof(ind); i += 2) {
            ind[i / 2] = (uint16_t)ind8[i + 1] << 8 | ind8[i];
        }

        for (i = 0; i < SYS_T * 2; i++) {
            ind[i] &= GFMASK;
        }

        // moving and counting indices in the correct range

        count = 0;
        for (i = 0; i < SYS_T * 2; i++) {
            if (ind[i] < SYS_N) {
                ind32[ count++ ] = ind[i];
            }
        }

        if (count < SYS_T) {
            continue;
        }

        // check for repetition

        eq = 0;

        for (i = 1; i < SYS_T; i++) {
            for (j = 0; j < i; j++) {
                if (ind32[i] == ind32[j]) {
                    eq = 1;
                }
            }
        }

        if (eq == 0) {
            break;
        }
    }

    for (j = 0; j < SYS_T; j++) {
        val[j] = one << (ind32[j] & 63);
    }

    for (i = 0; i < (SYS_N + 63) / 64; i++) {
        e_int[i] = 0;

        for (j = 0; j < SYS_T; j++) {
            mask = i ^ (ind32[j] >> 6);
            mask -= 1;
            mask >>= 63;
            mask = -mask;

            e_int[i] |= val[j] & mask;
        }
    }

    for (i = 0; i < (SYS_N + 63) / 64 - 1; i++) {
        PQCLEAN_MCELIECE6688128_VEC_store8(e, e_int[i]);
        e += 8;
    }

    for (j = 0; j < (SYS_N % 64); j += 8) {
        e[ j / 8 ] = (e_int[i] >> j) & 0xFF;
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
            b ^= PQCLEAN_MCELIECE6688128_VEC_load8(pk_ptr8 + 8 * j) & PQCLEAN_MCELIECE6688128_VEC_load8(e_ptr8 + 8 * j);
        }

        b ^= PQCLEAN_MCELIECE6688128_VEC_load4(pk_ptr8 + 8 * j) & PQCLEAN_MCELIECE6688128_VEC_load4(e_ptr8 + 8 * j);

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
void PQCLEAN_MCELIECE6688128_VEC_encrypt(unsigned char *s, unsigned char *e, const unsigned char *pk) {
    gen_e(e);
    syndrome(s, pk, e);
}

