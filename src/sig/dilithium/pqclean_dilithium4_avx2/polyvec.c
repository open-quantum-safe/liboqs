#include <stdint.h>
#include "params.h"
#include "polyvec.h"
#include "poly.h"
#include "ntt.h"
#include "consts.h"
#ifdef DILITHIUM_USE_AES
#include "aes256ctr.h"
#endif

/*************************************************
* Name:        expand_mat
*
* Description: Implementation of ExpandA. Generates matrix A with uniformly
*              random coefficients a_{i,j} by performing rejection
*              sampling on the output stream of SHAKE128(rho|j|i)
*              or AES256CTR(rho,j|i).
*
* Arguments:   - polyvecl mat[K]: output matrix
*              - const uint8_t rho[]: byte array containing seed rho
**************************************************/
#ifdef DILITHIUM_USE_AES
void expand_mat(polyvecl mat[K], const uint8_t rho[SEEDBYTES]) {
  unsigned int i, j;
  uint64_t nonce __attribute__((aligned(16)));
  aes256ctr_ctx state;

  aes256ctr_init(&state, rho, 0);

  for(i = 0; i < K; ++i) {
    for(j = 0; j < L; ++j) {
      nonce = (i << 8) | j;
      state.n = _mm_loadl_epi64((__m128i *)&nonce);
      poly_uniform_preinit(&mat[i].vec[j], &state);
    }
  }
}
#elif L == 2 && K == 3
void expand_mat(polyvecl mat[3], const uint8_t rho[SEEDBYTES])
{
  poly t0, t1;

  poly_uniform_4x(&mat[0].vec[0],
                  &mat[0].vec[1],
                  &mat[1].vec[0],
                  &mat[1].vec[1],
                  rho, 0, 1, 256, 257);
  poly_uniform_4x(&mat[2].vec[0],
                  &mat[2].vec[1],
                  &t0,
                  &t1,
                  rho, 512, 513, 0, 0);
}
#elif L == 3 && K == 4
void expand_mat(polyvecl mat[4], const uint8_t rho[SEEDBYTES])
{
  poly_uniform_4x(&mat[0].vec[0],
                  &mat[0].vec[1],
                  &mat[0].vec[2],
                  &mat[1].vec[0],
                  rho, 0, 1, 2, 256);
  poly_uniform_4x(&mat[1].vec[1],
                  &mat[1].vec[2],
                  &mat[2].vec[0],
                  &mat[2].vec[1],
                  rho, 257, 258, 512, 513);
  poly_uniform_4x(&mat[2].vec[2],
                  &mat[3].vec[0],
                  &mat[3].vec[1],
                  &mat[3].vec[2],
                  rho, 514, 768, 769, 770);
}
#elif L == 4 && K == 5
void expand_mat(polyvecl mat[5], const uint8_t rho[SEEDBYTES])
{
  poly_uniform_4x(&mat[0].vec[0],
                  &mat[0].vec[1],
                  &mat[0].vec[2],
                  &mat[0].vec[3],
                  rho, 0, 1, 2, 3);
  poly_uniform_4x(&mat[1].vec[0],
                  &mat[1].vec[1],
                  &mat[1].vec[2],
                  &mat[1].vec[3],
                  rho, 256, 257, 258, 259);
  poly_uniform_4x(&mat[2].vec[0],
                  &mat[2].vec[1],
                  &mat[2].vec[2],
                  &mat[2].vec[3],
                  rho, 512, 513, 514, 515);
  poly_uniform_4x(&mat[3].vec[0],
                  &mat[3].vec[1],
                  &mat[3].vec[2],
                  &mat[3].vec[3],
                  rho, 768, 769, 770, 771);
  poly_uniform_4x(&mat[4].vec[0],
                  &mat[4].vec[1],
                  &mat[4].vec[2],
                  &mat[4].vec[3],
                  rho, 1024, 1025, 1026, 1027);
}
#elif L == 5 && K == 6
void expand_mat(polyvecl mat[6], const uint8_t rho[SEEDBYTES])
{
  poly t0, t1;

  poly_uniform_4x(&mat[0].vec[0],
                  &mat[0].vec[1],
                  &mat[0].vec[2],
                  &mat[0].vec[3],
                  rho, 0, 1, 2, 3);
  poly_uniform_4x(&mat[0].vec[4],
                  &mat[1].vec[0],
                  &mat[1].vec[1],
                  &mat[1].vec[2],
                  rho, 4, 256, 257, 258);
  poly_uniform_4x(&mat[1].vec[3],
                  &mat[1].vec[4],
                  &mat[2].vec[0],
                  &mat[2].vec[1],
                  rho, 259, 260, 512, 513);
  poly_uniform_4x(&mat[2].vec[2],
                  &mat[2].vec[3],
                  &mat[2].vec[4],
                  &mat[3].vec[0],
                  rho, 514, 515, 516, 768);
  poly_uniform_4x(&mat[3].vec[1],
                  &mat[3].vec[2],
                  &mat[3].vec[3],
                  &mat[3].vec[4],
                  rho, 769, 770, 771, 772);
  poly_uniform_4x(&mat[4].vec[0],
                  &mat[4].vec[1],
                  &mat[4].vec[2],
                  &mat[4].vec[3],
                  rho, 1024, 1025, 1026, 1027);
  poly_uniform_4x(&mat[4].vec[4],
                  &mat[5].vec[0],
                  &mat[5].vec[1],
                  &mat[5].vec[2],
                  rho, 1028, 1280, 1281, 1282);
  poly_uniform_4x(&mat[5].vec[3],
                  &mat[5].vec[4],
                  &t0,
                  &t1,
                  rho, 1283, 1284, 0, 0);
}
#else
#error
#endif

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
    poly_freeze(&v->vec[i]);
}

