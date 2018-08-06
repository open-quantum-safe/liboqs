#include <stdint.h>
#include "api.h"
#include "params.h"
#include "sign.h"
#include "randombytes.h"
#include "fips202.h"
#include "fips202x4.h"
#include "poly.h"
#include "polyvec.h"
#include "packing.h"

/*************************************************
* Name:        expand_mat_ref
* 
* Description: Implementation of ExpandA. Generates matrix A with uniformly
*              random coefficients a_{i,j} by performing rejection
*              sampling on the output stream of SHAKE128(rho|i|j).
*              
* Arguments:   - polyvecl mat[K]: output matrix
*              - const unsigned char rho[]: byte array containing seed rho
**************************************************/
void expand_mat_ref(polyvecl mat[K], const unsigned char rho[SEEDBYTES]) {
  unsigned int i, j, pos, ctr;
  unsigned char inbuf[SEEDBYTES + 1];
  /* Don't change this to smaller values,
   * sampling later assumes sufficient SHAKE output!
   * Probability that we need more than 5 blocks: < 2^{-132}.
   * Probability that we need more than 6 blocks: < 2^{-546}. */
  unsigned char outbuf[5*SHAKE128_RATE];
  uint32_t val;

  for(i = 0; i < SEEDBYTES; ++i)
    inbuf[i] = rho[i];

  for(i = 0; i < K; ++i) {
    for(j = 0; j < L; ++j) {
      ctr = pos = 0;
      inbuf[SEEDBYTES] = i + (j << 4);

      shake128(outbuf, sizeof(outbuf), inbuf, SEEDBYTES + 1);

      while(ctr < N) {
        val  = outbuf[pos++];
        val |= (uint32_t)outbuf[pos++] << 8;
        val |= (uint32_t)outbuf[pos++] << 16;
        val &= 0x7FFFFF;

        /* Rejection sampling */
        if(val < Q)
          mat[i].vec[j].coeffs[ctr++] = val;
      }
    }
  }
}

#if L == 2 && K == 3

void expand_mat(polyvecl mat[3], const unsigned char rho[SEEDBYTES])
{
  unsigned int i;
  unsigned char inbuf[4][SEEDBYTES + 1];
  unsigned char outbuf[4][5*SHAKE128_RATE];

  for(i = 0; i < SEEDBYTES; ++i) {
    inbuf[0][i] = rho[i];
    inbuf[1][i] = rho[i];
    inbuf[2][i] = rho[i];
    inbuf[3][i] = rho[i];
  }

  inbuf[0][SEEDBYTES] = 0 + (0 << 4);
  inbuf[1][SEEDBYTES] = 0 + (1 << 4);
  inbuf[2][SEEDBYTES] = 1 + (0 << 4);
  inbuf[3][SEEDBYTES] = 1 + (1 << 4);

  shake128_4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5*SHAKE128_RATE,
              inbuf[0], inbuf[1], inbuf[2], inbuf[3], SEEDBYTES + 1);

  poly_uniform(&mat[0].vec[0], outbuf[0]);
  poly_uniform(&mat[0].vec[1], outbuf[1]);
  poly_uniform(&mat[1].vec[0], outbuf[2]);
  poly_uniform(&mat[1].vec[1], outbuf[3]);

  inbuf[0][SEEDBYTES] = 2 + (0 << 4);
  inbuf[1][SEEDBYTES] = 2 + (1 << 4);

  shake128_4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5*SHAKE128_RATE,
              inbuf[0], inbuf[1], inbuf[2], inbuf[3], SEEDBYTES + 1);

  poly_uniform(&mat[2].vec[0], outbuf[0]);
  poly_uniform(&mat[2].vec[1], outbuf[1]);
}

#elif L == 3 && K == 4

