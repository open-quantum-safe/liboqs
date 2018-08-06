#include <stdint.h>
#include "fips202.h"
#include "params.h"
#include "reduce.h"
#include "ntt.h"
#include "poly.h"

/*************************************************
* Name:        poly_freeze
* 
* Description: Reduce all coefficients of polynomial to standard
*              representatives.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_freeze(poly *a) {
  unsigned int i;

  for(i = 0; i < N; ++i)
    a->coeffs[i] = freeze(a->coeffs[i]);
}

/*************************************************
* Name:        poly_add
* 
* Description: Add polynomials. No modular reduction is performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - poly *a: pointer to first summand
*              - poly *b: pointer to second summand
**************************************************/
void poly_add(poly *c, const poly *a, const poly *b)  {
  unsigned int i;

  for(i = 0; i < N; ++i)
    c->coeffs[i] = a->coeffs[i] + b->coeffs[i];
}

/*************************************************
* Name:        poly_sub
* 
* Description: Subtract polynomials. Assumes coefficients of input polynomials
*              to be less than 2*Q. No modular reduction is performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - poly *a: pointer to first input polynomial
*              - poly *b: pointer to second input polynomial to be subtraced
*                         from first input polynomial
**************************************************/
void poly_sub(poly *c, const poly *a, const poly *b) {
  unsigned int i;

  for(i = 0; i < N; ++i)
    c->coeffs[i] = a->coeffs[i] + 2*Q - b->coeffs[i];
}

/*************************************************
* Name:        poly_neg
* 
* Description: Negate polynomial. Assumes input coefficients to be less
*              than 2*Q.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_neg(poly *a) {
  unsigned int i;

  for(i = 0; i < N; ++i)
    a->coeffs[i] = 2*Q - a->coeffs[i];
}

/*************************************************
* Name:        poly_shiftl
* 
* Description: Multiply polynomial by 2^k.
*
* Arguments:   - poly *a: pointer to input/output polynomial
*              - unsigned int k: exponent
**************************************************/
void poly_shiftl(poly *a, unsigned int k) {
  unsigned int i;

  for(i = 0; i < N; ++i)
    a->coeffs[i] <<= k;
}

/*************************************************
* Name:        poly_ntt
* 
* Description: Forward NTT. Output coefficients can be up to 16*Q larger than
*              input coefficients.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_ntt(poly *a) {
  ntt(a->coeffs);
}

/*************************************************
* Name:        poly_invntt_montgomery
* 
* Description: Inverse NTT and multiplication with 2^{32}. Output coefficients
*              are less than 2*Q.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_invntt_montgomery(poly *a) {
  invntt_frominvmont(a->coeffs);
}

/*************************************************
* Name:        poly_pointwise_invmontgomery
* 
* Description: Pointwise multiplication of polynomials in NTT domain
*              representation and multiplication of resulting polynomial
*              with 2^{-32}.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - poly *a: pointer to first input polynomial
*              - poly *b: pointer to second input polynomial
**************************************************/
void poly_pointwise_invmontgomery(poly *c, const poly *a, const poly *b) {
  unsigned int i;

  for(i = 0; i < N; ++i)
    c->coeffs[i] = montgomery_reduce((uint64_t)a->coeffs[i] * b->coeffs[i]);
}

/*************************************************
* Name:        poly_chknorm
* 
* Description: Check infinity norm of polynomial against given bound.
*              Assumes input coefficients to be standard representatives.
*
* Arguments:   - const poly *a: pointer to polynomial
*              - uint32_t B: norm bound
*
* Returns 0 if norm is strictly smaller than B and 1 otherwise.
**************************************************/
int poly_chknorm(const poly *a, uint32_t B) {
  unsigned int i;
  int32_t t;

  /* It is ok to leak which coefficient violates the bound since
     the probability for each coefficient is independent of secret
     data but we must not leak the sign of the centralized representative. */
  for(i = 0; i < N; ++i) {
    /* Absolute value of centralized representative */
    t = (Q-1)/2 - a->coeffs[i];
    t ^= (t >> 31);
    t = (Q-1)/2 - t;

    if((uint32_t)t >= B)
      return 1;
  }

  return 0;
}

