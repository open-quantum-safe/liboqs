#include <stdint.h>
#include "params.h"
#include "poly.h"
#include "ntt.h"
#include "reduce.h"
#include "cbd.h"
#include "symmetric.h"
extern param params[];

/*************************************************
* Name:        poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (of length KYBER_POLYCOMPRESSEDBYTES)
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_compress(uint8_t *r, const poly *a, int8_t security_level)
{
  unsigned int i,j;
  int16_t u;
  uint8_t t[8];

  if (params[security_level].KYBER_POLYCOMPRESSEDBYTES == 128)
    for(i=0;i<(unsigned int)params[security_level].KYBER_N/8;i++) {
      for(j=0;j<8;j++) {
      // map to positive standard representatives
        u  = a->coeffs[8*i+j];
        u += (u >> 15) & params[security_level].KYBER_Q;
        t[j] = ((((uint16_t)u << 4) + params[security_level].KYBER_Q/2)/params[security_level].KYBER_Q) & 15;
      }

    r[0] = t[0] | (t[1] << 4);
    r[1] = t[2] | (t[3] << 4);
    r[2] = t[4] | (t[5] << 4);
    r[3] = t[6] | (t[7] << 4);
    r += 4;
  }
else if (params[security_level].KYBER_POLYCOMPRESSEDBYTES == 160)
  for(i=0;i<(unsigned int)params[security_level].KYBER_N/8;i++) {
    for(j=0;j<8;j++) {
      // map to positive standard representatives
      u  = a->coeffs[8*i+j];
      u += (u >> 15) & params[security_level].KYBER_Q;
      t[j] = ((((uint32_t)u << 5) + params[security_level].KYBER_Q/2)/params[security_level].KYBER_Q) & 31;
    }

    r[0] = (t[0] >> 0) | (t[1] << 5);
    r[1] = (t[1] >> 3) | (t[2] << 2) | (t[3] << 7);
    r[2] = (t[3] >> 1) | (t[4] << 4);
    r[3] = (t[4] >> 4) | (t[5] << 1) | (t[6] << 6);
    r[4] = (t[6] >> 2) | (t[7] << 3);
    r += 5;
  }
// #error "KYBER_POLYCOMPRESSEDBYTES needs to be in {128, 160}"
}

/*************************************************
* Name:        poly_decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of poly_compress
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYCOMPRESSEDBYTES bytes)
**************************************************/
void poly_decompress(poly *r, const uint8_t *a, int8_t security_level)
{
  unsigned int i;

  if (params[security_level].KYBER_POLYCOMPRESSEDBYTES == 128)
    for(i=0;i<(unsigned int)params[security_level].KYBER_N/2;i++) {
      r->coeffs[2*i+0] = (((uint16_t)(a[0] & 15)*params[security_level].KYBER_Q) + 8) >> 4;
      r->coeffs[2*i+1] = (((uint16_t)(a[0] >> 4)*params[security_level].KYBER_Q) + 8) >> 4;
      a += 1;
    }
  else if (params[security_level].KYBER_POLYCOMPRESSEDBYTES == 160){
    unsigned int j;
    uint8_t t[8];
    for(i=0;i<(unsigned int)params[security_level].KYBER_N/8;i++) {
      t[0] = (a[0] >> 0);
      t[1] = (a[0] >> 5) | (a[1] << 3);
      t[2] = (a[1] >> 2);
      t[3] = (a[1] >> 7) | (a[2] << 1);
      t[4] = (a[2] >> 4) | (a[3] << 4);
      t[5] = (a[3] >> 1);
      t[6] = (a[3] >> 6) | (a[4] << 2);
      t[7] = (a[4] >> 3);
      a += 5;

      for(j=0;j<8;j++)
        r->coeffs[8*i+j] = ((uint32_t)(t[j] & 31)*params[security_level].KYBER_Q + 16) >> 5;
    }
  }
// #error "KYBER_POLYCOMPRESSEDBYTES needs to be in {128, 160}"

}

/*************************************************
* Name:        poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYBYTES bytes)
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_tobytes(uint8_t *r, const poly *a, int8_t security_level)
{
  unsigned int i;
  uint16_t t0, t1;

  for(i=0;i<(unsigned int)params[security_level].KYBER_N/2;i++) {
    // map to positive standard representatives
    t0  = a->coeffs[2*i];
    t0 += ((int16_t)t0 >> 15) & params[security_level].KYBER_Q;
    t1 = a->coeffs[2*i+1];
    t1 += ((int16_t)t1 >> 15) & params[security_level].KYBER_Q;
    r[3*i+0] = (t0 >> 0);
    r[3*i+1] = (t0 >> 8) | (t1 << 4);
    r[3*i+2] = (t1 >> 4);
  }
}

/*************************************************
* Name:        poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of poly_tobytes
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of KYBER_POLYBYTES bytes)
**************************************************/
void poly_frombytes(poly *r, const uint8_t *a, int8_t security_level)
{
  unsigned int i;
  for(i=0;i<(unsigned int)params[security_level].KYBER_N/2;i++) {
    r->coeffs[2*i]   = ((a[3*i+0] >> 0) | ((uint16_t)a[3*i+1] << 8)) & 0xFFF;
    r->coeffs[2*i+1] = ((a[3*i+1] >> 4) | ((uint16_t)a[3*i+2] << 4)) & 0xFFF;
  }
}

