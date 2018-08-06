#include <stdint.h>
#include "params.h"
#include "reduce.h"
#include "rounding.h"
#include "poly.h"
#include "polyvec.h"

/**************************************************************/
/************ Vectors of polynomials of length L **************/
/**************************************************************/

/*************************************************
* Name:        polyvecl_freeze
* 
* Description: Reduce coefficients of polynomials in vector of length L
*              to standard representatives.
*
* Arguments:   - polyvecl *v: pointer to input/output vector
**************************************************/
void polyvecl_freeze(polyvecl *v) {
  unsigned int i;

  for(i = 0; i < L; ++i)
    poly_freeze(v->vec+i);
}

/*************************************************
* Name:        polyvecl_add
* 
* Description: Add vectors of polynomials of length L.
*              No modular reduction is performed.
*
* Arguments:   - polyvecl *w: pointer to output vector
*              - polyvecl *u: pointer to first summand
*              - polyvecl *v: pointer to second summand
**************************************************/
void polyvecl_add(polyvecl *w, const polyvecl *u, const polyvecl *v) {
  unsigned int i;

  for(i = 0; i < L; ++i)
    poly_add(w->vec+i, u->vec+i, v->vec+i);
}

/*************************************************
* Name:        polyvecl_ntt
* 
* Description: Forward NTT of all polynomials in vector of length L. Output
*              coefficients can be up to 16*Q larger than input coefficients.
*
* Arguments:   - polyvecl *v: pointer to input/output vector
**************************************************/
void polyvecl_ntt(polyvecl *v) {
  unsigned int i;

  for(i = 0; i < L; ++i)
    poly_ntt(v->vec+i);
}

/*************************************************
* Name:        polyvecl_pointwise_acc_invmontgomery
* 
* Description: Pointwise multiply vectors of polynomials of length L, multiply *              resulting vector by 2^{-32} and add (accumulate) polynomials
*              in it. Input/output vectors are in NTT domain representation.
*              Output coeffcient are less than 2*Q.
*
* Arguments:   - poly *w: output polynomial
*              - const polyvecl *u: pointer to first input vector
*              - const polyvecl *v: pointer to second input vector
**************************************************/
void polyvecl_pointwise_acc_invmontgomery(poly *w,
                                          const polyvecl *u,
                                          const polyvecl *v) 
{
  unsigned int i;
  poly t;

  poly_pointwise_invmontgomery(w, u->vec+0, v->vec+0);

  for(i = 1; i < L; ++i) {
    poly_pointwise_invmontgomery(&t, u->vec+i, v->vec+i);
    poly_add(w, w, &t);
  }

  for(i = 0; i < N; ++i) 
    w->coeffs[i] = reduce32(w->coeffs[i]);
}

/*************************************************
* Name:        polyvecl_chknorm
* 
* Description: Check infinity norm of polynomials in vector of length L.
*              Assumes input coefficients to be standard representatives.
*
* Arguments:   - const polyvecl *v: pointer to vector
*              - uint32_t B: norm bound
*
* Returns 0 if norm of all polynomials is strictly smaller than B and 1
* otherwise.
**************************************************/
int polyvecl_chknorm(const polyvecl *v, uint32_t bound)  {
  unsigned int i;
  int ret = 0;

  for(i = 0; i < L; ++i)
    ret |= poly_chknorm(v->vec+i, bound);

  return ret;
}

/**************************************************************/
/************ Vectors of polynomials of length K **************/
/**************************************************************/

/*************************************************
* Name:        polyveck_freeze
* 
* Description: Reduce coefficients of polynomials in vector of length K
*              to standard representatives.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_freeze(polyveck *v)  {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_freeze(v->vec+i);
}

/*************************************************
* Name:        polyveck_add
* 
* Description: Add vectors of polynomials of length K.
*              No modular reduction is performed.
*
* Arguments:   - polyveck *w: pointer to output vector
*              - polyveck *u: pointer to first summand
*              - polyveck *v: pointer to second summand
**************************************************/
void polyveck_add(polyveck *w, const polyveck *u, const polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_add(w->vec+i, u->vec+i, v->vec+i);
}

/*************************************************
* Name:        polyveck_sub
* 
* Description: Subtract vectors of polynomials of length K.
*              Assumes coefficients of polynomials in input vectors to be less
*              than 2*Q. No modular reduction is performed.
*
* Arguments:   - polyveck *w: pointer to output vector
*              - polyveck *u: pointer to first input vector
*              - polyveck *v: pointer to second input vector to be subtracted
*                             from first input vector
**************************************************/
void polyveck_sub(polyveck *w, const polyveck *u, const polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_sub(w->vec+i, u->vec+i, v->vec+i);
}