void expand_mat(polyvecl mat[4], const unsigned char rho[SEEDBYTES])
{
  unsigned int i;
  unsigned char inbuf[4][SEEDBYTES + 1];
  unsigned char outbuf[4][5*SHAKE128_RATE];

  for(i = 0; i < SEEDBYTES; ++i) {
    inbuf[0][i] = rho[i];
    inbuf[1][i] = rho[i];
    inbuf[2][i] = rho[i];
    inbuf[3][i] = rho[i];
  }

  for(i = 0; i < 3; ++i) {
    inbuf[0][SEEDBYTES] = 0 + (i << 4);
    inbuf[1][SEEDBYTES] = 1 + (i << 4);
    inbuf[2][SEEDBYTES] = 2 + (i << 4);
    inbuf[3][SEEDBYTES] = 3 + (i << 4);

    shake128_4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5*SHAKE128_RATE,
                inbuf[0], inbuf[1], inbuf[2], inbuf[3], SEEDBYTES + 1);

    poly_uniform(&mat[0].vec[i], outbuf[0]);
    poly_uniform(&mat[1].vec[i], outbuf[1]);
    poly_uniform(&mat[2].vec[i], outbuf[2]);
    poly_uniform(&mat[3].vec[i], outbuf[3]);
  }
}

#elif L == 4 && K == 5

void expand_mat(polyvecl mat[5], const unsigned char rho[SEEDBYTES])
{
  unsigned int i;
  unsigned char inbuf[4][SEEDBYTES + 1];
  unsigned char outbuf[4][5*SHAKE128_RATE];

  for(i = 0; i < SEEDBYTES; ++i) {
    inbuf[0][i] = rho[i];
    inbuf[1][i] = rho[i];
    inbuf[2][i] = rho[i];
    inbuf[3][i] = rho[i];
  }

  for(i = 0; i < 5; ++i) {
    inbuf[0][SEEDBYTES] = i + (0 << 4);
    inbuf[1][SEEDBYTES] = i + (1 << 4);
    inbuf[2][SEEDBYTES] = i + (2 << 4);
    inbuf[3][SEEDBYTES] = i + (3 << 4);

    shake128_4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5*SHAKE128_RATE,
                inbuf[0], inbuf[1], inbuf[2], inbuf[3], SEEDBYTES + 1);

    poly_uniform(&mat[i].vec[0], outbuf[0]);
    poly_uniform(&mat[i].vec[1], outbuf[1]);
    poly_uniform(&mat[i].vec[2], outbuf[2]);
    poly_uniform(&mat[i].vec[3], outbuf[3]);
  }
}

#elif L == 5 && K == 6