/*************************************************
* Name:        poly_uniform
* 
* Description: Sample uniformly random polynomial using stream of random bytes.
*              Assumes that enough random bytes are given (e.g.
*              5*SHAKE128_RATE bytes).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - unsigned char *buf: array of random bytes
**************************************************/
void poly_uniform(poly *a, unsigned char *buf) {
  unsigned int ctr, pos;
  uint32_t t;

  ctr = pos = 0;
  while(ctr < N) {
    t  = buf[pos++];
    t |= (uint32_t)buf[pos++] << 8;
    t |= (uint32_t)buf[pos++] << 16;
    t &= 0x7FFFFF;

    if(t < Q)
      a->coeffs[ctr++] = t;
  }
}

/*************************************************
* Name:        rej_eta
* 
* Description: Sample uniformly random coefficients in [-ETA, ETA] by
*              performing rejection sampling using array of random bytes.
*
* Arguments:   - uint32_t *a: pointer to output array (allocated)
*              - unsigned int len: number of coefficients to be sampled
*              - const unsigned char *buf: array of random bytes
*              - unsigned int buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static unsigned int rej_eta(uint32_t *a,
                            unsigned int len,
                            const unsigned char *buf,
                            unsigned int buflen)
{
#if ETA > 7
#error "rej_eta() assumes ETA <= 7"
#endif
  unsigned int ctr, pos;
  unsigned char t0, t1;

  ctr = pos = 0;
  while(ctr < len) {
#if ETA <= 3
    t0 = buf[pos] & 0x07;
    t1 = buf[pos++] >> 5;
#else
    t0 = buf[pos] & 0x0F;
    t1 = buf[pos++] >> 4;
#endif

    if(t0 <= 2*ETA)
      a[ctr++] = Q + ETA - t0;
    if(t1 <= 2*ETA && ctr < len)
      a[ctr++] = Q + ETA - t1;

    if(pos >= buflen)
      break;
  }

  return ctr;
}

/*************************************************
* Name:        poly_uniform_eta
* 
* Description: Sample polynomial with uniformly random coefficients
*              in [-ETA,ETA] by performing rejection sampling using the
*              output stream from SHAKE256(seed|nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const unsigned char seed[]: byte array with seed of length
*                                            SEEDBYTES
*              - unsigned char nonce: nonce byte
**************************************************/
void poly_uniform_eta(poly *a,
                      const unsigned char seed[SEEDBYTES], 
                      unsigned char nonce)
{
  unsigned int i, ctr;
  unsigned char inbuf[SEEDBYTES + 1];
  /* Probability that we need more than 2 blocks: < 2^{-84} */
  unsigned char outbuf[2*SHAKE256_RATE];
  uint64_t state[25];

  for(i= 0; i < SEEDBYTES; ++i)
    inbuf[i] = seed[i];
  inbuf[SEEDBYTES] = nonce;

  shake256_absorb(state, inbuf, SEEDBYTES + 1);
  shake256_squeezeblocks(outbuf, 2, state);

  ctr = rej_eta(a->coeffs, N, outbuf, 2*SHAKE256_RATE);
  if(ctr < N) {
    shake256_squeezeblocks(outbuf, 1, state);
    rej_eta(a->coeffs + ctr, N - ctr, outbuf, SHAKE256_RATE);
  }
}

