#include <stdint.h>
#include <string.h>
#include "align.h"
#include "params.h"
#include "sign.h"
#include "packing.h"
#include "polyvec.h"
#include "poly.h"
#include "randombytes.h"
#include "symmetric.h"
#include "fips202.h"

static inline void polyvec_matrix_expand_row(polyvecl **row, polyvecl buf[2], const uint8_t rho[SEEDBYTES], unsigned int i) {
  switch(i) {
    case 0:
      polyvec_matrix_expand_row0(buf, buf + 1, rho);
      *row = buf;
      break;
    case 1:
      polyvec_matrix_expand_row1(buf + 1, buf, rho);
      *row = buf + 1;
      break;
    case 2:
      polyvec_matrix_expand_row2(buf, buf + 1, rho);
      *row = buf;
      break;
    case 3:
      polyvec_matrix_expand_row3(buf + 1, buf, rho);
      *row = buf + 1;
      break;
#if K > 4
    case 4:
      polyvec_matrix_expand_row4(buf, buf + 1, rho);
      *row = buf;
      break;
    case 5:
      polyvec_matrix_expand_row5(buf + 1, buf, rho);
      *row = buf + 1;
      break;
#endif
#if K > 6
    case 6:
      polyvec_matrix_expand_row6(buf, buf + 1, rho);
      *row = buf;
      break;
    case 7:
      polyvec_matrix_expand_row7(buf + 1, buf, rho);
      *row = buf + 1;
      break;
#endif
  }
}

// Forward declaration
static int crypto_sign_keypair_internal(uint8_t *pk, uint8_t *sk, const uint8_t seed[SEEDBYTES]);

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
  uint8_t seed[SEEDBYTES];
  int ret;

  /* Get randomness for seed */
  randombytes(seed, SEEDBYTES);
  
  /* Generate keypair using internal function */
  ret = crypto_sign_keypair_internal(pk, sk, seed);
  
  /* Clean up sensitive data */
  OQS_MEM_cleanse(seed, sizeof(seed));
  
  return ret;
}

// Internal function for keypair generation
static int crypto_sign_keypair_internal(uint8_t *pk, uint8_t *sk, const uint8_t seed[SEEDBYTES]) {
  unsigned int i;
  uint8_t seedbuf[2*SEEDBYTES + CRHBYTES];
  const uint8_t *rho, *rhoprime, *key;
  polyvecl rowbuf[2];
  polyvecl s1, *row = rowbuf;
  polyveck s2;
  poly t1, t0;

  /* Expand seed to rho, rhoprime and key */
  memcpy(seedbuf, seed, SEEDBYTES);
  seedbuf[SEEDBYTES] = K;
  seedbuf[SEEDBYTES + 1] = L;
  shake256(seedbuf, 2*SEEDBYTES + CRHBYTES, seedbuf, SEEDBYTES + 2);
  rho = seedbuf;
  rhoprime = rho + SEEDBYTES;
  key = rhoprime + CRHBYTES;

  /* Store rho, key */
  memcpy(pk, rho, SEEDBYTES);
  memcpy(sk, rho, SEEDBYTES);
  memcpy(sk + SEEDBYTES, key, SEEDBYTES);

  /* Sample short vectors s1 and s2 */
#if K == 4 && L == 4
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime, 0, 1, 2, 3);
  poly_uniform_eta_4x(&s2.vec[0], &s2.vec[1], &s2.vec[2], &s2.vec[3], rhoprime, 4, 5, 6, 7);
#elif K == 6 && L == 5
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime, 0, 1, 2, 3);
  poly_uniform_eta_4x(&s1.vec[4], &s2.vec[0], &s2.vec[1], &s2.vec[2], rhoprime, 4, 5, 6, 7);
  poly_uniform_eta_4x(&s2.vec[3], &s2.vec[4], &s2.vec[5], &t0, rhoprime, 8, 9, 10, 11);
#elif K == 8 && L == 7
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime, 0, 1, 2, 3);
  poly_uniform_eta_4x(&s1.vec[4], &s1.vec[5], &s1.vec[6], &s2.vec[0], rhoprime, 4, 5, 6, 7);
  poly_uniform_eta_4x(&s2.vec[1], &s2.vec[2], &s2.vec[3], &s2.vec[4], rhoprime, 8, 9, 10, 11);
  poly_uniform_eta_4x(&s2.vec[5], &s2.vec[6], &s2.vec[7], &t0, rhoprime, 12, 13, 14, 15);
