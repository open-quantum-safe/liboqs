/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [FIPS140_3_IG]
 *   Implementation Guidance for FIPS 140-3 and the Cryptographic Module
 *   Validation Program National Institute of Standards and Technology
 *   https://csrc.nist.gov/projects/cryptographic-module-validation-program/fips-140-3-ig-announcements
 *
 * - [FIPS203]
 *   FIPS 203 Module-Lattice-Based Key-Encapsulation Mechanism Standard
 *   National Institute of Standards and Technology
 *   https://csrc.nist.gov/pubs/fips/203/final
 *
 * - [REF]
 *   CRYSTALS-Kyber C reference implementation
 *   Bos, Ducas, Kiltz, Lepoint, Lyubashevsky, Schanck, Schwabe, Seiler, Stehlé
 *   https://github.com/pq-crystals/kyber/tree/main/ref
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "indcpa.h"
#include "kem.h"
#include "randombytes.h"
#include "symmetric.h"
#include "verify.h"

/* Parameter set namespacing
 * This is to facilitate building multiple instances
 * of mlkem-native (e.g. with varying security levels)
 * within a single compilation unit. */
#define mlk_check_pk MLK_ADD_PARAM_SET(mlk_check_pk)
#define mlk_check_sk MLK_ADD_PARAM_SET(mlk_check_sk)
#define mlk_check_pct MLK_ADD_PARAM_SET(mlk_check_pct)
/* End of parameter set namespacing */

#if defined(CBMC)
/* Redeclaration with contract needed for CBMC only */
int memcmp(const void *str1, const void *str2, size_t n)
__contract__(
  requires(memory_no_alias(str1, n))
  requires(memory_no_alias(str2, n))
);
#endif /* CBMC */

/*************************************************
 * Name:        mlk_check_pk
 *
 * Description: Implements modulus check mandated by FIPS 203,
 *              i.e., ensures that coefficients are in [0,q-1].
 *
 * Arguments:   - const uint8_t *pk: pointer to input public key
 *                (an already allocated array of MLKEM_INDCCA_PUBLICKEYBYTES
 *                 bytes)
 *
 * Returns: - 0 on success
 *          - -1 on failure
 *
 * Specification: Implements @[FIPS203, Section 7.2, 'modulus check']
 *
 **************************************************/

