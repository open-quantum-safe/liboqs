#include <stdio.h>
#include "api.h"
#include "poly.h"
#include "rng.h"
#include "fips202.h"

static void encode_pk(unsigned char *r, const poly *pk, const unsigned char *seed)
{
  int i;
  poly_tobytes(r, pk);
  for(i=0;i<NEWHOPE_SYMBYTES;i++)
    r[NEWHOPE_POLYBYTES+i] = seed[i];
}

static void decode_pk(poly *pk, unsigned char *seed, const unsigned char *r)
{
  int i;
  poly_frombytes(pk, r);
  for(i=0;i<NEWHOPE_SYMBYTES;i++)
    seed[i] = r[NEWHOPE_POLYBYTES+i];
}

static void encode_c(unsigned char *r, const poly *b, const poly *v)
{
  poly_tobytes(r,b);
  poly_compress(r+NEWHOPE_POLYBYTES,v);
}

static void decode_c(poly *b, poly *v, const unsigned char *r)
{
  poly_frombytes(b, r);
  poly_decompress(v, r+NEWHOPE_POLYBYTES);
}

static void gen_a(poly *a, const unsigned char *seed)
{
  poly_uniform(a,seed);
}


// API FUNCTIONS 

void cpapke_keypair(unsigned char *pk, 
                    unsigned char *sk)
{
  poly a, e, r, pkp, skp;
  unsigned char buf[2*NEWHOPE_SYMBYTES];
  unsigned char *publicseed = buf;
  unsigned char *noiseseed = buf+NEWHOPE_SYMBYTES;

  randombytes(buf, NEWHOPE_SYMBYTES);
  shake256(buf, 2*NEWHOPE_SYMBYTES, buf, NEWHOPE_SYMBYTES);

  gen_a(&a, publicseed);

  poly_sample(&skp,noiseseed,0);
  poly_ntt(&skp);
  
  poly_sample(&e,noiseseed,1);
  poly_ntt(&e);

  poly_pointwise(&r,&skp,&a);
  poly_add(&pkp,&e,&r);

  poly_tobytes(sk,&skp);
  encode_pk(pk, &pkp, publicseed);
}


void cpapke_enc(unsigned char *c,
                const unsigned char *m,
                const unsigned char *pk,
                const unsigned char *coins)
{
  poly sp, ep, v, a, pka, epp, bp, mp;
  unsigned char seed[NEWHOPE_SYMBYTES];
  
  poly_frommsg(&mp, m);

  decode_pk(&pka, seed, pk);
  gen_a(&a, seed);

  poly_sample(&sp,coins,0);

  poly_ntt(&sp);
  poly_sample(&ep,coins,1);
  poly_ntt(&ep);


  poly_pointwise(&bp, &a, &sp);
  poly_add(&bp, &bp, &ep);
 
  poly_pointwise(&v, &pka, &sp);
  poly_invntt(&v);

  poly_sample(&epp,coins,2);
  poly_add(&v, &v, &epp);
  poly_add(&v, &v, &mp); // add message

  encode_c(c, &bp, &v);
}


void cpapke_dec(unsigned char *m,
                const unsigned char *c,
                const unsigned char *sk)
{
  poly v,bp,kp,skp;

  poly_frombytes(&skp, sk);

  decode_c(&bp, &v, c);
  poly_pointwise(&kp,&skp,&bp);
  poly_invntt(&kp);

  poly_sub(&kp,&kp,&v);

  poly_tomsg(m, &kp);
}
