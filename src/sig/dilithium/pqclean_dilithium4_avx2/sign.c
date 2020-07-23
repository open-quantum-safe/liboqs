#include <stdint.h>
#include "params.h"
#include "sign.h"
#include "packing.h"
#include "polyvec.h"
#include "poly.h"
#include "randombytes.h"
#include "symmetric.h"
#include "fips202.h"
#ifdef DILITHIUM_USE_AES
#include "aes256ctr.h"
#endif

/*************************************************
* Name:        challenge
*
* Description: Implementation of H. Samples polynomial with 60 nonzero
*              coefficients in {-1,1} using the output stream of
*              SHAKE256(mu|w1).
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const uint8_t mu[]: byte array containing mu
*              - const polyveck *w1: pointer to vector w1
**************************************************/
void challenge(poly *c,
               const uint8_t mu[CRHBYTES],
               const polyveck *w1)
{
  unsigned int i, b, pos;
  uint64_t signs;
  __attribute__((aligned(32)))
  uint8_t buf[CRHBYTES + K*POLYW1_PACKEDBYTES];
  keccak_state state;

  for(i = 0; i < CRHBYTES; ++i)
    buf[i] = mu[i];
  for(i = 0; i < K; ++i)
    polyw1_pack(buf + CRHBYTES + i*POLYW1_PACKEDBYTES, &w1->vec[i]);

  shake256_init(&state);
  shake256_absorb(&state, buf, sizeof(buf));
  shake256_finalize(&state);
  shake256_squeezeblocks(buf, 1, &state);

  signs = 0;
  for(i = 0; i < 8; ++i)
    signs |= (uint64_t)buf[i] << 8*i;

  pos = 8;

  for(i = 0; i < N; ++i)
    c->coeffs[i] = 0;

  for(i = 196; i < 256; ++i) {
    do {
      if(pos >= SHAKE256_RATE) {
        shake256_squeezeblocks(buf, 1, &state);
        pos = 0;
      }

      b = buf[pos++];
    } while(b > i);

    c->coeffs[i] = c->coeffs[b];
    c->coeffs[b] = 1;
    c->coeffs[b] ^= -((uint32_t)signs & 1) & (1 ^ (Q-1));
    signs >>= 1;
  }
}

/*************************************************
* Name:        crypto_sign_keypair
*
* Description: Generates public and private key.
*
* Arguments:   - uint8_t *pk: pointer to output public key (allocated
*                             array of CRYPTO_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key (allocated
*                             array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int crypto_sign_keypair(uint8_t *pk, uint8_t *sk) {
  unsigned int i;
  __attribute__((aligned(32)))
  uint8_t seedbuf[3*SEEDBYTES];
  __attribute__((aligned(32)))
  uint8_t tr[CRHBYTES];
  const uint8_t *rho, *rhoprime, *key;
  polyvecl mat[K];
  polyvecl s1, s1hat;
  polyveck s2, t1, t0;

  /* Get randomness for rho, rhoprime and key */
  randombytes(seedbuf, 3*SEEDBYTES);
  rho = seedbuf;
  rhoprime = seedbuf + SEEDBYTES;
  key = seedbuf + 2*SEEDBYTES;

  /* Expand matrix */
  expand_mat(mat, rho);

  /* Sample short vectors s1 and s2 */
#ifdef DILITHIUM_USE_AES
  __attribute__((aligned(16)))
  uint64_t nonce = 0;
  aes256ctr_ctx aesctx;
  aes256ctr_init(&aesctx, rhoprime, nonce++);
  for(i = 0; i < L; ++i) {
    poly_uniform_eta_preinit(&s1.vec[i], &aesctx);
    aesctx.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
  }
  for(i = 0; i < K; ++i) {
    poly_uniform_eta_preinit(&s2.vec[i], &aesctx);
    aesctx.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
  }
#elif L == 2 && K == 3
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s2.vec[0], &s2.vec[1], rhoprime,
                      0, 1, 2, 3);
  poly_uniform_eta(&s2.vec[2], rhoprime, 4);
