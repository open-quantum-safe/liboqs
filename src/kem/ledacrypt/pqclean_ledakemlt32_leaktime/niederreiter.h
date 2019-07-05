#ifndef NIEDERREITER_H
#define NIEDERREITER_H

#include "gf2x_arith_mod_xPplusOne.h"
#include "qc_ldpc_parameters.h"
#include "rng.h"

typedef struct {
    /* raw entropy extracted from TRNG, will be deterministically expanded into
     * H and Q during decryption */
    unsigned char prng_seed[TRNG_BYTE_LENGTH];
    int8_t rejections;
    uint8_t threshold; // for round 2
} privateKeyNiederreiter_t;

typedef struct {
    DIGIT Mtr[(N0 - 1)*NUM_DIGITS_GF2X_ELEMENT];
    // Dense representation of the matrix M=Ln0*L,
    // An array including a sequence of (N0-1) gf2x elements;
    // each gf2x element is stored as a binary polynomial(mod x^P+1)
    // with P coefficients.
} publicKeyNiederreiter_t;

void PQCLEAN_LEDAKEMLT32_LEAKTIME_niederreiter_keygen(publicKeyNiederreiter_t *pk, privateKeyNiederreiter_t *sk, AES_XOF_struct *keys_expander);
void PQCLEAN_LEDAKEMLT32_LEAKTIME_niederreiter_encrypt(DIGIT syndrome[], const publicKeyNiederreiter_t *pk, const DIGIT *err);
int PQCLEAN_LEDAKEMLT32_LEAKTIME_niederreiter_decrypt(DIGIT *err, const privateKeyNiederreiter_t *sk, const DIGIT *syndrome);


#endif