/*************************************************
* Name:        rej_gamma1m1
* 
* Description: Sample uniformly random coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1] by performing rejection sampling
*              using array of random bytes.
*
* Arguments:   - uint32_t *a: pointer to output array (allocated)
*              - unsigned int len: number of coefficients to be sampled
*              - const unsigned char *buf: array of random bytes
*              - unsigned int buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static unsigned int rej_gamma1m1(uint32_t *a,
                                 unsigned int len,
                                 const unsigned char *buf,
                                 unsigned int buflen)
{
#if GAMMA1 > (1 << 19)
#error "rej_gamma1m1() assumes GAMMA1 - 1 fits in 19 bits"
#endif
  unsigned int ctr, pos;
  uint32_t t0, t1;

  ctr = pos = 0;
  while(ctr < len) {
    t0  = buf[pos];
    t0 |= (uint32_t)buf[pos + 1] << 8;
    t0 |= (uint32_t)buf[pos + 2] << 16;
    t0 &= 0xFFFFF;

    t1  = buf[pos + 2] >> 4;
    t1 |= (uint32_t)buf[pos + 3] << 4;
    t1 |= (uint32_t)buf[pos + 4] << 12;

    pos += 5;

    if(t0 <= 2*GAMMA1 - 2)
      a[ctr++] = Q + GAMMA1 - 1 - t0;
    if(t1 <= 2*GAMMA1 - 2 && ctr < len)
      a[ctr++] = Q + GAMMA1 - 1 - t1;

    if(pos > buflen - 5)
      break;
  }

  return ctr;
}

/*************************************************
* Name:        poly_uniform_gamma1m1
* 
* Description: Sample polynomial with uniformly random coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1] by performing rejection
*              sampling on output stream of SHAKE256(seed|nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const unsigned char seed[]: byte array with seed of length
*                                            SEEDBYTES + CRHBYTES
*              - uint16_t nonce: 16-bit nonce
**************************************************/
void poly_uniform_gamma1m1(poly *a,
                           const unsigned char seed[SEEDBYTES + CRHBYTES],
                           uint16_t nonce)
{
  unsigned int i, ctr;
  unsigned char inbuf[SEEDBYTES + CRHBYTES + 2];
  /* Probability that we need more than 5 blocks: < 2^{-81}
     Probability that we need more than 6 blocks: < ... */
  unsigned char outbuf[5*SHAKE256_RATE];
  uint64_t state[25];

  for(i = 0; i < SEEDBYTES + CRHBYTES; ++i)
    inbuf[i] = seed[i];
  inbuf[SEEDBYTES + CRHBYTES] = nonce & 0xFF;
  inbuf[SEEDBYTES + CRHBYTES + 1] = nonce >> 8;

  shake256_absorb(state, inbuf, SEEDBYTES + CRHBYTES + 2);
  shake256_squeezeblocks(outbuf, 5, state);

  ctr = rej_gamma1m1(a->coeffs, N, outbuf, 5*SHAKE256_RATE);
  if(ctr < N) {
    /* There are no bytes left in outbuf
       since 5*SHAKE256_RATE is divisible by 5 */
    shake256_squeezeblocks(outbuf, 1, state);
    rej_gamma1m1(a->coeffs + ctr, N - ctr, outbuf, SHAKE256_RATE);
  }
}

