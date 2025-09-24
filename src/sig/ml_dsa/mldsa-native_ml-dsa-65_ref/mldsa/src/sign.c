/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [FIPS140_3_IG]
 *   Implementation Guidance for FIPS 140-3 and the Cryptographic Module
 *   Validation Program
 *   National Institute of Standards and Technology
 *   https://csrc.nist.gov/projects/cryptographic-module-validation-program/fips-140-3-ig-announcements
 *
 * - [FIPS204]
 *   FIPS 204 Module-Lattice-Based Digital Signature Standard
 *   National Institute of Standards and Technology
 *   https://csrc.nist.gov/pubs/fips/204/final
 *
 * - [Round3_Spec]
 *   CRYSTALS-Dilithium Algorithm Specifications and Supporting Documentation
 *   (Version 3.1)
 *   Bai, Ducas, Kiltz, Lepoint, Lyubashevsky, Schwabe, Seiler, Stehlé
 *   https://pq-crystals.org/dilithium/data/dilithium-specification-round3-20210208.pdf
 */

#include <stdint.h>
#include <string.h>

#include "cbmc.h"
#include "debug.h"
#include "packing.h"
#include "poly.h"
#include "poly_kl.h"
#include "polyvec.h"
#include "prehash.h"
#include "randombytes.h"
#include "sign.h"
#include "symmetric.h"

/* Parameter set namespacing
 * This is to facilitate building multiple instances
 * of mldsa-native (e.g. with varying parameter sets)
 * within a single compilation unit. */
#define mld_check_pct MLD_ADD_PARAM_SET(mld_check_pct)
#define mld_sample_s1_s2 MLD_ADD_PARAM_SET(mld_sample_s1_s2)
#define mld_H MLD_ADD_PARAM_SET(mld_H)
#define mld_attempt_signature_generation \
  MLD_ADD_PARAM_SET(mld_attempt_signature_generation)
/* End of parameter set namespacing */


static int mld_check_pct(uint8_t const pk[CRYPTO_PUBLICKEYBYTES],
                         uint8_t const sk[CRYPTO_SECRETKEYBYTES])
__contract__(
  requires(memory_no_alias(pk, CRYPTO_PUBLICKEYBYTES))
  requires(memory_no_alias(sk, CRYPTO_SECRETKEYBYTES))
  ensures(return_value == 0 || return_value == -1)
);

#if defined(MLD_CONFIG_KEYGEN_PCT)
/*************************************************
 * @[FIPS140_3_IG]
 * (https://csrc.nist.gov/csrc/media/Projects/cryptographic-module-validation-program/documents/fips%20140-3/FIPS%20140-3%20IG.pdf)
 *
 * TE10.35.02: Pair-wise Consistency Test (PCT) for DSA keypairs
 *
 * Purpose: Validates that a generated public/private key pair can correctly
 * sign and verify data. Test performs signature generation using the private
 * key (sk), followed by signature verification using the public key (pk).
 * Returns 0 if the signature was successfully verified, non-zero if it cannot.
 *
 * Note: @[FIPS204] requires that public/private key pairs are to be used only
 * for the calculation and/of verification of digital signatures.
 **************************************************/
static int mld_check_pct(uint8_t const pk[CRYPTO_PUBLICKEYBYTES],
                         uint8_t const sk[CRYPTO_SECRETKEYBYTES])
{
  MLD_ALIGN uint8_t message[1] = {0};
  MLD_ALIGN uint8_t signature[CRYPTO_BYTES];
  MLD_ALIGN uint8_t pk_test[CRYPTO_PUBLICKEYBYTES];
  size_t siglen;
  int ret;

  /* Copy public key for testing */
  mld_memcpy(pk_test, pk, CRYPTO_PUBLICKEYBYTES);

  /* Sign a test message using the original secret key */
  ret = crypto_sign_signature(signature, &siglen, message, sizeof(message),
                              NULL, 0, sk);
  if (ret == 0)
  {
#if defined(MLD_CONFIG_KEYGEN_PCT_BREAKAGE_TEST)
    /* Deliberately break public key for testing purposes */
    if (mld_break_pct())
    {
      pk_test[0] = ~pk_test[0];
    }
#endif /* MLD_CONFIG_KEYGEN_PCT_BREAKAGE_TEST */

    /* Verify the signature using the (potentially corrupted) public key */
    ret = crypto_sign_verify(signature, siglen, message, sizeof(message), NULL,
                             0, pk_test);
  }

  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(signature, sizeof(signature));
  mld_zeroize(pk_test, sizeof(pk_test));

  return ret;
}
#else  /* MLD_CONFIG_KEYGEN_PCT */
static int mld_check_pct(uint8_t const pk[CRYPTO_PUBLICKEYBYTES],
                         uint8_t const sk[CRYPTO_SECRETKEYBYTES])
{
  /* Skip PCT */
  ((void)pk);
  ((void)sk);
  return 0;
}
#endif /* !MLD_CONFIG_KEYGEN_PCT */

static void mld_sample_s1_s2(mld_polyvecl *s1, mld_polyveck *s2,
                             const uint8_t seed[MLDSA_CRHBYTES])