void expand_mat(polyvecl mat[6], const unsigned char rho[SEEDBYTES])
{
  unsigned int i;
  unsigned char inbuf[4][SEEDBYTES + 1];
  unsigned char outbuf[4][5*SHAKE128_RATE];

  for(i = 0; i < SEEDBYTES; ++i) {
    inbuf[0][i] = rho[i];
    inbuf[1][i] = rho[i];
    inbuf[2][i] = rho[i];
    inbuf[3][i] = rho[i];
  }

  for(i = 0; i < 4; i+=2) {
    inbuf[0][SEEDBYTES] = 0 + (i << 4);
    inbuf[1][SEEDBYTES] = 1 + (i << 4);
    inbuf[2][SEEDBYTES] = 2 + (i << 4);
    inbuf[3][SEEDBYTES] = 3 + (i << 4);

    shake128_4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5*SHAKE128_RATE,
                inbuf[0], inbuf[1], inbuf[2], inbuf[3], SEEDBYTES + 1);

    poly_uniform(&mat[0].vec[i], outbuf[0]);
    poly_uniform(&mat[1].vec[i], outbuf[1]);
    poly_uniform(&mat[2].vec[i], outbuf[2]);
    poly_uniform(&mat[3].vec[i], outbuf[3]);

    inbuf[0][SEEDBYTES] = 4 + (i << 4);
    inbuf[1][SEEDBYTES] = 5 + (i << 4);
    inbuf[2][SEEDBYTES] = 0 + ((i+1) << 4);
    inbuf[3][SEEDBYTES] = 1 + ((i+1) << 4);

    shake128_4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5*SHAKE128_RATE,
                inbuf[0], inbuf[1], inbuf[2], inbuf[3], SEEDBYTES + 1);

    poly_uniform(&mat[4].vec[i], outbuf[0]);
    poly_uniform(&mat[5].vec[i], outbuf[1]);
    poly_uniform(&mat[0].vec[i+1], outbuf[2]);
    poly_uniform(&mat[1].vec[i+1], outbuf[3]);

    inbuf[0][SEEDBYTES] = 2 + ((i+1) << 4);
    inbuf[1][SEEDBYTES] = 3 + ((i+1) << 4);
    inbuf[2][SEEDBYTES] = 4 + ((i+1) << 4);
    inbuf[3][SEEDBYTES] = 5 + ((i+1) << 4);

    shake128_4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5*SHAKE128_RATE,
                inbuf[0], inbuf[1], inbuf[2], inbuf[3], SEEDBYTES + 1);

    poly_uniform(&mat[2].vec[i+1], outbuf[0]);
    poly_uniform(&mat[3].vec[i+1], outbuf[1]);
    poly_uniform(&mat[4].vec[i+1], outbuf[2]);
    poly_uniform(&mat[5].vec[i+1], outbuf[3]);
  }

  inbuf[0][SEEDBYTES] = 0 + (4 << 4);
  inbuf[1][SEEDBYTES] = 1 + (4 << 4);
  inbuf[2][SEEDBYTES] = 2 + (4 << 4);
  inbuf[3][SEEDBYTES] = 3 + (4 << 4);

  shake128_4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5*SHAKE128_RATE,
              inbuf[0], inbuf[1], inbuf[2], inbuf[3], SEEDBYTES + 1);

  poly_uniform(&mat[0].vec[4], outbuf[0]);
  poly_uniform(&mat[1].vec[4], outbuf[1]);
  poly_uniform(&mat[2].vec[4], outbuf[2]);
  poly_uniform(&mat[3].vec[4], outbuf[3]);

  inbuf[0][SEEDBYTES] = 4 + (4 << 4);
  inbuf[1][SEEDBYTES] = 5 + (4 << 4);

  shake128_4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5*SHAKE128_RATE,
              inbuf[0], inbuf[1], inbuf[2], inbuf[3], SEEDBYTES + 1);

  poly_uniform(&mat[4].vec[4], outbuf[0]);
  poly_uniform(&mat[5].vec[4], outbuf[1]);
}

#else
#error
#endif

/*************************************************
* Name:        challenge
* 
* Description: Implementation of H. Samples polynomial with 60 nonzero
*              coefficients in {-1,1} using the output stream of
*              SHAKE256(mu|w1).
*              
* Arguments:   - poly *c: pointer to output polynomial
*              - const unsigned char mu[]: byte array containing mu
*              - const polyveck *w1: pointer to vector w1
**************************************************/
void challenge(poly *c,
               const unsigned char mu[CRHBYTES],
               const polyveck *w1) 
{
  unsigned int i, b, pos;
  unsigned char inbuf[CRHBYTES + K*POLW1_SIZE_PACKED];
  unsigned char outbuf[SHAKE256_RATE];
  uint64_t state[25], signs, mask;

  for(i = 0; i < CRHBYTES; ++i)
    inbuf[i] = mu[i];
  for(i = 0; i < K; ++i)
    polyw1_pack(inbuf + CRHBYTES + i*POLW1_SIZE_PACKED, w1->vec+i);

  shake256_absorb(state, inbuf, sizeof(inbuf));
  shake256_squeezeblocks(outbuf, 1, state);

  signs = 0;
  for(i = 0; i < 8; ++i)
    signs |= (uint64_t)outbuf[i] << 8*i;

  pos = 8;
  mask = 1;

  for(i = 0; i < N; ++i)
    c->coeffs[i] = 0;

  for(i = 196; i < 256; ++i) {
    do {
      if(pos >= SHAKE256_RATE) {
        shake256_squeezeblocks(outbuf, 1, state);
        pos = 0;
      }

      b = outbuf[pos++];
    } while(b > i);

    c->coeffs[i] = c->coeffs[b];
    c->coeffs[b] = (signs & mask) ? Q - 1 : 1;
    mask <<= 1;
  }
}