#elif L == 3 && K == 4
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s2.vec[0], rhoprime,
                      0, 1, 2, 3);
  poly_uniform_eta_4x(&s2.vec[1], &s2.vec[2], &s2.vec[3], &t1.vec[0], rhoprime,
                      4, 5, 6, 7);
#elif L == 4 && K == 5
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime,
                      0, 1, 2, 3);
  poly_uniform_eta_4x(&s2.vec[0], &s2.vec[1], &s2.vec[2], &s2.vec[3], rhoprime,
                      4, 5, 6, 7);
  poly_uniform_eta(&s2.vec[4], rhoprime, 8);
#elif L == 5 && K == 6
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime,
                      0, 1, 2, 3);
  poly_uniform_eta_4x(&s1.vec[4], &s2.vec[0], &s2.vec[1], &s2.vec[2], rhoprime,
                      4, 5, 6, 7);
  poly_uniform_eta_4x(&s2.vec[3], &s2.vec[4], &s2.vec[5], &t1.vec[0], rhoprime,
                      8, 9, 10, 11);
#else
#error
#endif

  /* Matrix-vector multiplication */
  s1hat = s1;
  polyvecl_ntt(&s1hat);
  for(i = 0; i < K; ++i) {
    polyvecl_pointwise_acc_montgomery(&t1.vec[i], &mat[i], &s1hat);
    //poly_reduce(&t1.vec[i]);
    poly_invntt_tomont(&t1.vec[i]);
  }

  /* Add error vector s2 */
  polyveck_add(&t1, &t1, &s2);

  /* Extract t1 and write public key */
  polyveck_freeze(&t1);
  polyveck_power2round(&t1, &t0, &t1);
  pack_pk(pk, rho, &t1);

  /* Compute CRH(rho, t1) and write secret key */
  crh(tr, pk, CRYPTO_PUBLICKEYBYTES);
  pack_sk(sk, rho, key, tr, &s1, &s2, &t0);

  return 0;
}

/*************************************************
* Name:        crypto_sign_signature
*
* Description: Computes signature.
*
* Arguments:   - uint8_t *sig: pointer to output signature (of length CRYPTO_BYTES)
*              - size_t *siglen: pointer to output length of signed message
*              - uint8_t *m: pointer to message to be signed
*              - size_t mlen: length of message
*              - uint8_t *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int crypto_sign_signature(uint8_t *sig,
                          size_t *siglen,
                          const uint8_t *m,
                          size_t mlen,
                          const uint8_t *sk)
{
  unsigned int i, n;
  __attribute__((aligned(32)))
  uint8_t seedbuf[2*SEEDBYTES + 3*CRHBYTES];
  uint8_t *rho, *tr, *key, *mu, *rhoprime;
  __attribute__((aligned(16)))
  uint64_t nonce = 0;
  poly c;
  polyvecl mat[K], s1, z;
  polyveck t0, s2, w1, w0, h;
  keccak_state state;

  rho = seedbuf;
  tr = rho + SEEDBYTES;
  key = tr + CRHBYTES;
  mu = key + SEEDBYTES;
  rhoprime = mu + CRHBYTES;
  unpack_sk(rho, key, tr, &s1, &s2, &t0, sk);

  /* Compute CRH(tr, msg) */
  shake256_init(&state);
  shake256_absorb(&state, tr, CRHBYTES);
  shake256_absorb(&state, m, mlen);
  shake256_finalize(&state);
  shake256_squeeze(mu, CRHBYTES, &state);

#ifdef DILITHIUM_RANDOMIZED_SIGNING
  randombytes(rhoprime, CRHBYTES);
#else
  crh(rhoprime, key, SEEDBYTES + CRHBYTES);
#endif

  /* Expand matrix and transform vectors */
  expand_mat(mat, rho);
  polyvecl_ntt(&s1);
  polyveck_ntt(&s2);
  polyveck_ntt(&t0);

#ifdef DILITHIUM_USE_AES
  aes256ctr_ctx aesctx;
  aes256ctr_init(&aesctx, rhoprime, nonce++);
#endif

rej:
  /* Sample intermediate vector y */
#ifdef DILITHIUM_USE_AES
  for(i = 0; i < L; ++i) {
    poly_uniform_gamma1m1_preinit(&z.vec[i], &aesctx);
    aesctx.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
  }
