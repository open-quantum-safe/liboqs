/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef POLYVEC_H
#define POLYVEC_H

#include <stdint.h>
#include "common.h"
#include "poly.h"

#define polyvec MLKEM_NAMESPACE(polyvec)
typedef struct
{
  poly vec[MLKEM_K];
} ALIGN polyvec;

#define polyvec_mulcache MLKEM_NAMESPACE(polyvec_mulcache)
typedef struct
{
  poly_mulcache vec[MLKEM_K];
} polyvec_mulcache;

#define polyvec_compress_du MLKEM_NAMESPACE(polyvec_compress_du)
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

#define polyvec_decompress_du MLKEM_NAMESPACE(polyvec_decompress_du)
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

#define polyvec_tobytes MLKEM_NAMESPACE(polyvec_tobytes)
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

#define polyvec_frombytes MLKEM_NAMESPACE(polyvec_frombytes)
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

#define polyvec_ntt MLKEM_NAMESPACE(polyvec_ntt)
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

#define polyvec_invntt_tomont MLKEM_NAMESPACE(polyvec_invntt_tomont)
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
  MLKEM_NAMESPACE(polyvec_basemul_acc_montgomery)
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
  MLKEM_NAMESPACE(polyvec_basemul_acc_montgomery_cached)
/*************************************************
 * Name:        polyvec_basemul_acc_montgomery_cached
 *
 * Description: Scalar product of two vectors of polynomials in NTT domain,
 *              using mulcache for second operand.
 *
 *              Bounds:
 *              - a is assumed to be coefficient-wise < 4096 in absolute value.
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

#define polyvec_mulcache_compute MLKEM_NAMESPACE(polyvec_mulcache_compute)
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

#define polyvec_reduce MLKEM_NAMESPACE(polyvec_reduce)
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

#define polyvec_add MLKEM_NAMESPACE(polyvec_add)
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

#define polyvec_tomont MLKEM_NAMESPACE(polyvec_tomont)
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

#endif
