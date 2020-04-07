#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "params.h"


#include "aes256ctr.h"
#include "sha2.h"

#define hash_h(OUT, IN, INBYTES) sha256((OUT), (IN), (INBYTES))
#define hash_g(OUT, IN, INBYTES) sha512((OUT), (IN), (INBYTES))
#define xof_absorb(STATE, IN, X, Y) PQCLEAN_KYBER51290S_AVX2_aes256ctr_init((STATE), (IN), (Y) + ((uint16_t)(X) << 8))
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_KYBER51290S_AVX2_aes256ctr_squeezeblocks((OUT), (OUTBLOCKS), (STATE))
#define xof_ctx_release(STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) PQCLEAN_KYBER51290S_AVX2_aes256ctr_prf((OUT), (OUTBYTES), (KEY), (NONCE))
#define kdf(OUT, IN, INBYTES) sha256((OUT), (IN), (INBYTES))

#define XOF_BLOCKBYTES 128

typedef aes256ctr_ctx xof_state;


#endif /* SYMMETRIC_H */
