#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "symmetric.h"
#include "aes256ctr.h"

void kyber_aes256xof_absorb(aes256ctr_ctx *state, const uint8_t seed[32], uint8_t x, uint8_t y)
{
  uint8_t expnonce[12] = {0};
  expnonce[0] = x;
  expnonce[1] = y;
  aes256ctr_init(state, seed, expnonce);
}

void kyber_aes256ctr_prf(uint8_t *out, size_t outlen, const uint8_t key[32], uint8_t nonce)
{
  uint8_t expnonce[12] = {0};
  expnonce[0] = nonce;
  aes256ctr_prf(out, outlen, key, expnonce);
}