__contract__(
  requires(memory_no_alias(s1, sizeof(mld_polyvecl)))
  requires(memory_no_alias(s2, sizeof(mld_polyveck)))
  requires(memory_no_alias(seed, MLDSA_CRHBYTES))
  assigns(object_whole(s1), object_whole(s2))
  ensures(forall(l0, 0, MLDSA_L, array_abs_bound(s1->vec[l0].coeffs, 0, MLDSA_N, MLDSA_ETA + 1)))
  ensures(forall(k0, 0, MLDSA_K, array_abs_bound(s2->vec[k0].coeffs, 0, MLDSA_N, MLDSA_ETA + 1)))
)
{
/* Sample short vectors s1 and s2 */
#if defined(MLD_CONFIG_SERIAL_FIPS202_ONLY)
  int i;
  uint16_t nonce = 0;
  /* Safety: The nonces are at most 14 (MLDSA_L + MLDSA_K - 1), and, hence, the
   * casts are safe. */
  for (i = 0; i < MLDSA_L; i++)
  {
    mld_poly_uniform_eta(&s1->vec[i], seed, (uint8_t)(nonce + i));
  }
  for (i = 0; i < MLDSA_K; i++)
  {
    mld_poly_uniform_eta(&s2->vec[i], seed, (uint8_t)(nonce + MLDSA_L + i));
  }
#else /* MLD_CONFIG_SERIAL_FIPS202_ONLY */
#if MLD_CONFIG_PARAMETER_SET == 44
  mld_poly_uniform_eta_4x(&s1->vec[0], &s1->vec[1], &s1->vec[2], &s1->vec[3],
                          seed, 0, 1, 2, 3);
  mld_poly_uniform_eta_4x(&s2->vec[0], &s2->vec[1], &s2->vec[2], &s2->vec[3],
                          seed, 4, 5, 6, 7);
#elif MLD_CONFIG_PARAMETER_SET == 65
  mld_poly_uniform_eta_4x(&s1->vec[0], &s1->vec[1], &s1->vec[2], &s1->vec[3],
                          seed, 0, 1, 2, 3);
  mld_poly_uniform_eta_4x(&s1->vec[4], &s2->vec[0], &s2->vec[1],
                          &s2->vec[2] /* irrelevant */, seed, 4, 5, 6,
                          0xFF /* irrelevant */);
  mld_poly_uniform_eta_4x(&s2->vec[2], &s2->vec[3], &s2->vec[4], &s2->vec[5],
                          seed, 7, 8, 9, 10);
#elif MLD_CONFIG_PARAMETER_SET == 87
  mld_poly_uniform_eta_4x(&s1->vec[0], &s1->vec[1], &s1->vec[2], &s1->vec[3],
                          seed, 0, 1, 2, 3);
  mld_poly_uniform_eta_4x(&s1->vec[4], &s1->vec[5], &s1->vec[6],
                          &s2->vec[0] /* irrelevant */, seed, 4, 5, 6,
                          0xFF /* irrelevant */);
  mld_poly_uniform_eta_4x(&s2->vec[0], &s2->vec[1], &s2->vec[2], &s2->vec[3],
                          seed, 7, 8, 9, 10);
  mld_poly_uniform_eta_4x(&s2->vec[4], &s2->vec[5], &s2->vec[6], &s2->vec[7],
                          seed, 11, 12, 13, 14);
#endif /* MLD_CONFIG_PARAMETER_SET == 87 */
#endif /* !MLD_CONFIG_SERIAL_FIPS202_ONLY */
}

MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_keypair_internal(uint8_t pk[CRYPTO_PUBLICKEYBYTES],
                                 uint8_t sk[CRYPTO_SECRETKEYBYTES],
                                 const uint8_t seed[MLDSA_SEEDBYTES])
{
  MLD_ALIGN uint8_t seedbuf[2 * MLDSA_SEEDBYTES + MLDSA_CRHBYTES];
  MLD_ALIGN uint8_t inbuf[MLDSA_SEEDBYTES + 2];
  MLD_ALIGN uint8_t tr[MLDSA_TRBYTES];
  const uint8_t *rho, *rhoprime, *key;
  mld_polyvecl mat[MLDSA_K];
  mld_polyvecl s1, s1hat;
  mld_polyveck s2, t2, t1, t0;

  /* Get randomness for rho, rhoprime and key */
  mld_memcpy(inbuf, seed, MLDSA_SEEDBYTES);
  inbuf[MLDSA_SEEDBYTES + 0] = MLDSA_K;
  inbuf[MLDSA_SEEDBYTES + 1] = MLDSA_L;
  mld_shake256(seedbuf, 2 * MLDSA_SEEDBYTES + MLDSA_CRHBYTES, inbuf,
               MLDSA_SEEDBYTES + 2);
  rho = seedbuf;
  rhoprime = rho + MLDSA_SEEDBYTES;
  key = rhoprime + MLDSA_CRHBYTES;

  /* Constant time: rho is part of the public key and, hence, public. */
  MLD_CT_TESTING_DECLASSIFY(rho, MLDSA_SEEDBYTES);
  /* Expand matrix */
  mld_polyvec_matrix_expand(mat, rho);
  mld_sample_s1_s2(&s1, &s2, rhoprime);

  /* Matrix-vector multiplication */
  s1hat = s1;
  mld_polyvecl_ntt(&s1hat);
  mld_polyvec_matrix_pointwise_montgomery(&t1, mat, &s1hat);
  mld_polyveck_reduce(&t1);
  mld_polyveck_invntt_tomont(&t1);

  /* Add error vector s2 */
  mld_polyveck_add(&t1, &s2);

  /* Reference: The following reduction is not present in the reference
   *            implementation. Omitting this reduction requires the output of
   *            the invntt to be small enough such that the addition of s2 does
   *            not result in absolute values >= MLDSA_Q. While our C, x86_64,
   *            and AArch64 invntt implementations produce small enough
   *            values for this to work out, it complicates the bounds
   *            reasoning. We instead add an additional reduction, and can
   *            consequently, relax the bounds requirements for the invntt.
   */
  mld_polyveck_reduce(&t1);

  /* Extract t1 and write public key */
  mld_polyveck_caddq(&t1);
  mld_polyveck_power2round(&t2, &t0, &t1);
  mld_pack_pk(pk, rho, &t2);

  /* Compute H(rho, t1) and write secret key */
  mld_shake256(tr, MLDSA_TRBYTES, pk, CRYPTO_PUBLICKEYBYTES);
  mld_pack_sk(sk, rho, tr, key, &t0, &s1, &s2);

  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(seedbuf, sizeof(seedbuf));
  mld_zeroize(inbuf, sizeof(inbuf));
  mld_zeroize(tr, sizeof(tr));
  mld_zeroize(mat, sizeof(mat));
  mld_zeroize(&s1, sizeof(s1));
  mld_zeroize(&s1hat, sizeof(s1hat));
  mld_zeroize(&s2, sizeof(s2));
  mld_zeroize(&t1, sizeof(t1));
  mld_zeroize(&t2, sizeof(t2));
  mld_zeroize(&t0, sizeof(t0));

  /* Constant time: pk is the public key, inherently public data */
  MLD_CT_TESTING_DECLASSIFY(pk, CRYPTO_PUBLICKEYBYTES);

  /* Pairwise Consistency Test (PCT) @[FIPS140_3_IG, p.87] */
  if (mld_check_pct(pk, sk))
  {
    return -1;
  }

  return 0;
}