/*************************************************
* Name:        crypto_sign_keypair
*
* Description: Generates public and private key.
*
* Arguments:   - unsigned char *pk: pointer to output public key (allocated
*                                   array of CRYPTO_PUBLICKEYBYTES bytes)
*              - unsigned char *sk: pointer to output private key (allocated
*                                   array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
  unsigned int i;
  unsigned char seedbuf[3*SEEDBYTES];
  unsigned char tr[CRHBYTES];
  unsigned char *rho, *rhoprime, *key;
  uint16_t nonce = 0;
  polyvecl mat[K];
  polyvecl s1, s1hat;
  polyveck s2, t, t1, t0;

  /* Expand 32 bytes of randomness into rho, rhoprime and key */
  randombytes(seedbuf, SEEDBYTES);
  shake256(seedbuf, 3*SEEDBYTES, seedbuf, SEEDBYTES);
  rho = seedbuf;
  rhoprime = rho + SEEDBYTES;
  key = rho + 2*SEEDBYTES;

  /* Expand matrix */
  expand_mat(mat, rho);

  /* Sample short vectors s1 and s2 */
#if L == 2 && K == 3
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s2.vec[0], &s2.vec[1], rhoprime,
                      nonce, nonce + 1, nonce + 2, nonce + 3);
  poly_uniform_eta(&s2.vec[2], rhoprime, nonce + 4);
  nonce += 5;
#elif L == 3 && K == 4
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s2.vec[0], rhoprime,
                      nonce, nonce + 1, nonce + 2, nonce + 3);
  poly_uniform_eta_4x(&s2.vec[1], &s2.vec[2], &s2.vec[3], &t.vec[0], rhoprime,
                      nonce + 4, nonce + 5, nonce + 6, nonce + 7);
  nonce += 7;
#elif L == 4 && K == 5
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime,
                      nonce, nonce + 1, nonce + 2, nonce + 3);
  poly_uniform_eta_4x(&s2.vec[0], &s2.vec[1], &s2.vec[2], &s2.vec[3], rhoprime,
                      nonce + 4, nonce + 5, nonce + 6, nonce + 7);
  poly_uniform_eta(&s2.vec[4], rhoprime, nonce + 8);
  nonce += 9;
#elif L == 5 && K == 6
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime,
                      nonce, nonce + 1, nonce + 2, nonce + 3);
  poly_uniform_eta_4x(&s1.vec[4], &s2.vec[0], &s2.vec[1], &s2.vec[2], rhoprime,
                      nonce + 4, nonce + 5, nonce + 6, nonce + 7);
  poly_uniform_eta_4x(&s2.vec[3], &s2.vec[4], &s2.vec[5], &t.vec[0], rhoprime,
                      nonce + 8, nonce + 9, nonce + 10, nonce + 11);
  nonce += 11;
#else
#error ""
#endif

  /* Matrix-vector multiplication */
  s1hat = s1;
  polyvecl_ntt(&s1hat);
  for(i = 0; i < K; ++i) {
    polyvecl_pointwise_acc_invmontgomery(&t.vec[i], mat+i, &s1hat);
    poly_invntt_montgomery(t.vec+i);
  }

  /* Add noise vector s2 */
  polyveck_add(&t, &t, &s2);

  /* Extract t1 and write public key */
  polyveck_freeze(&t);
  polyveck_power2round(&t1, &t0, &t);
  pack_pk(pk, rho, &t1);

  /* Compute CRH(rho, t1) and write secret key */
  shake256(tr, CRHBYTES, pk, CRYPTO_PUBLICKEYBYTES);
  pack_sk(sk, rho, key, tr, &s1, &s2, &t0);

  return 0;
}

