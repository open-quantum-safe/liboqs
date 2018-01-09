#include "poly.h"
#include "ntt.h"
#include "reduce.h"
#include "fips202.h"
#include "fips202x4.h"

static uint16_t coeff_freeze(uint16_t x)
{
  uint16_t m,r;
  int16_t c;
  r = barrett_reduce(x);

  m = r - NEWHOPE_Q;
  c = m;
  c >>= 15;
  r = m ^ ((r^m)&c); 

  return r;
}

/* Computes abs(x-Q/2) */
static uint16_t flipabs(uint16_t x)
{
  int16_t r,m;
  r = coeff_freeze(x);

  r = r - NEWHOPE_Q/2;
  m = r >> 15;
  return (r + m) ^ m;
}


void poly_frombytes(poly *r, const unsigned char *a)
{
  int i;
  for(i=0;i<NEWHOPE_N/4;i++)
  {
    r->coeffs[4*i+0] =                               a[7*i+0]        | (((uint16_t)a[7*i+1] & 0x3f) << 8);
    r->coeffs[4*i+1] = (a[7*i+1] >> 6) | (((uint16_t)a[7*i+2]) << 2) | (((uint16_t)a[7*i+3] & 0x0f) << 10);
    r->coeffs[4*i+2] = (a[7*i+3] >> 4) | (((uint16_t)a[7*i+4]) << 4) | (((uint16_t)a[7*i+5] & 0x03) << 12);
    r->coeffs[4*i+3] = (a[7*i+5] >> 2) | (((uint16_t)a[7*i+6]) << 6); 
  }
}

void poly_tobytes(unsigned char *r, const poly *p)
{
  int i;
  uint16_t t0,t1,t2,t3;
  for(i=0;i<NEWHOPE_N/4;i++)
  {
    t0 = coeff_freeze(p->coeffs[4*i+0]);
    t1 = coeff_freeze(p->coeffs[4*i+1]);
    t2 = coeff_freeze(p->coeffs[4*i+2]);
    t3 = coeff_freeze(p->coeffs[4*i+3]);

    r[7*i+0] =  t0 & 0xff;
    r[7*i+1] = (t0 >> 8) | (t1 << 6);
    r[7*i+2] = (t1 >> 2);
    r[7*i+3] = (t1 >> 10) | (t2 << 4);
    r[7*i+4] = (t2 >> 4);
    r[7*i+5] = (t2 >> 12) | (t3 << 2);
    r[7*i+6] = (t3 >> 6);
  }
}

void poly_compress(unsigned char *r, const poly *p)
{
  unsigned int i,j,k=0;

  uint32_t t[8];

  for(i=0;i<NEWHOPE_N;i+=8)
  {
    for(j=0;j<8;j++)
    {
      t[j] = coeff_freeze(p->coeffs[i+j]);
      t[j] = (((t[j] << 3) + NEWHOPE_Q/2)/NEWHOPE_Q) & 0x7;
    }

    r[k]   =  t[0]       | (t[1] << 3) | (t[2] << 6);
    r[k+1] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[k+2] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
    k += 3;
  }
}

void poly_decompress(poly *r, const unsigned char *a)
{
  unsigned int i,j;
  for(i=0;i<NEWHOPE_N;i+=8)
  {
    r->coeffs[i+0] =  a[0] & 7;
    r->coeffs[i+1] = (a[0] >> 3) & 7;
    r->coeffs[i+2] = (a[0] >> 6) | ((a[1] << 2) & 4);
    r->coeffs[i+3] = (a[1] >> 1) & 7;
    r->coeffs[i+4] = (a[1] >> 4) & 7;
    r->coeffs[i+5] = (a[1] >> 7) | ((a[2] << 1) & 6);
    r->coeffs[i+6] = (a[2] >> 2) & 7;
    r->coeffs[i+7] = (a[2] >> 5);
    a += 3;
    for(j=0;j<8;j++)
      r->coeffs[i+j] = ((uint32_t)r->coeffs[i+j] * NEWHOPE_Q + 4) >> 3;
  }
}