#if !defined(MLD_CONFIG_NO_RANDOMIZED_API)
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_keypair(uint8_t pk[CRYPTO_PUBLICKEYBYTES],
                        uint8_t sk[CRYPTO_SECRETKEYBYTES])
{
  MLD_ALIGN uint8_t seed[MLDSA_SEEDBYTES];
  int result;
  mld_randombytes(seed, MLDSA_SEEDBYTES);
  MLD_CT_TESTING_SECRET(seed, sizeof(seed));
  result = crypto_sign_keypair_internal(pk, sk, seed);

  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(seed, sizeof(seed));
  return result;
}
#endif /* !MLD_CONFIG_NO_RANDOMIZED_API */

/*************************************************
 * Name:        mld_H
 *
 * Description: Abstracts application of SHAKE256 to
 *              one, two or three blocks of data,
 *              yielding a user-requested size of
 *              output.
 *
 * Arguments:   - uint8_t *out: pointer to output
 *              - size_t outlen: requested output length in bytes
 *              - const uint8_t *in1: pointer to input block 1
 *                                    Must NOT be NULL
 *              - size_t in1len: length of input in1 bytes
 *              - const uint8_t *in2: pointer to input block 2
 *                                    May be NULL if in2len=0, in which case
 *                                    this block is ignored
 *              - size_t in2len: length of input in2 bytes
 *              - const uint8_t *in3: pointer to input block 3
 *                                    May be NULL if in3len=0, in which case
 *                                    this block is ignored
 *              - size_t in3len: length of input in3 bytes
 **************************************************/
static void mld_H(uint8_t *out, size_t outlen, const uint8_t *in1,
                  size_t in1len, const uint8_t *in2, size_t in2len,
                  const uint8_t *in3, size_t in3len)
__contract__(
  requires(in1len <= MLD_MAX_BUFFER_SIZE)
  requires(in2len <= MLD_MAX_BUFFER_SIZE)
  requires(in3len <= MLD_MAX_BUFFER_SIZE)
  requires(outlen <= 8 * SHAKE256_RATE /* somewhat arbitrary bound */)
  requires(memory_no_alias(in1, in1len))
  requires(in2len == 0 || memory_no_alias(in2, in2len))
  requires(in3len == 0 || memory_no_alias(in3, in3len))
  requires(memory_no_alias(out, outlen))
  assigns(memory_slice(out, outlen))
)
{
  mld_shake256ctx state;
  mld_shake256_init(&state);
  mld_shake256_absorb(&state, in1, in1len);
  if (in2len != 0)
  {
    mld_shake256_absorb(&state, in2, in2len);
  }
  if (in3len != 0)
  {
    mld_shake256_absorb(&state, in3, in3len);
  }
  mld_shake256_finalize(&state);
  mld_shake256_squeeze(out, outlen, &state);
  mld_shake256_release(&state);

  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(&state, sizeof(state));
}

/* Reference: The reference implementation does not explicitly   */
/* check the maximum nonce value, but instead loops indefinitely */
/* (even when the nonce would overflow). Internally,             */
/* sampling of y uses (nonceL), (nonceL+1), ... (nonce*L+L-1).   */
/* Hence, there are no overflows if nonce < (UINT16_MAX - L)/L.  */
/* Explicitly checking for this explicitly allows us to prove    */
/* type-safety. Note that FIPS204 explicitly allows an upper-    */
/* bound this loop of 814 (< (UINT16_MAX - L)/L) - see           */
/* @[FIPS204, Appendix C].                                        */
#define NONCE_UB ((UINT16_MAX - MLDSA_L) / MLDSA_L)

