/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef POLY_K_H
#define POLY_K_H

#include <stdint.h>
#include "common.h"
#include "compress.h"
#include "poly.h"

#define polyvec MLKEM_NAMESPACE_K(polyvec)
typedef struct
{
  poly vec[MLKEM_K];
} ALIGN polyvec;

#define polyvec_mulcache MLKEM_NAMESPACE_K(polyvec_mulcache)
typedef struct
{
  poly_mulcache vec[MLKEM_K];
} polyvec_mulcache;

#define poly_compress_du MLKEM_NAMESPACE_K(poly_compress_du)
/*************************************************
 * Name:        poly_compress_du
 *
 * Description: Compression (du bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                  (of length MLKEM_POLYCOMPRESSEDBYTES_DU bytes)
 *              - const poly *a: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
static INLINE void poly_compress_du(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_DU],
                                    const poly *a)
__contract__(
  requires(memory_no_alias(r, MLKEM_POLYCOMPRESSEDBYTES_DU))
  requires(memory_no_alias(a, sizeof(poly)))
  requires(array_bound(a->coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  assigns(memory_slice(r, MLKEM_POLYCOMPRESSEDBYTES_DU)))
{
#if MLKEM_DU == 10
  poly_compress_d10(r, a);
#elif MLKEM_DU == 11
  poly_compress_d11(r, a);
#else
#error "Invalid value of MLKEM_DU"
#endif
}

#define poly_decompress_du MLKEM_NAMESPACE_K(poly_decompress_du)
/*************************************************
 * Name:        poly_decompress_du
 *
 * Description: De-serialization and subsequent decompression (du bits) of a
 *              polynomial; approximate inverse of poly_compress_du
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_DU bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
static INLINE void poly_decompress_du(
    poly *r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_DU])
__contract__(
  requires(memory_no_alias(a, MLKEM_POLYCOMPRESSEDBYTES_DU))
  requires(memory_no_alias(r, sizeof(poly)))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_bound(r->coeffs, 0, MLKEM_N, 0, MLKEM_Q)))
{
#if MLKEM_DU == 10
  poly_decompress_d10(r, a);
#elif MLKEM_DU == 11
  poly_decompress_d11(r, a);
#else
#error "Invalid value of MLKEM_DU"
#endif
}

#define poly_compress_dv MLKEM_NAMESPACE_K(poly_compress_dv)
/*************************************************
 * Name:        poly_compress_dv
 *
 * Description: Compression (dv bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                  (of length MLKEM_POLYCOMPRESSEDBYTES_DV bytes)
 *              - const poly *a: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
static INLINE void poly_compress_dv(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_DV],
                                    const poly *a)
__contract__(
  requires(memory_no_alias(r, MLKEM_POLYCOMPRESSEDBYTES_DV))
  requires(memory_no_alias(a, sizeof(poly)))
  requires(array_bound(a->coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  assigns(object_whole(r)))
{
#if MLKEM_DV == 4
  poly_compress_d4(r, a);
#elif MLKEM_DV == 5
  poly_compress_d5(r, a);
#else
#error "Invalid value of MLKEM_DV"
#endif
}


#define poly_decompress_dv MLKEM_NAMESPACE_K(poly_decompress_dv)
/*************************************************
 * Name:        poly_decompress_dv
 *
 * Description: De-serialization and subsequent decompression (dv bits) of a
 *              polynomial; approximate inverse of poly_compress
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                  (of length MLKEM_POLYCOMPRESSEDBYTES_DV bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
static INLINE void poly_decompress_dv(
    poly *r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_DV])
__contract__(
  requires(memory_no_alias(a, MLKEM_POLYCOMPRESSEDBYTES_DV))
  requires(memory_no_alias(r, sizeof(poly)))
  assigns(object_whole(r))
  ensures(array_bound(r->coeffs, 0, MLKEM_N, 0, MLKEM_Q)))
{
#if MLKEM_DV == 4
  poly_decompress_d4(r, a);
#elif MLKEM_DV == 5
  poly_decompress_d5(r, a);
#else
#error "Invalid value of MLKEM_DV"
#endif
}

#define polyvec_compress_du MLKEM_NAMESPACE_K(polyvec_compress_du)
/*************************************************
 * Name:        polyvec_compress_du
 *
 * Description: Compress and serialize vector of polynomials
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                            (needs space for MLKEM_POLYVECCOMPRESSEDBYTES_DU)
 *              - const polyvec *a: pointer to input vector of polynomials.
 *                                  Coefficients must be unsigned canonical,
 *                                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_compress_du(uint8_t r[MLKEM_POLYVECCOMPRESSEDBYTES_DU],
                         const polyvec *a)
__contract__(
  requires(memory_no_alias(r, MLKEM_POLYVECCOMPRESSEDBYTES_DU))
  requires(memory_no_alias(a, sizeof(polyvec)))
  requires(forall(k0, 0, MLKEM_K,
         array_bound(a->vec[k0].coeffs, 0, MLKEM_N, 0, MLKEM_Q)))
  assigns(object_whole(r))
);

#define polyvec_decompress_du MLKEM_NAMESPACE_K(polyvec_decompress_du)
/*************************************************
 * Name:        polyvec_decompress_du
 *
 * Description: De-serialize and decompress vector of polynomials;
 *              approximate inverse of polyvec_compress_du
 *
 * Arguments:   - polyvec *r:       pointer to output vector of polynomials.
 *                Output will have coefficients normalized to [0,..,q-1].
 *              - const uint8_t *a: pointer to input byte array
 *                                  (of length MLKEM_POLYVECCOMPRESSEDBYTES_DU)
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_decompress_du(polyvec *r,
                           const uint8_t a[MLKEM_POLYVECCOMPRESSEDBYTES_DU])
__contract__(
  requires(memory_no_alias(a, MLKEM_POLYVECCOMPRESSEDBYTES_DU))
  requires(memory_no_alias(r, sizeof(polyvec)))
  assigns(object_whole(r))
  ensures(forall(k0, 0, MLKEM_K,
         array_bound(r->vec[k0].coeffs, 0, MLKEM_N, 0, MLKEM_Q)))
);

#define polyvec_tobytes MLKEM_NAMESPACE_K(polyvec_tobytes)
/*************************************************
 * Name:        polyvec_tobytes
 *
 * Description: Serialize vector of polynomials
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                            (needs space for MLKEM_POLYVECBYTES)
 *              - const polyvec *a: pointer to input vector of polynomials
 *                  Each polynomial must have coefficients in [0,..,q-1].
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_tobytes(uint8_t r[MLKEM_POLYVECBYTES], const polyvec *a)
__contract__(
  requires(memory_no_alias(a, sizeof(polyvec)))
  requires(memory_no_alias(r, MLKEM_POLYVECBYTES))
  requires(forall(k0, 0, MLKEM_K,
         array_bound(a->vec[k0].coeffs, 0, MLKEM_N, 0, MLKEM_Q)))
  assigns(object_whole(r))
);

#define polyvec_frombytes MLKEM_NAMESPACE_K(polyvec_frombytes)
/*************************************************
 * Name:        polyvec_frombytes
 *
 * Description: De-serialize vector of polynomials;
 *              inverse of polyvec_tobytes
 *
 * Arguments:   - const polyvec *a: pointer to output vector of polynomials
 *                 (of length MLKEM_POLYVECBYTES). Output will have coefficients
 *                 normalized in [0..4095].
 *              - uint8_t *r: pointer to input byte array
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_frombytes(polyvec *r, const uint8_t a[MLKEM_POLYVECBYTES])
__contract__(
  requires(memory_no_alias(r, sizeof(polyvec)))
  requires(memory_no_alias(a, MLKEM_POLYVECBYTES))
  assigns(object_whole(r))
  ensures(forall(k0, 0, MLKEM_K,
        array_bound(r->vec[k0].coeffs, 0, MLKEM_N, 0, UINT12_LIMIT)))
);

#define polyvec_ntt MLKEM_NAMESPACE_K(polyvec_ntt)
/*************************************************
 * Name:        polyvec_ntt
 *
 * Description: Apply forward NTT to all elements of a vector of polynomials.
 *
 *              The input is assumed to be in normal order and
 *              coefficient-wise bound by MLKEM_Q in absolute value.
 *
 *              The output polynomial is in bitreversed order, and
 *              coefficient-wise bound by NTT_BOUND in absolute value.
 *
 * Arguments:   - polyvec *r: pointer to in/output vector of polynomials
 *
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_ntt(polyvec *r)
__contract__(
  requires(memory_no_alias(r, sizeof(polyvec)))
  requires(forall(j, 0, MLKEM_K,
  array_abs_bound(r->vec[j].coeffs, 0, MLKEM_N, MLKEM_Q)))
  assigns(object_whole(r))
  ensures(forall(j, 0, MLKEM_K,
  array_abs_bound(r->vec[j].coeffs, 0, MLKEM_N, NTT_BOUND)))
);

#define polyvec_invntt_tomont MLKEM_NAMESPACE_K(polyvec_invntt_tomont)
/*************************************************
 * Name:        polyvec_invntt_tomont
 *
 * Description: Apply inverse NTT to all elements of a vector of polynomials
 *              and multiply by Montgomery factor 2^16
 *
 *              The input is assumed to be in bitreversed order, and can
 *              have arbitrary coefficients in int16_t.
 *
 *              The output polynomial is in normal order, and
 *              coefficient-wise bound by INVNTT_BOUND in absolute value.
 *
 *
 * Arguments:   - polyvec *r: pointer to in/output vector of polynomials
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_invntt_tomont(polyvec *r)
__contract__(
  requires(memory_no_alias(r, sizeof(polyvec)))
  assigns(object_whole(r))
  ensures(forall(j, 0, MLKEM_K,
  array_abs_bound(r->vec[j].coeffs, 0, MLKEM_N, INVNTT_BOUND)))
);

#define polyvec_basemul_acc_montgomery \
  MLKEM_NAMESPACE_K(polyvec_basemul_acc_montgomery)
/*************************************************
 * Name:        polyvec_basemul_acc_montgomery
 *
 * Description: Multiply elements of a and b in NTT domain, accumulate into r,
 *              and multiply by 2^-16.
 *
 * Arguments: - poly *r: pointer to output polynomial
 *            - const polyvec *a: pointer to first input vector of polynomials
 *            - const polyvec *b: pointer to second input vector of polynomials
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_basemul_acc_montgomery(poly *r, const polyvec *a, const polyvec *b)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(a, sizeof(polyvec)))
  requires(memory_no_alias(b, sizeof(polyvec)))
  requires(forall(k1, 0, MLKEM_K,
    array_bound(a->vec[k1].coeffs, 0, MLKEM_N, 0, UINT12_LIMIT)))
  assigns(memory_slice(r, sizeof(poly)))
);


#define polyvec_basemul_acc_montgomery_cached \
  MLKEM_NAMESPACE_K(polyvec_basemul_acc_montgomery_cached)
/*************************************************
 * Name:        polyvec_basemul_acc_montgomery_cached
 *
 * Description: Scalar product of two vectors of polynomials in NTT domain,
 *              using mulcache for second operand.
 *
 *              Bounds:
 *              - Every coefficient of a is assumed to be in [0..4095]
 *              - No bounds guarantees for the coefficients in the result.
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const polyvec *a: pointer to first input polynomial vector
 *              - const polyvec *b: pointer to second input polynomial vector
 *              - const polyvec_mulcache *b_cache: pointer to mulcache
 *                  for second input polynomial vector. Can be computed
 *                  via polyvec_mulcache_compute().
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_basemul_acc_montgomery_cached(poly *r, const polyvec *a,
                                           const polyvec *b,
                                           const polyvec_mulcache *b_cache)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(a, sizeof(polyvec)))
  requires(memory_no_alias(b, sizeof(polyvec)))
  requires(memory_no_alias(b_cache, sizeof(polyvec_mulcache)))
  requires(forall(k1, 0, MLKEM_K,
     array_bound(a->vec[k1].coeffs, 0, MLKEM_N, 0, UINT12_LIMIT)))
  assigns(memory_slice(r, sizeof(poly)))
);

#define polyvec_mulcache_compute MLKEM_NAMESPACE_K(polyvec_mulcache_compute)
/************************************************************
 * Name: polyvec_mulcache_compute
 *
 * Description: Computes the mulcache for a vector of polynomials in NTT domain
 *
 *              The mulcache of a degree-2 polynomial b := b0 + b1*X
 *              in Fq[X]/(X^2-zeta) is the value b1*zeta, needed when
 *              computing products of b in Fq[X]/(X^2-zeta).
 *
 *              The mulcache of a polynomial in NTT domain -- which is
 *              a 128-tuple of degree-2 polynomials in Fq[X]/(X^2-zeta),
 *              for varying zeta, is the 128-tuple of mulcaches of those
 *              polynomials.
 *
 *              The mulcache of a vector of polynomials is the vector
 *              of mulcaches of its entries.
 *
 * Arguments: - x: Pointer to mulcache to be populated
 *            - a: Pointer to input polynomial vector
 ************************************************************/
