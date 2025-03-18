/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker, Shay Gueron, and Dusan Kostic,
 * AWS Cryptographic Algorithms Group.
 */

#include "decode.h"
#include "defs.h"
#include "gf2x.h"
#include "sampling.h"
#include "sha.h"

// m_t and seed_t have the same size and thus can be considered
// to be of the same type. However, for security reasons we distinguish
// these types, even on the costs of small extra complexity.
_INLINE_ void convert_seed_to_m_type(OUT m_t *m, IN const seed_t *seed)
{
  bike_static_assert(sizeof(*m) == sizeof(*seed), m_size_eq_seed_size);
  bike_memcpy(m->raw, seed->raw, sizeof(*m));
}

_INLINE_ void convert_m_to_seed_type(OUT seed_t *seed, IN const m_t *m)
{
  bike_static_assert(sizeof(*m) == sizeof(*seed), m_size_eq_seed_size);
  bike_memcpy(seed->raw, m->raw, sizeof(*seed));
}

// (e0, e1) = H(m)
_INLINE_ ret_t function_h(OUT pad_e_t *e, IN const m_t *m, IN const pk_t *pk)
{
  DEFER_CLEANUP(seed_t seed = {0}, seed_cleanup);
  // pk is unused parameter in this case so we do this to avoid
  // clang sanitizers complaining.
  (void)pk;

  convert_m_to_seed_type(&seed, m);

  return generate_error_vector(e, &seed);
}

// out = L(e)
_INLINE_ ret_t function_l(OUT m_t *out, IN const pad_e_t *e)
{
  DEFER_CLEANUP(sha_dgst_t dgst = {0}, sha_dgst_cleanup);
  DEFER_CLEANUP(e_t tmp, e_cleanup);

  // Take the padding away
  tmp.val[0] = e->val[0].val;
  tmp.val[1] = e->val[1].val;

  GUARD(sha(&dgst, sizeof(tmp), (uint8_t *)&tmp));

  // Truncate the SHA384 digest to a 256-bits m_t
  bike_static_assert(sizeof(dgst) >= sizeof(*out), dgst_size_lt_m_size);
  bike_memcpy(out->raw, dgst.u.raw, sizeof(*out));

  return SUCCESS;
}

// Generate the Shared Secret K(m, c0, c1)
_INLINE_ ret_t function_k(OUT ss_t *out, IN const m_t *m, IN const ct_t *ct)
{
  DEFER_CLEANUP(func_k_t tmp, func_k_cleanup);
  DEFER_CLEANUP(sha_dgst_t dgst = {0}, sha_dgst_cleanup);

  // Copy every element, padded to the nearest byte
  tmp.m  = *m;
  tmp.c0 = ct->c0;
  tmp.c1 = ct->c1;

  GUARD(sha(&dgst, sizeof(tmp), (uint8_t *)&tmp));

  // Truncate the SHA384 digest to a 256-bits value
  // to subsequently use it as a seed.
  bike_static_assert(sizeof(dgst) >= sizeof(*out), dgst_size_lt_out_size);
  bike_memcpy(out->raw, dgst.u.raw, sizeof(*out));

  return SUCCESS;
}

_INLINE_ ret_t encrypt(OUT ct_t *ct,
                       IN const pad_e_t *e,
                       IN const pk_t *pk,
                       IN const m_t *m)
{
  // Pad the public key and the ciphertext
  pad_r_t p_ct = {0};
  pad_r_t p_pk = {0};
  p_pk.val     = *pk;

  // Generate the ciphertext
  // ct = pk * e1 + e0
  gf2x_mod_mul(&p_ct, &e->val[1], &p_pk);
  gf2x_mod_add(&p_ct, &p_ct, &e->val[0]);

  ct->c0 = p_ct.val;

  // c1 = L(e0, e1)
  GUARD(function_l(&ct->c1, e));

  // m xor L(e0, e1)
  for(size_t i = 0; i < sizeof(*m); i++) {
    ct->c1.raw[i] ^= m->raw[i];
  }

  print("e0: ", (const uint64_t *)PE0_RAW(e), R_BITS);
  print("e1: ", (const uint64_t *)PE1_RAW(e), R_BITS);
  print("c0:  ", (uint64_t *)ct->c0.raw, R_BITS);
  print("c1:  ", (uint64_t *)ct->c1.raw, M_BITS);

  return SUCCESS;
}