/*************************************************
 * Name:        attempt_signature_generation
 *
 * Description: Attempts to generate a single signature.
 *
 * Arguments:   - uint8_t *sig: pointer to output signature
 *              - const uint8_t *mu: pointer to message or hash
 *                                   of exactly MLDSA_CRHBYTES bytes
 *              - const uint8_t *rhoprime: pointer to randomness seed
 *              - uint16_t nonce: current nonce value
 *              - const polyvecl mat[MLDSA_K]: expanded matrix
 *              - const polyvecl *s1: secret vector s1
 *              - const polyveck *s2: secret vector s2
 *              - const polyveck *t0: vector t0
 *
 * Returns 0 if signature generation succeeds, -1 if rejected
 *
 * Reference: This code differs from the reference implementation
 *            in that it factors out the core signature generation
 *            step into a distinct function here in order to improve
 *            efficiency of CBMC proof.
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
static int mld_attempt_signature_generation(
    uint8_t sig[CRYPTO_BYTES], const uint8_t *mu,
    const uint8_t rhoprime[MLDSA_CRHBYTES], uint16_t nonce,
    const mld_polyvecl mat[MLDSA_K], const mld_polyvecl *s1,
    const mld_polyveck *s2, const mld_polyveck *t0)
__contract__(
  requires(memory_no_alias(sig, CRYPTO_BYTES))
  requires(memory_no_alias(mu, MLDSA_CRHBYTES))
  requires(memory_no_alias(rhoprime, MLDSA_CRHBYTES))
  requires(memory_no_alias(mat, MLDSA_K * sizeof(mld_polyvecl)))
  requires(memory_no_alias(s1, sizeof(mld_polyvecl)))
  requires(memory_no_alias(s2, sizeof(mld_polyveck)))
  requires(memory_no_alias(t0, sizeof(mld_polyveck)))
  requires(nonce <= NONCE_UB)
  requires(forall(k1, 0, MLDSA_K, forall(l1, 0, MLDSA_L,
                                         array_bound(mat[k1].vec[l1].coeffs, 0, MLDSA_N, 0, MLDSA_Q))))
  requires(forall(k2, 0, MLDSA_K, array_abs_bound(t0->vec[k2].coeffs, 0, MLDSA_N, MLD_NTT_BOUND)))
  requires(forall(k3, 0, MLDSA_L, array_abs_bound(s1->vec[k3].coeffs, 0, MLDSA_N, MLD_NTT_BOUND)))
  requires(forall(k4, 0, MLDSA_K, array_abs_bound(s2->vec[k4].coeffs, 0, MLDSA_N, MLD_NTT_BOUND)))
  assigns(memory_slice(sig, CRYPTO_BYTES))
  ensures(return_value == 0 || return_value == -1)
)
{
  MLD_ALIGN uint8_t challenge_bytes[MLDSA_CTILDEBYTES];
  unsigned int n;
  mld_polyvecl y, z;
  mld_polyveck w, w1, w0, h;
  mld_poly cp;
  uint32_t z_invalid, w0_invalid, h_invalid;
  int res;

  /* Sample intermediate vector y */
  mld_polyvecl_uniform_gamma1(&y, rhoprime, nonce);

  /* Matrix-vector multiplication */
  z = y;
  mld_polyvecl_ntt(&z);
  mld_polyvec_matrix_pointwise_montgomery(&w, mat, &z);
  mld_polyveck_reduce(&w);
  mld_polyveck_invntt_tomont(&w);

  /* Decompose w and call the random oracle */
  mld_polyveck_caddq(&w);
  mld_polyveck_decompose(&w1, &w0, &w);
  mld_polyveck_pack_w1(sig, &w1);

  mld_H(challenge_bytes, MLDSA_CTILDEBYTES, mu, MLDSA_CRHBYTES, sig,
        MLDSA_K * MLDSA_POLYW1_PACKEDBYTES, NULL, 0);
  /* Constant time: Leaking challenge_bytes does not reveal any information
   * about the secret key as H() is modelled as random oracle.
   * This also applies to challenges for rejected signatures.
   * See Section 5.5 of @[Round3_Spec]. */
  MLD_CT_TESTING_DECLASSIFY(challenge_bytes, sizeof(challenge_bytes));
  mld_poly_challenge(&cp, challenge_bytes);
  mld_poly_ntt(&cp);

  /* Compute z, reject if it reveals secret */
  mld_polyvecl_pointwise_poly_montgomery(&z, &cp, s1);
  mld_polyvecl_invntt_tomont(&z);
  mld_polyvecl_add(&z, &y);
  mld_polyvecl_reduce(&z);

  z_invalid = mld_polyvecl_chknorm(&z, MLDSA_GAMMA1 - MLDSA_BETA);
  /* Constant time: It is fine (and prohibitively expensive to avoid)
   * leaking the result of the norm check. In case of rejection it
   * would even be okay to leak which coefficient led to rejection
   * as the candidate signature will be discarded anyway.
   * See Section 5.5 of @[Round3_Spec]. */
  MLD_CT_TESTING_DECLASSIFY(&z_invalid, sizeof(uint32_t));
  if (z_invalid)
  {
    res = -1; /* reject */
    goto cleanup;
  }

  /* If z is valid, then its coefficients are bounded by  */
  /* MLDSA_GAMMA1 - MLDSA_BETA. This will be needed below */
  /* to prove the pre-condition of pack_sig()             */
  mld_assert_abs_bound_2d(z.vec, MLDSA_L, MLDSA_N, (MLDSA_GAMMA1 - MLDSA_BETA));

  /* Check that subtracting cs2 does not change high bits of w and low bits
   * do not reveal secret information */
  mld_polyveck_pointwise_poly_montgomery(&h, &cp, s2);
  mld_polyveck_invntt_tomont(&h);
  mld_polyveck_sub(&w0, &h);
  mld_polyveck_reduce(&w0);

  w0_invalid = mld_polyveck_chknorm(&w0, MLDSA_GAMMA2 - MLDSA_BETA);
  /* Constant time: w0_invalid may be leaked - see comment for z_invalid. */
  MLD_CT_TESTING_DECLASSIFY(&w0_invalid, sizeof(uint32_t));
  if (w0_invalid)
  {
    res = -1; /* reject */
    goto cleanup;
  }

  /* Compute hints for w1 */
  mld_polyveck_pointwise_poly_montgomery(&h, &cp, t0);
  mld_polyveck_invntt_tomont(&h);
  mld_polyveck_reduce(&h);

  h_invalid = mld_polyveck_chknorm(&h, MLDSA_GAMMA2);
  /* Constant time: h_invalid may be leaked - see comment for z_invalid. */
  MLD_CT_TESTING_DECLASSIFY(&h_invalid, sizeof(uint32_t));
  if (h_invalid)
  {
    res = -1; /* reject */
    goto cleanup;
  }

  mld_polyveck_add(&w0, &h);

  /* Constant time: At this point all norm checks have passed and we, hence,
   * know that the signature does not leak any secret information.
   * Consequently, any value that can be computed from the signature and public
   * key is considered public.
   * w0 and w1 are public as they can be computed from Az - ct = \alpha w1 + w0.
   * h=c*t0 is public as both c and t0 are public.
   * For a more detailed discussion, refer to https://eprint.iacr.org/2022/1406.
   */
  MLD_CT_TESTING_DECLASSIFY(&w0, sizeof(w0));
  MLD_CT_TESTING_DECLASSIFY(&w1, sizeof(w1));
  n = mld_polyveck_make_hint(&h, &w0, &w1);
  if (n > MLDSA_OMEGA)
  {
    res = -1; /* reject */
    goto cleanup;
  }

  /* All is well - write signature */
  /* Constant time: At this point it is clear that the signature is valid - it
   * can, hence, be considered public. */
  MLD_CT_TESTING_DECLASSIFY(&h, sizeof(h));
  MLD_CT_TESTING_DECLASSIFY(&z, sizeof(z));
  mld_pack_sig(sig, challenge_bytes, &z, &h, n);

  res = 0; /* success */

