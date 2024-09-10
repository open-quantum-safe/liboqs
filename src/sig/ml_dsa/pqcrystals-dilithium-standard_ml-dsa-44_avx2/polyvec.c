#include <stdint.h>
#include "params.h"
#include "polyvec.h"
#include "poly.h"
#include "ntt.h"
#include "consts.h"

/*************************************************
* Name:        expand_mat
*
* Description: Implementation of ExpandA. Generates matrix A with uniformly
*              random coefficients a_{i,j} by performing rejection
*              sampling on the output stream of SHAKE128(rho|j|i)
*
* Arguments:   - polyvecl mat[K]: output matrix
*              - const uint8_t rho[]: byte array containing seed rho
**************************************************/

#if K == 4 && L == 4
void polyvec_matrix_expand(polyvecl mat[K], const uint8_t rho[SEEDBYTES]) {
  polyvec_matrix_expand_row0(&mat[0], NULL, rho);
  polyvec_matrix_expand_row1(&mat[1], NULL, rho);
  polyvec_matrix_expand_row2(&mat[2], NULL, rho);
  polyvec_matrix_expand_row3(&mat[3], NULL, rho);
}

void polyvec_matrix_expand_row0(polyvecl *rowa, __attribute__((unused)) polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[0], &rowa->vec[1], &rowa->vec[2], &rowa->vec[3], rho, 0, 1, 2, 3);
  poly_nttunpack(&rowa->vec[0]);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
}

void polyvec_matrix_expand_row1(polyvecl *rowa, __attribute__((unused)) polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[0], &rowa->vec[1], &rowa->vec[2], &rowa->vec[3], rho, 256, 257, 258, 259);
  poly_nttunpack(&rowa->vec[0]);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
}

void polyvec_matrix_expand_row2(polyvecl *rowa, __attribute__((unused)) polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[0], &rowa->vec[1], &rowa->vec[2], &rowa->vec[3], rho, 512, 513, 514, 515);
  poly_nttunpack(&rowa->vec[0]);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
}

void polyvec_matrix_expand_row3(polyvecl *rowa, __attribute__((unused)) polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[0], &rowa->vec[1], &rowa->vec[2], &rowa->vec[3], rho, 768, 769, 770, 771);
  poly_nttunpack(&rowa->vec[0]);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
}

#elif K == 6 && L == 5
void polyvec_matrix_expand(polyvecl mat[K], const uint8_t rho[SEEDBYTES]) {
  polyvecl tmp;
  polyvec_matrix_expand_row0(&mat[0], &mat[1], rho);
  polyvec_matrix_expand_row1(&mat[1], &mat[2], rho);
  polyvec_matrix_expand_row2(&mat[2], &mat[3], rho);
  polyvec_matrix_expand_row3(&mat[3], NULL, rho);
  polyvec_matrix_expand_row4(&mat[4], &mat[5], rho);
  polyvec_matrix_expand_row5(&mat[5], &tmp, rho);
}

void polyvec_matrix_expand_row0(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[0], &rowa->vec[1], &rowa->vec[2], &rowa->vec[3], rho, 0, 1, 2, 3);
  poly_uniform_4x(&rowa->vec[4], &rowb->vec[0], &rowb->vec[1], &rowb->vec[2], rho, 4, 256, 257, 258);
  poly_nttunpack(&rowa->vec[0]);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowb->vec[0]);
  poly_nttunpack(&rowb->vec[1]);
  poly_nttunpack(&rowb->vec[2]);
}

void polyvec_matrix_expand_row1(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[3], &rowa->vec[4], &rowb->vec[0], &rowb->vec[1], rho, 259, 260, 512, 513);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowb->vec[0]);
  poly_nttunpack(&rowb->vec[1]);
}

void polyvec_matrix_expand_row2(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[2], &rowa->vec[3], &rowa->vec[4], &rowb->vec[0], rho, 514, 515, 516, 768);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowb->vec[0]);
}

void polyvec_matrix_expand_row3(polyvecl *rowa, __attribute__((unused)) polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[1], &rowa->vec[2], &rowa->vec[3], &rowa->vec[4], rho, 769, 770, 771, 772);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
}

