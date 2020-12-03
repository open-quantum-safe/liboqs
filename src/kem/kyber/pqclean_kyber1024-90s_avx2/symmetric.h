#ifndef PQCLEAN_KYBER102490S_AVX2_SYMMETRIC_H
#define PQCLEAN_KYBER102490S_AVX2_SYMMETRIC_H
#include "aes256ctr.h"
#include "params.h"
#include "sha2.h"
#include <stddef.h>
#include <stdint.h>



typedef aes256ctr_ctx xof_state;

#define XOF_BLOCKBYTES AES256CTR_BLOCKBYTES

#define hash_h(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha512(OUT, IN, INBYTES)
#define xof_absorb(STATE, SEED, X, Y) PQCLEAN_KYBER102490S_AVX2_aes256ctr_init(STATE, SEED, (X) | ((uint16_t)(Y) << 8))
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_KYBER102490S_AVX2_aes256ctr_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define xof_ctx_release(STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) PQCLEAN_KYBER102490S_AVX2_aes256ctr_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)


#endif /* SYMMETRIC_H */