/*************************************************
* Name:        polyeta_pack
* 
* Description: Bit-pack polynomial with coefficients in [-ETA,ETA].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLETA_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyeta_pack(unsigned char *r, const poly *a) {
#if ETA > 7
#error "polyeta_pack() assumes ETA <= 7"
#endif
  unsigned int i;
  unsigned char t[8];

#if ETA <= 3
  for(i = 0; i < N/8; ++i) {
    t[0] = Q + ETA - a->coeffs[8*i+0];
    t[1] = Q + ETA - a->coeffs[8*i+1];
    t[2] = Q + ETA - a->coeffs[8*i+2];
    t[3] = Q + ETA - a->coeffs[8*i+3];
    t[4] = Q + ETA - a->coeffs[8*i+4];
    t[5] = Q + ETA - a->coeffs[8*i+5];
    t[6] = Q + ETA - a->coeffs[8*i+6];
    t[7] = Q + ETA - a->coeffs[8*i+7];

    r[3*i+0]  = t[0];
    r[3*i+0] |= t[1] << 3;
    r[3*i+0] |= t[2] << 6;
    r[3*i+1]  = t[2] >> 2;
    r[3*i+1] |= t[3] << 1;
    r[3*i+1] |= t[4] << 4;
    r[3*i+1] |= t[5] << 7;
    r[3*i+2]  = t[5] >> 1;
    r[3*i+2] |= t[6] << 2;
    r[3*i+2] |= t[7] << 5;
  }
#else
  for(i = 0; i < N/2; ++i) {
    t[0] = Q + ETA - a->coeffs[2*i+0];
    t[1] = Q + ETA - a->coeffs[2*i+1];
    r[i] = t[0] | (t[1] << 4);
  }
#endif
}

/*************************************************
* Name:        polyeta_unpack
* 
* Description: Unpack polynomial with coefficients in [-ETA,ETA].
*              Output coefficients are not standard representatives but
*              no greater than Q + ETA.
*              
* Arguments:   - poly *r: pointer to output polynomial
*              - const unsigned char *a: byte array with bit-packed polynomial
**************************************************/
void polyeta_unpack(poly *r, const unsigned char *a) {
  unsigned int i;

#if ETA <= 3
  for(i = 0; i < N/8; ++i) {
    r->coeffs[8*i+0] = a[3*i+0] & 0x07;
    r->coeffs[8*i+1] = (a[3*i+0] >> 3) & 0x07;
    r->coeffs[8*i+2] = (a[3*i+0] >> 6) | ((a[3*i+1] & 0x01) << 2);
    r->coeffs[8*i+3] = (a[3*i+1] >> 1) & 0x07;
    r->coeffs[8*i+4] = (a[3*i+1] >> 4) & 0x07;
    r->coeffs[8*i+5] = (a[3*i+1] >> 7) | ((a[3*i+2] & 0x03) << 1);
    r->coeffs[8*i+6] = (a[3*i+2] >> 2) & 0x07;
    r->coeffs[8*i+7] = (a[3*i+2] >> 5);

    r->coeffs[8*i+0] = Q + ETA - r->coeffs[8*i+0];
    r->coeffs[8*i+1] = Q + ETA - r->coeffs[8*i+1];
    r->coeffs[8*i+2] = Q + ETA - r->coeffs[8*i+2];
    r->coeffs[8*i+3] = Q + ETA - r->coeffs[8*i+3];
    r->coeffs[8*i+4] = Q + ETA - r->coeffs[8*i+4];
    r->coeffs[8*i+5] = Q + ETA - r->coeffs[8*i+5];
    r->coeffs[8*i+6] = Q + ETA - r->coeffs[8*i+6];
    r->coeffs[8*i+7] = Q + ETA - r->coeffs[8*i+7];
  }
#else
  for(i = 0; i < N/2; ++i) {
    r->coeffs[2*i+0] = a[i] & 0x0F;
    r->coeffs[2*i+1] = a[i] >> 4;
    r->coeffs[2*i+0] = Q + ETA - r->coeffs[2*i+0];
    r->coeffs[2*i+1] = Q + ETA - r->coeffs[2*i+1];
  }
#endif
}

