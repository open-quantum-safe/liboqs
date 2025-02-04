/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Public API for mlkem-native
 *
 * This header defines the public API of a single build of mlkem-native.
 *
 * To use this header, make sure one of the following holds:
 *
 * - The config.h used for the build is available in the include paths.
 * - The values of BUILD_INFO_LVL and BUILD_INFO_NAMESPACE are set, reflecting
 *   the security level (512/768/1024) and namespace of the build.
 *
 * This header specifies a build of mlkem-native for a fixed security level.
 * If you need multiple builds, e.g. to build a library offering multiple
 * security levels, you need multiple instances of this header.
 */

/* NOTE: To use multiple instances of this header, use separate guards. */
#ifndef MLKEM_NATIVE_H
#define MLKEM_NATIVE_H

#include <stdint.h>

/*************************** Build information ********************************/

/*
 * Provide security level (BUILD_INFO_LVL) and namespacing
 * (BUILD_INFO_NAMESPACE)
 *
 * By default, this is extracted from the configuration used for the build,
 * but you can also set it manually to avoid a dependency on the build config.
 */

/* Skip this if BUILD_INFO_LVL has already been set */
#if !defined(BUILD_INFO_LVL)

/* Option 1: Extract from config */
#if defined(MLKEM_NATIVE_CONFIG_FILE)
#include MLKEM_NATIVE_CONFIG_FILE
#else
#include "config.h"
#endif

#if MLKEM_K == 2
#define BUILD_INFO_LVL 512
#elif MLKEM_K == 3
#define BUILD_INFO_LVL 768
#elif MLKEM_K == 4
#define BUILD_INFO_LVL 1024
#else
#error MLKEM_K not set by config file
#endif

#ifndef MLKEM_NAMESPACE_PREFIX
#error MLKEM_NAMESPACE_PREFIX not set by config file
#endif

#if defined(MLKEM_NATIVE_NAMESPACE_PREFIX_ADD_LEVEL)
#define BUILD_INFO_CONCAT3_(x, y, z) x##y##_##z
#define BUILD_INFO_CONCAT3(x, y, z) BUILD_INFO_CONCAT_(x, y, z)
#define BUILD_INFO_NAMESPACE(sym) \
  BUILD_INFO_CONCAT3(MLKEM_NAMESPACE_PREFIX, BUILD_INFO_LVL, sym)
#else
#define BUILD_INFO_CONCAT2_(x, y) x##_##y
#define BUILD_INFO_CONCAT2(x, y) BUILD_INFO_CONCAT2_(x, y)
#define BUILD_INFO_NAMESPACE(sym) \
  BUILD_INFO_CONCAT2(MLKEM_NAMESPACE_PREFIX, sym)
#endif

#endif /* BUILD_INFO_LVL */

/* Option 2: Provide BUILD_INFO_LVL and BUILD_INFO_NAMESPACE manually */

/* #define BUILD_INFO_LVL            ADJUSTME */
/* #define BUILD_INFO_NAMESPACE(sym) ADJUSTME */

/******************************* Key sizes ************************************/

/* Sizes of cryptographic material, per level */
#define MLKEM512_SECRETKEYBYTES 1632
#define MLKEM512_PUBLICKEYBYTES 800
#define MLKEM512_CIPHERTEXTBYTES 768

#define MLKEM768_SECRETKEYBYTES 2400
#define MLKEM768_PUBLICKEYBYTES 1184
#define MLKEM768_CIPHERTEXTBYTES 1088

#define MLKEM1024_SECRETKEYBYTES 3168
#define MLKEM1024_PUBLICKEYBYTES 1568
#define MLKEM1024_CIPHERTEXTBYTES 1568

/* Size of randomness coins in bytes (level-independent) */
#define MLKEM_SYMBYTES 32
#define MLKEM512_SYMBYTES MLKEM_SYMBYTES
#define MLKEM768_SYMBYTES MLKEM_SYMBYTES
#define MLKEM1024_SYMBYTES MLKEM_SYMBYTES
/* Size of shared secret in bytes (level-independent) */
#define MLKEM_BYTES 32
#define MLKEM512_BYTES MLKEM_BYTES
#define MLKEM768_BYTES MLKEM_BYTES
#define MLKEM1024_BYTES MLKEM_BYTES