void polyvec_matrix_expand_row4(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[0], &rowa->vec[1], &rowa->vec[2], &rowa->vec[3], rho, 1024, 1025, 1026, 1027);
  poly_uniform_4x(&rowa->vec[4], &rowb->vec[0], &rowb->vec[1], &rowb->vec[2], rho, 1028, 1280, 1281, 1282);
  poly_nttunpack(&rowa->vec[0]);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowb->vec[0]);
  poly_nttunpack(&rowb->vec[1]);
  poly_nttunpack(&rowb->vec[2]);
}

void polyvec_matrix_expand_row5(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[3], &rowa->vec[4], &rowb->vec[0], &rowb->vec[1], rho, 1283, 1284, 1536, 1537);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
}

#elif K == 8 && L == 7
void polyvec_matrix_expand(polyvecl mat[K], const uint8_t rho[SEEDBYTES]) {
  polyvec_matrix_expand_row0(&mat[0], &mat[1], rho);
  polyvec_matrix_expand_row1(&mat[1], &mat[2], rho);
  polyvec_matrix_expand_row2(&mat[2], &mat[3], rho);
  polyvec_matrix_expand_row3(&mat[3], NULL, rho);
  polyvec_matrix_expand_row4(&mat[4], &mat[5], rho);
  polyvec_matrix_expand_row5(&mat[5], &mat[6], rho);
  polyvec_matrix_expand_row6(&mat[6], &mat[7], rho);
  polyvec_matrix_expand_row7(&mat[7], NULL, rho);
}

void polyvec_matrix_expand_row0(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[0], &rowa->vec[1], &rowa->vec[2], &rowa->vec[3], rho, 0, 1, 2, 3);
  poly_uniform_4x(&rowa->vec[4], &rowa->vec[5], &rowa->vec[6], &rowb->vec[0], rho, 4, 5, 6, 256);
  poly_nttunpack(&rowa->vec[0]);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowa->vec[5]);
  poly_nttunpack(&rowa->vec[6]);
  poly_nttunpack(&rowb->vec[0]);
}

void polyvec_matrix_expand_row1(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[1], &rowa->vec[2], &rowa->vec[3], &rowa->vec[4], rho, 257, 258, 259, 260);
  poly_uniform_4x(&rowa->vec[5], &rowa->vec[6], &rowb->vec[0], &rowb->vec[1], rho, 261, 262, 512, 513);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowa->vec[5]);
  poly_nttunpack(&rowa->vec[6]);
  poly_nttunpack(&rowb->vec[0]);
  poly_nttunpack(&rowb->vec[1]);
}

void polyvec_matrix_expand_row2(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[2], &rowa->vec[3], &rowa->vec[4], &rowa->vec[5], rho, 514, 515, 516, 517);
  poly_uniform_4x(&rowa->vec[6], &rowb->vec[0], &rowb->vec[1], &rowb->vec[2], rho, 518, 768, 769, 770);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowa->vec[5]);
  poly_nttunpack(&rowa->vec[6]);
  poly_nttunpack(&rowb->vec[0]);
  poly_nttunpack(&rowb->vec[1]);
  poly_nttunpack(&rowb->vec[2]);
}

void polyvec_matrix_expand_row3(polyvecl *rowa, __attribute__((unused)) polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[3], &rowa->vec[4], &rowa->vec[5], &rowa->vec[6], rho, 771, 772, 773, 774);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowa->vec[5]);
  poly_nttunpack(&rowa->vec[6]);
}

void polyvec_matrix_expand_row4(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[0], &rowa->vec[1], &rowa->vec[2], &rowa->vec[3], rho, 1024, 1025, 1026, 1027);
  poly_uniform_4x(&rowa->vec[4], &rowa->vec[5], &rowa->vec[6], &rowb->vec[0], rho, 1028, 1029, 1030, 1280);
  poly_nttunpack(&rowa->vec[0]);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowa->vec[5]);
  poly_nttunpack(&rowa->vec[6]);
  poly_nttunpack(&rowb->vec[0]);
}

