#include <stdint.h>
#include <immintrin.h>
#include "params.h"
#include "fips202x4.h"
#include "fips202.h"

#define NROUNDS 24
#define ROL(a, offset) ((a << offset) ^ (a >> (64-offset)))

static uint64_t load64(const unsigned char *x) {
  unsigned int i;
  uint64_t r = 0;

  for(i = 0; i < 8; ++i)
    r |= (uint64_t)x[i] << 8*i;

  return r;
}

static void store64(unsigned char *x, uint64_t u) {
  unsigned int i;

  for(i = 0; i < 8; ++i)
    x[i] = u >> 8*i;
}

/* Use implementation from the Keccak Code Package */
extern void KeccakP1600times4_PermuteAll_24rounds(__m256i *s);
#define KeccakF1600_StatePermute4x KeccakP1600times4_PermuteAll_24rounds

static void keccak_absorb4x(__m256i *s,
                            unsigned int r,
                            const unsigned char *m0,
                            const unsigned char *m1,
                            const unsigned char *m2,
                            const unsigned char *m3,
                            unsigned long long mlen,
                            unsigned char p)
{
  unsigned long long i;
  unsigned char t0[200];
  unsigned char t1[200];
  unsigned char t2[200];
  unsigned char t3[200];
  uint64_t *ss = (uint64_t *)s;

  for(i = 0; i < 25; ++i)
    s[i] = _mm256_xor_si256(s[i], s[i]);

  while (mlen >= r) {
    for (i = 0; i < r/8; ++i) {
      ss[4*i + 0] ^= load64(m0 + 8*i);
      ss[4*i + 1] ^= load64(m1 + 8*i);
      ss[4*i + 2] ^= load64(m2 + 8*i);
      ss[4*i + 3] ^= load64(m3 + 8*i);
    }
    
    KeccakF1600_StatePermute4x(s);
    mlen -= r;
    m0 += r;
    m1 += r;
    m2 += r;
    m3 += r;
  }

  for (i = 0; i < r; ++i) {
    t0[i] = 0;
    t1[i] = 0;
    t2[i] = 0;
    t3[i] = 0;
  }
  for (i = 0; i < mlen; ++i) {
    t0[i] = m0[i];
    t1[i] = m1[i];
    t2[i] = m2[i];
    t3[i] = m3[i];
  }

  t0[i] = p;
  t1[i] = p;
  t2[i] = p;
  t3[i] = p;

  t0[r - 1] |= 128;
  t1[r - 1] |= 128;
  t2[r - 1] |= 128;
  t3[r - 1] |= 128;

  for (i = 0; i < r/8; ++i) {
    ss[4*i + 0] ^= load64(t0 + 8*i);
    ss[4*i + 1] ^= load64(t1 + 8*i);
    ss[4*i + 2] ^= load64(t2 + 8*i);
    ss[4*i + 3] ^= load64(t3 + 8*i);
  }
}


static void keccak_squeezeblocks4x(unsigned char *h0,
                                   unsigned char *h1,
                                   unsigned char *h2,
                                   unsigned char *h3,
                                   unsigned long nblocks,
                                   unsigned int r,
                                   __m256i *s)
{
  unsigned int i;
  uint64_t *ss = (uint64_t *)s;

  while(nblocks > 0) {
    KeccakF1600_StatePermute4x(s);
    for(i=0; i < r/8; ++i) {
      store64(h0 + 8*i, ss[4*i + 0]);
      store64(h1 + 8*i, ss[4*i + 1]);
      store64(h2 + 8*i, ss[4*i + 2]);
      store64(h3 + 8*i, ss[4*i + 3]);
    }

    h0 += r;
    h1 += r;
    h2 += r;
    h3 += r;
    --nblocks;
  }
}

void shake128_absorb4x(__m256i *s,
                       const unsigned char *m0,
                       const unsigned char *m1,
                       const unsigned char *m2,
                       const unsigned char *m3,
                       unsigned long long mlen)
{
  keccak_absorb4x(s, SHAKE128_RATE, m0, m1, m2, m3, mlen, 0x1F);
}

void shake128_squeezeblocks4x(unsigned char *h0,
                              unsigned char *h1,
                              unsigned char *h2,
                              unsigned char *h3,
                              unsigned long nblocks,
                              __m256i *s)
{
  keccak_squeezeblocks4x(h0, h1, h2, h3, nblocks, SHAKE128_RATE, s);
}

void shake256_absorb4x(__m256i *s,
                       const unsigned char *m0,
                       const unsigned char *m1,
                       const unsigned char *m2,
                       const unsigned char *m3,
                       unsigned long long mlen)
{
  keccak_absorb4x(s, SHAKE256_RATE, m0, m1, m2, m3, mlen, 0x1F);
}

void shake256_squeezeblocks4x(unsigned char *h0,
                              unsigned char *h1,
                              unsigned char *h2,
                              unsigned char *h3,
                              unsigned long nblocks,
                              __m256i *s)
{
  keccak_squeezeblocks4x(h0, h1, h2, h3, nblocks, SHAKE256_RATE, s);
}

void shake128_4x(unsigned char *h0,
                 unsigned char *h1,
                 unsigned char *h2,
                 unsigned char *h3,
                 unsigned long long hlen, 
                 const unsigned char *m0,
                 const unsigned char *m1,
                 const unsigned char *m2,
                 const unsigned char *m3,
                 unsigned long long mlen)
{
  unsigned int i;
  unsigned long nblocks = hlen/SHAKE128_RATE;
  unsigned char t[4][SHAKE128_RATE];
  __m256i s[25];

  shake128_absorb4x(s, m0, m1, m2, m3, mlen);
  shake128_squeezeblocks4x(h0, h1, h2, h3, nblocks, s);

  h0 += nblocks*SHAKE128_RATE;
  h1 += nblocks*SHAKE128_RATE;
  h2 += nblocks*SHAKE128_RATE;
  h3 += nblocks*SHAKE128_RATE;
  hlen -= nblocks*SHAKE128_RATE;

  if(hlen) {
    shake128_squeezeblocks4x(t[0], t[1], t[2], t[3], 1, s);
    for(i = 0; i < hlen; ++i) {
      h0[i] = t[0][i];
      h1[i] = t[1][i];
      h2[i] = t[2][i];
      h3[i] = t[3][i];
    }
  }
}

void shake256_4x(unsigned char *h0,
                 unsigned char *h1,
                 unsigned char *h2,
                 unsigned char *h3,
                 unsigned long long hlen, 
                 const unsigned char *m0,
                 const unsigned char *m1,
                 const unsigned char *m2,
                 const unsigned char *m3,
                 unsigned long long mlen)
{
  unsigned int i;
  unsigned long nblocks = hlen/SHAKE256_RATE;
  unsigned char t[4][SHAKE256_RATE];
  __m256i s[25];

  shake256_absorb4x(s, m0, m1, m2, m3, mlen);
  shake256_squeezeblocks4x(h0, h1, h2, h3, nblocks, s);

  h0 += nblocks*SHAKE256_RATE;
  h1 += nblocks*SHAKE256_RATE;
  h2 += nblocks*SHAKE256_RATE;
  h3 += nblocks*SHAKE256_RATE;
  hlen -= nblocks*SHAKE256_RATE;

  if(hlen) {
    shake256_squeezeblocks4x(t[0], t[1], t[2], t[3], 1, s);
    for(i = 0; i < hlen; ++i) {
      h0[i] = t[0][i];
      h1[i] = t[1][i];
      h2[i] = t[2][i];
      h3[i] = t[3][i];
    }
  }
}