#else
#error
#endif

  /* Clear rhoprime immediately after use */
  OQS_MEM_cleanse((uint8_t*)(seedbuf + SEEDBYTES), CRHBYTES);

  /* Pack secret vectors */
  for(i = 0; i < L; i++)
    polyeta_pack(sk + 2*SEEDBYTES + TRBYTES + i*POLYETA_PACKEDBYTES, &s1.vec[i]);
  for(i = 0; i < K; i++)
    polyeta_pack(sk + 2*SEEDBYTES + TRBYTES + (L + i)*POLYETA_PACKEDBYTES, &s2.vec[i]);

  /* Transform s1 */
  polyvecl_ntt(&s1);

  for(i = 0; i < K; i++) {
    /* Expand matrix row */
    polyvec_matrix_expand_row(&row, rowbuf, rho, i);

    /* Compute inner-product */
    polyvecl_pointwise_acc_montgomery(&t1, row, &s1);
    poly_invntt_tomont(&t1);

    /* Add error polynomial */
    poly_add(&t1, &t1, &s2.vec[i]);

    /* Round t and pack t1, t0 */
    poly_caddq(&t1);
    poly_power2round(&t1, &t0, &t1);
    polyt1_pack(pk + SEEDBYTES + i*POLYT1_PACKEDBYTES, &t1);
    polyt0_pack(sk + 2*SEEDBYTES + TRBYTES + (L+K)*POLYETA_PACKEDBYTES + i*POLYT0_PACKEDBYTES, &t0);
  }

  /* Compute H(rho, t1) and store in secret key */
  shake256(sk + 2*SEEDBYTES, TRBYTES, pk, CRYPTO_PUBLICKEYBYTES);

  /* Clean up sensitive data */
  OQS_MEM_cleanse(seedbuf, sizeof(seedbuf));
  OQS_MEM_cleanse(&s1, sizeof(s1));
  OQS_MEM_cleanse(&s2, sizeof(s2));
  OQS_MEM_cleanse(rowbuf, sizeof(rowbuf));
  OQS_MEM_cleanse(&t0, sizeof(t0));
  OQS_MEM_cleanse(&t1, sizeof(t1));

  return 0;
}

/*************************************************
* Name:        crypto_sign_keypair_derand
*
* Description: Generates public and secret key for the ML-DSA-44 signature scheme.
*              Deterministic version using a seed.
*
* Arguments:   - uint8_t *pk: pointer to output public key (allocated
*                             array of CRYPTO_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output secret key (allocated
*                             array of CRYPTO_SECRETKEYBYTES bytes)
*              - const uint8_t seed[SEEDBYTES]: pointer to seed (32 bytes)
*
* Returns 0 (success)
**************************************************/
int crypto_sign_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t seed[SEEDBYTES]) {
  return crypto_sign_keypair_internal(pk, sk, seed);
}

/*************************************************
* Name:        crypto_sign_signature_internal
*
* Description: Computes signature. Internal API.
*
* Arguments:   - uint8_t *sig: pointer to output signature (of length CRYPTO_BYTES)
*              - size_t *siglen: pointer to output length of signature
*              - uint8_t *m: pointer to message to be signed
*              - size_t mlen: length of message
*              - uint8_t *pre: pointer to prefix string
*              - size_t prelen: length of prefix string
*              - uint8_t *rnd: pointer to random seed
*              - uint8_t *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int crypto_sign_signature_internal(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen,
                                   const uint8_t *pre, size_t prelen, const uint8_t rnd[RNDBYTES], const uint8_t *sk)
{
  unsigned int i, n, pos;
  uint8_t seedbuf[2*SEEDBYTES + TRBYTES + 2*CRHBYTES];
  uint8_t *rho, *tr, *key, *mu, *rhoprime;
  uint8_t hintbuf[N];
  uint8_t *hint = sig + CTILDEBYTES + L*POLYZ_PACKEDBYTES;
  uint64_t nonce = 0;
  polyvecl mat[K], s1, z;
  polyveck t0, s2, w1;
  poly c, tmp;
  union {
    polyvecl y;
    polyveck w0;
  } tmpv;
  shake256incctx state;

  rho = seedbuf;
  tr = rho + SEEDBYTES;
  key = tr + TRBYTES;
  mu = key + SEEDBYTES;
  rhoprime = mu + CRHBYTES;
  unpack_sk(rho, tr, key, &t0, &s1, &s2, sk);

  /* Compute mu = CRH(tr, pre, msg) */
  shake256_inc_init(&state);
  shake256_inc_absorb(&state, tr, TRBYTES);
  shake256_inc_absorb(&state, pre, prelen);
  shake256_inc_absorb(&state, m, mlen);
  shake256_inc_finalize(&state);
  shake256_inc_squeeze(mu, CRHBYTES, &state);

  /* Compute rhoprime = CRH(key, rnd, mu) */
  shake256_inc_ctx_reset(&state);
  shake256_inc_absorb(&state, key, SEEDBYTES);
  shake256_inc_absorb(&state, rnd, RNDBYTES);
  shake256_inc_absorb(&state, mu, CRHBYTES);
  shake256_inc_finalize(&state);
  shake256_inc_squeeze(rhoprime, CRHBYTES, &state);

  /* Expand matrix and transform vectors */
  polyvec_matrix_expand(mat, rho);
  polyvecl_ntt(&s1);
  polyveck_ntt(&s2);
  polyveck_ntt(&t0);