void polyvec_matrix_expand_row5(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[1], &rowa->vec[2], &rowa->vec[3], &rowa->vec[4], rho, 1281, 1282, 1283, 1284);
  poly_uniform_4x(&rowa->vec[5], &rowa->vec[6], &rowb->vec[0], &rowb->vec[1], rho, 1285, 1286, 1536, 1537);
  poly_nttunpack(&rowa->vec[1]);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowa->vec[5]);
  poly_nttunpack(&rowa->vec[6]);
  poly_nttunpack(&rowb->vec[0]);
  poly_nttunpack(&rowb->vec[1]);
}

void polyvec_matrix_expand_row6(polyvecl *rowa, polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[2], &rowa->vec[3], &rowa->vec[4], &rowa->vec[5], rho, 1538, 1539, 1540, 1541);
  poly_uniform_4x(&rowa->vec[6], &rowb->vec[0], &rowb->vec[1], &rowb->vec[2], rho, 1542, 1792, 1793, 1794);
  poly_nttunpack(&rowa->vec[2]);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowa->vec[5]);
  poly_nttunpack(&rowa->vec[6]);
  poly_nttunpack(&rowb->vec[0]);
  poly_nttunpack(&rowb->vec[1]);
  poly_nttunpack(&rowb->vec[2]);
}

void polyvec_matrix_expand_row7(polyvecl *rowa, __attribute__((unused)) polyvecl *rowb, const uint8_t rho[SEEDBYTES]) {
  poly_uniform_4x(&rowa->vec[3], &rowa->vec[4], &rowa->vec[5], &rowa->vec[6], rho, 1795, 1796, 1797, 1798);
  poly_nttunpack(&rowa->vec[3]);
  poly_nttunpack(&rowa->vec[4]);
  poly_nttunpack(&rowa->vec[5]);
  poly_nttunpack(&rowa->vec[6]);
}

#else
#error
#endif

void polyvec_matrix_pointwise_montgomery(polyveck *t, const polyvecl mat[K], const polyvecl *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    polyvecl_pointwise_acc_montgomery(&t->vec[i], &mat[i], v);
}

/**************************************************************/
/************ Vectors of polynomials of length L **************/
/**************************************************************/

void polyvecl_uniform_eta(polyvecl *v, const uint8_t seed[CRHBYTES], uint16_t nonce) {
  unsigned int i;

  for(i = 0; i < L; ++i)
    poly_uniform_eta(&v->vec[i], seed, nonce++);
}

void polyvecl_uniform_gamma1(polyvecl *v, const uint8_t seed[CRHBYTES], uint16_t nonce) {
  unsigned int i;

  for(i = 0; i < L; ++i)
    poly_uniform_gamma1(&v->vec[i], seed, L*nonce + i);
}

void polyvecl_reduce(polyvecl *v) {
  unsigned int i;

  for(i = 0; i < L; ++i)
    poly_reduce(&v->vec[i]);
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

void polyvecl_invntt_tomont(polyvecl *v) {
  unsigned int i;

  for(i = 0; i < L; ++i)
    poly_invntt_tomont(&v->vec[i]);
}

void polyvecl_pointwise_poly_montgomery(polyvecl *r, const poly *a, const polyvecl *v) {
  unsigned int i;

  for(i = 0; i < L; ++i)
    poly_pointwise_montgomery(&r->vec[i], a, &v->vec[i]);
}

/*************************************************
* Name:        polyvecl_pointwise_acc_montgomery
*
* Description: Pointwise multiply vectors of polynomials of length L, multiply
*              resulting vector by 2^{-32} and add (accumulate) polynomials
*              in it. Input/output vectors are in NTT domain representation.
*
* Arguments:   - poly *w: output polynomial
*              - const polyvecl *u: pointer to first input vector
*              - const polyvecl *v: pointer to second input vector
**************************************************/
void polyvecl_pointwise_acc_montgomery(poly *w, const polyvecl *u, const polyvecl *v) {
  pointwise_acc_avx(w->vec, u->vec->vec, v->vec->vec, qdata.vec);
}

/*************************************************
* Name:        polyvecl_chknorm
*
* Description: Check infinity norm of polynomials in vector of length L.
*              Assumes input polyvecl to be reduced by polyvecl_reduce().
*
* Arguments:   - const polyvecl *v: pointer to vector
*              - int32_t B: norm bound
*
* Returns 0 if norm of all polynomials is strictly smaller than B <= (Q-1)/8
* and 1 otherwise.
**************************************************/
int polyvecl_chknorm(const polyvecl *v, int32_t bound)  {
  unsigned int i;

  for(i = 0; i < L; ++i)
    if(poly_chknorm(&v->vec[i], bound))
      return 1;

  return 0;
}

/**************************************************************/
/************ Vectors of polynomials of length K **************/
/**************************************************************/

void polyveck_uniform_eta(polyveck *v, const uint8_t seed[CRHBYTES], uint16_t nonce) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_uniform_eta(&v->vec[i], seed, nonce++);
}