_INLINE_ ret_t reencrypt(OUT m_t *m, IN const pad_e_t *e, IN const ct_t *l_ct)
{
  DEFER_CLEANUP(m_t tmp, m_cleanup);

  GUARD(function_l(&tmp, e));

  // m' = c1 ^ L(e')
  for(size_t i = 0; i < sizeof(*m); i++) {
    m->raw[i] = tmp.raw[i] ^ l_ct->c1.raw[i];
  }

  return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// The three APIs below (keypair, encapsulate, decapsulate) are defined by NIST:
////////////////////////////////////////////////////////////////////////////////
OQS_API int keypair(OUT unsigned char *pk, OUT unsigned char *sk)
{
  DEFER_CLEANUP(aligned_sk_t l_sk = {0}, sk_cleanup);

  // The secret key is (h0, h1),
  // and the public key h=(h0^-1 * h1).
  // Padded structures are used internally, and are required by the
  // decoder and the gf2x multiplication.
  DEFER_CLEANUP(pad_r_t h0 = {0}, pad_r_cleanup);
  DEFER_CLEANUP(pad_r_t h1 = {0}, pad_r_cleanup);
  DEFER_CLEANUP(pad_r_t h0inv = {0}, pad_r_cleanup);
  DEFER_CLEANUP(pad_r_t h = {0}, pad_r_cleanup);

  // The randomness of the key generation
  DEFER_CLEANUP(seeds_t seeds = {0}, seeds_cleanup);

  get_seeds(&seeds);
  GUARD(generate_secret_key(&h0, &h1,
                            l_sk.wlist[0].val, l_sk.wlist[1].val,
                            &seeds.seed[0]));

  // Generate sigma
  convert_seed_to_m_type(&l_sk.sigma, &seeds.seed[1]);

  // Calculate the public key
  gf2x_mod_inv(&h0inv, &h0);
  gf2x_mod_mul(&h, &h1, &h0inv);

  // Fill the secret key data structure with contents - cancel the padding
  l_sk.bin[0] = h0.val;
  l_sk.bin[1] = h1.val;
  l_sk.pk     = h.val;

  // Copy the data to the output buffers
  bike_memcpy(sk, &l_sk, sizeof(l_sk));
  bike_memcpy(pk, &l_sk.pk, sizeof(l_sk.pk));

  print("h:  ", (uint64_t *)&l_sk.pk, R_BITS);
  print("h0: ", (uint64_t *)&l_sk.bin[0], R_BITS);
  print("h1: ", (uint64_t *)&l_sk.bin[1], R_BITS);
  print("h0 wlist:", (uint64_t *)&l_sk.wlist[0], SIZEOF_BITS(compressed_idx_d_t));
  print("h1 wlist:", (uint64_t *)&l_sk.wlist[1], SIZEOF_BITS(compressed_idx_d_t));
  print("sigma: ", (uint64_t *)l_sk.sigma.raw, M_BITS);

  return SUCCESS;
}

// Encapsulate - pk is the public key,
//               ct is a key encapsulation message (ciphertext),
//               ss is the shared secret.
OQS_API int encaps(OUT unsigned char *     ct,
                   OUT unsigned char *     ss,
                   IN const unsigned char *pk)
{
  // Public values (they do not require cleanup on exit).
  pk_t l_pk;
  ct_t l_ct;

  DEFER_CLEANUP(m_t m, m_cleanup);
  DEFER_CLEANUP(ss_t l_ss, ss_cleanup);
  DEFER_CLEANUP(seeds_t seeds = {0}, seeds_cleanup);
  DEFER_CLEANUP(pad_e_t e, pad_e_cleanup);

  // Copy the data from the input buffer. This is required in order to avoid
  // alignment issues on non x86_64 processors.
  bike_memcpy(&l_pk, pk, sizeof(l_pk));

  get_seeds(&seeds);

  // e = H(m) = H(seed[0])
  convert_seed_to_m_type(&m, &seeds.seed[0]);
  GUARD(function_h(&e, &m, &l_pk));

  // Calculate the ciphertext
  GUARD(encrypt(&l_ct, &e, &l_pk, &m));

  // Generate the shared secret
  GUARD(function_k(&l_ss, &m, &l_ct));

  print("ss: ", (uint64_t *)l_ss.raw, SIZEOF_BITS(l_ss));

  // Copy the data to the output buffers
  bike_memcpy(ct, &l_ct, sizeof(l_ct));
  bike_memcpy(ss, &l_ss, sizeof(l_ss));

  return SUCCESS;
}

// Decapsulate - ct is a key encapsulation message (ciphertext),
//               sk is the private key,
//               ss is the shared secret
OQS_API int decaps(OUT unsigned char *     ss,
                   IN const unsigned char *ct,
                   IN const unsigned char *sk)
{
  // Public values, does not require a cleanup on exit
  ct_t l_ct;

  DEFER_CLEANUP(ss_t l_ss, ss_cleanup);
  DEFER_CLEANUP(aligned_sk_t l_sk, sk_cleanup);
  DEFER_CLEANUP(e_t e, e_cleanup);
  DEFER_CLEANUP(m_t m_prime, m_cleanup);
  DEFER_CLEANUP(pad_e_t e_tmp, pad_e_cleanup);
  DEFER_CLEANUP(pad_e_t e_prime = {0}, pad_e_cleanup);

  // Copy the data from the input buffers. This is required in order to avoid
  // alignment issues on non x86_64 processors.
  bike_memcpy(&l_ct, ct, sizeof(l_ct));
  bike_memcpy(&l_sk, sk, sizeof(l_sk));

  // Decode.
  decode(&e, &l_ct, &l_sk);

  // Copy the error vector in the padded struct.
  e_prime.val[0].val = e.val[0];
  e_prime.val[1].val = e.val[1];

  GUARD(reencrypt(&m_prime, &e_prime, &l_ct));

  // Check if H(m') is equal to (e0', e1')
  // (in constant-time)
  GUARD(function_h(&e_tmp, &m_prime, &l_sk.pk));
  uint32_t success_cond;
  success_cond = secure_cmp(PE0_RAW(&e_prime), PE0_RAW(&e_tmp), R_BYTES);
  success_cond &= secure_cmp(PE1_RAW(&e_prime), PE1_RAW(&e_tmp), R_BYTES);

  // Compute either K(m', C) or K(sigma, C) based on the success condition
  uint32_t mask = secure_l32_mask(0, success_cond);
  for(size_t i = 0; i < M_BYTES; i++) {
    m_prime.raw[i] &= u8_barrier(~mask);
    m_prime.raw[i] |= (u8_barrier(mask) & l_sk.sigma.raw[i]);
  }

  // Generate the shared secret
  GUARD(function_k(&l_ss, &m_prime, &l_ct));

  // Copy the data into the output buffer
  bike_memcpy(ss, &l_ss, sizeof(l_ss));

  return SUCCESS;
}

OQS_API int keypair_derand(OUT unsigned char *     pk,
                           OUT unsigned char *     sk,
                           IN const unsigned char *coins)
{
  (void)pk;
  (void)sk;
  (void)coins;
  return OQS_ERROR;
}

OQS_API int encaps_derand(OUT unsigned char *     ct,
                          OUT unsigned char *     ss,
                          IN const unsigned char *pk,
                          IN const unsigned char *coins)
{
  (void)ct;
  (void)ss;
  (void)pk;
  (void)coins;
  return OQS_ERROR;
}