/*
 * NOTE: The default C implementation of this function populates
 * the mulcache with values in (-q,q), but this is not needed for the
 * higher level safety proofs, and thus not part of the spec.
 */
MLKEM_NATIVE_INTERNAL_API
void polyvec_mulcache_compute(polyvec_mulcache *x, const polyvec *a)
__contract__(
  requires(memory_no_alias(x, sizeof(polyvec_mulcache)))
  requires(memory_no_alias(a, sizeof(polyvec)))
  assigns(object_whole(x))
);

#define polyvec_reduce MLKEM_NAMESPACE_K(polyvec_reduce)
/*************************************************
 * Name:        polyvec_reduce
 *
 * Description: Applies Barrett reduction to each coefficient
 *              of each element of a vector of polynomials;
 *              for details of the Barrett reduction see comments in reduce.c
 *
 * Arguments:   - polyvec *r: pointer to input/output polynomial
 **************************************************/
/*
 * NOTE: The semantics of polyvec_reduce() is different in
 *       the reference implementation, which requires
 *       signed canonical output data. Unsigned canonical
 *       outputs are better suited to the only remaining
 *       use of poly_reduce() in the context of (de)serialization.
 */
MLKEM_NATIVE_INTERNAL_API
void polyvec_reduce(polyvec *r)
__contract__(
  requires(memory_no_alias(r, sizeof(polyvec)))
  assigns(object_whole(r))
  ensures(forall(k0, 0, MLKEM_K,
    array_bound(r->vec[k0].coeffs, 0, MLKEM_N, 0, MLKEM_Q)))
);