/*************************************************
* Name:        polyt1_pack
* 
* Description: Bit-pack polynomial t1 with coefficients fitting in 9 bits.
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLT1_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyt1_pack(unsigned char *r, const poly *a) {
#if D != 14
#error "polyt1_pack() assumes D == 14"
#endif
  unsigned int i;

  for(i = 0; i < N/8; ++i) {
    r[9*i+0]  =  a->coeffs[8*i+0] & 0xFF;
    r[9*i+1]  = (a->coeffs[8*i+0] >> 8) | ((a->coeffs[8*i+1] & 0x7F) << 1);
    r[9*i+2]  = (a->coeffs[8*i+1] >> 7) | ((a->coeffs[8*i+2] & 0x3F) << 2);
    r[9*i+3]  = (a->coeffs[8*i+2] >> 6) | ((a->coeffs[8*i+3] & 0x1F) << 3);
    r[9*i+4]  = (a->coeffs[8*i+3] >> 5) | ((a->coeffs[8*i+4] & 0x0F) << 4);
    r[9*i+5]  = (a->coeffs[8*i+4] >> 4) | ((a->coeffs[8*i+5] & 0x07) << 5);
    r[9*i+6]  = (a->coeffs[8*i+5] >> 3) | ((a->coeffs[8*i+6] & 0x03) << 6);
    r[9*i+7]  = (a->coeffs[8*i+6] >> 2) | ((a->coeffs[8*i+7] & 0x01) << 7);
    r[9*i+8]  =  a->coeffs[8*i+7] >> 1;
  }
}

/*************************************************
* Name:        polyt1_unpack
* 
* Description: Unpack polynomial t1 with 9-bit coefficients.
*              Output coefficients are not standard representatives.
*              
* Arguments:   - poly *r: pointer to output polynomial
*              - const unsigned char *a: byte array with bit-packed polynomial
**************************************************/
void polyt1_unpack(poly *r, const unsigned char *a) {
  unsigned int i;

  for(i = 0; i < N/8; ++i) {
    r->coeffs[8*i+0] =  a[9*i+0]       | ((uint32_t)(a[9*i+1] & 0x01) << 8);
    r->coeffs[8*i+1] = (a[9*i+1] >> 1) | ((uint32_t)(a[9*i+2] & 0x03) << 7);
    r->coeffs[8*i+2] = (a[9*i+2] >> 2) | ((uint32_t)(a[9*i+3] & 0x07) << 6);
    r->coeffs[8*i+3] = (a[9*i+3] >> 3) | ((uint32_t)(a[9*i+4] & 0x0F) << 5);
    r->coeffs[8*i+4] = (a[9*i+4] >> 4) | ((uint32_t)(a[9*i+5] & 0x1F) << 4);
    r->coeffs[8*i+5] = (a[9*i+5] >> 5) | ((uint32_t)(a[9*i+6] & 0x3F) << 3);
    r->coeffs[8*i+6] = (a[9*i+6] >> 6) | ((uint32_t)(a[9*i+7] & 0x7F) << 2);
    r->coeffs[8*i+7] = (a[9*i+7] >> 7) | ((uint32_t)(a[9*i+8] & 0xFF) << 1);
  }
}

/*************************************************
* Name:        polyt0_pack
* 
* Description: Bit-pack polynomial t0 with coefficients in ]-2^{D/2}, 2^{D/2}].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLT0_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyt0_pack(unsigned char *r, const poly *a) {
  unsigned int i;
  uint32_t t[4];

  for(i = 0; i < N/4; ++i) {
    t[0] = Q + (1 << (D-1)) - a->coeffs[4*i+0];
    t[1] = Q + (1 << (D-1)) - a->coeffs[4*i+1];
    t[2] = Q + (1 << (D-1)) - a->coeffs[4*i+2];
    t[3] = Q + (1 << (D-1)) - a->coeffs[4*i+3];

    r[7*i+0]  =  t[0];
    r[7*i+1]  =  t[0] >> 8;
    r[7*i+1] |=  t[1] << 6;
    r[7*i+2]  =  t[1] >> 2;
    r[7*i+3]  =  t[1] >> 10;
    r[7*i+3] |=  t[2] << 4;
    r[7*i+4]  =  t[2] >> 4;
    r[7*i+5]  =  t[2] >> 12;
    r[7*i+5] |=  t[3] << 2;
    r[7*i+6]  =  t[3] >> 6;
  }
}

/*************************************************
* Name:        polyt0_unpack
* 
* Description: Unpack polynomial t0 with coefficients in ]-2^{D/2}, 2^{D/2}].
*              Output coefficients are not standard representatives but at most
*              Q + 2^{D/2}.
*              
* Arguments:   - poly *r: pointer to output polynomial
*              - const unsigned char *a: byte array with bit-packed polynomial
**************************************************/
void polyt0_unpack(poly *r, const unsigned char *a) {
  unsigned int i;

  for(i = 0; i < N/4; ++i) {
    r->coeffs[4*i+0]  = a[7*i+0];
    r->coeffs[4*i+0] |= (uint32_t)(a[7*i+1] & 0x3F) << 8;

    r->coeffs[4*i+1]  = a[7*i+1] >> 6;
    r->coeffs[4*i+1] |= (uint32_t)a[7*i+2] << 2;
    r->coeffs[4*i+1] |= (uint32_t)(a[7*i+3] & 0x0F) << 10;
    
    r->coeffs[4*i+2]  = a[7*i+3] >> 4;
    r->coeffs[4*i+2] |= (uint32_t)a[7*i+4] << 4;
    r->coeffs[4*i+2] |= (uint32_t)(a[7*i+5] & 0x03) << 12;

    r->coeffs[4*i+3]  = a[7*i+5] >> 2;
    r->coeffs[4*i+3] |= (uint32_t)a[7*i+6] << 6;

    r->coeffs[4*i+0] = Q + (1 << (D-1)) - r->coeffs[4*i+0];
    r->coeffs[4*i+1] = Q + (1 << (D-1)) - r->coeffs[4*i+1];
    r->coeffs[4*i+2] = Q + (1 << (D-1)) - r->coeffs[4*i+2];
    r->coeffs[4*i+3] = Q + (1 << (D-1)) - r->coeffs[4*i+3];
  }
}