/*************************************************
* Name:        polyvecl_add
*
* Description: Add vectors of polynomials of length L.
*              No modular reduction is performed.
*
* Arguments:   - polyvecl *w: pointer to output vector
*              - const polyvecl *u: pointer to first summand
*              - const polyvecl *v: pointer to second summand
**************************************************/
void polyvecl_add(polyvecl *w, const polyvecl *u, const polyvecl *v) {
  unsigned int i;

  for(i = 0; i < L; ++i)
    poly_add(&w->vec[i], &u->vec[i], &v->vec[i]);
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
    poly_ntt(&v->vec[i]);
}

/*************************************************
* Name:        polyvecl_pointwise_acc_montgomery
*
* Description: Pointwise multiply vectors of polynomials of length L, multiply
*              resulting vector by 2^{-32} and add (accumulate) polynomials
*              in it. Input/output vectors are in NTT domain representation.
*              Input coefficients are assumed to be less than 22*Q. Output
*              coeffcient are less than 2*L*Q.
*
* Arguments:   - poly *w: output polynomial
*              - const polyvecl *u: pointer to first input vector
*              - const polyvecl *v: pointer to second input vector
**************************************************/
void polyvecl_pointwise_acc_montgomery(poly *w,
                                       const polyvecl *u,
                                       const polyvecl *v)
{
  pointwise_acc_avx(w->coeffs, u->vec->coeffs, v->vec->coeffs, qdata);
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

  for(i = 0; i < L; ++i)
    if(poly_chknorm(&v->vec[i], bound))
      return 1;

  return 0;
}

/**************************************************************/
/************ Vectors of polynomials of length K **************/
/**************************************************************/


/*************************************************
* Name:        polyveck_reduce
*
* Description: Reduce coefficients of polynomials in vector of length K
*              to representatives in [0,2*Q[.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_reduce(polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_reduce(&v->vec[i]);
}

/*************************************************
* Name:        polyveck_csubq
*
* Description: For all coefficients of polynomials in vector of length K
*              subtract Q if coefficient is bigger than Q.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_csubq(polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_csubq(&v->vec[i]);
}

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
    poly_freeze(&v->vec[i]);
}

/*************************************************
* Name:        polyveck_add
*
* Description: Add vectors of polynomials of length K.
*              No modular reduction is performed.
*
* Arguments:   - polyveck *w: pointer to output vector
*              - const polyveck *u: pointer to first summand
*              - const polyveck *v: pointer to second summand
**************************************************/
void polyveck_add(polyveck *w, const polyveck *u, const polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_add(&w->vec[i], &u->vec[i], &v->vec[i]);
}