#define polyvec_add MLKEM_NAMESPACE_K(polyvec_add)
/*************************************************
 * Name:        polyvec_add
 *
 * Description: Add vectors of polynomials
 *
 * Arguments: - polyvec *r: pointer to input-output vector of polynomials to be
 *              added to
 *            - const polyvec *b: pointer to second input vector of polynomials
 *
 * The coefficients of r and b must be so that the addition does
 * not overflow. Otherwise, the behaviour of this function is undefined.
 *
 * The coefficients returned in *r are in int16_t which is sufficient
 * to prove type-safety of calling units. Therefore, no stronger
 * ensures clause is required on this function.
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_add(polyvec *r, const polyvec *b)
__contract__(
  requires(memory_no_alias(r, sizeof(polyvec)))
  requires(memory_no_alias(b, sizeof(polyvec)))
  requires(forall(j0, 0, MLKEM_K,
          forall(k0, 0, MLKEM_N,
            (int32_t)r->vec[j0].coeffs[k0] + b->vec[j0].coeffs[k0] <= INT16_MAX)))
  requires(forall(j1, 0, MLKEM_K,
          forall(k1, 0, MLKEM_N,
            (int32_t)r->vec[j1].coeffs[k1] + b->vec[j1].coeffs[k1] >= INT16_MIN)))
  assigns(object_whole(r))
);

#define polyvec_tomont MLKEM_NAMESPACE_K(polyvec_tomont)
/*************************************************
 * Name:        polyvec_tomont
 *
 * Description: Inplace conversion of all coefficients of a polynomial
 *              vector from normal domain to Montgomery domain
 *
 *              Bounds: Output < q in absolute value.
 *
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void polyvec_tomont(polyvec *r)
__contract__(
  requires(memory_no_alias(r, sizeof(polyvec)))
  assigns(memory_slice(r, sizeof(polyvec)))
  assigns(object_whole(r))
  ensures(forall(j, 0, MLKEM_K,
    array_abs_bound(r->vec[j].coeffs, 0, MLKEM_N, MLKEM_Q)))
);

#define poly_getnoise_eta1_4x MLKEM_NAMESPACE_K(poly_getnoise_eta1_4x)
/*************************************************
 * Name:        poly_getnoise_eta1_4x
 *
 * Description: Batch sample four polynomials deterministically from a seed
 * and nonces, with output polynomials close to centered binomial distribution
 * with parameter MLKEM_ETA1.
 *
 * Arguments:   - poly *r{0,1,2,3}: pointer to output polynomial
 *              - const uint8_t *seed: pointer to input seed
 *                                     (of length MLKEM_SYMBYTES bytes)
 *              - uint8_t nonce{0,1,2,3}: one-byte input nonce
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_getnoise_eta1_4x(poly *r0, poly *r1, poly *r2, poly *r3,
                           const uint8_t seed[MLKEM_SYMBYTES], uint8_t nonce0,
                           uint8_t nonce1, uint8_t nonce2, uint8_t nonce3)
/* Depending on MLKEM_K, the pointers passed to this function belong
   to the same objects, so we cannot use memory_no_alias for r0-r3.

   NOTE: Somehow it is important to use memory_no_alias() first in the
         conjunctions defining each case.
*/
#if MLKEM_K == 2
__contract__(
  requires(memory_no_alias(seed, MLKEM_SYMBYTES))
  requires( /* Case A: r0, r1 consecutive, r2, r3 consecutive */
    (memory_no_alias(r0, 2 * sizeof(poly)) && memory_no_alias(r2, 2 * sizeof(poly)) &&
     r1 == r0 + 1 && r3 == r2 + 1 && !same_object(r0, r2)))
  assigns(memory_slice(r0, sizeof(poly)))
  assigns(memory_slice(r1, sizeof(poly)))
  assigns(memory_slice(r2, sizeof(poly)))
  assigns(memory_slice(r3, sizeof(poly)))
  ensures(
    array_abs_bound(r0->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
    && array_abs_bound(r1->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
    && array_abs_bound(r2->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
    && array_abs_bound(r3->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1));
);
#elif MLKEM_K == 4
__contract__(
  requires(memory_no_alias(seed, MLKEM_SYMBYTES))
  requires( /* Case B: r0, r1, r2, r3 consecutive */
    (memory_no_alias(r0, 4 * sizeof(poly)) && r1 == r0 + 1 && r2 == r0 + 2 && r3 == r0 + 3))
  assigns(memory_slice(r0, sizeof(poly)))
  assigns(memory_slice(r1, sizeof(poly)))
  assigns(memory_slice(r2, sizeof(poly)))
  assigns(memory_slice(r3, sizeof(poly)))
  ensures(
    array_abs_bound(r0->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
    && array_abs_bound(r1->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
    && array_abs_bound(r2->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
    && array_abs_bound(r3->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1));
);
#elif MLKEM_K == 3
__contract__(
  requires(memory_no_alias(seed, MLKEM_SYMBYTES))
  requires( /* Case C: r0, r1, r2 consecutive */
 (memory_no_alias(r0, 3 * sizeof(poly)) && memory_no_alias(r3, 1 * sizeof(poly)) &&
  r1 == r0 + 1 && r2 == r0 + 2 && !same_object(r3, r0)))
  assigns(memory_slice(r0, sizeof(poly)))
  assigns(memory_slice(r1, sizeof(poly)))
  assigns(memory_slice(r2, sizeof(poly)))
  assigns(memory_slice(r3, sizeof(poly)))
  ensures(
    array_abs_bound(r0->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
    && array_abs_bound(r1->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
    && array_abs_bound(r2->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
    && array_abs_bound(r3->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1));
);
#endif /* MLKEM_K */

#if MLKEM_ETA1 == MLKEM_ETA2
/*
 * We only require poly_getnoise_eta2_4x for ml-kem-768 and ml-kem-1024
 * where MLKEM_ETA2 = MLKEM_ETA1 = 2.
 * For ml-kem-512, poly_getnoise_eta1122_4x is used instead.
 */
#define poly_getnoise_eta2_4x poly_getnoise_eta1_4x
#endif /* MLKEM_ETA1 == MLKEM_ETA2 */

#if MLKEM_K == 2 || MLKEM_K == 4
#define poly_getnoise_eta2 MLKEM_NAMESPACE_K(poly_getnoise_eta2)
/*************************************************
 * Name:        poly_getnoise_eta2
 *
 * Description: Sample a polynomial deterministically from a seed and a nonce,
 *              with output polynomial close to centered binomial distribution
 *              with parameter MLKEM_ETA2
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *seed: pointer to input seed
 *                                     (of length MLKEM_SYMBYTES bytes)
 *              - uint8_t nonce: one-byte input nonce
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_getnoise_eta2(poly *r, const uint8_t seed[MLKEM_SYMBYTES],
                        uint8_t nonce)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(seed, MLKEM_SYMBYTES))
  assigns(object_whole(r))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, MLKEM_ETA2 + 1))
);
#endif /* MLKEM_K == 2 || MLKEM_K == 4 */

#if MLKEM_K == 2
#define poly_getnoise_eta1122_4x MLKEM_NAMESPACE_K(poly_getnoise_eta1122_4x)
/*************************************************
 * Name:        poly_getnoise_eta1122_4x
 *
 * Description: Batch sample four polynomials deterministically from a seed
 * and a nonces, with output polynomials close to centered binomial
 * distribution with parameter MLKEM_ETA1 and MLKEM_ETA2
 *
 * Arguments:   - poly *r{0,1,2,3}: pointer to output polynomial
 *              - const uint8_t *seed: pointer to input seed
 *                                     (of length MLKEM_SYMBYTES bytes)
 *              - uint8_t nonce{0,1,2,3}: one-byte input nonce
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_getnoise_eta1122_4x(poly *r0, poly *r1, poly *r2, poly *r3,
                              const uint8_t seed[MLKEM_SYMBYTES],
                              uint8_t nonce0, uint8_t nonce1, uint8_t nonce2,
                              uint8_t nonce3)
__contract__(
  requires( /* r0, r1 consecutive, r2, r3 consecutive */
 (memory_no_alias(r0, 2 * sizeof(poly)) && memory_no_alias(r2, 2 * sizeof(poly)) &&
   r1 == r0 + 1 && r3 == r2 + 1 && !same_object(r0, r2)))
  requires(memory_no_alias(seed, MLKEM_SYMBYTES))
  assigns(object_whole(r0), object_whole(r1), object_whole(r2), object_whole(r3))
  ensures(array_abs_bound(r0->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
     && array_abs_bound(r1->coeffs,0, MLKEM_N, MLKEM_ETA1 + 1)
     && array_abs_bound(r2->coeffs,0, MLKEM_N, MLKEM_ETA2 + 1)
     && array_abs_bound(r3->coeffs,0, MLKEM_N, MLKEM_ETA2 + 1));
);
#endif /* MLKEM_K == 2 */

#endif