/*************************************************
* Name:        polyveck_reduce
*
* Description: Reduce coefficients of polynomials in vector of length K
*              to representatives in [-6283009,6283008].
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_reduce(polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_reduce(&v->vec[i]);
}

/*************************************************
* Name:        polyveck_caddq
*
* Description: For all coefficients of polynomials in vector of length K
*              add Q if coefficient is negative.
*
* Arguments:   - polyveck *v: pointer to input/output vector
**************************************************/
void polyveck_caddq(polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_caddq(&v->vec[i]);
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
*              No modular reduction is performed.
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
*              reduction. Assumes input coefficients to be less than 2^{31-D}.
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

void polyveck_pointwise_poly_montgomery(polyveck *r, const poly *a, const polyveck *v) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    poly_pointwise_montgomery(&r->vec[i], a, &v->vec[i]);
}

/*************************************************
* Name:        polyveck_chknorm
*
* Description: Check infinity norm of polynomials in vector of length K.
*              Assumes input polyveck to be reduced by polyveck_reduce().
*
* Arguments:   - const polyveck *v: pointer to vector
*              - int32_t B: norm bound
*
* Returns 0 if norm of all polynomials are strictly smaller than B <= (Q-1)/8
* and 1 otherwise.
**************************************************/
int polyveck_chknorm(const polyveck *v, int32_t bound) {
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
*              compute a0, a1 such that a mod^+ Q = a1*2^D + a0
*              with -2^{D-1} < a0 <= 2^{D-1}. Assumes coefficients to be
*              standard representatives.
*
* Arguments:   - polyveck *v1: pointer to output vector of polynomials with
*                              coefficients a1
*              - polyveck *v0: pointer to output vector of polynomials with
*                              coefficients a0
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
*              compute high and low bits a0, a1 such a mod^+ Q = a1*ALPHA + a0
*              with -ALPHA/2 < a0 <= ALPHA/2 except a1 = (Q-1)/ALPHA where we
*              set a1 = 0 and -ALPHA/2 <= a0 = a mod Q - Q < 0.
*              Assumes coefficients to be standard representatives.
*
* Arguments:   - polyveck *v1: pointer to output vector of polynomials with
*                              coefficients a1
*              - polyveck *v0: pointer to output vector of polynomials with
*                              coefficients a0
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
* Arguments:   - uint8_t *hint: pointer to output hint array
*              - const polyveck *v0: pointer to low part of input vector
*              - const polyveck *v1: pointer to high part of input vector
*
* Returns number of 1 bits.
**************************************************/
unsigned int polyveck_make_hint(uint8_t *hint, const polyveck *v0, const polyveck *v1)
{
  unsigned int i, n = 0;

  for(i = 0; i < K; ++i)
    n += poly_make_hint(&hint[n], &v0->vec[i], &v1->vec[i]);

  return n;
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

void polyveck_pack_w1(uint8_t r[K*POLYW1_PACKEDBYTES], const polyveck *w1) {
  unsigned int i;

  for(i = 0; i < K; ++i)
    polyw1_pack(&r[i*POLYW1_PACKEDBYTES], &w1->vec[i]);
}
