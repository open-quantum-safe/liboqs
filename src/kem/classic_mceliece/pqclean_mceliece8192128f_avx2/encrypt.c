#define syndrome_asm CRYPTO_NAMESPACE(syndrome_asm)
#define _syndrome_asm _CRYPTO_NAMESPACE(syndrome_asm)
/*
  This file is for Niederreiter encryption
*/

#include "util.h"
#include "int32_sort.h"
#include "params.h"
#include "randombytes.h"

#include "crypto_declassify.h"
#include "crypto_uint32.h"
#include <stdint.h>

/* include last because of conflict with unistd.h's encrypt function */
#include "encrypt.h"

static inline crypto_uint32 uint32_is_equal_declassify(uint32_t t, uint32_t u) {
    crypto_uint32 mask = crypto_uint32_equal_mask(t, u);
    crypto_declassify(&mask, sizeof mask);
    return mask;
}

/* input: public key pk, error vector e */
/* output: syndrome s */
extern void syndrome_asm(unsigned char *s, const unsigned char *pk, unsigned char *e);

/* output: e, an error vector of weight t */
static void gen_e(unsigned char *e) {
    int i, j, eq;

    int32_t ind[ SYS_T ]; // can also use uint16 or int16
    unsigned char bytes[ SYS_T * 2 ];
    uint64_t e_int[ SYS_N / 64 ];
    uint64_t one = 1;
    uint64_t mask;
    uint64_t val[ SYS_T ];

    while (1) {
        randombytes(bytes, sizeof(bytes));

        for (i = 0; i < SYS_T; i++) {
            ind[i] = load_gf(bytes + i * 2);
        }

        // check for repetition

        int32_sort(ind, SYS_T);

        eq = 0;
        for (i = 1; i < SYS_T; i++) {
            if (uint32_is_equal_declassify(ind[i - 1], ind[i])) {
                eq = 1;
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
        store8(e + i * 8, e_int[i]);
    }
}

/* input: public key pk */
/* output: error vector e, syndrome s */
void encrypt(unsigned char *s, const unsigned char *pk, unsigned char *e) {
    gen_e(e);

    syndrome_asm(s, pk, e);
}