/*************************************************
* Name:        crypto_sign
*
* Description: Compute signed message.
*
* Arguments:   - unsigned char *sm: pointer to output signed message (allocated
*                                   array with CRYPTO_BYTES + mlen bytes)
*              - unsigned long long *smlen: pointer to output length of signed
*                                           message
*              - const unsigned char *m: pointer to message to be signed
*              - unsigned long long mlen: length of message
*              - const unsigned char *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int crypto_sign(unsigned char *sm,
                unsigned long long *smlen,
                const unsigned char *m,
                unsigned long long mlen,
                const unsigned char *sk) 
{
  unsigned long long i, j;
  unsigned int n;
  unsigned char seedbuf[2*SEEDBYTES + CRHBYTES];
  unsigned char *rho, *key, *mu, *tr;
  uint16_t nonce = 0;
  poly     c, chat;
  polyvecl mat[K], s1, y, yhat, z;
  polyveck s2, t0, w, w1;
  polyveck h, wcs2, wcs20, ct0, tmp;

  rho = seedbuf;
  key = seedbuf + SEEDBYTES;
  mu = seedbuf + 2*SEEDBYTES;
  tr = sm + CRYPTO_BYTES - CRHBYTES;
  unpack_sk(rho, key, tr, &s1, &s2, &t0, sk);

  /* Copy message at the end of the sm buffer */
  for(i = 0; i < mlen; ++i)
    sm[CRYPTO_BYTES + i] = m[i];

  /* Compute CRH(tr, msg) */
  shake256(mu, CRHBYTES, sm + CRYPTO_BYTES - CRHBYTES, CRHBYTES + mlen);

  /* Expand matrix and transform vectors */
  expand_mat(mat, rho);
  polyvecl_ntt(&s1);
  polyveck_ntt(&s2);
  polyveck_ntt(&t0);

  rej:
  /* Sample intermediate vector y */
#if L == 2
  poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &tmp.vec[0], &tmp.vec[1], key,
                           nonce, nonce + 1, nonce + 2, nonce + 3);
  nonce += 2;
#elif L == 3
  poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &y.vec[2], &tmp.vec[0], key,
                           nonce, nonce + 1, nonce + 2, nonce + 3);
  nonce += 3;
#elif L == 4
  poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &y.vec[2], &y.vec[3], key,
                           nonce, nonce + 1, nonce + 2, nonce + 3);
  nonce += 4;
#elif L == 5
  poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &y.vec[2], &y.vec[3], key,
                           nonce, nonce + 1, nonce + 2, nonce + 3);
  poly_uniform_gamma1m1(&y.vec[4], key, nonce + 4);
  nonce += 5;
#else
#error
#endif

  /* Matrix-vector multiplication */
  yhat = y;
  polyvecl_ntt(&yhat);
  for(i = 0; i < K; ++i) {
    polyvecl_pointwise_acc_invmontgomery(w.vec+i, mat+i, &yhat);
    poly_invntt_montgomery(w.vec+i);
  }

  /* Decompose w and call the random oracle */
  polyveck_freeze(&w);
  polyveck_decompose(&w1, &tmp, &w);
  challenge(&c, mu, &w1);

  /* Compute z, reject if it reveals secret */
  chat = c;
  poly_ntt(&chat);
  for(i = 0; i < L; ++i) {
    poly_pointwise_invmontgomery(z.vec+i, &chat, s1.vec+i);
    poly_invntt_montgomery(z.vec+i);
  }
  polyvecl_add(&z, &z, &y);
  polyvecl_freeze(&z);
  if(polyvecl_chknorm(&z, GAMMA1 - BETA))
    goto rej;

  /* Compute w - cs2, reject if w1 can not be computed from it */
  for(i = 0; i < K; ++i) {
    poly_pointwise_invmontgomery(wcs2.vec+i, &chat, s2.vec+i);
    poly_invntt_montgomery(wcs2.vec+i);
  }
  polyveck_sub(&wcs2, &w, &wcs2);
  polyveck_freeze(&wcs2);
  polyveck_decompose(&tmp, &wcs20, &wcs2);
  polyveck_freeze(&wcs20);
  if(polyveck_chknorm(&wcs20, GAMMA2 - BETA))
    goto rej;


  for(i = 0; i < K; ++i)
    for(j = 0; j < N; ++j)
      if(tmp.vec[i].coeffs[j] != w1.vec[i].coeffs[j])
        goto rej;

  /* Compute hints for w1 */
  for(i = 0; i < K; ++i) {
    poly_pointwise_invmontgomery(ct0.vec+i, &chat, t0.vec+i);
    poly_invntt_montgomery(ct0.vec+i);
  }

  polyveck_freeze(&ct0);
  if(polyveck_chknorm(&ct0, GAMMA2))
    goto rej;

  polyveck_add(&tmp, &wcs2, &ct0);
  polyveck_neg(&ct0);
  polyveck_freeze(&tmp);
  n = polyveck_make_hint(&h, &tmp, &ct0);
  if(n > OMEGA)
    goto rej;

  /* Write signature */
  pack_sig(sm, &z, &h, &c);

  *smlen = mlen + CRYPTO_BYTES;
  return 0;
}