/* Reference: Not implemented in the reference implementation @[REF]. */
MLK_MUST_CHECK_RETURN_VALUE
static int mlk_check_pk(const uint8_t pk[MLKEM_INDCCA_PUBLICKEYBYTES])
{
  int res;
  mlk_polyvec p;
  uint8_t p_reencoded[MLKEM_POLYVECBYTES];

  mlk_polyvec_frombytes(p, pk);
  mlk_polyvec_reduce(p);
  mlk_polyvec_tobytes(p_reencoded, p);

  /* We use a constant-time memcmp here to avoid having to
   * declassify the PK before the PCT has succeeded. */
  res = mlk_ct_memcmp(pk, p_reencoded, MLKEM_POLYVECBYTES) ? -1 : 0;

  /* Specification: Partially implements
   * @[FIPS203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(p_reencoded, sizeof(p_reencoded));
  mlk_zeroize(&p, sizeof(p));
  return res;
}

/*************************************************
 * Name:        mlk_check_sk
 *
 * Description: Implements public key hash check mandated by FIPS 203,
 *              i.e., ensures that
 *              sk[768𝑘+32 ∶ 768𝑘+64] = H(pk)= H(sk[384𝑘 : 768𝑘+32])
 *
 * Arguments:   - const uint8_t *sk: pointer to input private key
 *                (an already allocated array of MLKEM_INDCCA_SECRETKEYBYTES
 *                 bytes)
 *
 * Returns: - 0 on success
 *          - -1 on failure
 *
 * Specification: Implements @[FIPS203, Section 7.3, 'hash check']
 *
 **************************************************/

/* Reference: Not implemented in the reference implementation @[REF]. */
MLK_MUST_CHECK_RETURN_VALUE
static int mlk_check_sk(const uint8_t sk[MLKEM_INDCCA_SECRETKEYBYTES])
{
  int res;
  MLK_ALIGN uint8_t test[MLKEM_SYMBYTES];
  /*
   * The parts of `sk` being hashed and compared here are public, so
   * no public information is leaked through the runtime or the return value
   * of this function.
   */

  /* Declassify the public part of the secret key */
  MLK_CT_TESTING_DECLASSIFY(sk + MLKEM_INDCPA_SECRETKEYBYTES,
                            MLKEM_INDCCA_PUBLICKEYBYTES);
  MLK_CT_TESTING_DECLASSIFY(
      sk + MLKEM_INDCCA_SECRETKEYBYTES - 2 * MLKEM_SYMBYTES, MLKEM_SYMBYTES);

  mlk_hash_h(test, sk + MLKEM_INDCPA_SECRETKEYBYTES,
             MLKEM_INDCCA_PUBLICKEYBYTES);
  res = memcmp(sk + MLKEM_INDCCA_SECRETKEYBYTES - 2 * MLKEM_SYMBYTES, test,
               MLKEM_SYMBYTES)
            ? -1
            : 0;

  /* Specification: Partially implements
   * @[FIPS203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(test, sizeof(test));
  return res;
}

MLK_MUST_CHECK_RETURN_VALUE
static int mlk_check_pct(uint8_t const pk[MLKEM_INDCCA_PUBLICKEYBYTES],
                         uint8_t const sk[MLKEM_INDCCA_SECRETKEYBYTES])
__contract__(
  requires(memory_no_alias(pk, MLKEM_INDCCA_PUBLICKEYBYTES))
  requires(memory_no_alias(sk, MLKEM_INDCCA_SECRETKEYBYTES)));

#if defined(MLK_CONFIG_KEYGEN_PCT)
/* Specification:
 * Partially implements 'Pairwise Consistency Test' @[FIPS140_3_IG, p.87] and
 * @[FIPS203, Section 7.1, Pairwise Consistency]. */

/* Reference: Not implemented in the reference implementation @[REF]. */
static int mlk_check_pct(uint8_t const pk[MLKEM_INDCCA_PUBLICKEYBYTES],
                         uint8_t const sk[MLKEM_INDCCA_SECRETKEYBYTES])
{
  int res;
  uint8_t ct[MLKEM_INDCCA_CIPHERTEXTBYTES];
  uint8_t ss_enc[MLKEM_SSBYTES], ss_dec[MLKEM_SSBYTES];

  res = crypto_kem_enc(ct, ss_enc, pk);
  if (res != 0)
  {
    goto cleanup;
  }

  res = crypto_kem_dec(ss_dec, ct, sk);
  if (res != 0)
  {
    goto cleanup;
  }

#if defined(MLK_CONFIG_KEYGEN_PCT_BREAKAGE_TEST)
  /* Deliberately break PCT for testing purposes */
  if (mlk_break_pct())
  {
    ss_enc[0] = ~ss_enc[0];
  }
#endif /* MLK_CONFIG_KEYGEN_PCT_BREAKAGE_TEST */

  res = mlk_ct_memcmp(ss_enc, ss_dec, sizeof(ss_dec));

cleanup:
  /* The result of the PCT is public. */
  MLK_CT_TESTING_DECLASSIFY(&res, sizeof(res));

  /* Specification: Partially implements
   * @[FIPS203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(ct, sizeof(ct));
  mlk_zeroize(ss_enc, sizeof(ss_enc));
  mlk_zeroize(ss_dec, sizeof(ss_dec));
  return res;
}
#else  /* MLK_CONFIG_KEYGEN_PCT */
static int mlk_check_pct(uint8_t const pk[MLKEM_INDCCA_PUBLICKEYBYTES],
                         uint8_t const sk[MLKEM_INDCCA_SECRETKEYBYTES])
{
  /* Skip PCT */
  ((void)pk);
  ((void)sk);
  return 0;
}
#endif /* !MLK_CONFIG_KEYGEN_PCT */

/* Reference: `crypto_kem_keypair_derand()` in the reference implementation
 *            @[REF].
 *            - We optionally include PCT which is not present in
 *              the reference code. */
MLK_EXTERNAL_API
int crypto_kem_keypair_derand(uint8_t pk[MLKEM_INDCCA_PUBLICKEYBYTES],
                              uint8_t sk[MLKEM_INDCCA_SECRETKEYBYTES],
                              const uint8_t coins[2 * MLKEM_SYMBYTES])
{
  mlk_indcpa_keypair_derand(pk, sk, coins);
  memcpy(sk + MLKEM_INDCPA_SECRETKEYBYTES, pk, MLKEM_INDCCA_PUBLICKEYBYTES);
  mlk_hash_h(sk + MLKEM_INDCCA_SECRETKEYBYTES - 2 * MLKEM_SYMBYTES, pk,
             MLKEM_INDCCA_PUBLICKEYBYTES);
  /* Value z for pseudo-random output on reject */
  memcpy(sk + MLKEM_INDCCA_SECRETKEYBYTES - MLKEM_SYMBYTES,
         coins + MLKEM_SYMBYTES, MLKEM_SYMBYTES);

  /* Declassify public key */
  MLK_CT_TESTING_DECLASSIFY(pk, MLKEM_INDCCA_PUBLICKEYBYTES);

  /* Pairwise Consistency Test (PCT) @[FIPS140_3_IG, p.87] */
  if (mlk_check_pct(pk, sk))
  {
    return -1;
  }

  return 0;
}

/* Reference: `crypto_kem_keypair()` in the reference implementation @[REF]
 *            - We zeroize the stack buffer */
MLK_EXTERNAL_API
int crypto_kem_keypair(uint8_t pk[MLKEM_INDCCA_PUBLICKEYBYTES],
                       uint8_t sk[MLKEM_INDCCA_SECRETKEYBYTES])
{
  int res;
  MLK_ALIGN uint8_t coins[2 * MLKEM_SYMBYTES];

  /* Acquire necessary randomness, and mark it as secret. */
  mlk_randombytes(coins, 2 * MLKEM_SYMBYTES);
  MLK_CT_TESTING_SECRET(coins, sizeof(coins));

  res = crypto_kem_keypair_derand(pk, sk, coins);

  /* Specification: Partially implements
   * @[FIPS203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(coins, sizeof(coins));
  return res;
}

/* Reference: `crypto_kem_enc_derand()` in the reference implementation @[REF]
 *            - We include public key check
 *            - We include stack buffer zeroization */
MLK_EXTERNAL_API
int crypto_kem_enc_derand(uint8_t ct[MLKEM_INDCCA_CIPHERTEXTBYTES],
                          uint8_t ss[MLKEM_SSBYTES],
                          const uint8_t pk[MLKEM_INDCCA_PUBLICKEYBYTES],
                          const uint8_t coins[MLKEM_SYMBYTES])
{
  MLK_ALIGN uint8_t buf[2 * MLKEM_SYMBYTES];
  /* Will contain key, coins */
  MLK_ALIGN uint8_t kr[2 * MLKEM_SYMBYTES];

  /* Specification: Implements @[FIPS203, Section 7.2, Modulus check] */
  if (mlk_check_pk(pk))
  {
    return -1;
  }

  memcpy(buf, coins, MLKEM_SYMBYTES);

  /* Multitarget countermeasure for coins + contributory KEM */
  mlk_hash_h(buf + MLKEM_SYMBYTES, pk, MLKEM_INDCCA_PUBLICKEYBYTES);
  mlk_hash_g(kr, buf, 2 * MLKEM_SYMBYTES);

  /* coins are in kr+MLKEM_SYMBYTES */
  mlk_indcpa_enc(ct, buf, pk, kr + MLKEM_SYMBYTES);

  memcpy(ss, kr, MLKEM_SYMBYTES);

  /* Specification: Partially implements
   * @[FIPS203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(buf, sizeof(buf));
  mlk_zeroize(kr, sizeof(kr));

  return 0;
}

/* Reference: `crypto_kem_enc()` in the reference implementation @[REF]
 *            - We include stack buffer zeroization */
MLK_EXTERNAL_API
int crypto_kem_enc(uint8_t ct[MLKEM_INDCCA_CIPHERTEXTBYTES],
                   uint8_t ss[MLKEM_SSBYTES],
                   const uint8_t pk[MLKEM_INDCCA_PUBLICKEYBYTES])
{
  int res;
  MLK_ALIGN uint8_t coins[MLKEM_SYMBYTES];

  mlk_randombytes(coins, MLKEM_SYMBYTES);
  MLK_CT_TESTING_SECRET(coins, sizeof(coins));

  res = crypto_kem_enc_derand(ct, ss, pk, coins);

  /* Specification: Partially implements
   * @[FIPS203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(coins, sizeof(coins));
  return res;
}

/* Reference: `crypto_kem_dec()` in the reference implementation @[REF]
 *            - We include secret key check
 *            - We include stack buffer zeroization */
MLK_EXTERNAL_API
int crypto_kem_dec(uint8_t ss[MLKEM_SSBYTES],
                   const uint8_t ct[MLKEM_INDCCA_CIPHERTEXTBYTES],
                   const uint8_t sk[MLKEM_INDCCA_SECRETKEYBYTES])
{
  uint8_t fail;
  MLK_ALIGN uint8_t buf[2 * MLKEM_SYMBYTES];
  /* Will contain key, coins */
  MLK_ALIGN uint8_t kr[2 * MLKEM_SYMBYTES];
  MLK_ALIGN uint8_t tmp[MLKEM_SYMBYTES + MLKEM_INDCCA_CIPHERTEXTBYTES];

  const uint8_t *pk = sk + MLKEM_INDCPA_SECRETKEYBYTES;

  /* Specification: Implements @[FIPS203, Section 7.3, Hash check] */
  if (mlk_check_sk(sk))
  {
    return -1;
  }

  mlk_indcpa_dec(buf, ct, sk);

  /* Multitarget countermeasure for coins + contributory KEM */
  memcpy(buf + MLKEM_SYMBYTES,
         sk + MLKEM_INDCCA_SECRETKEYBYTES - 2 * MLKEM_SYMBYTES, MLKEM_SYMBYTES);
  mlk_hash_g(kr, buf, 2 * MLKEM_SYMBYTES);

  /* Recompute and compare ciphertext */
  /* coins are in kr+MLKEM_SYMBYTES */
  mlk_indcpa_enc(tmp, buf, pk, kr + MLKEM_SYMBYTES);
  fail = mlk_ct_memcmp(ct, tmp, MLKEM_INDCCA_CIPHERTEXTBYTES);

  /* Compute rejection key */
  memcpy(tmp, sk + MLKEM_INDCCA_SECRETKEYBYTES - MLKEM_SYMBYTES,
         MLKEM_SYMBYTES);
  memcpy(tmp + MLKEM_SYMBYTES, ct, MLKEM_INDCCA_CIPHERTEXTBYTES);
  mlk_hash_j(ss, tmp, sizeof(tmp));

  /* Copy true key to return buffer if fail is 0 */
  mlk_ct_cmov_zero(ss, kr, MLKEM_SYMBYTES, fail);

  /* Specification: Partially implements
   * @[FIPS203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(buf, sizeof(buf));
  mlk_zeroize(kr, sizeof(kr));
  mlk_zeroize(tmp, sizeof(tmp));

  return 0;
}

/* To facilitate single-compilation-unit (SCU) builds, undefine all macros.
 * Don't modify by hand -- this is auto-generated by scripts/autogen. */
#undef mlk_check_pk
#undef mlk_check_sk
#undef mlk_check_pct
