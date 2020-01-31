/** Ring arithmetic implementation */
#include "ring.h"

/** Return the i'th limb of the modulus */
limb_t PQCLEAN_PAPABEAR_CLEAN_modulus(size_t i) {
    return (i == DIGITS / 2) ? LMASK - 1 : LMASK;
}

/** Multiply and accumulate c += a*b */
void PQCLEAN_PAPABEAR_CLEAN_mac(gf_t c, const gf_t a, const gf_t b) {
    /* Reference non-Karatsuba MAC */
    dslimb_t accum[2 * DIGITS] = {0};
    dslimb_t chain;
    size_t i, j;

    /* Initialize accumulator = unclarify(c) */
    for (i = 0; i < DIGITS; i++) {
        accum[i + DIGITS / 2] = c[i];
    }

    /* Multiply */
    for (i = 0; i < DIGITS; i++) {
        for (j = 0; j < DIGITS; j++) {
            accum[i + j] += (dslimb_t)a[i] * b[j];
        }
    }

    /* Clarify and reduce */
    for (i = 0; i < DIGITS / 2; i++) {
        accum[i + DIGITS / 2] -= accum[i];
        accum[i + DIGITS]   += accum[i];
        accum[i + DIGITS / 2] += accum[i + 3 * DIGITS / 2];
        accum[i + DIGITS]   += accum[i + 3 * DIGITS / 2];
    }

    /* Carry propagate */
    chain = accum[3 * DIGITS / 2 - 1];
    accum[3 * DIGITS / 2 - 1] = chain & LMASK;
    chain >>= LGX;
    accum[DIGITS] += chain;
    for (i = DIGITS / 2; i < 3 * DIGITS / 2; i++) {
        chain += accum[i];
        c[i - DIGITS / 2] = chain & LMASK;
        chain >>= LGX;
    }
    c[0] = (limb_t) (c[0] + chain);
    c[DIGITS / 2] = (limb_t) (c[DIGITS / 2] + chain);
}

/** Reduce a gf_t to canonical form, i.e. strictly less than N. */
void PQCLEAN_PAPABEAR_CLEAN_canon(gf_t c) {
    const limb_t DELTA = (limb_t)1 << (LGX - 1);
    slimb_t hi;
    dslimb_t scarry;
    dlimb_t carry;

    /* Reduce to 0..2p */
    hi = (slimb_t) (c[DIGITS - 1] - DELTA);
    c[DIGITS - 1] = (limb_t) ((hi & LMASK) + DELTA);
    c[DIGITS / 2] = (limb_t) (c[DIGITS / 2] + (hi >> LGX));

    /* Strong reduce.  First subtract modulus */
    scarry = hi >> LGX;
    for (size_t i = 0; i < DIGITS; i++) {
        scarry = scarry + (slimb_t)c[i] - PQCLEAN_PAPABEAR_CLEAN_modulus(i);
        c[i] = scarry & LMASK;
        scarry >>= LGX;
    }

    /* add it back */
    carry = 0;
    for (size_t i = 0; i < DIGITS; i++) {
        carry = carry + c[i] + ((dlimb_t)scarry & PQCLEAN_PAPABEAR_CLEAN_modulus(i));
        c[i] = carry & LMASK;
        carry >>= LGX;
    }
}

/** Serialize a gf_t to bytes */
void PQCLEAN_PAPABEAR_CLEAN_contract(uint8_t ch[GF_BYTES], gf_t a) {
    size_t pos;

    PQCLEAN_PAPABEAR_CLEAN_canon(a);
    for (size_t i = 0; i < GF_BYTES; i++) {
        pos = (i * 8) / LGX;
        ch[i] = (uint8_t)(a[pos] >> ((i * 8) % LGX));
        if (i < GF_BYTES - 1) {
            ch[i] |= (uint8_t)(a[pos + 1] << (LGX - ((i * 8) % LGX)));
        }
    }
}

/** Deserialize a gf_t from bytes */
void PQCLEAN_PAPABEAR_CLEAN_expand(gf_t ll, const uint8_t ch[GF_BYTES]) {
    limb_t tmp, buffer = 0;

    for (size_t i = 0, j = 0, bbits = 0; i < GF_BYTES; i++) {
        tmp = ch[i];
        buffer |= (limb_t)(tmp << bbits);
        bbits += 8;
        if (bbits >= LGX) {
            ll[j++] = buffer & LMASK;
            buffer = (limb_t)(tmp >> (LGX - (bbits - 8)));
            bbits = bbits - LGX;
        }
    }
}
