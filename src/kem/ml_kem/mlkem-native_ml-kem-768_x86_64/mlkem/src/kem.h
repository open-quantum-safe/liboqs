/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [FIPS203]
 *   FIPS 203 Module-Lattice-Based Key-Encapsulation Mechanism Standard
 *   National Institute of Standards and Technology
 *   https://csrc.nist.gov/pubs/fips/203/final
 */

#ifndef MLK_KEM_H
#define MLK_KEM_H

#include <stdint.h>
#include "cbmc.h"
#include "common.h"
#include "sys.h"

#if defined(MLK_CHECK_APIS)
/* Include to ensure consistency between internal kem.h
 * and external mlkem_native.h. */
#define MLK_CONFIG_API_NO_SUPERCOP
#include "mlkem_native.h"
#undef MLK_CONFIG_API_NO_SUPERCOP

#if MLKEM_INDCCA_SECRETKEYBYTES != \
    MLKEM_SECRETKEYBYTES(MLK_CONFIG_PARAMETER_SET)
#error Mismatch for SECRETKEYBYTES between kem.h and mlkem_native.h
#endif

#if MLKEM_INDCCA_PUBLICKEYBYTES != \
    MLKEM_PUBLICKEYBYTES(MLK_CONFIG_PARAMETER_SET)
#error Mismatch for PUBLICKEYBYTES between kem.h and mlkem_native.h
#endif

#if MLKEM_INDCCA_CIPHERTEXTBYTES != \
    MLKEM_CIPHERTEXTBYTES(MLK_CONFIG_PARAMETER_SET)
#error Mismatch for CIPHERTEXTBYTES between kem.h and mlkem_native.h
#endif

#endif /* MLK_CHECK_APIS */

#define crypto_kem_keypair_derand MLK_NAMESPACE_K(keypair_derand)
#define crypto_kem_keypair MLK_NAMESPACE_K(keypair)
#define crypto_kem_enc_derand MLK_NAMESPACE_K(enc_derand)
#define crypto_kem_enc MLK_NAMESPACE_K(enc)
#define crypto_kem_dec MLK_NAMESPACE_K(dec)

/*************************************************
 * Name:        crypto_kem_keypair_derand
 *
 * Description: Generates public and private key
 *              for CCA-secure ML-KEM key encapsulation mechanism
 *
 * Arguments:   - uint8_t *pk: pointer to output public key
 *                (an already allocated array of MLKEM_INDCCA_PUBLICKEYBYTES
 *                 bytes)
 *              - uint8_t *sk: pointer to output private key
 *                (an already allocated array of MLKEM_INDCCA_SECRETKEYBYTES
 *                 bytes)
 *              - uint8_t *coins: pointer to input randomness
 *                (an already allocated array filled with 2*MLKEM_SYMBYTES
 *                 random bytes)
 *
 * Returns:     - 0: On success
 *              - -1: On PCT failure (if MLK_CONFIG_KEYGEN_PCT) is enabled.
 *
 * Specification: Implements @[FIPS203, Algorithm 16, ML-KEM.KeyGen_Internal]
 *
 **************************************************/
MLK_EXTERNAL_API
MLK_MUST_CHECK_RETURN_VALUE
int crypto_kem_keypair_derand(uint8_t pk[MLKEM_INDCCA_PUBLICKEYBYTES],
                              uint8_t sk[MLKEM_INDCCA_SECRETKEYBYTES],
                              const uint8_t coins[2 * MLKEM_SYMBYTES])
__contract__(
  requires(memory_no_alias(pk, MLKEM_INDCCA_PUBLICKEYBYTES))
  requires(memory_no_alias(sk, MLKEM_INDCCA_SECRETKEYBYTES))
  requires(memory_no_alias(coins, 2 * MLKEM_SYMBYTES))
  assigns(object_whole(pk))
  assigns(object_whole(sk))
);

/*************************************************
 * Name:        crypto_kem_keypair
 *
 * Description: Generates public and private key
 *              for CCA-secure ML-KEM key encapsulation mechanism
 *
 * Arguments:   - uint8_t *pk: pointer to output public key
 *                (an already allocated array of MLKEM_INDCCA_PUBLICKEYBYTES
 *                 bytes)
 *              - uint8_t *sk: pointer to output private key
 *                (an already allocated array of MLKEM_INDCCA_SECRETKEYBYTES
 *                 bytes)
 *
 * Returns:     - 0: On success
 *              - -1: On PCT failure (if MLK_CONFIG_KEYGEN_PCT) is enabled.
 *
 * Specification: Implements @[FIPS203, Algorithm 19, ML-KEM.KeyGen]
 *
 **************************************************/
MLK_EXTERNAL_API
MLK_MUST_CHECK_RETURN_VALUE
int crypto_kem_keypair(uint8_t pk[MLKEM_INDCCA_PUBLICKEYBYTES],
                       uint8_t sk[MLKEM_INDCCA_SECRETKEYBYTES])
__contract__(
  requires(memory_no_alias(pk, MLKEM_INDCCA_PUBLICKEYBYTES))
  requires(memory_no_alias(sk, MLKEM_INDCCA_SECRETKEYBYTES))
  assigns(object_whole(pk))
  assigns(object_whole(sk))
);