cleanup:
  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(challenge_bytes, MLDSA_CTILDEBYTES);
  mld_zeroize(&y, sizeof(y));
  mld_zeroize(&z, sizeof(z));
  mld_zeroize(&w, sizeof(w));
  mld_zeroize(&w1, sizeof(w1));
  mld_zeroize(&w0, sizeof(w0));
  mld_zeroize(&h, sizeof(h));
  mld_zeroize(&cp, sizeof(cp));

  return res;
}
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature_internal(uint8_t sig[CRYPTO_BYTES], size_t *siglen,
                                   const uint8_t *m, size_t mlen,
                                   const uint8_t *pre, size_t prelen,
                                   const uint8_t rnd[MLDSA_RNDBYTES],
                                   const uint8_t sk[CRYPTO_SECRETKEYBYTES],
                                   int externalmu)
{
  int result;
  MLD_ALIGN uint8_t
      seedbuf[2 * MLDSA_SEEDBYTES + MLDSA_TRBYTES + 2 * MLDSA_CRHBYTES];
  uint8_t *rho, *tr, *key, *mu, *rhoprime;
  mld_polyvecl mat[MLDSA_K], s1;
  mld_polyveck t0, s2;

  uint16_t nonce = 0;

  rho = seedbuf;
  tr = rho + MLDSA_SEEDBYTES;
  key = tr + MLDSA_TRBYTES;
  mu = key + MLDSA_SEEDBYTES;
  rhoprime = mu + MLDSA_CRHBYTES;
  mld_unpack_sk(rho, tr, key, &t0, &s1, &s2, sk);

  if (!externalmu)
  {
    /* Compute mu = CRH(tr, pre, msg) */
    mld_H(mu, MLDSA_CRHBYTES, tr, MLDSA_TRBYTES, pre, prelen, m, mlen);
  }
  else
  {
    /* mu has been provided directly */
    mld_memcpy(mu, m, MLDSA_CRHBYTES);
  }

  /* Compute rhoprime = CRH(key, rnd, mu) */
  mld_H(rhoprime, MLDSA_CRHBYTES, key, MLDSA_SEEDBYTES, rnd, MLDSA_RNDBYTES, mu,
        MLDSA_CRHBYTES);

  /* Constant time: rho is part of the public key and, hence, public. */
  MLD_CT_TESTING_DECLASSIFY(rho, MLDSA_SEEDBYTES);
  /* Expand matrix and transform vectors */
  mld_polyvec_matrix_expand(mat, rho);
  mld_polyvecl_ntt(&s1);
  mld_polyveck_ntt(&s2);
  mld_polyveck_ntt(&t0);

  /* By default, return failure. Flip to success and write output
   * once signature generation succeeds.
   *
   * This is required to satisfy the initial loop invariant. */
  *siglen = 0;
  result = -1;

  /* Reference: This code is re-structured using a while(1),  */
  /* with explicit "continue" statements (rather than "goto") */
  /* to implement rejection of invalid signatures.            */
  while (1)
  __loop__(
    assigns(nonce, result, object_whole(siglen), memory_slice(sig, CRYPTO_BYTES))
    invariant(nonce <= NONCE_UB)

    /* t0, s1, s2, and mat are initialized above and are NOT changed by this */
    /* loop. We can therefore re-assert their bounds here as part of the     */
    /* loop invariant. This makes proof noticeably faster with CBMC          */
    invariant(forall(k1, 0, MLDSA_K, forall(l1, 0, MLDSA_L,
              array_bound(mat[k1].vec[l1].coeffs, 0, MLDSA_N, 0, MLDSA_Q))))
    invariant(forall(k2, 0, MLDSA_K, array_abs_bound(t0.vec[k2].coeffs, 0, MLDSA_N, MLD_NTT_BOUND)))
    invariant(forall(k3, 0, MLDSA_L, array_abs_bound(s1.vec[k3].coeffs, 0, MLDSA_N, MLD_NTT_BOUND)))
    invariant(forall(k4, 0, MLDSA_K, array_abs_bound(s2.vec[k4].coeffs, 0, MLDSA_N, MLD_NTT_BOUND)))
    invariant((result == 0 && *siglen == CRYPTO_BYTES) ||
              (result == -1 && *siglen == 0))
  )
  {
    int attempt_result;
    /* Reference: this code explicitly checks for exhaustion of nonce     */
    /* values to provide predictable termination and results in that case */
    /* Checking here also means that incrementing nonce below can also    */
    /* be proven to be type-safe.                                         */
    if (nonce == NONCE_UB)
    {
      /* To be on the safe-side, we zeroize the signature buffer.
       * Note that *siglen == 0 and result == -1 by default, so we
       * don't need to set them here. */
      mld_memset(sig, 0, CRYPTO_BYTES);
      break;
    }

    attempt_result = mld_attempt_signature_generation(sig, mu, rhoprime, nonce,
                                                      mat, &s1, &s2, &t0);
    nonce++;
    if (attempt_result == 0)
    {
      *siglen = CRYPTO_BYTES;
      result = 0;
      break;
    }
  }

  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(seedbuf, sizeof(seedbuf));
  mld_zeroize(mat, sizeof(mat));
  mld_zeroize(&s1, sizeof(s1));
  mld_zeroize(&s2, sizeof(s2));
  mld_zeroize(&t0, sizeof(t0));
  return result;
}

