/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include "indcpa.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "fips202.h"
#include "fips202x4.h"
#include "indcpa.h"
#include "ntt.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include "rej_uniform.h"
#include "symmetric.h"

#include "arith_backend.h"
#include "debug/debug.h"

#include "cbmc.h"

/* Static namespacing
 * This is to facilitate building multiple instances
 * of mlkem-native (e.g. with varying security levels)
 * within a single compilation unit. */
#define pack_pk MLKEM_NAMESPACE(pack_pk)
#define unpack_pk MLKEM_NAMESPACE(unpack_pk)
#define pack_sk MLKEM_NAMESPACE(pack_sk)
#define unpack_sk MLKEM_NAMESPACE(unpack_sk)
#define pack_ciphertext MLKEM_NAMESPACE(pack_ciphertext)
#define unpack_ciphertext MLKEM_NAMESPACE(unpack_ciphertext)
#define gen_matrix_entry_x4 MLKEM_NAMESPACE(gen_matrix_entry_x4)
#define gen_matrix_entry MLKEM_NAMESPACE(gen_matrix_entry)
#define matvec_mul MLKEM_NAMESPACE(matvec_mul)
/* End of static namespacing */

/*************************************************
 * Name:        pack_pk
 *
 * Description: Serialize the public key as concatenation of the
 *              serialized vector of polynomials pk
 *              and the public seed used to generate the matrix A.
 *
 * Arguments:   uint8_t *r: pointer to the output serialized public key
 *              polyvec *pk: pointer to the input public-key polyvec.
 *                Must have coefficients within [0,..,q-1].
 *              const uint8_t *seed: pointer to the input public seed
 **************************************************/
static void pack_pk(uint8_t r[MLKEM_INDCPA_PUBLICKEYBYTES], polyvec *pk,
                    const uint8_t seed[MLKEM_SYMBYTES])
{
  POLYVEC_BOUND(pk, MLKEM_Q);
  polyvec_tobytes(r, pk);
  memcpy(r + MLKEM_POLYVECBYTES, seed, MLKEM_SYMBYTES);
}

/*************************************************
 * Name:        unpack_pk
 *
 * Description: De-serialize public key from a byte array;
 *              approximate inverse of pack_pk
 *
 * Arguments:   - polyvec *pk: pointer to output public-key polynomial vector
 *                  Coefficients will be normalized to [0,..,q-1].
 *              - uint8_t *seed: pointer to output seed to generate matrix A
 *              - const uint8_t *packedpk: pointer to input serialized public
 *                  key.
 **************************************************/
static void unpack_pk(polyvec *pk, uint8_t seed[MLKEM_SYMBYTES],
                      const uint8_t packedpk[MLKEM_INDCPA_PUBLICKEYBYTES])
{
  polyvec_frombytes(pk, packedpk);
  memcpy(seed, packedpk + MLKEM_POLYVECBYTES, MLKEM_SYMBYTES);

  /* NOTE: If a modulus check was conducted on the PK, we know at this
   * point that the coefficients of `pk` are unsigned canonical. The
   * specifications and proofs, however, do _not_ assume this, and instead
   * work with the easily provable bound by 4096. */
}

/*************************************************
 * Name:        pack_sk
 *
 * Description: Serialize the secret key
 *
 * Arguments:   - uint8_t *r: pointer to output serialized secret key
 *              - polyvec *sk: pointer to input vector of polynomials (secret
 *key)
 **************************************************/
static void pack_sk(uint8_t r[MLKEM_INDCPA_SECRETKEYBYTES], polyvec *sk)
{
  POLYVEC_BOUND(sk, MLKEM_Q);
  polyvec_tobytes(r, sk);
}

/*************************************************
 * Name:        unpack_sk
 *
 * Description: De-serialize the secret key; inverse of pack_sk
 *
 * Arguments:   - polyvec *sk: pointer to output vector of polynomials (secret
 *                key)
 *              - const uint8_t *packedsk: pointer to input serialized secret
 *                key
 **************************************************/
static void unpack_sk(polyvec *sk,
                      const uint8_t packedsk[MLKEM_INDCPA_SECRETKEYBYTES])
{
  polyvec_frombytes(sk, packedsk);
}

