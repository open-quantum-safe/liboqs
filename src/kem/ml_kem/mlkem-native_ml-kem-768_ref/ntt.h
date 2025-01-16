/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "cbmc.h"
#include "common.h"
#include "poly.h"
#include "reduce.h"

#define zetas MLKEM_NAMESPACE(zetas)
extern const int16_t zetas[128];

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

#define basemul_cached MLKEM_NAMESPACE(basemul_cached)
/************************************************************
 * Name: basemul_cached
 *
 * Description: Computes a representative modulo q of
 *              (a0*b0 + a1*b_cached, a0*b1 + a1*b0)/65536
 *
 *              If b_cached is b1*zeta, this represents the
 *              product of (a0 + a1*X) and (b0 + b1*X) in
 *              Fq[X]/(X^2 - zeta).
 *
 * Arguments: - r: Pointer to output polynomial
 *                   Upon return, coefficients are bound by
 *                   2*MLKEM_Q in absolute value.
 *            - a: Pointer to first input polynomial
 *                   Must be coefficient-wise < 4096 in absolute value.
 *            - b: Pointer to second input polynomial
 *                   Can have arbitrary int16_t coefficients
 *            - b_cached: Some precomputed value, typically derived from
 *                   b1 and a twiddle factor. Can be an arbitary int16_t.
 ************************************************************/
MLKEM_NATIVE_INTERNAL_API
void basemul_cached(int16_t r[2], const int16_t a[2], const int16_t b[2],
                    int16_t b_cached)
__contract__(
  requires(memory_no_alias(r, 2 * sizeof(int16_t)))
  requires(memory_no_alias(a, 2 * sizeof(int16_t)))
  requires(memory_no_alias(b, 2 * sizeof(int16_t)))
  requires(array_bound(a, 0, 2, 0, UINT12_LIMIT))
  assigns(memory_slice(r, 2 * sizeof(int16_t)))
  ensures(array_abs_bound(r, 0, 2, 2 * MLKEM_Q))
);


#endif
