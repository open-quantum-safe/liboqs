#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "fips202.h"
#include "params.h"

#include <stdlib.h>

void PQCLEAN_KYBER1024_CLEAN_kyber_shake128_absorb(shake128ctx *s, const unsigned char *input, unsigned char x, unsigned char y);
void PQCLEAN_KYBER1024_CLEAN_kyber_shake128_squeezeblocks(unsigned char *output, size_t nblocks, shake128ctx *s);
void PQCLEAN_KYBER1024_CLEAN_shake256_prf(unsigned char *output, size_t outlen, const unsigned char *key, unsigned char nonce);

#define hash_h(OUT, IN, INBYTES) sha3_256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha3_512(OUT, IN, INBYTES)
#define xof_absorb(STATE, IN, X, Y) PQCLEAN_KYBER1024_CLEAN_kyber_shake128_absorb(STATE, IN, X, Y)
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_KYBER1024_CLEAN_kyber_shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) PQCLEAN_KYBER1024_CLEAN_shake256_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) shake256(OUT, KYBER_SSBYTES, IN, INBYTES)

#define XOF_BLOCKBYTES 168

typedef shake128ctx xof_state;

#endif /* SYMMETRIC_H */
