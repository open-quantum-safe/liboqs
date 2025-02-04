/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef POLY_H
#define POLY_H

#include <stddef.h>
#include <stdint.h>
#include "cbmc.h"
#include "common.h"
#include "debug.h"
#include "verify.h"

/* Absolute exclusive upper bound for the output of the inverse NTT */
#define INVNTT_BOUND (8 * MLKEM_Q)

/* Absolute exclusive upper bound for the output of the forward NTT */
#define NTT_BOUND (8 * MLKEM_Q)

#define zetas MLKEM_NAMESPACE(zetas)
extern const int16_t zetas[128];

/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*coeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
#define poly MLKEM_NAMESPACE(poly)
typedef struct
{
  int16_t coeffs[MLKEM_N];
} ALIGN poly;

/*
 * INTERNAL presentation of precomputed data speeding up
 * the base multiplication of two polynomials in NTT domain.
 */
#define poly_mulcache MLKEM_NAMESPACE(poly_mulcache)
typedef struct
{
  int16_t coeffs[MLKEM_N >> 1];
} poly_mulcache;

#define poly_basemul_montgomery_cached \
  MLKEM_NAMESPACE(poly_basemul_montgomery_cached)
/*************************************************
 * Name:        poly_basemul_montgomery_cached
 *
 * Description: Multiplication of two polynomials in NTT domain,
 *              using mulcache for second operand.
 *
 *              Bounds:
 *              - a is assumed to be coefficient-wise < q in absolute value.
 *
 *              The result is coefficient-wise bound by 2*q in absolute value.
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const poly *a: pointer to first input polynomial
 *              - const poly *b: pointer to second input polynomial
 *              - const poly_mulcache *b_cache: pointer to mulcache
 *                  for second input polynomial. Can be computed
 *                  via poly_mulcache_compute().
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_basemul_montgomery_cached(poly *r, const poly *a, const poly *b,
                                    const poly_mulcache *b_cache)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(a, sizeof(poly)))
  requires(memory_no_alias(b, sizeof(poly)))
  requires(memory_no_alias(b_cache, sizeof(poly_mulcache)))
  requires(array_bound(a->coeffs, 0, MLKEM_N, 0, UINT12_LIMIT))
  assigns(object_whole(r))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, 2 * MLKEM_Q))
);

#define poly_tomont MLKEM_NAMESPACE(poly_tomont)
/*************************************************
 * Name:        poly_tomont
 *
 * Description: Inplace conversion of all coefficients of a polynomial
 *              from normal domain to Montgomery domain
 *
 *              Bounds: Output < q in absolute value.
 *
 * Arguments:   - poly *r: pointer to input/output polynomial
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_tomont(poly *r)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, MLKEM_Q))
);

#define poly_mulcache_compute MLKEM_NAMESPACE(poly_mulcache_compute)
/************************************************************
 * Name: poly_mulcache_compute
 *
 * Description: Computes the mulcache for a polynomial in NTT domain
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
 * Arguments: - x: Pointer to mulcache to be populated
 *            - a: Pointer to input polynomial
 ************************************************************/
/*
 * NOTE: The default C implementation of this function populates
 * the mulcache with values in (-q,q), but this is not needed for the
 * higher level safety proofs, and thus not part of the spec.
 */
MLKEM_NATIVE_INTERNAL_API
void poly_mulcache_compute(poly_mulcache *x, const poly *a)
__contract__(
  requires(memory_no_alias(x, sizeof(poly_mulcache)))
  requires(memory_no_alias(a, sizeof(poly)))
  assigns(object_whole(x))
);

#define poly_reduce MLKEM_NAMESPACE(poly_reduce)
/*************************************************
 * Name:        poly_reduce
 *
 * Description: Converts polynomial to _unsigned canonical_ representatives.
 *
 *              The input coefficients can be arbitrary integers in int16_t.
 *              The output coefficients are in [0,1,...,MLKEM_Q-1].
 *
 * Arguments:   - poly *r: pointer to input/output polynomial
 **************************************************/
/*
 * NOTE: The semantics of poly_reduce() is different in
 * the reference implementation, which requires
 * signed canonical output data. Unsigned canonical
 * outputs are better suited to the only remaining
 * use of poly_reduce() in the context of (de)serialization.
 */