/*************************************************
* Name:        crypto_sign_open
*
* Description: Verify signed message.
*
* Arguments:   - unsigned char *m: pointer to output message (allocated
*                                  array with smlen bytes), can be equal to sm
*              - unsigned long long *mlen: pointer to output length of message
*              - const unsigned char *sm: pointer to signed message
*              - unsigned long long smlen: length of signed message
*              - const unsigned char *sk: pointer to bit-packed public key
*
* Returns 0 if signed message could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_open(unsigned char *m,
                     unsigned long long *mlen,
                     const unsigned char *sm,
                     unsigned long long smlen,
                     const unsigned char *pk)
{
  unsigned long long i;
  unsigned char rho[SEEDBYTES];
  unsigned char mu[CRHBYTES];
  poly     c, chat, cp;
  polyvecl mat[K], z;
  polyveck t1, w1, h, tmp1, tmp2;

  if(smlen < CRYPTO_BYTES)
    goto badsig;

  *mlen = smlen - CRYPTO_BYTES;

  unpack_pk(rho, &t1, pk);
  unpack_sig(&z, &h, &c, sm);
  if(polyvecl_chknorm(&z, GAMMA1 - BETA))
    goto badsig;

  /* Compute CRH(CRH(rho, t1), msg) using m as "playground" buffer */
  for(i = 0; i < CRYPTO_PUBLICKEYBYTES; ++i)
    m[CRYPTO_BYTES - CRYPTO_PUBLICKEYBYTES + i] = pk[i];
  
  if(sm != m)
    for(i = 0; i < *mlen; ++i)
      m[CRYPTO_BYTES + i] = sm[CRYPTO_BYTES + i];

  shake256(m + CRYPTO_BYTES - CRHBYTES, CRHBYTES,
           m + CRYPTO_BYTES - CRYPTO_PUBLICKEYBYTES, CRYPTO_PUBLICKEYBYTES);
  shake256(mu, CRHBYTES, m + CRYPTO_BYTES - CRHBYTES, CRHBYTES + *mlen);

  expand_mat(mat, rho);
  
  /* Matrix-vector multiplication; compute Az - c2^dt1 */
  polyvecl_ntt(&z);
  for(i = 0; i < K ; ++i)
    polyvecl_pointwise_acc_invmontgomery(tmp1.vec+i, mat+i, &z);

  chat = c;
  poly_ntt(&chat);
  polyveck_shiftl(&t1, D);
  polyveck_ntt(&t1);
  for(i = 0; i < K; ++i)
    poly_pointwise_invmontgomery(tmp2.vec+i, &chat, t1.vec+i);

  polyveck_sub(&tmp1, &tmp1, &tmp2);
  polyveck_freeze(&tmp1);  // reduce32 would be sufficient
  polyveck_invntt_montgomery(&tmp1);

  /* Reconstruct w1 */
  polyveck_freeze(&tmp1);
  polyveck_use_hint(&w1, &tmp1, &h);

  /* Call random oracle and verify challenge */
  challenge(&cp, mu, &w1);
  for(i = 0; i < N; ++i)
    if(c.coeffs[i] != cp.coeffs[i])
      goto badsig;

  /* All good, copy msg, return 0 */
  for(i = 0; i < *mlen; ++i)
    m[i] = sm[CRYPTO_BYTES + i];
  
  return 0;

  /* Signature verification failed */
  badsig:
  *mlen = (unsigned long long) -1;
  for(i = 0; i < smlen; ++i)
    m[i] = 0;
  
  return -1;
}