void poly_frommsg(poly *r, const unsigned char *msg)
{
  unsigned int i,j,mask;
  for(i=0;i<32;i++) // XXX: MACRO for 32
  {
    for(j=0;j<8;j++)
    {
      mask = -((msg[i] >> j)&1);
      r->coeffs[8*i+j+  0] = mask & (NEWHOPE_Q/2);
      r->coeffs[8*i+j+256] = mask & (NEWHOPE_Q/2);
#if (NEWHOPE_N == 1024)
      r->coeffs[8*i+j+512] = mask & (NEWHOPE_Q/2);
      r->coeffs[8*i+j+768] = mask & (NEWHOPE_Q/2);
#endif
    }
  }
}

void poly_tomsg(unsigned char *msg, const poly *x)
{
  unsigned int i;
  uint16_t t;

  for(i=0;i<32;i++)
    msg[i] = 0;
  
  for(i=0;i<256;i++) 
  {
    t  = flipabs(x->coeffs[i+  0]);
    t += flipabs(x->coeffs[i+256]);
#if (NEWHOPE_N == 1024)
    t += flipabs(x->coeffs[i+512]);
    t += flipabs(x->coeffs[i+768]);
    t = ((t - NEWHOPE_Q));
#else
    t = ((t - NEWHOPE_Q/2));
#endif

    t >>= 15;
    msg[i>>3] |= t<<(i&7);
  }
}

static void poly_uniform_ref(poly *a, const unsigned char *seed)
{
  unsigned int ctr=0;
  uint16_t val;
  uint64_t state[25];
  uint8_t buf[SHAKE128_RATE];
  uint8_t extseed[(NEWHOPE_SYMBYTES+1)];
  int i,j;
    
  for(i=0;i<NEWHOPE_SYMBYTES;i++)
    extseed[i] = seed[i];

  for(i=0;i<NEWHOPE_N/64;i++) /* generate a in blocks of 256 coefficients */
  {
    ctr = 0;
    extseed[NEWHOPE_SYMBYTES] = i; /* domain-separate the 16 independent calls */
    shake128_absorb(state, extseed, NEWHOPE_SYMBYTES+1);
    while(ctr < 64) /* Very unlikely to run more than once */
    {
      shake128_squeezeblocks(buf,1,state);
      for(j=0;j<SHAKE128_RATE && ctr < 64;j+=2)
      {
        val = (buf[j] | ((uint16_t) buf[j+1] << 8));
        if(val < 5*NEWHOPE_Q)
        {
          a->coeffs[i*64+ctr] = val;
          ctr++;
        }
      }
    }
  }
}

/* Get 64 coefficients out of buf, store in r */
static int rej_sample(uint32_t *r, unsigned char *buf, size_t buflen)
{
    /* Rejection sampling */
    unsigned int ctr = 0, j;
    uint16_t val;

    for(j=0;j<buflen && ctr < 64;j+=2)
    {
      val = (buf[j] | ((uint16_t) buf[j+1] << 8));
      if(val < 5*NEWHOPE_Q)
        r[ctr++] = val;
    }
    if(ctr != 64) 
      return -1;
    return 0;
}


void poly_uniform(poly *a, const unsigned char *seed)
{
  uint8_t buf0[SHAKE128_RATE];
  uint8_t buf1[SHAKE128_RATE];
  uint8_t buf2[SHAKE128_RATE];
  uint8_t buf3[SHAKE128_RATE];
  uint8_t extseed0[NEWHOPE_SYMBYTES+1];
  uint8_t extseed1[NEWHOPE_SYMBYTES+1];
  uint8_t extseed2[NEWHOPE_SYMBYTES+1];
  uint8_t extseed3[NEWHOPE_SYMBYTES+1];
  int i;

  for(i=0;i<NEWHOPE_SYMBYTES;i++)
  {
    extseed0[i] = seed[i];
    extseed1[i] = seed[i];
    extseed2[i] = seed[i];
    extseed3[i] = seed[i];
  }

  for(i=0;i<NEWHOPE_N/256;i++)
  {
    extseed0[NEWHOPE_SYMBYTES] = 4*i;
    extseed1[NEWHOPE_SYMBYTES] = 4*i+1;
    extseed2[NEWHOPE_SYMBYTES] = 4*i+2;
    extseed3[NEWHOPE_SYMBYTES] = 4*i+3;

    shake128x4(buf0, buf1, buf2, buf3, SHAKE128_RATE, extseed0, extseed1, extseed2, extseed3, NEWHOPE_SYMBYTES+1);

    /* Rejection sampling */
    if(rej_sample(a->coeffs+i*256,buf0,SHAKE128_RATE))
    {
      poly_uniform_ref(a, seed);
      return;
    }
    if(rej_sample(a->coeffs+i*256+64,buf1,SHAKE128_RATE))
    {
      poly_uniform_ref(a, seed);
      return;
    }
    if(rej_sample(a->coeffs+i*256+128,buf2,SHAKE128_RATE))
    {
      poly_uniform_ref(a, seed);
      return;
    }
    if(rej_sample(a->coeffs+i*256+192,buf3,SHAKE128_RATE))
    {
      poly_uniform_ref(a, seed);
      return;
    }
  }
}

