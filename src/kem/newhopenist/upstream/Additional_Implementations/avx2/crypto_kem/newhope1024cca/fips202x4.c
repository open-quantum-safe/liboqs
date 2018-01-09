#include <immintrin.h>
#include <stdint.h>
#include <assert.h>
#include "fips202.h"

#define NROUNDS 24
#define ROL(a, offset) ((a << offset) ^ (a >> (64-offset)))

static uint64_t load64(const unsigned char *x)
{
  unsigned long long r = 0, i;

  for (i = 0; i < 8; ++i) {
    r |= (unsigned long long)x[i] << 8 * i;
  }
  return r;
}

static void store64(uint8_t *x, uint64_t u)
{
  unsigned int i;

  for(i=0; i<8; ++i) {
    x[i] = u;
    u >>= 8;
  }
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
                          unsigned long long int mlen,
                          unsigned char p)
{
  unsigned long long i;
  unsigned char t0[200];
  unsigned char t1[200];
  unsigned char t2[200];
  unsigned char t3[200];

  unsigned long long *ss = (unsigned long long *)s;

 
  while (mlen >= r) 
  {
    for (i = 0; i < r / 8; ++i)
    {
      ss[4*i+0] ^= load64(m0 + 8 * i);
      ss[4*i+1] ^= load64(m1 + 8 * i);
      ss[4*i+2] ^= load64(m2 + 8 * i);
      ss[4*i+3] ^= load64(m3 + 8 * i);
    }
    
    KeccakF1600_StatePermute4x(s);
    mlen -= r;
    m0 += r;
    m1 += r;
    m2 += r;
    m3 += r;
  }

  for (i = 0; i < r; ++i)
  {
    t0[i] = 0;
    t1[i] = 0;
    t2[i] = 0;
    t3[i] = 0;
  }
  for (i = 0; i < mlen; ++i)
  {
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

  for (i = 0; i < r / 8; ++i)
  {
    ss[4*i+0] ^= load64(t0 + 8 * i);
    ss[4*i+1] ^= load64(t1 + 8 * i);
    ss[4*i+2] ^= load64(t2 + 8 * i);
    ss[4*i+3] ^= load64(t3 + 8 * i);
  }
}


static void keccak_squeezeblocks4x(unsigned char *h0, 
                                   unsigned char *h1, 
                                   unsigned char *h2, 
                                   unsigned char *h3, 
                                   unsigned long long int nblocks,
                                   __m256i *s, 
                                   unsigned int r)
{
  unsigned int i;

  unsigned long long *ss = (unsigned long long *)s;

  while(nblocks > 0) 
  {
    KeccakF1600_StatePermute4x(s);
    for(i=0;i<(r>>3);i++)
    {
      store64(h0+8*i, ss[4*i+0]);
      store64(h1+8*i, ss[4*i+1]);
      store64(h2+8*i, ss[4*i+2]);
      store64(h3+8*i, ss[4*i+3]);
    }
    h0 += r;
    h1 += r;
    h2 += r;
    h3 += r;
    nblocks--;
  }
}



void shake128x4(unsigned char *out0, 
                unsigned char *out1,
                unsigned char *out2,
                unsigned char *out3, unsigned long long outlen, 
                unsigned char *in0,
                unsigned char *in1,
                unsigned char *in2,
                unsigned char *in3, unsigned long long inlen)
{
  __m256i s[25];
  unsigned char t0[SHAKE128_RATE];
  unsigned char t1[SHAKE128_RATE];
  unsigned char t2[SHAKE128_RATE];
  unsigned char t3[SHAKE128_RATE];
  unsigned int i;

  /* zero state */
  for(i=0;i<25;i++)
    s[i] = _mm256_xor_si256(s[i], s[i]); 

  /* absorb 4 message of identical length in parallel */
  keccak_absorb4x(s, SHAKE128_RATE, in0, in1, in2, in3, inlen, 0x1F);

  /* Squeeze output */
  keccak_squeezeblocks4x(out0, out1, out2, out3, outlen/SHAKE128_RATE, s, SHAKE128_RATE);

  out0 += (outlen/SHAKE128_RATE)*SHAKE128_RATE;
  out1 += (outlen/SHAKE128_RATE)*SHAKE128_RATE;
  out2 += (outlen/SHAKE128_RATE)*SHAKE128_RATE;
  out3 += (outlen/SHAKE128_RATE)*SHAKE128_RATE;

  if(outlen%SHAKE128_RATE)
  {
    keccak_squeezeblocks4x(t0, t1, t2, t3, 1, s, SHAKE128_RATE);
    for(i=0;i<outlen%SHAKE128_RATE;i++)
    {
      out0[i] = t0[i];
      out1[i] = t1[i];
      out2[i] = t2[i];
      out3[i] = t3[i];
    }
  }
}


void shake256x4(unsigned char *out0, 
                unsigned char *out1,
                unsigned char *out2,
                unsigned char *out3, unsigned long long outlen, 
                unsigned char *in0,
                unsigned char *in1,
                unsigned char *in2,
                unsigned char *in3, unsigned long long inlen)
{
  __m256i s[25];
  unsigned char t0[SHAKE256_RATE];
  unsigned char t1[SHAKE256_RATE];
  unsigned char t2[SHAKE256_RATE];
  unsigned char t3[SHAKE256_RATE];
  unsigned int i;

  /* zero state */
  for(i=0;i<25;i++)
    s[i] = _mm256_xor_si256(s[i], s[i]); 

  /* absorb 4 message of identical length in parallel */
  keccak_absorb4x(s, SHAKE256_RATE, in0, in1, in2, in3, inlen, 0x1F);

  /* Squeeze output */
  keccak_squeezeblocks4x(out0, out1, out2, out3, outlen/SHAKE256_RATE, s, SHAKE256_RATE);

  out0 += (outlen/SHAKE256_RATE)*SHAKE256_RATE;
  out1 += (outlen/SHAKE256_RATE)*SHAKE256_RATE;
  out2 += (outlen/SHAKE256_RATE)*SHAKE256_RATE;
  out3 += (outlen/SHAKE256_RATE)*SHAKE256_RATE;

  if(outlen%SHAKE256_RATE)
  {
    keccak_squeezeblocks4x(t0, t1, t2, t3, 1, s, SHAKE256_RATE);
    for(i=0;i<outlen%SHAKE256_RATE;i++)
    {
      out0[i] = t0[i];
      out1[i] = t1[i];
      out2[i] = t2[i];
      out3[i] = t3[i];
    }
  }
}