MLKEM_NATIVE_INTERNAL_API
void poly_reduce(poly *r)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_bound(r->coeffs, 0, MLKEM_N, 0, MLKEM_Q))
);

#define poly_add MLKEM_NAMESPACE(poly_add)
/************************************************************
 * Name: poly_add
 *
 * Description: Adds two polynomials in place
 *
 * Arguments: - r: Pointer to input-output polynomial to be added to.
 *            - b: Pointer to input polynomial that should be added
 *                 to r. Must be disjoint from r.
 *
 * The coefficients of r and b must be so that the addition does
 * not overflow. Otherwise, the behaviour of this function is undefined.
 *
 ************************************************************/
/*
 * NOTE: The reference implementation uses a 3-argument poly_add.
 * We specialize to the accumulator form to avoid reasoning about aliasing.
 */
MLKEM_NATIVE_INTERNAL_API
void poly_add(poly *r, const poly *b)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(b, sizeof(poly)))
  requires(forall(k0, 0, MLKEM_N, (int32_t) r->coeffs[k0] + b->coeffs[k0] <= INT16_MAX))
  requires(forall(k1, 0, MLKEM_N, (int32_t) r->coeffs[k1] + b->coeffs[k1] >= INT16_MIN))
  ensures(forall(k, 0, MLKEM_N, r->coeffs[k] == old(*r).coeffs[k] + b->coeffs[k]))
  assigns(memory_slice(r, sizeof(poly)))
);

#define poly_sub MLKEM_NAMESPACE(poly_sub)
/*************************************************
 * Name:        poly_sub
 *
 * Description: Subtract two polynomials; no modular reduction is performed
 *
 * Arguments: - poly *r:       Pointer to input-output polynomial to be added
 *to.
 *            - const poly *b: Pointer to second input polynomial
 **************************************************/
/*
 * NOTE: The reference implementation uses a 3-argument poly_sub.
 * We specialize to the accumulator form to avoid reasoning about aliasing.
 */
MLKEM_NATIVE_INTERNAL_API
void poly_sub(poly *r, const poly *b)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(b, sizeof(poly)))
  requires(forall(k0, 0, MLKEM_N, (int32_t) r->coeffs[k0] - b->coeffs[k0] <= INT16_MAX))
  requires(forall(k1, 0, MLKEM_N, (int32_t) r->coeffs[k1] - b->coeffs[k1] >= INT16_MIN))
  ensures(forall(k, 0, MLKEM_N, r->coeffs[k] == old(*r).coeffs[k] - b->coeffs[k]))
  assigns(object_whole(r))
);

#define poly_ntt MLKEM_NAMESPACE(poly_ntt)
/*************************************************
 * Name:        poly_ntt
 *
 * Description: Computes negacyclic number-theoretic transform (NTT) of
 *              a polynomial in place.
 *
 *              The input is assumed to be in normal order and
 *              coefficient-wise bound by MLKEM_Q in absolute value.
 *
 *              The output polynomial is in bitreversed order, and
 *              coefficient-wise bound by NTT_BOUND in absolute value.
 *
 *              (NOTE: Sometimes the input to the NTT is actually smaller,
 *               which gives better bounds.)
 *
 * Arguments:   - poly *p: pointer to in/output polynomial
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_ntt(poly *r)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(array_abs_bound(r->coeffs, 0, MLKEM_N, MLKEM_Q))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, NTT_BOUND))
);

#define poly_invntt_tomont MLKEM_NAMESPACE(poly_invntt_tomont)
/*************************************************
 * Name:        poly_invntt_tomont
 *
 * Description: Computes inverse of negacyclic number-theoretic transform (NTT)
 *              of a polynomial in place;
 *              inputs assumed to be in bitreversed order, output in normal
 *              order
 *
 *              The input is assumed to be in bitreversed order, and can
 *              have arbitrary coefficients in int16_t.
 *
 *              The output polynomial is in normal order, and
 *              coefficient-wise bound by INVNTT_BOUND in absolute value.
 *
 * Arguments:   - uint16_t *a: pointer to in/output polynomial
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_invntt_tomont(poly *r)
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, INVNTT_BOUND))
);

#endif /* POLY_H */
