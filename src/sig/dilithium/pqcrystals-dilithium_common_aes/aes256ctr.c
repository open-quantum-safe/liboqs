/* Based heavily on public-domain code by Romain Dolbeau
 * Different handling of nonce+counter than original version using
 * separated 64-bit nonce and internal 64-bit counter, starting from zero
 * Public Domain */

#include <stddef.h>
#include <stdint.h>
#include <immintrin.h>
#include "aes256ctr.h"

static inline void aesni_encrypt4(uint8_t out[64], __m128i *n, const __m128i rkeys[16])
{
  __m128i f,f0,f1,f2,f3;
  const __m128i idx = _mm_set_epi8(8,9,10,11,12,13,14,15,7,6,5,4,3,2,1,0);

  /* Load current counter value */
  f = _mm_load_si128(n);

  /* Increase counter in 4 consecutive blocks */
  f0 = _mm_shuffle_epi8(_mm_add_epi64(f,_mm_set_epi64x(0,0)),idx);
  f1 = _mm_shuffle_epi8(_mm_add_epi64(f,_mm_set_epi64x(1,0)),idx);
  f2 = _mm_shuffle_epi8(_mm_add_epi64(f,_mm_set_epi64x(2,0)),idx);
  f3 = _mm_shuffle_epi8(_mm_add_epi64(f,_mm_set_epi64x(3,0)),idx);

  /* Write counter for next iteration, increased by 4 */
  _mm_store_si128(n,_mm_add_epi64(f,_mm_set_epi64x(4,0)));

  /* Actual AES encryption, 4x interleaved */
  f  = _mm_load_si128(&rkeys[0]);
  f0 = _mm_xor_si128(f0,f);
  f1 = _mm_xor_si128(f1,f);
  f2 = _mm_xor_si128(f2,f);
  f3 = _mm_xor_si128(f3,f);

  for (int i = 1; i < 14; i++) {
    f  = _mm_load_si128(&rkeys[i]);
    f0 = _mm_aesenc_si128(f0,f);
    f1 = _mm_aesenc_si128(f1,f);
    f2 = _mm_aesenc_si128(f2,f);
    f3 = _mm_aesenc_si128(f3,f);
  }

  f  = _mm_load_si128(&rkeys[14]);
  f0 = _mm_aesenclast_si128(f0,f);
  f1 = _mm_aesenclast_si128(f1,f);
  f2 = _mm_aesenclast_si128(f2,f);
  f3 = _mm_aesenclast_si128(f3,f);

  /* Write results */
  _mm_storeu_si128((__m128i*)(out+ 0),f0);
  _mm_storeu_si128((__m128i*)(out+16),f1);
  _mm_storeu_si128((__m128i*)(out+32),f2);
  _mm_storeu_si128((__m128i*)(out+48),f3);
}

void aes256ctr_init(aes256ctr_ctx *state, const uint8_t key[32], uint64_t nonce)
{
  __m128i key0, key1, temp0, temp1, temp2, temp4;
  int idx = 0;

  key0 = _mm_loadu_si128((__m128i *)(key+ 0));
  key1 = _mm_loadu_si128((__m128i *)(key+16));
  state->n = _mm_loadl_epi64((__m128i *)&nonce);

  state->rkeys[idx++] = key0;
  temp0 = key0;
  temp2 = key1;
  temp4 = _mm_setzero_si128();

#define BLOCK1(IMM)                                                     \
  temp1 = _mm_aeskeygenassist_si128(temp2, IMM);                        \
  state->rkeys[idx++] = temp2;                                          \
  temp4 = (__m128i)_mm_shuffle_ps((__m128)temp4, (__m128)temp0, 0x10);  \
  temp0 = _mm_xor_si128(temp0, temp4);                                  \
  temp4 = (__m128i)_mm_shuffle_ps((__m128)temp4, (__m128)temp0, 0x8c);  \
  temp0 = _mm_xor_si128(temp0, temp4);                                  \
  temp1 = (__m128i)_mm_shuffle_ps((__m128)temp1, (__m128)temp1, 0xff);  \
  temp0 = _mm_xor_si128(temp0, temp1)

#define BLOCK2(IMM)                                                     \
  temp1 = _mm_aeskeygenassist_si128(temp0, IMM);                        \
  state->rkeys[idx++] = temp0;                                          \
  temp4 = (__m128i)_mm_shuffle_ps((__m128)temp4, (__m128)temp2, 0x10);  \
  temp2 = _mm_xor_si128(temp2, temp4);                                  \
  temp4 = (__m128i)_mm_shuffle_ps((__m128)temp4, (__m128)temp2, 0x8c);  \
  temp2 = _mm_xor_si128(temp2, temp4);                                  \
  temp1 = (__m128i)_mm_shuffle_ps((__m128)temp1, (__m128)temp1, 0xaa);  \
  temp2 = _mm_xor_si128(temp2, temp1)

  BLOCK1(0x01);
  BLOCK2(0x01);

  BLOCK1(0x02);
  BLOCK2(0x02);

  BLOCK1(0x04);
  BLOCK2(0x04);

  BLOCK1(0x08);
  BLOCK2(0x08);

  BLOCK1(0x10);
  BLOCK2(0x10);

  BLOCK1(0x20);
  BLOCK2(0x20);

  BLOCK1(0x40);
  state->rkeys[idx++] = temp0;
}

void aes256ctr_squeezeblocks(uint8_t *out,
                             size_t nblocks,
                             aes256ctr_ctx *state)
{
  size_t i;
  for(i=0;i<nblocks;i++) {
    aesni_encrypt4(out, &state->n, state->rkeys);
    out += 64;
  }
}

void aes256ctr_prf(uint8_t *out,
                   size_t outlen,
                   const uint8_t seed[32],
                   uint64_t nonce)
{
  unsigned int i;
  uint8_t buf[64];
  aes256ctr_ctx state;

  aes256ctr_init(&state, seed, nonce);

  while(outlen >= 64) {
    aesni_encrypt4(out, &state.n, state.rkeys);
    outlen -= 64;
    out += 64;
  }

  if(outlen) {
    aesni_encrypt4(buf, &state.n, state.rkeys);
    for(i=0;i<outlen;i++)
      out[i] = buf[i];
  }
}
