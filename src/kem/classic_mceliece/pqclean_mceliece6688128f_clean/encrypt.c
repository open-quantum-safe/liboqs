/*
  This file is for Niederreiter encryption
*/

#include "util.h"
#include "params.h"
#include "randombytes.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "crypto_declassify.h"
#include "crypto_uint16.h"
#include "crypto_uint32.h"
#include "gf.h"

/* include last because of conflict with unistd.h encrypt function */
#include "encrypt.h"

static inline crypto_uint16 uint16_is_smaller_declassify(uint16_t t, uint16_t u) {
    crypto_uint16 mask = crypto_uint16_smaller_mask(t, u);
    crypto_declassify(&mask, sizeof mask);
    return mask;
}

static inline crypto_uint32 uint32_is_equal_declassify(uint32_t t, uint32_t u) {
    crypto_uint32 mask = crypto_uint32_equal_mask(t, u);
    crypto_declassify(&mask, sizeof mask);
    return mask;
}

static inline unsigned char same_mask(uint16_t x, uint16_t y) {
    uint32_t mask;

    mask = x ^ y;
    mask -= 1;
    mask >>= 31;
    mask = -mask;

    return mask & 0xFF;
}

/* output: e, an error vector of weight t */
static void gen_e(unsigned char *e) {
    int i, j, eq, count;

    union {
        uint16_t nums[ SYS_T * 2 ];
        unsigned char bytes[ SYS_T * 2 * sizeof(uint16_t) ];
    } buf;

    uint16_t ind[ SYS_T ];
    unsigned char mask;
    unsigned char val[ SYS_T ];

    while (1) {
        randombytes(buf.bytes, sizeof(buf));

        for (i = 0; i < SYS_T * 2; i++) {
            buf.nums[i] = load_gf(buf.bytes + i * 2);
        }

        // moving and counting indices in the correct range

        count = 0;
        for (i = 0; i < SYS_T * 2 && count < SYS_T; i++) {
            if (uint16_is_smaller_declassify(buf.nums[i], SYS_N)) {
                ind[ count++ ] = buf.nums[i];
            }
        }

        if (count < SYS_T) {
            continue;
        }

        // check for repetition

        eq = 0;

        for (i = 1; i < SYS_T; i++) {
            for (j = 0; j < i; j++) {
                if (uint32_is_equal_declassify(ind[i], ind[j])) {
                    eq = 1;
                }
            }
        }

        if (eq == 0) {
            break;
        }
    }

    for (j = 0; j < SYS_T; j++) {
        val[j] = 1 << (ind[j] & 7);
    }

    for (i = 0; i < SYS_N / 8; i++) {
        e[i] = 0;

        for (j = 0; j < SYS_T; j++) {
            mask = same_mask((uint16_t)i, ind[j] >> 3);

            e[i] |= val[j] & mask;
        }
    }
}

/* input: public key pk, error vector e */
/* output: syndrome s */
static void syndrome(unsigned char *s, const unsigned char *pk, const unsigned char *e) {
    unsigned char b, row[SYS_N / 8];
    const unsigned char *pk_ptr = pk;

    int i, j;

    for (i = 0; i < SYND_BYTES; i++) {
        s[i] = 0;
    }

    for (i = 0; i < PK_NROWS; i++) {
        for (j = 0; j < SYS_N / 8; j++) {
            row[j] = 0;
        }

        for (j = 0; j < PK_ROW_BYTES; j++) {
            row[ SYS_N / 8 - PK_ROW_BYTES + j ] = pk_ptr[j];
        }

        row[i / 8] |= 1 << (i % 8);

        b = 0;
        for (j = 0; j < SYS_N / 8; j++) {
            b ^= row[j] & e[j];
        }

        b ^= b >> 4;
        b ^= b >> 2;
        b ^= b >> 1;
        b &= 1;

        s[ i / 8 ] |= (b << (i % 8));

        pk_ptr += PK_ROW_BYTES;
    }
}

void encrypt(unsigned char *s, const unsigned char *pk, unsigned char *e) {
    gen_e(e);

    syndrome(s, pk, e);
}
