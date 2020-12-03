#ifndef PQCLEAN_KYBER76890S_CLEAN_SYMMETRIC_H
#define PQCLEAN_KYBER76890S_CLEAN_SYMMETRIC_H
#include "params.h"
#include "sha2.h"
#include "symmetric-aes.h"
#include <stddef.h>
#include <stdint.h>




typedef aes256xof_ctx xof_state;

void PQCLEAN_KYBER76890S_CLEAN_kyber_aes256xof_absorb(aes256xof_ctx *state, const uint8_t seed[32], uint8_t x, uint8_t y);

void PQCLEAN_KYBER76890S_CLEAN_kyber_aes256ctr_prf(uint8_t *out, size_t outlen, const uint8_t key[32], uint8_t nonce);

#define XOF_BLOCKBYTES 64

#define hash_h(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha512(OUT, IN, INBYTES)
#define xof_absorb(STATE, SEED, X, Y) PQCLEAN_KYBER76890S_CLEAN_aes256xof_absorb(STATE, SEED, X, Y)
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_KYBER76890S_CLEAN_aes256xof_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define xof_ctx_release(STATE) PQCLEAN_KYBER76890S_CLEAN_aes256xof_ctx_release(STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) PQCLEAN_KYBER76890S_CLEAN_aes256ctr_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)


#endif /* SYMMETRIC_H */