/*************************************************
* Name:        polyveck_neg
* 
* Description: Negate vector of polynomials of length K.
*              Assumes input coefficients to be less than 2*Q.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_neg(polyveck *v) { 
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_neg(v->vec+i);
}

/*************************************************
* Name:        polyveck_shiftl
* 
* Description: Multiply vector of polynomials of Length K by 2^k.
*
* Arguments:   - polyveck *v: pointer to input/output vector
*              - unsigned int k: exponent
**************************************************/
void polyveck_shiftl(polyveck *v, unsigned int k) { 
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_shiftl(v->vec+i, k);
}

/*************************************************
* Name:        polyveck_ntt
* 
* Description: Forward NTT of all polynomials in vector of length K. Output *              coefficients can be up to 16*Q larger than input coefficients.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_ntt(polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_ntt(v->vec+i);
}

/*************************************************
* Name:        polyveck_invntt_montgomery
* 
* Description: Inverse NTT and multiplication by 2^{32} of polynomials
*              in vector of length K. Input coefficients need to be less
*              than 2*Q.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_invntt_montgomery(polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_invntt_montgomery(v->vec+i);
}

/*************************************************
* Name:        polyveck_chknorm
* 
* Description: Check infinity norm of polynomials in vector of length K.
*              Assumes input coefficients to be standard representatives.
*
* Arguments:   - const polyveck *v: pointer to vector
*              - uint32_t B: norm bound
*
* Returns 0 if norm of all polynomials is strictly smaller than B and 1
* otherwise.
**************************************************/
int polyveck_chknorm(const polyveck *v, uint32_t bound) {
  unsigned int i;
  int ret = 0;

  for(i = 0; i < K; ++i)
    ret |= poly_chknorm(v->vec+i, bound);

  return ret;
}

/*************************************************
* Name:        polyveck_power2round
* 
* Description: For all coefficients a of polynomials in vector of length K,
*              compute a0, a1 such that a = a1*2^D + a0
*              with -2^{D/2} < a0 <= 2^{D/2}. Assumes a to be standard
*              representative.
*
* Arguments:   - polyveck *v1: pointer to output vector of polynomials with
*                              coefficients a1
*              - polyveck *v0: pointer to output vector of polynomials with
*                              coefficients a0
*              - polyveck *v: pointer to input vector
**************************************************/
void polyveck_power2round(polyveck *v1, polyveck *v0, const polyveck *v) {
  unsigned int i, j;

  for(i = 0; i < K; ++i)
    for(j = 0; j < N; ++j)
      v1->vec[i].coeffs[j] = power2round(v->vec[i].coeffs[j],
                                         &v0->vec[i].coeffs[j]);
}

/*************************************************
* Name:        polyveck_decompose
* 
* Description: For all coefficients a of polynomials in vector of length K,
*              compute high and low bits a0, a1 such a = a1*ALPHA + a0
*              with -ALPHA/2 < a0 <= ALPHA/2 except if a = Q-1 where
*              a1 = 0 and a0 = -1. Assumes a to be standard representative.
*
* Arguments:   - polyveck *v1: pointer to output vector of polynomials with
*                              coefficients a1
*              - polyveck *v0: pointer to output vector of polynomials with
*                              coefficients a0
*              - polyveck *v: pointer to input vector
**************************************************/
void polyveck_decompose(polyveck *v1, polyveck *v0, const polyveck *v) {
  unsigned int i, j;

  for(i = 0; i < K; ++i)
    for(j = 0; j < N; ++j)
      v1->vec[i].coeffs[j] = decompose(v->vec[i].coeffs[j],
                                       &v0->vec[i].coeffs[j]);
}

/*************************************************
* Name:        polyveck_make_hint
* 
* Description: Compute hint vector. The coefficients of the polynomials indicate
*              whether or not the high bits of the corresponding coefficients
*              of the input polynomials differ.
*
* Arguments:   - polyveck *h: pointer to output vector
*              - const polyveck *u: pointer to first input vector
*              - const polyveck *u: pointer to second input vector
*
* Returns number of 1 bits.
**************************************************/
unsigned int polyveck_make_hint(polyveck *h,
                                const polyveck *u,
                                const polyveck *v)
{
  unsigned int i, j, s = 0;

  for(i = 0; i < K; ++i)
    for(j = 0; j < N; ++j) {
      h->vec[i].coeffs[j] = make_hint(u->vec[i].coeffs[j], v->vec[i].coeffs[j]);
      s += h->vec[i].coeffs[j];
    }

  return s;
}

/*************************************************
* Name:        polyveck_use_hint
* 
* Description: Use hint vector to correct the high bits of input vector.
*
* Arguments:   - polyveck *w: pointer to output vector of polynomials with
*                             corrected high bits
*              - polyveck *u: pointer to input vector
*              - polyveck *h: pointer to input hint vector
**************************************************/
void polyveck_use_hint(polyveck *w, const polyveck *u, const polyveck *h) {
  unsigned int i, j;

  for(i = 0; i < K; ++i)
    for(j = 0; j < N; ++j)
      w->vec[i].coeffs[j] = use_hint(u->vec[i].coeffs[j], h->vec[i].coeffs[j]);
}
