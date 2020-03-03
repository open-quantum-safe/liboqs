/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker and Shay Gueron
 * AWS Cryptographic Algorithms Group.
 * (ndrucker@amazon.com, gueron@amazon.com)
 */

#pragma once

#include "aes_ctr_prf.h"
#include "utilities.h"
#include <oqs/rand.h>

typedef enum {
	NO_RESTRICTION = 0,
	MUST_BE_ODD    = 1
} must_be_odd_t;

_INLINE_ void
get_seeds(OUT seeds_t *seeds) {
	OQS_randombytes((uint8_t *)seeds, NUM_OF_SEEDS * sizeof(seed_t));
	for (uint32_t i = 0; i < NUM_OF_SEEDS; ++i) {
		print("s: ", (uint64_t *)&seeds->seed[i], sizeof(seed_t) * 8);
	}
}

// Return's an array of r pseudorandom bits
// No restrictions exist for the top or bottom bits -
// in case an odd number is requried then set must_be_odd=1
// Uses the provided prf context
ret_t
sample_uniform_r_bits_with_fixed_prf_context(OUT r_t *r,
        IN OUT
        aes_ctr_prf_state_t *prf_state,
        IN must_be_odd_t must_be_odd);

// Return's an array of r pseudorandom bits
// No restrictions exist for the top or bottom bits -
// in case an odd number is  requried then set must_be_odd=1
_INLINE_ ret_t
sample_uniform_r_bits(OUT r_t *r,
                      IN const seed_t       *seed,
                      IN const must_be_odd_t must_be_odd) {
	// For the seedexpander
	DEFER_CLEANUP(aes_ctr_prf_state_t prf_state = {0}, aes_ctr_prf_state_cleanup);

	GUARD(init_aes_ctr_prf_state(&prf_state, MAX_AES_INVOKATION, seed));

	GUARD(sample_uniform_r_bits_with_fixed_prf_context(r, &prf_state, must_be_odd));

	return SUCCESS;
}

// Generate a pseudorandom r of length len with a set weight
// Using the pseudorandom ctx supplied
// Outputs also a compressed (not ordered) list of indices
ret_t
generate_sparse_rep(OUT uint64_t *a,
                    OUT idx_t     wlist[],
                    IN uint32_t   weight,
                    IN uint32_t   len,
                    IN uint32_t   padded_len,
                    IN OUT aes_ctr_prf_state_t *prf_state);

EXTERNC void
secure_set_bits(IN OUT uint64_t *a,
                IN const idx_t   wlist[],
                IN uint32_t      a_len,
                IN uint32_t      weight);