#if !defined(MLD_CONFIG_NO_RANDOMIZED_API)
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature(uint8_t sig[CRYPTO_BYTES], size_t *siglen,
                          const uint8_t *m, size_t mlen, const uint8_t *ctx,
                          size_t ctxlen,
                          const uint8_t sk[CRYPTO_SECRETKEYBYTES])
{
  MLD_ALIGN uint8_t pre[257];
  MLD_ALIGN uint8_t rnd[MLDSA_RNDBYTES];
  int result;

  if (ctxlen > 255)
  {
    /* To be on the safe-side, make sure *siglen has a well-defined */
    /* value, even in the case of error.                            */
    *siglen = 0;
    result = -1;
    goto cleanup;
  }

  /* Prepare pre = (0, ctxlen, ctx) */
  pre[0] = 0;
  /* Safety: Truncation is safe due to the check above. */
  pre[1] = (uint8_t)ctxlen;
  if (ctxlen > 0)
  {
    mld_memcpy(pre + 2, ctx, ctxlen);
  }

  /* Randomized variant of ML-DSA. If you need the deterministic variant,
   * call crypto_sign_signature_internal directly with all-zero rnd. */
  mld_randombytes(rnd, MLDSA_RNDBYTES);
  MLD_CT_TESTING_SECRET(rnd, sizeof(rnd));

  result = crypto_sign_signature_internal(sig, siglen, m, mlen, pre, 2 + ctxlen,
                                          rnd, sk, 0);

cleanup:
  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(pre, sizeof(pre));
  mld_zeroize(rnd, sizeof(rnd));

  return result;
}
#endif /* !MLD_CONFIG_NO_RANDOMIZED_API */

#if !defined(MLD_CONFIG_NO_RANDOMIZED_API)
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature_extmu(uint8_t sig[CRYPTO_BYTES], size_t *siglen,
                                const uint8_t mu[MLDSA_CRHBYTES],
                                const uint8_t sk[CRYPTO_SECRETKEYBYTES])
{
  MLD_ALIGN uint8_t rnd[MLDSA_RNDBYTES];
  int result;

  /* Randomized variant of ML-DSA. If you need the deterministic variant,
   * call crypto_sign_signature_internal directly with all-zero rnd. */
  mld_randombytes(rnd, MLDSA_RNDBYTES);
  MLD_CT_TESTING_SECRET(rnd, sizeof(rnd));

  result = crypto_sign_signature_internal(sig, siglen, mu, MLDSA_CRHBYTES, NULL,
                                          0, rnd, sk, 1);

  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(rnd, sizeof(rnd));

  return result;
}
#endif /* !MLD_CONFIG_NO_RANDOMIZED_API */