/*************************************************
* Name:        poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *msg: pointer to input message
**************************************************/
void poly_frommsg(poly *r, const uint8_t *msg,int8_t security_level)
{
  unsigned int i,j;
  int16_t mask;

// #if (KYBER_INDCPA_MSGBYTES != KYBER_N/8)
// #error "KYBER_INDCPA_MSGBYTES must be equal to KYBER_N/8 bytes!"
// #endif

  for(i=0;i<(unsigned int)params[security_level].KYBER_N/8;i++) {
    for(j=0;j<8;j++) {
      mask = -(int16_t)((msg[i] >> j)&1);
      r->coeffs[8*i+j] = mask & ((params[security_level].KYBER_Q+1)/2);
    }
  }
}

/*************************************************
* Name:        poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_tomsg(uint8_t *msg, const poly *a,int8_t security_level)
{
  unsigned int i,j;
  uint16_t t;

  for(i=0;i<(unsigned int)params[security_level].KYBER_N/8;i++) {
    msg[i] = 0;
    for(j=0;j<8;j++) {
      t  = a->coeffs[8*i+j];
      t += ((int16_t)t >> 15) & params[security_level].KYBER_Q;
      t  = (((t << 1) + params[security_level].KYBER_Q/2)/params[security_level].KYBER_Q) & 1;
      msg[i] |= t << j;
    }
  }
}

/*************************************************
* Name:        poly_getnoise_eta1
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA1
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void poly_getnoise_eta1(poly *r, const uint8_t *seed, uint8_t nonce, int8_t security_level)
{
  uint8_t buf[params[security_level].KYBER_ETA1*params[security_level].KYBER_N/4];
  prf(buf, sizeof(buf), seed, nonce, security_level);
  poly_cbd_eta1(r, buf, security_level);
}

/*************************************************
* Name:        poly_getnoise_eta2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA2
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void poly_getnoise_eta2(poly *r, const uint8_t *seed, uint8_t nonce,int8_t security_level)
{
  uint8_t buf[params[security_level].KYBER_ETA2*params[security_level].KYBER_N/4];
  prf(buf, sizeof(buf), seed, nonce, security_level);
  poly_cbd_eta2(r, buf, security_level);
}


/*************************************************
* Name:        poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16_t *r: pointer to in/output polynomial
**************************************************/
void poly_ntt(poly *r,int8_t security_level)
{
  ntt(r->coeffs, security_level);
  poly_reduce(r,security_level);
}

/*************************************************
* Name:        poly_invntt_tomont
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
void poly_invntt_tomont(poly *r, int8_t security_level)
{
  invntt(r->coeffs, security_level);
}

/*************************************************
* Name:        poly_basemul_montgomery
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void poly_basemul_montgomery(poly *r, const poly *a, const poly *b, int8_t security_level)
{
  unsigned int i;
  for(i=0;i<(unsigned int)params[security_level].KYBER_N/4;i++) {
    basemul(&r->coeffs[4*i], &a->coeffs[4*i], &b->coeffs[4*i], zetas[64+i], security_level);
    basemul(&r->coeffs[4*i+2], &a->coeffs[4*i+2], &b->coeffs[4*i+2], -zetas[64+i], security_level);
  }
}

/*************************************************
* Name:        poly_tomont
*
* Description: Inplace conversion of all coefficients of a polynomial
*              from normal domain to Montgomery domain
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void poly_tomont(poly *r,int8_t security_level)
{
  unsigned int i;
  const int16_t f = (1ULL << 32) % params[security_level].KYBER_Q;
  for(i=0;i<(unsigned int)params[security_level].KYBER_N;i++)
    r->coeffs[i] = montgomery_reduce((int32_t)r->coeffs[i]*f, security_level);
}

/*************************************************
* Name:        poly_reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void poly_reduce(poly *r,int8_t security_level)
{
  unsigned int i;
  for(i=0;i<(unsigned int)params[security_level].KYBER_N;i++)
    r->coeffs[i] = barrett_reduce(r->coeffs[i], security_level);
}

/*************************************************
* Name:        poly_add
*
* Description: Add two polynomials; no modular reduction is performed
*
* Arguments: - poly *r: pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void poly_add(poly *r, const poly *a, const poly *b,int8_t security_level)
{
  unsigned int i;
  for(i=0;i<(unsigned int)params[security_level].KYBER_N;i++)
    r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
}

/*************************************************
* Name:        poly_sub
*
* Description: Subtract two polynomials; no modular reduction is performed
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void poly_sub(poly *r, const poly *a, const poly *b, int8_t security_level)
{
  unsigned int i;
  for(i=0;i<(unsigned int)params[security_level].KYBER_N;i++)
    r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
}