#elif L == 2
  poly_uniform_gamma1m1_4x(&z.vec[0], &z.vec[1], &h.vec[0], &h.vec[1],
                           rhoprime, nonce, nonce + 1, 0, 0);
  nonce += 2;
#elif L == 3
  poly_uniform_gamma1m1_4x(&z.vec[0], &z.vec[1], &z.vec[2], &h.vec[0],
                           rhoprime, nonce, nonce + 1, nonce + 2, 0);
  nonce += 3;
#elif L == 4
  poly_uniform_gamma1m1_4x(&z.vec[0], &z.vec[1], &z.vec[2], &z.vec[3],
                           rhoprime, nonce, nonce + 1, nonce + 2, nonce + 3);
  nonce += 4;
#elif L == 5
  poly_uniform_gamma1m1_4x(&z.vec[0], &z.vec[1], &z.vec[2], &z.vec[3],
                           rhoprime, nonce, nonce + 1, nonce + 2, nonce + 3);
  poly_uniform_gamma1m1(&z.vec[4], rhoprime, nonce + 4);
  nonce += 5;
#else
#error
#endif

  /* Matrix-vector multiplication */
  polyvecl *tmpl = (polyvecl *)&h;
  *tmpl = z;
  polyvecl_ntt(tmpl);
  for(i = 0; i < K; ++i) {
    polyvecl_pointwise_acc_montgomery(&w1.vec[i], &mat[i], tmpl);
    //poly_reduce(&w1.vec[i]);
    poly_invntt_tomont(&w1.vec[i]);
  }

  /* Decompose w and call the random oracle */
  polyveck_csubq(&w1);
  polyveck_decompose(&w1, &w0, &w1);
  challenge(&c, mu, &w1);
  h.vec[K-1] = c;
  poly_ntt(&h.vec[K-1]);

  /* Compute z, reject if it reveals secret */
  for(i = 0; i < L; ++i) {
    poly_pointwise_montgomery(&h.vec[0], &h.vec[K-1], &s1.vec[i]);
    poly_invntt_tomont(&h.vec[0]);
    poly_add(&z.vec[i], &z.vec[i], &h.vec[0]);
  }
  polyvecl_freeze(&z);
  if(polyvecl_chknorm(&z, GAMMA1 - BETA))
    goto rej;

  /* Check that subtracting cs2 does not change high bits of w and low bits
   * do not reveal secret information */
  for(i = 0; i < K; ++i) {
    poly_pointwise_montgomery(&h.vec[0], &h.vec[K-1], &s2.vec[i]);
    poly_invntt_tomont(&h.vec[0]);
    poly_sub(&w0.vec[i], &w0.vec[i], &h.vec[0]);
  }
  polyveck_freeze(&w0);
  if(polyveck_chknorm(&w0, GAMMA2 - BETA))
    goto rej;

  /* Compute hints for w1 */
  for(i = 0; i < K; ++i) {
    poly_pointwise_montgomery(&h.vec[i], &h.vec[K-1], &t0.vec[i]);
    poly_invntt_tomont(&h.vec[i]);
  }
  polyveck_csubq(&h);
  if(polyveck_chknorm(&h, GAMMA2))
    goto rej;

  polyveck_add(&w0, &w0, &h);
  polyveck_csubq(&w0);
  n = polyveck_make_hint(&h, &w0, &w1);
  if(n > OMEGA)
    goto rej;

  /* Write signature */
  pack_sig(sig, &z, &h, &c);
  *siglen = CRYPTO_BYTES;
  return 0;
}