/*************************************************
 * Name:        pack_ciphertext
 *
 * Description: Serialize the ciphertext as concatenation of the
 *              compressed and serialized vector of polynomials b
 *              and the compressed and serialized polynomial v
 *
 * Arguments:   uint8_t *r: pointer to the output serialized ciphertext
 *              poly *pk: pointer to the input vector of polynomials b
 *              poly *v: pointer to the input polynomial v
 **************************************************/
static void pack_ciphertext(uint8_t r[MLKEM_INDCPA_BYTES], polyvec *b, poly *v)
{
  polyvec_compress_du(r, b);
  poly_compress_dv(r + MLKEM_POLYVECCOMPRESSEDBYTES_DU, v);
}

/*************************************************
 * Name:        unpack_ciphertext
 *
 * Description: De-serialize and decompress ciphertext from a byte array;
 *              approximate inverse of pack_ciphertext
 *
 * Arguments:   - polyvec *b: pointer to the output vector of polynomials b
 *              - poly *v: pointer to the output polynomial v
 *              - const uint8_t *c: pointer to the input serialized ciphertext
 **************************************************/
static void unpack_ciphertext(polyvec *b, poly *v,
                              const uint8_t c[MLKEM_INDCPA_BYTES])
{
  polyvec_decompress_du(b, c);
  poly_decompress_dv(v, c + MLKEM_POLYVECCOMPRESSEDBYTES_DU);
}

#ifndef MLKEM_GEN_MATRIX_NBLOCKS
#define MLKEM_GEN_MATRIX_NBLOCKS \
  ((12 * MLKEM_N / 8 * (1 << 12) / MLKEM_Q + XOF_RATE) / XOF_RATE)
#endif

/*
 * Generate four A matrix entries from a seed, using rejection
 * sampling on the output of a XOF.
 */
