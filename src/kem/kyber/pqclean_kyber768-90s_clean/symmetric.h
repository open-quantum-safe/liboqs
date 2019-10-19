#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "params.h"


#include "aes256ctr.h"
#include "sha2.h"

#define hash_h(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha512(OUT, IN, INBYTES)
#define xof_absorb(STATE, IN, X, Y) PQCLEAN_KYBER76890S_CLEAN_aes256xof_absorb(STATE, IN, X, Y)
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_KYBER76890S_CLEAN_aes256xof_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) PQCLEAN_KYBER76890S_CLEAN_aes256_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)

#define XOF_BLOCKBYTES 64

typedef aes256xof_ctx xof_state;


#endif /* SYMMETRIC_H */