/*************************************************
 * Name:        crypto_kem_enc_derand
 *
 * Description: Generates cipher text and shared
 *              secret for given public key
 *
 * Arguments:   - uint8_t *ct: pointer to output cipher text
 *                (an already allocated array of MLKEM_INDCCA_CIPHERTEXTBYTES
 *                 bytes)
 *              - uint8_t *ss: pointer to output shared secret
 *                (an already allocated array of MLKEM_SSBYTES bytes)
 *              - const uint8_t *pk: pointer to input public key
 *                (an already allocated array of MLKEM_INDCCA_PUBLICKEYBYTES
 *                 bytes)
 *              - const uint8_t *coins: pointer to input randomness
 *                (an already allocated array filled with MLKEM_SYMBYTES random
 *                 bytes)
 *
 * Returns: - 0 on success
 *          - -1 if the 'modulus check' @[FIPS203, Section 7.2]
 *            for the public key fails.
 *
 * Specification: Implements @[FIPS203, Algorithm 17, ML-KEM.Encaps_Internal]
 *
 **************************************************/
MLK_EXTERNAL_API
MLK_MUST_CHECK_RETURN_VALUE
int crypto_kem_enc_derand(uint8_t ct[MLKEM_INDCCA_CIPHERTEXTBYTES],
                          uint8_t ss[MLKEM_SSBYTES],
                          const uint8_t pk[MLKEM_INDCCA_PUBLICKEYBYTES],
                          const uint8_t coins[MLKEM_SYMBYTES])
__contract__(
  requires(memory_no_alias(ct, MLKEM_INDCCA_CIPHERTEXTBYTES))
  requires(memory_no_alias(ss, MLKEM_SSBYTES))
  requires(memory_no_alias(pk, MLKEM_INDCCA_PUBLICKEYBYTES))
  requires(memory_no_alias(coins, MLKEM_SYMBYTES))
  assigns(object_whole(ct))
  assigns(object_whole(ss))
);

/*************************************************
 * Name:        crypto_kem_enc
 *
 * Description: Generates cipher text and shared
 *              secret for given public key
 *
 * Arguments:   - uint8_t *ct: pointer to output cipher text
 *                (an already allocated array of MLKEM_INDCCA_CIPHERTEXTBYTES
 *                 bytes)
 *              - uint8_t *ss: pointer to output shared secret
 *                (an already allocated array of MLKEM_SSBYTES bytes)
 *              - const uint8_t *pk: pointer to input public key
 *                (an already allocated array of MLKEM_INDCCA_PUBLICKEYBYTES
 *                 bytes)
 *
 * Returns: - 0 on success
 *          - -1 if the 'modulus check' @[FIPS203, Section 7.2]
 *            for the public key fails.
 *
 * Specification: Implements @[FIPS203, Algorithm 20, ML-KEM.Encaps]
 *
 **************************************************/
MLK_EXTERNAL_API
MLK_MUST_CHECK_RETURN_VALUE
int crypto_kem_enc(uint8_t ct[MLKEM_INDCCA_CIPHERTEXTBYTES],
                   uint8_t ss[MLKEM_SSBYTES],
                   const uint8_t pk[MLKEM_INDCCA_PUBLICKEYBYTES])
__contract__(
  requires(memory_no_alias(ct, MLKEM_INDCCA_CIPHERTEXTBYTES))
  requires(memory_no_alias(ss, MLKEM_SSBYTES))
  requires(memory_no_alias(pk, MLKEM_INDCCA_PUBLICKEYBYTES))
  assigns(object_whole(ct))
  assigns(object_whole(ss))
);

/*************************************************
 * Name:        crypto_kem_dec
 *
 * Description: Generates shared secret for given
 *              cipher text and private key
 *
 * Arguments:   - uint8_t *ss: pointer to output shared secret
 *                (an already allocated array of MLKEM_SSBYTES bytes)
 *              - const uint8_t *ct: pointer to input cipher text
 *                (an already allocated array of MLKEM_INDCCA_CIPHERTEXTBYTES
 *                 bytes)
 *              - const uint8_t *sk: pointer to input private key
 *                (an already allocated array of MLKEM_INDCCA_SECRETKEYBYTES
 *                 bytes)
 *
 * Returns: - 0 on success
 *          - -1 if the 'hash check' @[FIPS203, Section 7.3]
 *            for the secret key fails.
 *
 * Specification: Implements @[FIPS203, Algorithm 21, ML-KEM.Decaps]
 *
 **************************************************/
MLK_EXTERNAL_API
MLK_MUST_CHECK_RETURN_VALUE
int crypto_kem_dec(uint8_t ss[MLKEM_SSBYTES],
                   const uint8_t ct[MLKEM_INDCCA_CIPHERTEXTBYTES],
                   const uint8_t sk[MLKEM_INDCCA_SECRETKEYBYTES])
__contract__(
  requires(memory_no_alias(ss, MLKEM_SSBYTES))
  requires(memory_no_alias(ct, MLKEM_INDCCA_CIPHERTEXTBYTES))
  requires(memory_no_alias(sk, MLKEM_INDCCA_SECRETKEYBYTES))
  assigns(object_whole(ss))
);

#endif /* !MLK_KEM_H */
