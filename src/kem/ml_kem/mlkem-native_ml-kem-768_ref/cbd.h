/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef CBD_H
#define CBD_H

#include <stdint.h>
#include "common.h"
#include "poly.h"

#define poly_cbd_eta1 MLKEM_NAMESPACE(poly_cbd_eta1)
/*************************************************
 * Name:        poly_cbd_eta1
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter MLKEM_ETA1.
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_cbd_eta1(poly *r, const uint8_t buf[MLKEM_ETA1 * MLKEM_N / 4])
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(buf, MLKEM_ETA1 * MLKEM_N / 4))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, MLKEM_ETA1 + 1))
);

#if MLKEM_K == 2 || MLKEM_K == 4
#define poly_cbd_eta2 MLKEM_NAMESPACE(poly_cbd_eta2)
/*************************************************
 * Name:        poly_cbd_eta1
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter MLKEM_ETA2.
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_cbd_eta2(poly *r, const uint8_t buf[MLKEM_ETA2 * MLKEM_N / 4])
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(buf, MLKEM_ETA2 * MLKEM_N / 4))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, MLKEM_ETA2 + 1))
);
#endif /* MLKEM_K == 2 || MLKEM_K == 4 */

#endif