/* Sizes of cryptographic material, as a function of LVL=512,768,1024 */
#define MLKEM_SECRETKEYBYTES_(LVL) MLKEM##LVL##_SECRETKEYBYTES
#define MLKEM_PUBLICKEYBYTES_(LVL) MLKEM##LVL##_PUBLICKEYBYTES
#define MLKEM_CIPHERTEXTBYTES_(LVL) MLKEM##LVL##_CIPHERTEXTBYTES
#define MLKEM_SECRETKEYBYTES(LVL) MLKEM_SECRETKEYBYTES_(LVL)
#define MLKEM_PUBLICKEYBYTES(LVL) MLKEM_PUBLICKEYBYTES_(LVL)
#define MLKEM_CIPHERTEXTBYTES(LVL) MLKEM_CIPHERTEXTBYTES_(LVL)

/****************************** Function API **********************************/

/*************************************************
 * Name:        crypto_kem_keypair_derand
 *
 * Description: Generates public and private key
 *              for CCA-secure ML-KEM key encapsulation mechanism
 *
 * Arguments:   - uint8_t pk[]: pointer to output public key, an array of
 *                 length MLKEM{512,768,1024}_PUBLICKEYBYTES bytes.
 *              - uint8_t sk[]: pointer to output private key, an array of
 *                  of MLKEM{512,768,1024}_SECRETKEYBYTES bytes.
 *              - uint8_t *coins: pointer to input randomness, an array of
 *                  2*MLKEM_SYMBYTES uniformly random bytes.
 *
 * Returns 0 (success)
 **************************************************/
int BUILD_INFO_NAMESPACE(keypair_derand)(
    uint8_t pk[MLKEM_PUBLICKEYBYTES(BUILD_INFO_LVL)],
    uint8_t sk[MLKEM_SECRETKEYBYTES(BUILD_INFO_LVL)], const uint8_t *coins);

/*************************************************
 * Name:        crypto_kem_keypair
 *
 * Description: Generates public and private key
 *              for CCA-secure ML-KEM key encapsulation mechanism
 *
 * Arguments:   - uint8_t *pk: pointer to output public key, an array of
 *                 MLKEM{512,768,1024}_PUBLICKEYBYTES bytes.
 *              - uint8_t *sk: pointer to output private key, an array of
 *                 MLKEM{512,768,1024}_SECRETKEYBYTES bytes.
 *
 * Returns 0 (success)
 **************************************************/
int BUILD_INFO_NAMESPACE(keypair)(
    uint8_t pk[MLKEM_PUBLICKEYBYTES(BUILD_INFO_LVL)],
    uint8_t sk[MLKEM_SECRETKEYBYTES(BUILD_INFO_LVL)]);

/*************************************************
 * Name:        crypto_kem_enc_derand
 *
 * Description: Generates cipher text and shared
 *              secret for given public key
 *
 * Arguments:   - uint8_t *ct: pointer to output cipher text, an array of
 *                 MLKEM{512,768,1024}_CIPHERTEXTBYTES bytes.
 *              - uint8_t *ss: pointer to output shared secret, an array of
 *                 MLKEM_BYTES bytes.
 *              - const uint8_t *pk: pointer to input public key, an array of
 *                 MLKEM{512,768,1024}_PUBLICKEYBYTES bytes.
 *              - const uint8_t *coins: pointer to input randomness, an array of
 *                 MLKEM_SYMBYTES bytes.
 *
 * Returns 0 on success, and -1 if the public key modulus check (see Section 7.2
 * of FIPS203) fails.
 **************************************************/
int BUILD_INFO_NAMESPACE(enc_derand)(
    uint8_t ct[MLKEM_CIPHERTEXTBYTES(BUILD_INFO_LVL)], uint8_t ss[MLKEM_BYTES],
    const uint8_t pk[MLKEM_PUBLICKEYBYTES(BUILD_INFO_LVL)],
    const uint8_t coins[MLKEM_SYMBYTES]);