rej:
  /* Sample intermediate vector y */
#if L == 4
  poly_uniform_gamma1_4x(&z.vec[0], &z.vec[1], &z.vec[2], &z.vec[3],
                         rhoprime, nonce, nonce + 1, nonce + 2, nonce + 3);
  nonce += 4;
#elif L == 5
  poly_uniform_gamma1_4x(&z.vec[0], &z.vec[1], &z.vec[2], &z.vec[3],
                         rhoprime, nonce, nonce + 1, nonce + 2, nonce + 3);
  poly_uniform_gamma1(&z.vec[4], rhoprime, nonce + 4);
  nonce += 5;
#elif L == 7
  poly_uniform_gamma1_4x(&z.vec[0], &z.vec[1], &z.vec[2], &z.vec[3],
                         rhoprime, nonce, nonce + 1, nonce + 2, nonce + 3);
  poly_uniform_gamma1_4x(&z.vec[4], &z.vec[5], &z.vec[6], &tmp,
                         rhoprime, nonce + 4, nonce + 5, nonce + 6, 0);
  nonce += 7;
#else
#error
#endif

  /* Matrix-vector product */
  tmpv.y = z;
  polyvecl_ntt(&tmpv.y);
  polyvec_matrix_pointwise_montgomery(&w1, mat, &tmpv.y);
  polyveck_invntt_tomont(&w1);

  /* Decompose w and call the random oracle */
  polyveck_caddq(&w1);
  polyveck_decompose(&w1, &tmpv.w0, &w1);
  polyveck_pack_w1(sig, &w1);

  shake256_inc_ctx_reset(&state);
  shake256_inc_absorb(&state, mu, CRHBYTES);
  shake256_inc_absorb(&state, sig, K*POLYW1_PACKEDBYTES);
  shake256_inc_finalize(&state);
  shake256_inc_squeeze(sig, CTILDEBYTES, &state);
  poly_challenge(&c, sig);
  poly_ntt(&c);

  /* Compute z, reject if it reveals secret */
  for(i = 0; i < L; i++) {
    poly_pointwise_montgomery(&tmp, &c, &s1.vec[i]);
    poly_invntt_tomont(&tmp);
    poly_add(&z.vec[i], &z.vec[i], &tmp);
    poly_reduce(&z.vec[i]);
    if(poly_chknorm(&z.vec[i], GAMMA1 - BETA))
      goto rej;
  }

  /* Zero hint vector in signature */
  pos = 0;
  memset(hint, 0, OMEGA);

  for(i = 0; i < K; i++) {
    /* Check that subtracting cs2 does not change high bits of w and low bits
     * do not reveal secret information */
    poly_pointwise_montgomery(&tmp, &c, &s2.vec[i]);
    poly_invntt_tomont(&tmp);
    poly_sub(&tmpv.w0.vec[i], &tmpv.w0.vec[i], &tmp);
    poly_reduce(&tmpv.w0.vec[i]);
    if(poly_chknorm(&tmpv.w0.vec[i], GAMMA2 - BETA))
      goto rej;

    /* Compute hints */
    poly_pointwise_montgomery(&tmp, &c, &t0.vec[i]);
    poly_invntt_tomont(&tmp);
    poly_reduce(&tmp);
    if(poly_chknorm(&tmp, GAMMA2))
      goto rej;

    poly_add(&tmpv.w0.vec[i], &tmpv.w0.vec[i], &tmp);
    n = poly_make_hint(hintbuf, &tmpv.w0.vec[i], &w1.vec[i]);
    if(pos + n > OMEGA)
      goto rej;

    /* Store hints in signature */
    memcpy(&hint[pos], hintbuf, n);
    hint[OMEGA + i] = pos = pos + n;
  }

  shake256_inc_ctx_release(&state);
  /* Pack z into signature */
  for(i = 0; i < L; i++)
    polyz_pack(sig + CTILDEBYTES + i*POLYZ_PACKEDBYTES, &z.vec[i]);

  *siglen = CRYPTO_BYTES;
  return 0;
}