/*************************************************
* Name:        polyveck_sub
*
* Description: Subtract vectors of polynomials of length K.
*              Assumes coefficients of polynomials in second input vector
*              to be less than 2*Q. No modular reduction is performed.
*
* Arguments:   - polyveck *w: pointer to output vector
*              - const polyveck *u: pointer to first input vector
*              - const polyveck *v: pointer to second input vector to be
*                                   subtracted from first input vector
**************************************************/
void polyveck_sub(polyveck *w, const polyveck *u, const polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_sub(&w->vec[i], &u->vec[i], &v->vec[i]);
}

/*************************************************
* Name:        polyveck_shiftl
*
* Description: Multiply vector of polynomials of Length K by 2^D without modular
*              reduction. Assumes input coefficients to be less than 2^{32-D}.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_shiftl(polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_shiftl(&v->vec[i]);
}

/*************************************************
* Name:        polyveck_ntt
*
* Description: Forward NTT of all polynomials in vector of length K. Output
*              coefficients can be up to 16*Q larger than input coefficients.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_ntt(polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_ntt(&v->vec[i]);
}

/*************************************************
* Name:        polyveck_invntt_tomont
*
* Description: Inverse NTT and multiplication by 2^{32} of polynomials
*              in vector of length K. Input coefficients need to be less
*              than 2*Q.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_invntt_tomont(polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_invntt_tomont(&v->vec[i]);
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
* Returns 0 if norm of all polynomials are strictly smaller than B and 1
* otherwise.
**************************************************/
int polyveck_chknorm(const polyveck *v, uint32_t bound) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    if(poly_chknorm(&v->vec[i], bound))
      return 1;

  return 0;
}

/*************************************************
* Name:        polyveck_power2round
*
* Description: For all coefficients a of polynomials in vector of length K,
*              compute a0, a1 such that a mod Q = a1*2^D + a0
*              with -2^{D-1} < a0 <= 2^{D-1}. Assumes coefficients to be
*              standard representatives.
*
* Arguments:   - polyveck *v1: pointer to output vector of polynomials with
*                              coefficients a1
*              - polyveck *v0: pointer to output vector of polynomials with
*                              coefficients Q + a0
*              - const polyveck *v: pointer to input vector
**************************************************/
void polyveck_power2round(polyveck *v1, polyveck *v0, const polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_power2round(&v1->vec[i], &v0->vec[i], &v->vec[i]);
}

/*************************************************
* Name:        polyveck_decompose
*
* Description: For all coefficients a of polynomials in vector of length K,
*              compute high and low bits a0, a1 such a mod Q = a1*ALPHA + a0
*              with -ALPHA/2 < a0 <= ALPHA/2 except a1 = (Q-1)/ALPHA where we
*              set a1 = 0 and -ALPHA/2 <= a0 = a mod Q - Q < 0.
*              Assumes coefficients to be standard representatives.
*
* Arguments:   - polyveck *v1: pointer to output vector of polynomials with
*                              coefficients a1
*              - polyveck *v0: pointer to output vector of polynomials with
*                              coefficients Q + a0
*              - const polyveck *v: pointer to input vector
**************************************************/
void polyveck_decompose(polyveck *v1, polyveck *v0, const polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_decompose(&v1->vec[i], &v0->vec[i], &v->vec[i]);
}

/*************************************************
* Name:        polyveck_make_hint
*
* Description: Compute hint vector.
*
* Arguments:   - polyveck *h: pointer to output vector
*              - const polyveck *v0: pointer to low part of input vector
*              - const polyveck *v1: pointer to high part of input vector
*
* Returns number of 1 bits.
**************************************************/
unsigned int polyveck_make_hint(polyveck *h,
                                const polyveck *v0,
                                const polyveck *v1)
{
  unsigned int i, s = 0;

  for(i = 0; i < K; ++i)
    s += poly_make_hint(&h->vec[i], &v0->vec[i], &v1->vec[i]);

  return s;
}

/*************************************************
* Name:        polyveck_use_hint
*
* Description: Use hint vector to correct the high bits of input vector.
*
* Arguments:   - polyveck *w: pointer to output vector of polynomials with
*                             corrected high bits
*              - const polyveck *u: pointer to input vector
*              - const polyveck *h: pointer to input hint vector
**************************************************/
void polyveck_use_hint(polyveck *w, const polyveck *u, const polyveck *h) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_use_hint(&w->vec[i], &u->vec[i], &h->vec[i]);
}
