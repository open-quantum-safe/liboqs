/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [FIPS204]
 *   FIPS 204 Module-Lattice-Based Digital Signature Standard
 *   National Institute of Standards and Technology
 *   https://csrc.nist.gov/pubs/fips/204/final
 */

#ifndef MLD_SIGN_H
#define MLD_SIGN_H

#include <stddef.h>
#include <stdint.h>
#include "cbmc.h"
#include "common.h"
#include "poly.h"
#include "polyvec.h"
#include "sys.h"

#if defined(MLD_CHECK_APIS)
/* Include to ensure consistency between internal sign.h
 * and external mldsa_native.h. */
#define MLD_CONFIG_API_NO_SUPERCOP
#include "mldsa_native.h"
#undef MLD_CONFIG_API_NO_SUPERCOP

#if CRYPTO_SECRETKEYBYTES != MLDSA_SECRETKEYBYTES(MLD_CONFIG_API_PARAMETER_SET)
#error Mismatch for SECRETKEYBYTES between sign.h and mldsa_native.h
#endif

#if CRYPTO_PUBLICKEYBYTES != MLDSA_PUBLICKEYBYTES(MLD_CONFIG_API_PARAMETER_SET)
#error Mismatch for PUBLICKEYBYTES between sign.h and mldsa_native.h
#endif

#if CRYPTO_BYTES != MLDSA_BYTES(MLD_CONFIG_API_PARAMETER_SET)
#error Mismatch for CRYPTO_BYTES between sign.h and mldsa_native.h
#endif

#endif /* MLD_CHECK_APIS */

#define crypto_sign_keypair_internal MLD_NAMESPACE_KL(keypair_internal)
#define crypto_sign_keypair MLD_NAMESPACE_KL(keypair)
#define crypto_sign_signature_internal MLD_NAMESPACE_KL(signature_internal)
#define crypto_sign_signature MLD_NAMESPACE_KL(signature)
#define crypto_sign_signature_extmu MLD_NAMESPACE_KL(signature_extmu)
#define crypto_sign MLD_NAMESPACE_KL(sign)
#define crypto_sign_verify_internal MLD_NAMESPACE_KL(verify_internal)
#define crypto_sign_verify MLD_NAMESPACE_KL(verify)
#define crypto_sign_verify_extmu MLD_NAMESPACE_KL(verify_extmu)
#define crypto_sign_open MLD_NAMESPACE_KL(open)
#define crypto_sign_signature_pre_hash_internal \
  MLD_NAMESPACE_KL(signature_pre_hash_internal)
#define crypto_sign_verify_pre_hash_internal \
  MLD_NAMESPACE_KL(verify_pre_hash_internal)
#define crypto_sign_signature_pre_hash_shake256 \
  MLD_NAMESPACE_KL(signature_pre_hash_shake256)
#define crypto_sign_verify_pre_hash_shake256 \
  MLD_NAMESPACE_KL(verify_pre_hash_shake256)

/*************************************************
 * Hash algorithm constants for pre-hash functions
 **************************************************/
#define MLD_PREHASH_SHA2_224 1
#define MLD_PREHASH_SHA2_256 2
#define MLD_PREHASH_SHA2_384 3
#define MLD_PREHASH_SHA2_512 4
#define MLD_PREHASH_SHA2_512_224 5
#define MLD_PREHASH_SHA2_512_256 6
#define MLD_PREHASH_SHA3_224 7
#define MLD_PREHASH_SHA3_256 8
#define MLD_PREHASH_SHA3_384 9
#define MLD_PREHASH_SHA3_512 10
#define MLD_PREHASH_SHAKE_128 11
#define MLD_PREHASH_SHAKE_256 12

