#ifndef PQCLEAN_DILITHIUM5AES_AVX2_SYMMETRIC_H
#define PQCLEAN_DILITHIUM5AES_AVX2_SYMMETRIC_H
#include "aes256ctr.h"
#include "fips202.h"
#include "params.h"
#include <stdint.h>



typedef aes256ctr_ctx stream128_state;
typedef aes256ctr_ctx stream256_state;

#define STREAM128_BLOCKBYTES AES256CTR_BLOCKBYTES
#define STREAM256_BLOCKBYTES AES256CTR_BLOCKBYTES

#define crh(OUT, IN, INBYTES) shake256(OUT, CRHBYTES, IN, INBYTES)
#define stream128_init(STATE, SEED, NONCE) PQCLEAN_DILITHIUM5AES_AVX2_aes256ctr_init(STATE, SEED, NONCE)
#define stream128_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_DILITHIUM5AES_AVX2_aes256ctr_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define stream128_release(STATE)
#define stream256_init(STATE, SEED, NONCE) PQCLEAN_DILITHIUM5AES_AVX2_aes256ctr_init(STATE, SEED, NONCE)
#define stream256_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_DILITHIUM5AES_AVX2_aes256ctr_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define stream256_release(STATE)


#endif