/*************************************************
 * Name:        crypto_kem_enc
 *
 * Description: Generates cipher text and shared
 *              secret for given public key
 *
 * Arguments:   - uint8_t *ct: pointer to output cipher text, an array of
 *                 MLKEM{512,768,1024}_CIPHERTEXTBYTES bytes.
 *              - uint8_t *ss: pointer to output shared secret, an array of
 *                 MLKEM_BYTES bytes.
 *              - const uint8_t *pk: pointer to input public key, an array of
 *                 MLKEM{512,768,1024}_PUBLICKEYBYTES bytes.
 *
 * Returns 0 on success, and -1 if the public key modulus check (see Section 7.2
 * of FIPS203) fails.
 **************************************************/
int BUILD_INFO_NAMESPACE(enc)(
    uint8_t ct[MLKEM_CIPHERTEXTBYTES(BUILD_INFO_LVL)], uint8_t ss[MLKEM_BYTES],
    const uint8_t pk[MLKEM_PUBLICKEYBYTES(BUILD_INFO_LVL)]);

/*************************************************
 * Name:        crypto_kem_dec
 *
 * Description: Generates shared secret for given
 *              cipher text and private key
 *
 * Arguments:   - uint8_t *ss: pointer to output shared secret, an array of
 *                 MLKEM_BYTES bytes.
 *              - const uint8_t *ct: pointer to input cipher text, an array of
 *                 MLKEM{512,768,1024}_CIPHERTEXTBYTES bytes.
 *              - const uint8_t *sk: pointer to input private key, an array of
 *                 MLKEM{512,768,1024}_SECRETKEYBYTES bytes.
 *
 * Returns 0 on success, and -1 if the secret key hash check (see Section 7.3 of
 * FIPS203) fails.
 *
 * On failure, ss will contain a pseudo-random value.
 **************************************************/
int BUILD_INFO_NAMESPACE(dec)(
    uint8_t ss[MLKEM_BYTES],
    const uint8_t ct[MLKEM_CIPHERTEXTBYTES(BUILD_INFO_LVL)],
    const uint8_t sk[MLKEM_SECRETKEYBYTES(BUILD_INFO_LVL)]);

/****************************** Standard API *********************************/

/* If desired, export API in CRYPTO_xxx and crypto_kem_xxx format as used
 * e.g. by SUPERCOP and NIST.
 *
 * Remove this if you don't need it, or if you need multiple instances
 * of this header. */

#if !defined(BUILD_INFO_NO_STANDARD_API)
#define CRYPTO_SECRETKEYBYTES MLKEM_SECRETKEYBYTES(BUILD_INFO_LVL)
#define CRYPTO_PUBLICKEYBYTES MLKEM_PUBLICKEYBYTES(BUILD_INFO_LVL)
#define CRYPTO_CIPHERTEXTBYTES MLKEM_CIPHERTEXTBYTES(BUILD_INFO_LVL)

#define CRYPTO_SYMBYTES MLKEM_SYMBYTES
#define CRYPTO_BYTES MLKEM_BYTES

#define crypto_kem_keypair_derand BUILD_INFO_NAMESPACE(keypair_derand)
#define crypto_kem_keypair BUILD_INFO_NAMESPACE(keypair)
#define crypto_kem_enc_derand BUILD_INFO_NAMESPACE(enc_derand)
#define crypto_kem_enc BUILD_INFO_NAMESPACE(enc)
#define crypto_kem_dec BUILD_INFO_NAMESPACE(dec)
#endif /* BUILD_INFO_NO_STANDARD_API */

/********************************* Cleanup ************************************/

/* Unset build information to allow multiple instances of this header.
 * Keep this commented out when using the standard API. */
/* #undef BUILD_INFO_LVL */
/* #undef BUILD_INFO_NAMESPACE */

#endif /* MLKEM_NATIVE_API_H */