/*************************************************
* Name:        crypto_sign
*
* Description: Compute signed message.
*
* Arguments:   - uint8_t *sm: pointer to output signed message (allocated
*                             array with CRYPTO_BYTES + mlen bytes),
*                             can be equal to m
*              - size_t *smlen: pointer to output length of signed
*                               message
*              - const uint8_t *m: pointer to message to be signed
*              - size_t mlen: length of message
*              - const uint8_t *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int crypto_sign(uint8_t *sm,
                size_t *smlen,
                const uint8_t *m,
                size_t mlen,
                const uint8_t *sk)
{
  size_t i;

  for(i = 0; i < mlen; ++i)
    sm[CRYPTO_BYTES + mlen - 1 - i] = m[mlen - 1 - i];
  crypto_sign_signature(sm, smlen, sm + CRYPTO_BYTES, mlen, sk);
  *smlen += mlen;
  return 0;
}

/*************************************************
* Name:        crypto_sign_verify
*
* Description: Verifies signature.
*
* Arguments:   - uint8_t *m: pointer to input signature
*              - size_t siglen: length of signature
*              - const uint8_t *m: pointer to message
*              - size_t mlen: length of message
*              - const uint8_t *pk: pointer to bit-packed public key
*
* Returns 0 if signature could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_verify(const uint8_t *sig,
                       size_t siglen,
                       const uint8_t *m,
                       size_t mlen,
                       const uint8_t *pk)
{
  unsigned int i;
  __attribute__((aligned(32)))
  uint8_t rho[SEEDBYTES];
  __attribute__((aligned(32)))
  uint8_t mu[CRHBYTES];
  poly c, cp;
  polyvecl mat[K], z;
  polyveck t1, h, w1;
  keccak_state state;

  if(siglen != CRYPTO_BYTES)
    return -1;

  unpack_pk(rho, &t1, pk);
  if(unpack_sig(&z, &h, &c, sig))
    return -1;
  if(polyvecl_chknorm(&z, GAMMA1 - BETA))
    return -1;

  /* Compute CRH(CRH(rho, t1), msg) */
  crh(mu, pk, CRYPTO_PUBLICKEYBYTES);
  shake256_init(&state);
  shake256_absorb(&state, mu, CRHBYTES);
  shake256_absorb(&state, m, mlen);
  shake256_finalize(&state);
  shake256_squeeze(mu, CRHBYTES, &state);

  /* Matrix-vector multiplication; compute Az - c2^dt1 */
  expand_mat(mat, rho);
  polyvecl_ntt(&z);
  for(i = 0; i < K ; ++i)
    polyvecl_pointwise_acc_montgomery(&w1.vec[i], &mat[i], &z);

  cp = c;
  poly_ntt(&cp);
  polyveck_shiftl(&t1);
  polyveck_ntt(&t1);
  for(i = 0; i < K; ++i)
    poly_pointwise_montgomery(&t1.vec[i], &cp, &t1.vec[i]);

  polyveck_sub(&w1, &w1, &t1);
  polyveck_reduce(&w1);
  polyveck_invntt_tomont(&w1);

  /* Reconstruct w1 */
  polyveck_csubq(&w1);
  polyveck_use_hint(&w1, &w1, &h);

  /* Call random oracle and verify challenge */
  challenge(&cp, mu, &w1);
  for(i = 0; i < N; ++i)
    if(c.coeffs[i] != cp.coeffs[i])
      return -1;

  return 0;
}

/*************************************************
* Name:        crypto_sign_open
*
* Description: Verify signed message.
*
* Arguments:   - uint8_t *m: pointer to output message (allocated
*                            array with smlen bytes), can be equal to sm
*              - size_t *mlen: pointer to output length of message
*              - const uint8_t *sm: pointer to signed message
*              - size_t smlen: length of signed message
*              - const uint8_t *pk: pointer to bit-packed public key
*
* Returns 0 if signed message could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_open(uint8_t *m,
                     size_t *mlen,
                     const uint8_t *sm,
                     size_t smlen,
                     const uint8_t *pk)
{
  size_t i;

  if(smlen < CRYPTO_BYTES)
    goto badsig;

  *mlen = smlen - CRYPTO_BYTES;
  if(crypto_sign_verify(sm, CRYPTO_BYTES, sm + CRYPTO_BYTES, *mlen, pk))
    goto badsig;
  else {
    /* All good, copy msg, return 0 */
    for(i = 0; i < *mlen; ++i)
      m[i] = sm[CRYPTO_BYTES + i];

    return 0;
  }

badsig:
  /* Signature verification failed */
  *mlen = -1;
  for(i = 0; i < smlen; ++i)
    m[i] = 0;

  return -1;
}
