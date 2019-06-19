#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "fips202.h"
#include "params.h"

#define crh(OUT, IN, INBYTES) shake256(OUT, CRHBYTES, IN, INBYTES)
#define stream128_init(STATE, SEED, NONCE) PQCLEAN_DILITHIUM2_CLEAN_shake128_stream_init(STATE, SEED, NONCE)
#define stream128_squeezeblocks(OUT, OUTBLOCKS, STATE) shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define stream256_init(STATE, SEED, NONCE) PQCLEAN_DILITHIUM2_CLEAN_shake256_stream_init(STATE, SEED, NONCE)
#define stream256_squeezeblocks(OUT, OUTBLOCKS, STATE) shake256_squeezeblocks(OUT, OUTBLOCKS, STATE)

#define STREAM128_BLOCKBYTES SHAKE128_RATE
#define STREAM256_BLOCKBYTES SHAKE256_RATE

void PQCLEAN_DILITHIUM2_CLEAN_shake128_stream_init(shake128ctx *state,
        const unsigned char *seed,
        uint16_t nonce);
void PQCLEAN_DILITHIUM2_CLEAN_shake256_stream_init(shake256ctx *state,
        const unsigned char *seed,
        uint16_t nonce);

#endif