static void gen_matrix_entry_x4(poly *vec, uint8_t *seed[4])
__contract__(
  requires(memory_no_alias(vec, sizeof(poly) * 4))
  requires(memory_no_alias(seed, sizeof(uint8_t*) * 4))
  requires(memory_no_alias(seed[0], MLKEM_SYMBYTES + 2))
  requires(memory_no_alias(seed[1], MLKEM_SYMBYTES + 2))
  requires(memory_no_alias(seed[2], MLKEM_SYMBYTES + 2))
  requires(memory_no_alias(seed[3], MLKEM_SYMBYTES + 2))
  assigns(memory_slice(vec, sizeof(poly) * 4))
  ensures(array_bound(vec[0].coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  ensures(array_bound(vec[1].coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  ensures(array_bound(vec[2].coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  ensures(array_bound(vec[3].coeffs, 0, MLKEM_N, 0, MLKEM_Q)))
{
  /* Temporary buffers for XOF output before rejection sampling */
  uint8_t buf0[MLKEM_GEN_MATRIX_NBLOCKS * XOF_RATE];
  uint8_t buf1[MLKEM_GEN_MATRIX_NBLOCKS * XOF_RATE];
  uint8_t buf2[MLKEM_GEN_MATRIX_NBLOCKS * XOF_RATE];
  uint8_t buf3[MLKEM_GEN_MATRIX_NBLOCKS * XOF_RATE];

  /* Tracks the number of coefficients we have already sampled */
  unsigned int ctr[KECCAK_WAY];
  xof_x4_ctx statex;
  unsigned int buflen;

  shake128x4_inc_init(&statex);

  /* seed is MLKEM_SYMBYTES + 2 bytes long, but padded to MLKEM_SYMBYTES + 16 */
  xof_x4_absorb(&statex, seed[0], seed[1], seed[2], seed[3],
                MLKEM_SYMBYTES + 2);

  /*
   * Initially, squeeze heuristic number of MLKEM_GEN_MATRIX_NBLOCKS.
   * This should generate the matrix entries with high probability.
   */
  xof_x4_squeezeblocks(buf0, buf1, buf2, buf3, MLKEM_GEN_MATRIX_NBLOCKS,
                       &statex);
  buflen = MLKEM_GEN_MATRIX_NBLOCKS * XOF_RATE;
  ctr[0] = rej_uniform(vec[0].coeffs, MLKEM_N, 0, buf0, buflen);
  ctr[1] = rej_uniform(vec[1].coeffs, MLKEM_N, 0, buf1, buflen);
  ctr[2] = rej_uniform(vec[2].coeffs, MLKEM_N, 0, buf2, buflen);
  ctr[3] = rej_uniform(vec[3].coeffs, MLKEM_N, 0, buf3, buflen);

  /*
   * So long as not all matrix entries have been generated, squeeze
   * one more block a time until we're done.
   */
  buflen = XOF_RATE;
  while (ctr[0] < MLKEM_N || ctr[1] < MLKEM_N || ctr[2] < MLKEM_N ||
         ctr[3] < MLKEM_N)
  __loop__(
    assigns(ctr, statex, memory_slice(vec, sizeof(poly) * 4), object_whole(buf0),
       object_whole(buf1), object_whole(buf2), object_whole(buf3))
    invariant(ctr[0] <= MLKEM_N && ctr[1] <= MLKEM_N)
    invariant(ctr[2] <= MLKEM_N && ctr[3] <= MLKEM_N)
    invariant(ctr[0] > 0 ==> array_bound(vec[0].coeffs, 0, ctr[0], 0, MLKEM_Q))
    invariant(ctr[1] > 0 ==> array_bound(vec[1].coeffs, 0, ctr[1], 0, MLKEM_Q))
    invariant(ctr[2] > 0 ==> array_bound(vec[2].coeffs, 0, ctr[2], 0, MLKEM_Q))
    invariant(ctr[3] > 0 ==> array_bound(vec[3].coeffs, 0, ctr[3], 0, MLKEM_Q)))
  {
    xof_x4_squeezeblocks(buf0, buf1, buf2, buf3, 1, &statex);
    ctr[0] = rej_uniform(vec[0].coeffs, MLKEM_N, ctr[0], buf0, buflen);
    ctr[1] = rej_uniform(vec[1].coeffs, MLKEM_N, ctr[1], buf1, buflen);
    ctr[2] = rej_uniform(vec[2].coeffs, MLKEM_N, ctr[2], buf2, buflen);
    ctr[3] = rej_uniform(vec[3].coeffs, MLKEM_N, ctr[3], buf3, buflen);
  }

  xof_x4_release(&statex);
}

/*
 * Generate a single A matrix entry from a seed, using rejection
 * sampling on the output of a XOF.
 */
static void gen_matrix_entry(poly *entry, uint8_t seed[MLKEM_SYMBYTES + 2])
__contract__(
  requires(memory_no_alias(entry, sizeof(poly)))
  requires(memory_no_alias(seed, MLKEM_SYMBYTES + 2))
  assigns(memory_slice(entry, sizeof(poly)))
  ensures(array_bound(entry->coeffs, 0, MLKEM_N, 0, MLKEM_Q)))
{
  xof_ctx state;
  uint8_t buf[MLKEM_GEN_MATRIX_NBLOCKS * XOF_RATE];
  unsigned int ctr, buflen;

  shake128_inc_init(&state);
  xof_absorb(&state, seed, MLKEM_SYMBYTES + 2);

  /* Initially, squeeze + sample heuristic number of MLKEM_GEN_MATRIX_NBLOCKS.
   */
  /* This should generate the matrix entry with high probability. */
  xof_squeezeblocks(buf, MLKEM_GEN_MATRIX_NBLOCKS, &state);
  buflen = MLKEM_GEN_MATRIX_NBLOCKS * XOF_RATE;
  ctr = rej_uniform(entry->coeffs, MLKEM_N, 0, buf, buflen);

  /* Squeeze + sample one more block a time until we're done */
  buflen = XOF_RATE;
  while (ctr < MLKEM_N)
  __loop__(
    assigns(ctr, state, memory_slice(entry, sizeof(poly)), object_whole(buf))
    invariant(0 <= ctr && ctr <= MLKEM_N)
    invariant(ctr > 0 ==> array_bound(entry->coeffs, 0, ctr,
                                          0, MLKEM_Q)))
  {
    xof_squeezeblocks(buf, 1, &state);
    ctr = rej_uniform(entry->coeffs, MLKEM_N, ctr, buf, buflen);
  }

  xof_release(&state);
}

#if !defined(MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER)
/* This namespacing is not done at the top to avoid a naming conflict
 * with native backends, which are currently not yet namespaced. */
#define poly_permute_bitrev_to_custom \
  MLKEM_NAMESPACE(poly_permute_bitrev_to_custom)

static INLINE void poly_permute_bitrev_to_custom(poly *data)
__contract__(
  /* We don't specify that this should be a permutation, but only
   * that it does not change the bound established at the end of gen_matrix. */
  requires(memory_no_alias(data, sizeof(poly)))
  requires(array_bound(data->coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  assigns(memory_slice(data, sizeof(poly)))
  ensures(array_bound(data->coeffs, 0, MLKEM_N, 0, MLKEM_Q))) { ((void)data); }
#endif /* MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER */

/* Not static for benchmarking */
MLKEM_NATIVE_INTERNAL_API
void gen_matrix(polyvec *a, const uint8_t seed[MLKEM_SYMBYTES], int transposed)
{
  unsigned i, j;
  /*
   * We generate four separate seed arrays rather than a single one to work
   * around limitations in CBMC function contracts dealing with disjoint slices
   * of the same parent object.
   */

  ALIGN uint8_t seed0[MLKEM_SYMBYTES + 2];
  ALIGN uint8_t seed1[MLKEM_SYMBYTES + 2];
  ALIGN uint8_t seed2[MLKEM_SYMBYTES + 2];
  ALIGN uint8_t seed3[MLKEM_SYMBYTES + 2];
  uint8_t *seedxy[4];
  seedxy[0] = seed0;
  seedxy[1] = seed1;
  seedxy[2] = seed2;
  seedxy[3] = seed3;

  for (j = 0; j < KECCAK_WAY; j++)
  {
    memcpy(seedxy[j], seed, MLKEM_SYMBYTES);
  }

  for (i = 0; i < (MLKEM_K * MLKEM_K / KECCAK_WAY) * KECCAK_WAY;
       i += KECCAK_WAY)
  {
    uint8_t x, y;

    for (j = 0; j < KECCAK_WAY; j++)
    {
      x = (i + j) / MLKEM_K;
      y = (i + j) % MLKEM_K;
      if (transposed)
      {
        seedxy[j][MLKEM_SYMBYTES + 0] = x;
        seedxy[j][MLKEM_SYMBYTES + 1] = y;
      }
      else
      {
        seedxy[j][MLKEM_SYMBYTES + 0] = y;
        seedxy[j][MLKEM_SYMBYTES + 1] = x;
      }
    }

    /*
     * This call writes across polyvec boundaries for K=2 and K=3.
     * This is intentional and safe.
     */
    gen_matrix_entry_x4(&a[0].vec[0] + i, seedxy);
  }

  /* For left over polynomial, we use single keccak. */
  if (i < MLKEM_K * MLKEM_K)
  {
    uint8_t x, y;
    x = i / MLKEM_K;
    y = i % MLKEM_K;

    if (transposed)
    {
      seed0[MLKEM_SYMBYTES + 0] = x;
      seed0[MLKEM_SYMBYTES + 1] = y;
    }
    else
    {
      seed0[MLKEM_SYMBYTES + 0] = y;
      seed0[MLKEM_SYMBYTES + 1] = x;
    }

    gen_matrix_entry(&a[0].vec[0] + i, seed0);
    i++;
  }

  cassert(i == MLKEM_K * MLKEM_K,
          "gen_matrix: failed to generate whole matrix");

  /*
   * The public matrix is generated in NTT domain. If the native backend
   * uses a custom order in NTT domain, permute A accordingly.
   */
  for (i = 0; i < MLKEM_K; i++)
  {
    for (j = 0; j < MLKEM_K; j++)
    {
      poly_permute_bitrev_to_custom(&a[i].vec[j]);
    }
  }
}

/*************************************************
 * Name:        matvec_mul
 *
 * Description: Computes matrix-vector product in NTT domain,
 *              via Montgomery multiplication.
 *
 * Arguments:   - polyvec *out: Pointer to output polynomial vector
 *              - polyvec a[MLKEM_K]: Input matrix. Must be in NTT domain
 *                  and have coefficients of absolute value < 4096.
 *              - polyvec *v: Input polynomial vector. Must be in NTT domain.
 *              - polyvec *vc: Mulcache for v, computed via
 *                  polyvec_mulcache_compute().
 **************************************************/
static void matvec_mul(polyvec *out, const polyvec a[MLKEM_K], const polyvec *v,
                       const polyvec_mulcache *vc)
__contract__(
  requires(memory_no_alias(out, sizeof(polyvec)))
  requires(memory_no_alias(a, sizeof(polyvec) * MLKEM_K))
  requires(memory_no_alias(v, sizeof(polyvec)))
  requires(memory_no_alias(vc, sizeof(polyvec_mulcache)))
  requires(forall(k0, 0, MLKEM_K,
    forall(k1, 0, MLKEM_K,
      array_bound(a[k0].vec[k1].coeffs, 0, MLKEM_N, 0, UINT12_LIMIT))))
  assigns(object_whole(out)))
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  __loop__(
    assigns(i, object_whole(out))
    invariant(i >= 0 && i <= MLKEM_K))
  {
    polyvec_basemul_acc_montgomery_cached(&out->vec[i], &a[i], v, vc);
  }
}



STATIC_ASSERT(NTT_BOUND + MLKEM_Q < INT16_MAX, indcpa_enc_bound_0)

MLKEM_NATIVE_INTERNAL_API
void indcpa_keypair_derand(uint8_t pk[MLKEM_INDCPA_PUBLICKEYBYTES],
                           uint8_t sk[MLKEM_INDCPA_SECRETKEYBYTES],
                           const uint8_t coins[MLKEM_SYMBYTES])
{
  ALIGN uint8_t buf[2 * MLKEM_SYMBYTES];
  const uint8_t *publicseed = buf;
  const uint8_t *noiseseed = buf + MLKEM_SYMBYTES;
  polyvec a[MLKEM_K], e, pkpv, skpv;
  polyvec_mulcache skpv_cache;

  ALIGN uint8_t coins_with_domain_separator[MLKEM_SYMBYTES + 1];
  /* Concatenate coins with MLKEM_K for domain separation of security levels */
  memcpy(coins_with_domain_separator, coins, MLKEM_SYMBYTES);
  coins_with_domain_separator[MLKEM_SYMBYTES] = MLKEM_K;

  hash_g(buf, coins_with_domain_separator, MLKEM_SYMBYTES + 1);

  gen_matrix(a, publicseed, 0 /* no transpose */);

#if MLKEM_K == 2
  poly_getnoise_eta1_4x(skpv.vec + 0, skpv.vec + 1, e.vec + 0, e.vec + 1,
                        noiseseed, 0, 1, 2, 3);
#elif MLKEM_K == 3
  /*
   * Only the first three output buffers are needed.
   * The laster parameter is a dummy that's overwritten later.
   */
  poly_getnoise_eta1_4x(skpv.vec + 0, skpv.vec + 1, skpv.vec + 2,
                        pkpv.vec + 0 /* irrelevant */, noiseseed, 0, 1, 2,
                        0xFF /* irrelevant */);
  /* Same here */
  poly_getnoise_eta1_4x(e.vec + 0, e.vec + 1, e.vec + 2,
                        pkpv.vec + 0 /* irrelevant */, noiseseed, 3, 4, 5,
                        0xFF /* irrelevant */);
#elif MLKEM_K == 4
  poly_getnoise_eta1_4x(skpv.vec + 0, skpv.vec + 1, skpv.vec + 2, skpv.vec + 3,
                        noiseseed, 0, 1, 2, 3);
  poly_getnoise_eta1_4x(e.vec + 0, e.vec + 1, e.vec + 2, e.vec + 3, noiseseed,
                        4, 5, 6, 7);
#endif

  polyvec_ntt(&skpv);
  polyvec_ntt(&e);

  polyvec_mulcache_compute(&skpv_cache, &skpv);
  matvec_mul(&pkpv, a, &skpv, &skpv_cache);
  polyvec_tomont(&pkpv);

  /* Arithmetic cannot overflow, see static assertion at the top */
  polyvec_add(&pkpv, &e);
  polyvec_reduce(&pkpv);
  polyvec_reduce(&skpv);

  pack_sk(sk, &skpv);
  pack_pk(pk, &pkpv, publicseed);
}


/* Check that the arithmetic in indcpa_enc() does not overflow */
STATIC_ASSERT(INVNTT_BOUND + MLKEM_ETA1 < INT16_MAX, indcpa_enc_bound_0)
STATIC_ASSERT(INVNTT_BOUND + MLKEM_ETA2 + MLKEM_Q < INT16_MAX,
              indcpa_enc_bound_1)

MLKEM_NATIVE_INTERNAL_API
void indcpa_enc(uint8_t c[MLKEM_INDCPA_BYTES],
                const uint8_t m[MLKEM_INDCPA_MSGBYTES],
                const uint8_t pk[MLKEM_INDCPA_PUBLICKEYBYTES],
                const uint8_t coins[MLKEM_SYMBYTES])
{
  ALIGN uint8_t seed[MLKEM_SYMBYTES];
  polyvec sp, pkpv, ep, at[MLKEM_K], b;
  poly v, k, epp;
  polyvec_mulcache sp_cache;

  unpack_pk(&pkpv, seed, pk);
  poly_frommsg(&k, m);
  gen_matrix(at, seed, 1 /* transpose */);

#if MLKEM_K == 2
  poly_getnoise_eta1122_4x(sp.vec + 0, sp.vec + 1, ep.vec + 0, ep.vec + 1,
                           coins, 0, 1, 2, 3);
  poly_getnoise_eta2(&epp, coins, 4);
#elif MLKEM_K == 3
  /*
   * In this call, only the first three output buffers are needed.
   * The last parameter is a dummy that's overwritten later.
   */
  poly_getnoise_eta1_4x(sp.vec + 0, sp.vec + 1, sp.vec + 2, &b.vec[0], coins, 0,
                        1, 2, 0xFF);
  /* The fourth output buffer in this call _is_ used. */
  poly_getnoise_eta2_4x(ep.vec + 0, ep.vec + 1, ep.vec + 2, &epp, coins, 3, 4,
                        5, 6);
#elif MLKEM_K == 4
  poly_getnoise_eta1_4x(sp.vec + 0, sp.vec + 1, sp.vec + 2, sp.vec + 3, coins,
                        0, 1, 2, 3);
  poly_getnoise_eta2_4x(ep.vec + 0, ep.vec + 1, ep.vec + 2, ep.vec + 3, coins,
                        4, 5, 6, 7);
  poly_getnoise_eta2(&epp, coins, 8);
#endif

  polyvec_ntt(&sp);

  polyvec_mulcache_compute(&sp_cache, &sp);
  matvec_mul(&b, at, &sp, &sp_cache);
  polyvec_basemul_acc_montgomery_cached(&v, &pkpv, &sp, &sp_cache);

  polyvec_invntt_tomont(&b);
  poly_invntt_tomont(&v);

  /* Arithmetic cannot overflow, see static assertion at the top */
  polyvec_add(&b, &ep);
  poly_add(&v, &epp);
  poly_add(&v, &k);

  polyvec_reduce(&b);
  poly_reduce(&v);

  pack_ciphertext(c, &b, &v);
}

/* Check that the arithmetic in indcpa_dec() does not overflow */
STATIC_ASSERT(INVNTT_BOUND + MLKEM_Q < INT16_MAX, indcpa_dec_bound_0)

MLKEM_NATIVE_INTERNAL_API
void indcpa_dec(uint8_t m[MLKEM_INDCPA_MSGBYTES],
                const uint8_t c[MLKEM_INDCPA_BYTES],
                const uint8_t sk[MLKEM_INDCPA_SECRETKEYBYTES])
{
  polyvec b, skpv;
  poly v, sb;

  unpack_ciphertext(&b, &v, c);
  unpack_sk(&skpv, sk);

  polyvec_ntt(&b);
  polyvec_basemul_acc_montgomery(&sb, &skpv, &b);
  poly_invntt_tomont(&sb);

  /* Arithmetic cannot overflow, see static assertion at the top */
  poly_sub(&v, &sb);
  poly_reduce(&v);

  poly_tomsg(m, &v);
}