/*************************************************
* Name:        polyz_pack
* 
* Description: Bit-pack polynomial z with coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLZ_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyz_pack(unsigned char *r, const poly *a) {
#if GAMMA1 > (1 << 19)
#error "polyz_pack() assumes GAMMA1 <= 2^{19}"
#endif
  unsigned int i;
  uint32_t t[2];

  for(i = 0; i < N/2; ++i) {
    /* Map to {0,...,2*GAMMA1 - 2} */
    t[0] = GAMMA1 - 1 - a->coeffs[2*i+0];
    t[0] += ((int32_t)t[0] >> 31) & Q;
    t[1] = GAMMA1 - 1 - a->coeffs[2*i+1];
    t[1] += ((int32_t)t[1] >> 31) & Q;

    r[5*i+0]  = t[0];
    r[5*i+1]  = t[0] >> 8;
    r[5*i+2]  = t[0] >> 16;
    r[5*i+2] |= t[1] << 4;
    r[5*i+3]  = t[1] >> 4;
    r[5*i+4]  = t[1] >> 12;
  }
}

/*************************************************
* Name:        polyz_unpack
* 
* Description: Unpack polynomial z with coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1].
*              Output coefficients are not standard representatives but at
*              most Q + GAMMA1 - 1.
*              
* Arguments:   - poly *r: pointer to output polynomial
*              - const unsigned char *a: byte array with bit-packed polynomial
**************************************************/
void polyz_unpack(poly *r, const unsigned char *a) {
  unsigned int i;

  for(i = 0; i < N/2; ++i) {
    r->coeffs[2*i+0]  = a[5*i+0];
    r->coeffs[2*i+0] |= (uint32_t)a[5*i+1] << 8;
    r->coeffs[2*i+0] |= (uint32_t)(a[5*i+2] & 0x0F) << 16;

    r->coeffs[2*i+1]  = a[5*i+2] >> 4;
    r->coeffs[2*i+1] |= (uint32_t)a[5*i+3] << 4;
    r->coeffs[2*i+1] |= (uint32_t)a[5*i+4] << 12;

    r->coeffs[2*i+0] = GAMMA1 - 1 - r->coeffs[2*i+0];
    r->coeffs[2*i+0] += ((int32_t)r->coeffs[2*i+0] >> 31) & Q;
    r->coeffs[2*i+1] = GAMMA1 - 1 - r->coeffs[2*i+1];
    r->coeffs[2*i+1] += ((int32_t)r->coeffs[2*i+1] >> 31) & Q;
  }
}

/*************************************************
* Name:        polyw1_pack
* 
* Description: Bit-pack polynomial w1 with coefficients in [0, 15].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLW1_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyw1_pack(unsigned char *r, const poly *a) {
  unsigned int i;

  for(i = 0; i < N/2; ++i)
    r[i] = a->coeffs[2*i+0] | (a->coeffs[2*i+1] << 4);
}
