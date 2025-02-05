/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef SAMPLING_H
#define SAMPLING_H

#include <stdint.h>
#include <stdlib.h>
#include "cbmc.h"
#include "common.h"
#include "poly.h"

#define poly_cbd2 MLKEM_NAMESPACE(poly_cbd2)
/*************************************************
 * Name:        poly_cbd2
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter eta=2
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_cbd2(poly *r, const uint8_t buf[2 * MLKEM_N / 4]);

#if defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_ETA1 == 3
#define poly_cbd3 MLKEM_NAMESPACE(poly_cbd3)
/*************************************************
 * Name:        poly_cbd3
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter eta=3.
 *              This function is only needed for ML-KEM-512
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_cbd3(poly *r, const uint8_t buf[3 * MLKEM_N / 4]);
#endif /* MLKEM_NATIVE_MULTILEVEL_BUILD || MLKEM_ETA1 == 3 */

#define poly_rej_uniform_x4 MLKEM_NAMESPACE(poly_rej_uniform_x4)
/*************************************************
 * Name:        poly_rej_uniform_x4
 *
 * Description: Generate four polynomials using rejection sampling
 *              on (pseudo-)uniformly random bytes sampled from a seed.
 *
 * Arguments:   - poly *vec:           Pointer to an array of 4 polynomials
 *                                     to be sampled.
 *              - uint8_t *seed[4]:    Pointer to array of four pointers
 *                                     pointing to the seed buffers of size
 *                                     MLKEM_SYMBYTES + 2 each.
 *
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_rej_uniform_x4(poly *vec, uint8_t *seed[4])
__contract__(
  requires(memory_no_alias(vec, sizeof(poly) * 4))
  requires(memory_no_alias(seed, sizeof(uint8_t*) * 4))
  requires(memory_no_alias(seed[0], MLKEM_SYMBYTES + 2))
  requires(memory_no_alias(seed[1], MLKEM_SYMBYTES + 2))
  requires(memory_no_alias(seed[2], MLKEM_SYMBYTES + 2))
  requires(memory_no_alias(seed[3], MLKEM_SYMBYTES + 2))
  assigns(memory_slice(vec, sizeof(poly) * 4))
  ensures(array_bound(vec[0].coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  ensures(array_bound(vec[1].coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  ensures(array_bound(vec[2].coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  ensures(array_bound(vec[3].coeffs, 0, MLKEM_N, 0, MLKEM_Q)));

#define poly_rej_uniform MLKEM_NAMESPACE(poly_rej_uniform)
/*************************************************
 * Name:        poly_rej_uniform
 *
 * Description: Generate polynomial using rejection sampling
 *              on (pseudo-)uniformly random bytes sampled from a seed.
 *
 * Arguments:   - poly *vec:           Pointer to polynomial to be sampled.
 *              - uint8_t *seed:       Pointer to seed buffer of size
 *                                     MLKEM_SYMBYTES + 2 each.
 *
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_rej_uniform(poly *entry, uint8_t seed[MLKEM_SYMBYTES + 2])
__contract__(
  requires(memory_no_alias(entry, sizeof(poly)))
  requires(memory_no_alias(seed, MLKEM_SYMBYTES + 2))
  assigns(memory_slice(entry, sizeof(poly)))
  ensures(array_bound(entry->coeffs, 0, MLKEM_N, 0, MLKEM_Q)));

#endif /* SAMPLING_H */
