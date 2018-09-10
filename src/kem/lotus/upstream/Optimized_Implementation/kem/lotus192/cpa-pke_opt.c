/**
 * @file cpa-pke_opt.c
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief An optimized implementaion of LOTUS-CPA-PKE
 */

#include "type.h"
#include "param.h"
#include "crypto.h"
#include "lwe-arithmetics_opt.h"
#include "sampler.h"
#include "randombytes.h"
#include "pack.h"

#include <stdio.h>
#include <string.h>

typedef struct {
  U16 A[_LOTUS_LWE_DIM * _LOTUS_LWE_DIM];
  U16 P[_LOTUS_LWE_DIM * _LOTUS_LWE_PT];
} pubkey_t;

typedef struct {
  U16 S[_LOTUS_LWE_DIM * _LOTUS_LWE_PT];
  pubkey_t pk;
} prvkey_t;

typedef struct {
  U16 c1[_LOTUS_LWE_DIM];
  U16 c2[_LOTUS_LWE_PT];
} ct_t;


/**
 * @param[out] pk a public key
 * @param[out] sk a secret key
 */
void lotus_cpa_pke_keypair(U16 *pk, U16 *sk){
  U8 A_seed[_LOTUS_SEED_BYTES];
  pubkey_t *pkt = (pubkey_t*)pk;
  prvkey_t *skt = (prvkey_t*)sk;
  /* A_seed is a seed for sampling the uniform matrix A, which means A can be compressed 
     to A_seed for transmission then be extracted via sampling, i.e., sample_uniform(). 
     However, sample_uniform() is relatively slow and it is better to be called just once 
     (after transmission), but current API does not match the case since API does not allow
     precomputation. 
     Therefore in this implementation we just store A and P to the public key. */
  randombytes(A_seed, _LOTUS_SEED_BYTES);
  sampler_set_seed(A_seed);
  sample_uniform(pkt->A, _LOTUS_LWE_DIM * _LOTUS_LWE_DIM);

  /* Sampler should be initialized again for sampling independently from the matrix A. */
  sampler_init();
  sample_discrete_gaussian(skt->S, _LOTUS_LWE_DIM * _LOTUS_LWE_PT);
  sample_discrete_gaussian(pkt->P, _LOTUS_LWE_DIM * _LOTUS_LWE_PT); /* P = R */
  
  submul(pkt->P, pkt->A, skt->S); /* P -= A * S, so P = R - AS. */
  redc(pkt->P, _LOTUS_LWE_DIM * _LOTUS_LWE_PT);
}


/**
 * @param[out] ct a ciphertext corresponding to sigma
 * @param[in] sigma a message to be encrypted
 * @param[in] pk a public key
 */
void lotus_cpa_pke_enc_packed(U8 *ct_packed, const U8 *sigma, const U8 *pk_packed){
  int i, j;
  ct_t c = {0};
  U16 e[_LOTUS_LWE_DIM], t[_LOTUS_LWE_DIM * 2];
  
  sample_discrete_gaussian(e, _LOTUS_LWE_DIM);
  sample_discrete_gaussian((U16*)&c, _LOTUS_LWE_DIM + _LOTUS_LWE_PT); /* c = [e2|e3] */

  /* addmul_concat(), interleaving unpacking with addmul */
  for(i = 0; i < _LOTUS_LWE_DIM; i += 2){
    for(j = 0; j < _LOTUS_LWE_DIM * 2; j += 128){
      unpack_128elems(t + j, pk_packed);
      pk_packed += 208;
    }
    for(j = 0; j < _LOTUS_LWE_DIM; ++j) c.c1[j] += e[i] * t[j];
    for(j = 0; j < _LOTUS_LWE_DIM; ++j) c.c1[j] += e[i + 1] * t[j + _LOTUS_LWE_DIM];
  }
  for(i = 0; i < _LOTUS_LWE_DIM; i += 2){
    unpack_128elems(t, pk_packed);
    pk_packed += 208;
    unpack_128elems(t + 128, pk_packed);
    pk_packed += 208;
    unpack_128elems(t + 256, pk_packed);
    pk_packed += 208;
    for(j = 0; j < _LOTUS_LWE_PT; ++j) c.c2[j] += e[i] * t[j];
    for(j = 0; j < _LOTUS_LWE_PT; ++j) c.c2[j] += e[i + 1] * t[j + _LOTUS_LWE_PT];
  }

  add_sigma(c.c2, sigma); /* c2 += sigma * \lceil q / 2 \rceil */

  redc((U16*)&c, _LOTUS_LWE_DIM + _LOTUS_LWE_PT); /* c = c mod q */

  pack_ct(ct_packed, (U16*)&c);

  OPENSSL_cleanse(e, _LOTUS_LWE_DIM * sizeof(U16)); /* cleanse e */
  OPENSSL_cleanse(t, _LOTUS_LWE_DIM * 2 * sizeof(U16)); /* cleanse t */
}

/**
 * @param[out] sigma a message corresponding to ct
 * @param[in] ct a ciphertext to be decrypted
 * @param[in] sk a secret key
 */
void lotus_cpa_pke_dec_packed(U8 *sigma, const U8 *ct_packed, const U8 *sk_packed){
  int i, j;
  ct_t c;
  U16 t[_LOTUS_LWE_PT], s[_LOTUS_LWE_PT * 2];

  unpack_ct((U16*)&c, ct_packed);
  for(i = 0; i < _LOTUS_LWE_PT; ++i) t[i] = c.c2[i]; /* t = c2 */

  /* addmul(), interleaving unpacking with addmul */
  for(i = 0; i < _LOTUS_LWE_DIM; i += 2){ /* t += c1 * S */
    unpack_128dg(s, sk_packed);
    sk_packed += 96;
    unpack_128dg(s + 128, sk_packed);
    sk_packed += 96;
    unpack_128dg(s + 256, sk_packed);
    sk_packed += 96;
    for(j = 0; j < _LOTUS_LWE_PT; ++j) t[j] += c.c1[i] * s[j];
    for(j = 0; j < _LOTUS_LWE_PT; ++j) t[j] += c.c1[i + 1] * s[j +_LOTUS_LWE_PT];
  }
  
  reconstruct(sigma, t); /* compute sigma from t */

  OPENSSL_cleanse(t, _LOTUS_LWE_PT * sizeof(U16)); /* cleanse t */
  OPENSSL_cleanse(s, _LOTUS_LWE_PT * sizeof(U16)); /* cleanse s */
}