#if !defined(MLD_CONFIG_NO_RANDOMIZED_API)
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign(uint8_t *sm, size_t *smlen, const uint8_t *m, size_t mlen,
                const uint8_t *ctx, size_t ctxlen,
                const uint8_t sk[CRYPTO_SECRETKEYBYTES])
{
  int ret;
  size_t i;

  for (i = 0; i < mlen; ++i)
  __loop__(
    assigns(i, object_whole(sm))
    invariant(i <= mlen)
  )
  {
    sm[CRYPTO_BYTES + mlen - 1 - i] = m[mlen - 1 - i];
  }
  ret = crypto_sign_signature(sm, smlen, sm + CRYPTO_BYTES, mlen, ctx, ctxlen,
                              sk);
  *smlen += mlen;
  return ret;
}
#endif /* !MLD_CONFIG_NO_RANDOMIZED_API */

MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify_internal(const uint8_t *sig, size_t siglen,
                                const uint8_t *m, size_t mlen,
                                const uint8_t *pre, size_t prelen,
                                const uint8_t pk[CRYPTO_PUBLICKEYBYTES],
                                int externalmu)
{
  unsigned int i;
  int res;
  MLD_ALIGN uint8_t buf[MLDSA_K * MLDSA_POLYW1_PACKEDBYTES];
  MLD_ALIGN uint8_t rho[MLDSA_SEEDBYTES];
  MLD_ALIGN uint8_t mu[MLDSA_CRHBYTES];
  MLD_ALIGN uint8_t c[MLDSA_CTILDEBYTES];
  MLD_ALIGN uint8_t c2[MLDSA_CTILDEBYTES];
  mld_poly cp;
  mld_polyvecl mat[MLDSA_K], z;
  mld_polyveck t1, w1, tmp, h;

  if (siglen != CRYPTO_BYTES)
  {
    res = -1;
    goto cleanup;
  }

  mld_unpack_pk(rho, &t1, pk);
  if (mld_unpack_sig(c, &z, &h, sig))
  {
    res = -1;
    goto cleanup;
  }
  if (mld_polyvecl_chknorm(&z, MLDSA_GAMMA1 - MLDSA_BETA))
  {
    res = -1;
    goto cleanup;
  }

  if (!externalmu)
  {
    /* Compute CRH(H(rho, t1), pre, msg) */
    MLD_ALIGN uint8_t hpk[MLDSA_CRHBYTES];
    mld_H(hpk, MLDSA_TRBYTES, pk, CRYPTO_PUBLICKEYBYTES, NULL, 0, NULL, 0);
    mld_H(mu, MLDSA_CRHBYTES, hpk, MLDSA_TRBYTES, pre, prelen, m, mlen);

    /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
    mld_zeroize(hpk, sizeof(hpk));
  }
  else
  {
    /* mu has been provided directly */
    mld_memcpy(mu, m, MLDSA_CRHBYTES);
  }

  /* Matrix-vector multiplication; compute Az - c2^dt1 */
  mld_poly_challenge(&cp, c);
  mld_polyvec_matrix_expand(mat, rho);

  mld_polyvecl_ntt(&z);
  mld_polyvec_matrix_pointwise_montgomery(&w1, mat, &z);

  mld_poly_ntt(&cp);
  mld_polyveck_shiftl(&t1);
  mld_polyveck_ntt(&t1);

  mld_polyveck_pointwise_poly_montgomery(&tmp, &cp, &t1);

  mld_polyveck_sub(&w1, &tmp);
  mld_polyveck_reduce(&w1);
  mld_polyveck_invntt_tomont(&w1);

  /* Reconstruct w1 */
  mld_polyveck_caddq(&w1);
  mld_polyveck_use_hint(&tmp, &w1, &h);
  mld_polyveck_pack_w1(buf, &tmp);
  /* Call random oracle and verify challenge */
  mld_H(c2, MLDSA_CTILDEBYTES, mu, MLDSA_CRHBYTES, buf,
        MLDSA_K * MLDSA_POLYW1_PACKEDBYTES, NULL, 0);

  /* Constant time: All data in verification is usually considered public.
   * However, in our constant-time tests we do not declassify the message and
   * context string.
   * The following conditional is the only place in verification whose run-time
   * depends on the message. As all that can be leakaged here is the output of
   * a hash call (that should behave like a random oracle), it is safe to
   * declassify here even with a secret message.
   */
  MLD_CT_TESTING_DECLASSIFY(c2, sizeof(c2));
  for (i = 0; i < MLDSA_CTILDEBYTES; ++i)
  __loop__(
    invariant(i <= MLDSA_CTILDEBYTES)
  )
  {
    if (c[i] != c2[i])
    {
      res = -1;
      goto cleanup;
    }
  }

  res = 0;

cleanup:
  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(buf, sizeof(buf));
  mld_zeroize(rho, sizeof(rho));
  mld_zeroize(mu, sizeof(mu));
  mld_zeroize(c, sizeof(c));
  mld_zeroize(c2, sizeof(c2));
  mld_zeroize(&cp, sizeof(cp));
  mld_zeroize(&z, sizeof(z));
  mld_zeroize(&w1, sizeof(w1));
  mld_zeroize(&tmp, sizeof(tmp));
  mld_zeroize(&h, sizeof(h));
  mld_zeroize(mat, sizeof(mat));
  return res;
}

MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m,
                       size_t mlen, const uint8_t *ctx, size_t ctxlen,
                       const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
{
  MLD_ALIGN uint8_t pre[257];
  int result;

  if (ctxlen > 255)
  {
    result = -1;
    goto cleanup;
  }

  pre[0] = 0;
  /* Safety: Truncation is safe due to the check above. */
  pre[1] = (uint8_t)ctxlen;
  if (ctxlen > 0)
  {
    mld_memcpy(pre + 2, ctx, ctxlen);
  }

  result =
      crypto_sign_verify_internal(sig, siglen, m, mlen, pre, 2 + ctxlen, pk, 0);


cleanup:
  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(pre, sizeof(pre));

  return result;
}

MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify_extmu(const uint8_t *sig, size_t siglen,
                             const uint8_t mu[MLDSA_CRHBYTES],
                             const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
{
  return crypto_sign_verify_internal(sig, siglen, mu, MLDSA_CRHBYTES, NULL, 0,
                                     pk, 1);
}

MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen,
                     const uint8_t *ctx, size_t ctxlen,
                     const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
{
  size_t i;

  if (smlen < CRYPTO_BYTES)
  {
    goto badsig;
  }

  *mlen = smlen - CRYPTO_BYTES;
  if (crypto_sign_verify(sm, CRYPTO_BYTES, sm + CRYPTO_BYTES, *mlen, ctx,
                         ctxlen, pk))
  {
    goto badsig;
  }
  else
  {
    /* All good, copy msg, return 0 */
    for (i = 0; i < *mlen; ++i)
    __loop__(
      assigns(i, memory_slice(m, *mlen))
      invariant(i <= *mlen)
    )
    {
      m[i] = sm[CRYPTO_BYTES + i];
    }
    return 0;
  }

badsig:
  /* Signature verification failed */
  *mlen = 0;
  mld_memset(m, 0, smlen);

  return -1;
}


MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature_pre_hash_internal(
    uint8_t sig[CRYPTO_BYTES], size_t *siglen, const uint8_t *ph, size_t phlen,
    const uint8_t *ctx, size_t ctxlen, const uint8_t rnd[MLDSA_RNDBYTES],
    const uint8_t sk[CRYPTO_SECRETKEYBYTES], int hashalg)
{
  MLD_ALIGN uint8_t fmsg[MLD_PRE_HASH_MAX_FORMATTED_MESSAGE_BYTES];
  size_t fmsg_len;
  int result;

  if (ctxlen > 255)
  {
    *siglen = 0;
    result = -1;
    goto cleanup;
  }

  if (mld_validate_hash_length(hashalg, phlen))
  {
    *siglen = 0;
    result = -1;
    goto cleanup;
  }

  fmsg_len = mld_format_pre_hash_message(fmsg, ph, phlen, ctx, ctxlen, hashalg);

  result = crypto_sign_signature_internal(sig, siglen, fmsg, fmsg_len, NULL, 0,
                                          rnd, sk, 0);
cleanup:
  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(fmsg, sizeof(fmsg));
  return result;
}

MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify_pre_hash_internal(
    const uint8_t *sig, size_t siglen, const uint8_t *ph, size_t phlen,
    const uint8_t *ctx, size_t ctxlen, const uint8_t pk[CRYPTO_PUBLICKEYBYTES],
    int hashalg)
{
  MLD_ALIGN uint8_t fmsg[MLD_PRE_HASH_MAX_FORMATTED_MESSAGE_BYTES];
  size_t fmsg_len;
  int result;

  if (ctxlen > 255)
  {
    result = -1;
    goto cleanup;
  }

  if (mld_validate_hash_length(hashalg, phlen))
  {
    result = -1;
    goto cleanup;
  }

  fmsg_len = mld_format_pre_hash_message(fmsg, ph, phlen, ctx, ctxlen, hashalg);

  result =
      crypto_sign_verify_internal(sig, siglen, fmsg, fmsg_len, NULL, 0, pk, 0);

cleanup:
  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(fmsg, sizeof(fmsg));
  return result;
}

MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature_pre_hash_shake256(
    uint8_t sig[CRYPTO_BYTES], size_t *siglen, const uint8_t *m, size_t mlen,
    const uint8_t *ctx, size_t ctxlen, const uint8_t rnd[MLDSA_RNDBYTES],
    const uint8_t sk[CRYPTO_SECRETKEYBYTES])
{
  MLD_ALIGN uint8_t ph[64];
  int result;
  mld_shake256(ph, sizeof(ph), m, mlen);
  result = crypto_sign_signature_pre_hash_internal(
      sig, siglen, ph, sizeof(ph), ctx, ctxlen, rnd, sk, MLD_PREHASH_SHAKE_256);
  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(ph, sizeof(ph));
  return result;
}

MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify_pre_hash_shake256(
    const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen,
    const uint8_t *ctx, size_t ctxlen, const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
{
  MLD_ALIGN uint8_t ph[64];
  int result;
  mld_shake256(ph, sizeof(ph), m, mlen);
  result = crypto_sign_verify_pre_hash_internal(
      sig, siglen, ph, sizeof(ph), ctx, ctxlen, pk, MLD_PREHASH_SHAKE_256);
  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(ph, sizeof(ph));
  return result;
}

/* To facilitate single-compilation-unit (SCU) builds, undefine all macros.
 * Don't modify by hand -- this is auto-generated by scripts/autogen. */
#undef mld_check_pct
#undef mld_sample_s1_s2
#undef mld_H
#undef mld_attempt_signature_generation
#undef NONCE_UB
