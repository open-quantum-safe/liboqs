/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLK_SAMPLING_H
#define MLK_SAMPLING_H

#include <stdint.h>
#include <stdlib.h>
#include "cbmc.h"
#include "common.h"
#include "poly.h"

#define mlk_poly_cbd2 MLK_NAMESPACE(poly_cbd2)
/*************************************************
 * Name:        mlk_poly_cbd2
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter eta=2
 *
 * Arguments:   - mlk_poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 *
 * Specification: Implements [FIPS 203, Algorithm 8, SamplePolyCBD_2]
 *
 **************************************************/
MLK_INTERNAL_API
void mlk_poly_cbd2(mlk_poly *r, const uint8_t buf[2 * MLKEM_N / 4]);

#if defined(MLK_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_ETA1 == 3
#define mlk_poly_cbd3 MLK_NAMESPACE(poly_cbd3)
/*************************************************
 * Name:        mlk_poly_cbd3
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter eta=3.
 *              This function is only needed for ML-KEM-512
 *
 * Arguments:   - mlk_poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 *
 * Specification: Implements [FIPS 203, Algorithm 8, SamplePolyCBD_3]
 *
 **************************************************/
MLK_INTERNAL_API
void mlk_poly_cbd3(mlk_poly *r, const uint8_t buf[3 * MLKEM_N / 4]);
#endif /* MLK_MULTILEVEL_BUILD || MLKEM_ETA1 == 3 */

#define mlk_poly_rej_uniform_x4 MLK_NAMESPACE(poly_rej_uniform_x4)
/*************************************************
 * Name:        mlk_poly_rej_uniform_x4
 *
 * Description: Generate four polynomials using rejection sampling
 *              on (pseudo-)uniformly random bytes sampled from a seed.
 *
 * Arguments:   - mlk_poly *vec:           Pointer to an array of 4 polynomials
 *                                     to be sampled.
 *              - uint8_t *seed[4]:    Pointer to array of four pointers
 *                                     pointing to the seed buffers of size
 *                                     MLKEM_SYMBYTES + 2 each.
 *
 * Specification: Implements [FIPS 203, Algorithm 7, SampleNTT]
 *
 **************************************************/
MLK_INTERNAL_API
void mlk_poly_rej_uniform_x4(mlk_poly *vec, uint8_t *seed[4])
__contract__(
  requires(memory_no_alias(vec, sizeof(mlk_poly) * 4))
  requires(memory_no_alias(seed, sizeof(uint8_t*) * 4))
  requires(memory_no_alias(seed[0], MLKEM_SYMBYTES + 2))
  requires(memory_no_alias(seed[1], MLKEM_SYMBYTES + 2))
  requires(memory_no_alias(seed[2], MLKEM_SYMBYTES + 2))
  requires(memory_no_alias(seed[3], MLKEM_SYMBYTES + 2))
  assigns(memory_slice(vec, sizeof(mlk_poly) * 4))
  ensures(array_bound(vec[0].coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  ensures(array_bound(vec[1].coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  ensures(array_bound(vec[2].coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  ensures(array_bound(vec[3].coeffs, 0, MLKEM_N, 0, MLKEM_Q)));

#define mlk_poly_rej_uniform MLK_NAMESPACE(poly_rej_uniform)
/*************************************************
 * Name:        mlk_poly_rej_uniform
 *
 * Description: Generate polynomial using rejection sampling
 *              on (pseudo-)uniformly random bytes sampled from a seed.
 *
 * Arguments:   - mlk_poly *vec:           Pointer to polynomial to be sampled.
 *              - uint8_t *seed:       Pointer to seed buffer of size
 *                                     MLKEM_SYMBYTES + 2 each.
 *
 * Specification: Implements [FIPS 203, Algorithm 7, SampleNTT]
 *
 **************************************************/
MLK_INTERNAL_API
void mlk_poly_rej_uniform(mlk_poly *entry, uint8_t seed[MLKEM_SYMBYTES + 2])
__contract__(
  requires(memory_no_alias(entry, sizeof(mlk_poly)))
  requires(memory_no_alias(seed, MLKEM_SYMBYTES + 2))
  assigns(memory_slice(entry, sizeof(mlk_poly)))
  ensures(array_bound(entry->coeffs, 0, MLKEM_N, 0, MLKEM_Q)));

#endif /* MLK_SAMPLING_H */
