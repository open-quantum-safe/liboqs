#ifndef NIEDERREITER_H
#define NIEDERREITER_H

#include "gf2x_arith_mod_xPplusOne.h"
#include "qc_ldpc_parameters.h"
#include "rng.h"

typedef struct {
    unsigned char prng_seed[TRNG_BYTE_LENGTH];
    uint8_t rejections;
    uint8_t secondIterThreshold;
    uint8_t decryption_failure_secret[TRNG_BYTE_LENGTH];
} privateKeyNiederreiter_t;

typedef struct {
    DIGIT Mtr[(N0 - 1)*NUM_DIGITS_GF2X_ELEMENT];
} publicKeyNiederreiter_t;


void PQCLEAN_LEDAKEMLT32_LEAKTIME_niederreiter_keygen(publicKeyNiederreiter_t *pk, privateKeyNiederreiter_t *sk);
void PQCLEAN_LEDAKEMLT32_LEAKTIME_niederreiter_encrypt(DIGIT syndrome[], const publicKeyNiederreiter_t *pk, const DIGIT err[]);
int PQCLEAN_LEDAKEMLT32_LEAKTIME_niederreiter_decrypt(DIGIT *err, const privateKeyNiederreiter_t *sk, const DIGIT *syndrome);

#endif