/*************************************************
* Name:        crypto_sign_signature
*
* Description: Computes signature.
*
* Arguments:   - uint8_t *sig: pointer to output signature (of length CRYPTO_BYTES)
*              - size_t *siglen: pointer to output length of signature
*              - uint8_t *m: pointer to message to be signed
*              - size_t mlen: length of message
*              - uint8_t *ctx: pointer to context string
*              - size_t ctxlen: length of context string
*              - uint8_t *sk: pointer to bit-packed secret key
*
* Returns 0 (success) or -1 (context string too long)
**************************************************/
int crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen,
                          const uint8_t *ctx, size_t ctxlen, const uint8_t *sk)
{
  uint8_t pre[257];
  uint8_t rnd[RNDBYTES];

  if(ctxlen > 255)
    return -1;

  /* Prepare pre = (0, ctxlen, ctx) */
  pre[0] = 0;
  pre[1] = ctxlen;
  memcpy(&pre[2], ctx, ctxlen);

#ifdef DILITHIUM_RANDOMIZED_SIGNING
  randombytes(rnd, RNDBYTES);
#else
  memset(rnd, 0, RNDBYTES);
#endif

  crypto_sign_signature_internal(sig,siglen,m,mlen,pre,2+ctxlen,rnd,sk);
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
*              - const uint8_t *ctx: pointer to context string
*              - size_t ctxlen: length of context string
*              - const uint8_t *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int crypto_sign(uint8_t *sm, size_t *smlen, const uint8_t *m, size_t mlen, const uint8_t *ctx, size_t ctxlen,
                const uint8_t *sk)
{
  size_t i;
  int ret;

  for(i = 0; i < mlen; ++i)
    sm[CRYPTO_BYTES + mlen - 1 - i] = m[mlen - 1 - i];
  ret = crypto_sign_signature(sm, smlen, sm + CRYPTO_BYTES, mlen, ctx, ctxlen, sk);
  *smlen += mlen;
  return ret;
}

/*************************************************
* Name:        crypto_sign_verify_internal
*
* Description: Verifies signature. Internal API.
*
* Arguments:   - uint8_t *m: pointer to input signature
*              - size_t siglen: length of signature
*              - const uint8_t *m: pointer to message
*              - size_t mlen: length of message
*              - const uint8_t *pre: pointer to prefix string
*              - size_t prelen: length of prefix string
*              - const uint8_t *pk: pointer to bit-packed public key
*
* Returns 0 if signature could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_verify_internal(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen,
                                const uint8_t *pre, size_t prelen, const uint8_t *pk) {
  unsigned int i, j, pos = 0;
  /* polyw1_pack writes additional 14 bytes */
  ALIGNED_UINT8(K*POLYW1_PACKEDBYTES+14) buf;
  uint8_t mu[CRHBYTES];
  const uint8_t *hint = sig + CTILDEBYTES + L*POLYZ_PACKEDBYTES;
  polyvecl rowbuf[2];
  polyvecl *row = rowbuf;
  polyvecl z;
  poly c, w1, h;
  shake256incctx state;

  if(siglen != CRYPTO_BYTES)
    return -1;

  /* Compute CRH(H(rho, t1), pre, msg) */
  shake256(mu, TRBYTES, pk, CRYPTO_PUBLICKEYBYTES);
  shake256_inc_init(&state);
  shake256_inc_absorb(&state, mu, CRHBYTES);
  shake256_inc_absorb(&state, pre, prelen);
  shake256_inc_absorb(&state, m, mlen);
  shake256_inc_finalize(&state);
  shake256_inc_squeeze(mu, CRHBYTES, &state);

  /* Expand challenge */
  poly_challenge(&c, sig);
  poly_ntt(&c);

  /* Unpack z; shortness follows from unpacking */
  for(i = 0; i < L; i++) {
    polyz_unpack(&z.vec[i], sig + CTILDEBYTES + i*POLYZ_PACKEDBYTES);
    poly_ntt(&z.vec[i]);
  }

  for(i = 0; i < K; i++) {
    /* Expand matrix row */
    polyvec_matrix_expand_row(&row, rowbuf, pk, i);

    /* Compute i-th row of Az - c2^Dt1 */
    polyvecl_pointwise_acc_montgomery(&w1, row, &z);

    polyt1_unpack(&h, pk + SEEDBYTES + i*POLYT1_PACKEDBYTES);
    poly_shiftl(&h);
    poly_ntt(&h);
    poly_pointwise_montgomery(&h, &c, &h);

    poly_sub(&w1, &w1, &h);
    poly_reduce(&w1);
    poly_invntt_tomont(&w1);

    /* Get hint polynomial and reconstruct w1 */
    memset(h.vec, 0, sizeof(poly));
    if(hint[OMEGA + i] < pos || hint[OMEGA + i] > OMEGA) {
      shake256_inc_ctx_release(&state);
      return -1;
    }

    for(j = pos; j < hint[OMEGA + i]; ++j) {
      /* Coefficients are ordered for strong unforgeability */
      if(j > pos && hint[j] <= hint[j-1]) {
        shake256_inc_ctx_release(&state);
        return -1;
      }
      h.coeffs[hint[j]] = 1;
    }
    pos = hint[OMEGA + i];

    poly_caddq(&w1);
    poly_use_hint(&w1, &w1, &h);
    polyw1_pack(buf.coeffs + i*POLYW1_PACKEDBYTES, &w1);
  }

  /* Extra indices are zero for strong unforgeability */
  for(j = pos; j < OMEGA; ++j)
    if(hint[j]) {
      shake256_inc_ctx_release(&state);
      return -1;
    }

  /* Call random oracle and verify challenge */
  shake256_inc_ctx_reset(&state);
  shake256_inc_absorb(&state, mu, CRHBYTES);
  shake256_inc_absorb(&state, buf.coeffs, K*POLYW1_PACKEDBYTES);
  shake256_inc_finalize(&state);
  shake256_inc_squeeze(buf.coeffs, CTILDEBYTES, &state);
  shake256_inc_ctx_release(&state);
  for(i = 0; i < CTILDEBYTES; ++i)
    if(buf.coeffs[i] != sig[i])
      return -1;

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
*              - const uint8_t *ctx: pointer to context string
*              - size_t ctxlen: length of context string
*              - const uint8_t *pk: pointer to bit-packed public key
*
* Returns 0 if signature could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen,
                       const uint8_t *ctx, size_t ctxlen, const uint8_t *pk)
{
  uint8_t pre[257];

  if(ctxlen > 255)
    return -1;

  pre[0] = 0;
  pre[1] = ctxlen;
  memcpy(&pre[2], ctx, ctxlen);
  return crypto_sign_verify_internal(sig,siglen,m,mlen,pre,2+ctxlen,pk);
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
*              - const uint8_t *ctx: pointer to context string
*              - size_t ctxlen: length of context string
*              - const uint8_t *pk: pointer to bit-packed public key
*
* Returns 0 if signed message could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen,
                     const uint8_t *ctx, size_t ctxlen, const uint8_t *pk) {
  size_t i;

  if(smlen < CRYPTO_BYTES)
    goto badsig;

  *mlen = smlen - CRYPTO_BYTES;
  if(crypto_sign_verify(sm, CRYPTO_BYTES, sm + CRYPTO_BYTES, *mlen, ctx, ctxlen, pk))
    goto badsig;
  else {
    /* All good, copy msg, return 0 */
    for(i = 0; i < *mlen; ++i)
      m[i] = sm[CRYPTO_BYTES + i];
    return 0;
  }

badsig:
  /* Signature verification failed */
  *mlen = 0;
  for(i = 0; i < smlen; ++i)
    m[i] = 0;

  return -1;
}