/*************************************************
 * Name:        crypto_sign_keypair_internal
 *
 * Description: Generates public and private key. Internal API.
 *              When MLD_CONFIG_KEYGEN_PCT is set, performs a Pairwise
 *              Consistency Test (PCT) as required by FIPS 140-3 IG.
 *
 * Arguments:   - uint8_t pk[CRYPTO_PUBLICKEYBYTES]:   output public key
 *              - uint8_t sk[CRYPTO_SECRETKEYBYTES]:   output private key
 *              - const uint8_t seed[MLDSA_SEEDBYTES]: input random seed
 *
 * Returns 0 (success) or -1 (PCT failure)
 *
 * Specification: Implements @[FIPS204 Algorithm 6 (ML-DSA.KeyGen_internal)]
 *
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_keypair_internal(uint8_t pk[CRYPTO_PUBLICKEYBYTES],
                                 uint8_t sk[CRYPTO_SECRETKEYBYTES],
                                 const uint8_t seed[MLDSA_SEEDBYTES])
__contract__(
  requires(memory_no_alias(pk, CRYPTO_PUBLICKEYBYTES))
  requires(memory_no_alias(sk, CRYPTO_SECRETKEYBYTES))
  requires(memory_no_alias(seed, MLDSA_SEEDBYTES))
  assigns(object_whole(pk))
  assigns(object_whole(sk))
  ensures(return_value == 0 || return_value == -1)
);

/*************************************************
 * Name:        crypto_sign_keypair
 *
 * Description: Generates public and private key.
 *              When MLD_CONFIG_KEYGEN_PCT is set, performs a Pairwise
 *              Consistency Test (PCT) as required by FIPS 140-3 IG.
 *
 * Arguments:   - uint8_t pk[CRYPTO_PUBLICKEYBYTES]: output public key
 *              - uint8_t sk[CRYPTO_SECRETKEYBYTES]: output private key
 *
 * Returns 0 (success) or -1 (PCT failure)
 *
 * Specification: Implements @[FIPS204 Algorithm 1 (ML-DSA.KeyGen)]
 *
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_keypair(uint8_t pk[CRYPTO_PUBLICKEYBYTES],
                        uint8_t sk[CRYPTO_SECRETKEYBYTES])
__contract__(
  requires(memory_no_alias(pk, CRYPTO_PUBLICKEYBYTES))
  requires(memory_no_alias(sk, CRYPTO_SECRETKEYBYTES))
  assigns(object_whole(pk))
  assigns(object_whole(sk))
  ensures(return_value == 0 || return_value == -1)
);

/*************************************************
 * Name:        crypto_sign_signature_internal
 *
 * Description: Computes signature. Internal API.
 *
 * Arguments:   - uint8_t sig[CRYPTO_BYTES]: output signature
 *              - size_t *siglen:            pointer to output length of
 *                                           signature
 *              - const uint8_t *m:          pointer to message to be signed
 *              - size_t mlen:               length of message
 *              - const uint8_t *pre:        pointer to prefix string
 *              - size_t prelen:             length of prefix string
 *              - const uint8_t rnd[MLDSA_RNDBYTES]:
 *                                           random seed
 *              - const uint8_t sk[CRYPTO_SECRETKEYBYTES]:
 *                                           bit-packed secret key
 *              - int externalmu:            indicates input message m is
 *                                           processed as mu
 *
 * Returns 0 (success) or -1 (indicating nonce exhaustion)
 *
 * If the returned value is -1, then the values of *sig and
 * *siglen should not be referenced.
 *
 * Reference: This code differs from the reference implementation
 *            in that it adds an explicit check for nonce exhaustion
 *            and can return -1 in that case.
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature_internal(uint8_t sig[CRYPTO_BYTES], size_t *siglen,
                                   const uint8_t *m, size_t mlen,
                                   const uint8_t *pre, size_t prelen,
                                   const uint8_t rnd[MLDSA_RNDBYTES],
                                   const uint8_t sk[CRYPTO_SECRETKEYBYTES],
                                   int externalmu)
__contract__(
  requires(mlen <= MLD_MAX_BUFFER_SIZE)
  requires(prelen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sig, CRYPTO_BYTES))
  requires(memory_no_alias(siglen, sizeof(size_t)))
  requires(memory_no_alias(m, mlen))
  requires(memory_no_alias(rnd, MLDSA_RNDBYTES))
  requires(memory_no_alias(sk, CRYPTO_SECRETKEYBYTES))
  requires((externalmu == 0 && (prelen == 0 || memory_no_alias(pre, prelen))) ||
           (externalmu == 1 && mlen == MLDSA_CRHBYTES))
  assigns(memory_slice(sig, CRYPTO_BYTES))
  assigns(object_whole(siglen))
  ensures((return_value == 0 && *siglen == CRYPTO_BYTES) ||
          (return_value == -1 && *siglen == 0))
);

/*************************************************
 * Name:        crypto_sign_signature
 *
 * Description: Computes signature. This function implements the randomized
 *              variant of ML-DSA. If you require the deterministic variant,
 *              use crypto_sign_signature_internal directly.
 *
 * Arguments:   - uint8_t sig[CRYPTO_BYTES]: output signature
 *              - size_t *siglen:            pointer to output length of
 *                                           signature
 *              - const uint8_t *m:          pointer to message to be signed
 *              - size_t mlen:               length of message
 *              - uint8_t *ctx:              pointer to context string.
 *                                           May be NULL if ctxlen == 0.
 *              - size_t ctxlen:             length of context string.
 *                                           Should be <= 255.
 *              - const uint8_t sk[CRYPTO_SECRETKEYBYTES]:
 *                                           bit-packed secret key
 *
 * Returns 0 (success) or -1 (context string too long OR nonce exhaustion)
 *
 * Specification: Implements @[FIPS204 Algorithm 2 (ML-DSA.Sign)].
 *
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature(uint8_t sig[CRYPTO_BYTES], size_t *siglen,
                          const uint8_t *m, size_t mlen, const uint8_t *ctx,
                          size_t ctxlen,
                          const uint8_t sk[CRYPTO_SECRETKEYBYTES])
__contract__(
  requires(mlen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sig, CRYPTO_BYTES))
  requires(memory_no_alias(siglen, sizeof(size_t)))
  requires(memory_no_alias(m, mlen))
  requires(ctxlen <= MLD_MAX_BUFFER_SIZE)
  requires(ctxlen == 0 || memory_no_alias(ctx, ctxlen))
  requires(memory_no_alias(sk, CRYPTO_SECRETKEYBYTES))
  assigns(memory_slice(sig, CRYPTO_BYTES))
  assigns(object_whole(siglen))
  ensures((return_value == 0 && *siglen == CRYPTO_BYTES) ||
          (return_value == -1 && *siglen == 0))
);

/*************************************************
 * Name:        crypto_sign_signature_extmu
 *
 * Description: Computes signature. This function implements the randomized
 *              variant of ML-DSA. If you require the deterministic variant,
 *              use crypto_sign_signature_internal directly.
 *
 * Arguments:   - uint8_t sig[CRYPTO_BYTES]: output signature
 *              - size_t *siglen:            pointer to output length of
 *                                           signature
 *              - const uint8_t mu[MLDSA_CRHBYTES]:
 *                                           input mu to be signed
 *              - const uint8_t sk[CRYPTO_SECRETKEYBYTES]:
 *                                           bit-packed secret key
 *
 * Returns 0 (success) or -1 (context string too long OR nonce exhaustion)
 *
 * Specification: Implements @[FIPS204 Algorithm 2 (ML-DSA.Sign external mu
 *                variant)]
 *
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature_extmu(uint8_t sig[CRYPTO_BYTES], size_t *siglen,
                                const uint8_t mu[MLDSA_CRHBYTES],
                                const uint8_t sk[CRYPTO_SECRETKEYBYTES])
__contract__(
  requires(memory_no_alias(sig, CRYPTO_BYTES))
  requires(memory_no_alias(siglen, sizeof(size_t)))
  requires(memory_no_alias(mu, MLDSA_CRHBYTES))
  requires(memory_no_alias(sk, CRYPTO_SECRETKEYBYTES))
  assigns(memory_slice(sig, CRYPTO_BYTES))
  assigns(object_whole(siglen))
  ensures((return_value == 0 && *siglen == CRYPTO_BYTES) ||
          (return_value == -1 && *siglen == 0))
);

/*************************************************
 * Name:        crypto_sign
 *
 * Description: Compute signed message.
 *
 * Arguments:   - uint8_t *sm:        pointer to output signed message
 *                                    (allocated array with CRYPTO_BYTES + mlen
 *                                    bytes), can be equal to m
 *              - size_t *smlen:      pointer to output length of signed message
 *              - const uint8_t *m:   pointer to message to be signed
 *              - size_t mlen:        length of message
 *              - const uint8_t *ctx: pointer to context string
 *              - size_t ctxlen:      length of context string
 *              - const uint8_t sk[CRYPTO_SECRETKEYBYTES]:
 *                                    bit-packed secret key
 *
 * Returns 0 (success) or -1 (context string too long OR nonce exhausted)
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign(uint8_t *sm, size_t *smlen, const uint8_t *m, size_t mlen,
                const uint8_t *ctx, size_t ctxlen,
                const uint8_t sk[CRYPTO_SECRETKEYBYTES])
__contract__(
  requires(mlen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sm, CRYPTO_BYTES + mlen))
  requires(memory_no_alias(smlen, sizeof(size_t)))
  requires(m == sm || memory_no_alias(m, mlen))
  requires(ctxlen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(ctx, ctxlen))
  requires(memory_no_alias(sk, CRYPTO_SECRETKEYBYTES))
  assigns(memory_slice(sm, CRYPTO_BYTES + mlen))
  assigns(object_whole(smlen))
  ensures((return_value == 0 && *smlen == CRYPTO_BYTES + mlen) ||
          (return_value == -1))
);

/*************************************************
 * Name:        crypto_sign_verify_internal
 *
 * Description: Verifies signature. Internal API.
 *
 * Arguments:   - const uint8_t *sig: pointer to input signature
 *              - size_t siglen:      length of signature
 *              - const uint8_t *m:   pointer to message
 *              - size_t mlen:        length of message
 *              - const uint8_t *pre: pointer to prefix string
 *              - size_t prelen:      length of prefix string
 *              - const uint8_t pk[CRYPTO_PUBLICKEYBYTES]:
 *                                    bit-packed public key
 *              - int externalmu:     indicates input message m is processed as
 *                                    mu
 *
 * Returns 0 if signature could be verified correctly and -1 otherwise
 *
 * Specification: Implements @[FIPS204 Algorithm 8 (ML-DSA.Verify_internal)]
 *
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify_internal(const uint8_t *sig, size_t siglen,
                                const uint8_t *m, size_t mlen,
                                const uint8_t *pre, size_t prelen,
                                const uint8_t pk[CRYPTO_PUBLICKEYBYTES],
                                int externalmu)
__contract__(
  requires(prelen <= MLD_MAX_BUFFER_SIZE)
  requires(mlen <= MLD_MAX_BUFFER_SIZE)
  requires(siglen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sig, siglen))
  requires(memory_no_alias(m, mlen))
  requires(externalmu == 0 || (externalmu == 1 && mlen == MLDSA_CRHBYTES))
  requires(externalmu == 1 || prelen == 0 || memory_no_alias(pre, prelen))
  requires(memory_no_alias(pk, CRYPTO_PUBLICKEYBYTES))
  ensures(return_value == 0 || return_value == -1)
);

/*************************************************
 * Name:        crypto_sign_verify
 *
 * Description: Verifies signature.
 *
 * Arguments:   - const uint8_t *sig: pointer to input signature
 *              - size_t siglen:      length of signature
 *              - const uint8_t *m:   pointer to message
 *              - size_t mlen:        length of message
 *              - const uint8_t *ctx: pointer to context string.
 *                                    May be NULL if ctxlen == 0.
 *              - size_t ctxlen:      length of context string
 *              - const uint8_t pk[CRYPTO_PUBLICKEYBYTES]:
 *                                    bit-packed public key
 *
 * Returns 0 if signature could be verified correctly and -1 otherwise
 *
 * Specification: Implements @[FIPS204 Algorithm 3 (ML-DSA.Verify)]
 *
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m,
                       size_t mlen, const uint8_t *ctx, size_t ctxlen,
                       const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
__contract__(
  requires(mlen <= MLD_MAX_BUFFER_SIZE)
  requires(siglen <= MLD_MAX_BUFFER_SIZE)
  requires(ctxlen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sig, siglen))
  requires(memory_no_alias(m, mlen))
  requires(ctxlen == 0 || memory_no_alias(ctx, ctxlen))
  requires(memory_no_alias(pk, CRYPTO_PUBLICKEYBYTES))
  ensures(return_value == 0 || return_value == -1)
);

/*************************************************
 * Name:        crypto_sign_verify_extmu
 *
 * Description: Verifies signature.
 *
 * Arguments:   - const uint8_t *sig: pointer to input signature
 *              - size_t siglen:      length of signature
 *              - const uint8_t mu[MLDSA_CRHBYTES]:
 *                                    input mu
 *              - const uint8_t pk[CRYPTO_PUBLICKEYBYTES]:
 *                                    bit-packed public key
 *
 * Returns 0 if signature could be verified correctly and -1 otherwise
 *
 * Specification: Implements @[FIPS204 Algorithm 3 (ML-DSA.Verify external mu
 *                variant)]
 *
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify_extmu(const uint8_t *sig, size_t siglen,
                             const uint8_t mu[MLDSA_CRHBYTES],
                             const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
__contract__(
  requires(siglen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sig, siglen))
  requires(memory_no_alias(mu, MLDSA_CRHBYTES))
  requires(memory_no_alias(pk, CRYPTO_PUBLICKEYBYTES))
  ensures(return_value == 0 || return_value == -1)
);

/*************************************************
 * Name:        crypto_sign_open
 *
 * Description: Verify signed message.
 *
 * Arguments:   - uint8_t *m:         pointer to output message (allocated array
 *                                    with smlen bytes), can be equal to sm
 *              - size_t *mlen:       pointer to output length of message
 *              - const uint8_t *sm:  pointer to signed message
 *              - size_t smlen:       length of signed message
 *              - const uint8_t *ctx: pointer to context tring
 *              - size_t ctxlen:      length of context string
 *              - const uint8_t pk[CRYPTO_PUBLICKEYBYTES]:
 *                                    bit-packed public key
 *
 * Returns 0 if signed message could be verified correctly and -1 otherwise
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen,
                     const uint8_t *ctx, size_t ctxlen,
                     const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
__contract__(
  requires(smlen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(m, smlen))
  requires(memory_no_alias(mlen, sizeof(size_t)))
  requires(m == sm || memory_no_alias(sm, smlen))
  requires(ctxlen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(ctx, ctxlen))
  requires(memory_no_alias(pk, CRYPTO_PUBLICKEYBYTES))
  assigns(memory_slice(m, smlen))
  assigns(memory_slice(mlen, sizeof(size_t)))
  ensures(return_value == 0 || return_value == -1)
);

/*************************************************
 * Name:        crypto_sign_signature_pre_hash_internal
 *
 * Description: FIPS 204: Algorithm 4 HashML-DSA.Sign.
 *              Computes signature with pre-hashed message.
 *
 * Arguments:   - uint8_t sig[CRYPTO_BYTES]:
 *                                        output signature
 *              - size_t *siglen:         pointer to output length of signature
 *              - const uint8_t *ph:      pointer to pre-hashed message
 *              - size_t phlen:           length of pre-hashed message
 *              - const uint8_t *ctx:     pointer to context string
 *              - size_t ctxlen:          length of context string
 *              - const uint8_t rnd[MLDSA_RNDBYTES]:
 *                                        random seed
 *              - const uint8_t sk[CRYPTO_SECRETKEYBYTES]:
 *                                        bit-packed secret key
 *              - int hashalg:            hash algorithm constant (one of
 *                                        MLD_PREHASH_*)
 *
 * Supported hash algorithm constants:
 *   MLD_PREHASH_SHA2_224, MLD_PREHASH_SHA2_256, MLD_PREHASH_SHA2_384,
 *   MLD_PREHASH_SHA2_512, MLD_PREHASH_SHA2_512_224, MLD_PREHASH_SHA2_512_256,
 *   MLD_PREHASH_SHA3_224, MLD_PREHASH_SHA3_256, MLD_PREHASH_SHA3_384,
 *   MLD_PREHASH_SHA3_512, MLD_PREHASH_SHAKE_128, MLD_PREHASH_SHAKE_256
 *
 * Warning: This is an unstable API that may change in the future. If you need
 * a stable API use crypto_sign_signature_pre_hash_shake256.
 *
 * Returns 0 (success) or -1 (context string too long OR invalid phlen OR nonce
 * exhaustion)
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature_pre_hash_internal(
    uint8_t sig[CRYPTO_BYTES], size_t *siglen, const uint8_t *ph, size_t phlen,
    const uint8_t *ctx, size_t ctxlen, const uint8_t rnd[MLDSA_RNDBYTES],
    const uint8_t sk[CRYPTO_SECRETKEYBYTES], int hashalg)
__contract__(
  requires(ctxlen <= MLD_MAX_BUFFER_SIZE)
  requires(phlen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sig, CRYPTO_BYTES))
  requires(memory_no_alias(siglen, sizeof(size_t)))
  requires(memory_no_alias(ph, phlen))
  requires(ctxlen == 0 || memory_no_alias(ctx, ctxlen))
  requires(memory_no_alias(rnd, MLDSA_RNDBYTES))
  requires(memory_no_alias(sk, CRYPTO_SECRETKEYBYTES))
  assigns(memory_slice(sig, CRYPTO_BYTES))
  assigns(object_whole(siglen))
  ensures((return_value == 0 && *siglen == CRYPTO_BYTES) ||
          (return_value == -1 && *siglen == 0))
);

/*************************************************
 * Name:        crypto_sign_verify_pre_hash_internal
 *
 * Description: FIPS 204: Algorithm 5 HashML-DSA.Verify.
 *              Verifies signature with pre-hashed message.
 *
 * Arguments:   - const uint8_t *sig:     pointer to input signature
 *              - size_t siglen:          length of signature
 *              - const uint8_t *ph:      pointer to pre-hashed message
 *              - size_t phlen:           length of pre-hashed message
 *              - const uint8_t *ctx:     pointer to context string
 *              - size_t ctxlen:          length of context string
 *              - const uint8_t pk[CRYPTO_PUBLICKEYBYTES]:
 *                                        bit-packed public key
 *              - int hashalg:            hash algorithm constant (one of
 *                                        MLD_PREHASH_*)
 *
 * Supported hash algorithm constants:
 *   MLD_PREHASH_SHA2_224, MLD_PREHASH_SHA2_256, MLD_PREHASH_SHA2_384,
 *   MLD_PREHASH_SHA2_512, MLD_PREHASH_SHA2_512_224, MLD_PREHASH_SHA2_512_256,
 *   MLD_PREHASH_SHA3_224, MLD_PREHASH_SHA3_256, MLD_PREHASH_SHA3_384,
 *   MLD_PREHASH_SHA3_512, MLD_PREHASH_SHAKE_128, MLD_PREHASH_SHAKE_256
 *
 * Warning: This is an unstable API that may change in the future. If you need
 * a stable API use crypto_sign_verify_pre_hash_shake256.
 *
 * Returns 0 if signature could be verified correctly and -1 otherwise
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify_pre_hash_internal(
    const uint8_t *sig, size_t siglen, const uint8_t *ph, size_t phlen,
    const uint8_t *ctx, size_t ctxlen, const uint8_t pk[CRYPTO_PUBLICKEYBYTES],
    int hashalg)
__contract__(
  requires(phlen <= MLD_MAX_BUFFER_SIZE)
  requires(ctxlen <= MLD_MAX_BUFFER_SIZE - 77)
  requires(siglen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sig, siglen))
  requires(memory_no_alias(ph, phlen))
  requires(ctxlen == 0 || memory_no_alias(ctx, ctxlen))
  requires(memory_no_alias(pk, CRYPTO_PUBLICKEYBYTES))
  ensures(return_value == 0 || return_value == -1)
);

/*************************************************
 * Name:        crypto_sign_signature_pre_hash_shake256
 *
 * Description: FIPS 204: Algorithm 4 HashML-DSA.Sign with SHAKE256.
 *              Computes signature with pre-hashed message using SHAKE256.
 *              This function computes the SHAKE256 hash of the message
 *internally.
 *
 * Arguments:   - uint8_t sig[CRYPTO_BYTES]:
 *                                    output signature
 *              - size_t *siglen:     pointer to output length of signature
 *              - const uint8_t *m:   pointer to message to be hashed and signed
 *              - size_t mlen:        length of message
 *              - const uint8_t *ctx: pointer to context string
 *              - size_t ctxlen:      length of context string
 *              - const uint8_t rnd[MLDSA_RNDBYTES]:
 *                                    random seed
 *              - const uint8_t sk[CRYPTO_SECRETKEYBYTES]:
 *                                    bit-packed secret key
 *
 * Returns 0 (success) or -1 (context string too long OR nonce exhaustion)
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_signature_pre_hash_shake256(
    uint8_t sig[CRYPTO_BYTES], size_t *siglen, const uint8_t *m, size_t mlen,
    const uint8_t *ctx, size_t ctxlen, const uint8_t rnd[MLDSA_RNDBYTES],
    const uint8_t sk[CRYPTO_SECRETKEYBYTES])
__contract__(
  requires(mlen <= MLD_MAX_BUFFER_SIZE)
  requires(ctxlen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sig, CRYPTO_BYTES))
  requires(memory_no_alias(siglen, sizeof(size_t)))
  requires(memory_no_alias(m, mlen))
  requires(ctxlen == 0 || memory_no_alias(ctx, ctxlen))
  requires(memory_no_alias(rnd, MLDSA_RNDBYTES))
  requires(memory_no_alias(sk, CRYPTO_SECRETKEYBYTES))
  assigns(memory_slice(sig, CRYPTO_BYTES))
  assigns(object_whole(siglen))
  ensures((return_value == 0 && *siglen == CRYPTO_BYTES) ||
          (return_value == -1 && *siglen == 0))
);

/*************************************************
 * Name:        crypto_sign_verify_pre_hash_shake256
 *
 * Description: FIPS 204: Algorithm 5 HashML-DSA.Verify with SHAKE256.
 *              Verifies signature with pre-hashed message using SHAKE256.
 *              This function computes the SHAKE256 hash of the message
 *              internally.
 *
 * Arguments:   - const uint8_t *sig: pointer to input signature
 *              - size_t siglen:      length of signature
 *              - const uint8_t *m:   pointer to message to be hashed and
 *                                    verified
 *              - size_t mlen:        length of message
 *              - const uint8_t *ctx: pointer to context string
 *              - size_t ctxlen:      length of context string
 *              - const uint8_t pk[CRYPTO_PUBLICKEYBYTES]:
 *                                    bit-packed public key
 *
 * Returns 0 if signature could be verified correctly and -1 otherwise
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_EXTERNAL_API
int crypto_sign_verify_pre_hash_shake256(
    const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen,
    const uint8_t *ctx, size_t ctxlen, const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
__contract__(
  requires(mlen <= MLD_MAX_BUFFER_SIZE)
  requires(ctxlen <= MLD_MAX_BUFFER_SIZE - 77)
  requires(siglen <= MLD_MAX_BUFFER_SIZE)
  requires(memory_no_alias(sig, siglen))
  requires(memory_no_alias(m, mlen))
  requires(ctxlen == 0 || memory_no_alias(ctx, ctxlen))
  requires(memory_no_alias(pk, CRYPTO_PUBLICKEYBYTES))
  ensures(return_value == 0 || return_value == -1)
);

#endif /* !MLD_SIGN_H */
