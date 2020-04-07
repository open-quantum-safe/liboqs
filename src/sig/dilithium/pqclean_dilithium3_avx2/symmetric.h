#ifndef PQCLEAN_DILITHIUM3_AVX2_SYMMETRIC_H
#define PQCLEAN_DILITHIUM3_AVX2_SYMMETRIC_H

#include "params.h"
#include "stream.h"


#include "fips202.h"

#define crh(OUT, IN, INBYTES) shake256(OUT, CRHBYTES, IN, INBYTES)
#define stream128_init(STATE, SEED, NONCE) PQCLEAN_DILITHIUM3_AVX2_shake128_stream_init(STATE, SEED, NONCE)
#define stream128_squeezeblocks(OUT, OUTBLOCKS, STATE) shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define stream128_ctx_release(STATE) shake128_ctx_release(STATE)
#define stream256_init(STATE, SEED, NONCE) PQCLEAN_DILITHIUM3_AVX2_shake256_stream_init(STATE, SEED, NONCE)
#define stream256_squeezeblocks(OUT, OUTBLOCKS, STATE) shake256_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define stream256_ctx_release(STATE) shake256_ctx_release(STATE)

#define STREAM128_BLOCKBYTES SHAKE128_RATE
#define STREAM256_BLOCKBYTES SHAKE256_RATE

typedef shake128ctx stream128_state;
typedef shake256ctx stream256_state;


#endif