static void cbd(uint32_t *r, const unsigned char *buf)
{
  uint32_t t, d, a, b, c;
  int j,k;

  for(j=0;j<128;j+=4)
  {
    t = buf[j] | ((uint32_t)buf[j+1] << 8) | ((uint32_t)buf[j+2] << 16) | ((uint32_t)buf[j+3] << 24);
    d = 0;
    for(k=0;k<8;k++)
      d += (t >> k) & 0x01010101;
    a = d & 0xff;
    b = ((d >>  8) & 0xff);
    c = ((d >> 16) & 0xff);
    d >>= 24;
    r[j/2]   = a + NEWHOPE_Q - b;
    r[j/2+1] = c + NEWHOPE_Q - d;
  }
}

void poly_sample(poly *r, const unsigned char *seed, unsigned char nonce)
{
#if NEWHOPE_K != 8
#error "poly_getnoise in poly.c only supports k=8"
#endif
  unsigned char buf0[128];
  unsigned char buf1[128];
  unsigned char buf2[128];
  unsigned char buf3[128];
  int i;

  unsigned char extseed0[NEWHOPE_SYMBYTES+2];
  unsigned char extseed1[NEWHOPE_SYMBYTES+2];
  unsigned char extseed2[NEWHOPE_SYMBYTES+2];
  unsigned char extseed3[NEWHOPE_SYMBYTES+2];

  for(i=0;i<NEWHOPE_SYMBYTES;i++)
  {
    extseed0[i] = seed[i];
    extseed1[i] = seed[i];
    extseed2[i] = seed[i];
    extseed3[i] = seed[i];
  }
  extseed0[NEWHOPE_SYMBYTES] = nonce;
  extseed1[NEWHOPE_SYMBYTES] = nonce;
  extseed2[NEWHOPE_SYMBYTES] = nonce;
  extseed3[NEWHOPE_SYMBYTES] = nonce;

  for(i=0;i<NEWHOPE_N/256;i++) /* Generate noise in blocks of 64 coefficients */
  {
    extseed0[NEWHOPE_SYMBYTES+1] = 4*i;
    extseed1[NEWHOPE_SYMBYTES+1] = 4*i+1;
    extseed2[NEWHOPE_SYMBYTES+1] = 4*i+2;
    extseed3[NEWHOPE_SYMBYTES+1] = 4*i+3;

    shake256x4(buf0, buf1, buf2, buf3,128,extseed0, extseed1, extseed2, extseed3, NEWHOPE_SYMBYTES+2);

    cbd(r->coeffs+i*256,     buf0);
    cbd(r->coeffs+i*256+ 64, buf1);
    cbd(r->coeffs+i*256+128, buf2);
    cbd(r->coeffs+i*256+192, buf3);

  }
}

void poly_pointwise(poly *r, const poly *a, const poly *b)
{
  mul_coefficients(r->coeffs, a->coeffs, b->coeffs); 
}

void poly_add(poly *r, const poly *a, const poly *b)
{
  int i;
  for(i=0;i<NEWHOPE_N;i++)
    r->coeffs[i] = barrett_reduce(a->coeffs[i] + b->coeffs[i]);
}

void poly_sub(poly *r, const poly *a, const poly *b)
{
  int i;
  for(i=0;i<NEWHOPE_N;i++)
    r->coeffs[i] = barrett_reduce(a->coeffs[i] + 3*NEWHOPE_Q - b->coeffs[i]);
}

void poly_ntt(poly *r)
{
  mul_coefficients(r->coeffs, psis_bitrev, r->coeffs); 
  ntt(r->coeffs, omegas_double);
}

void poly_invntt(poly *r)
{
  bitrev_vector(r->coeffs);
  ntt(r->coeffs, omegas_inv_double);
  mul_coefficients(r->coeffs, psis_inv, r->coeffs);
}
