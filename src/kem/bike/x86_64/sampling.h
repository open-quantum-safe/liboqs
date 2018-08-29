/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include "stdio.h"
#include "aes_ctr_prf.h"
#include "utilities.h"

#ifndef FIXED_SEED
#include "oqs/rand.h"
#endif

typedef enum {
	NO_RESTRICTION = 0,
	MUST_BE_ODD = 1
} must_be_odd_t;

_INLINE_ void get_seeds(OUT double_seed_t *seeds) {
	OQS_randombytes(seeds->s1.u.raw, sizeof(double_seed_t));

	EDMSG("s1: ");
	print(seeds->s1.u.qw, sizeof(seed_t) * 8);
	EDMSG("s2: ");
	print(seeds->s2.u.qw, sizeof(seed_t) * 8);
}

// Return's an array of r pseudorandom bits
// No restrictions exist for the top or bottom bits -
// in case an odd number is  requried then set must_be_odd=1
OQS_STATUS sample_uniform_r_bits(OUT uint8_t *r,
                                 IN const seed_t *seed,
                                 IN const must_be_odd_t must_be_odd);

// Generate a pseudorandom r of length len with a set weight
// Using the pseudorandom ctx supplied.
// Outputs also a compressed (not ordered) list of indices
OQS_STATUS generate_sparse_fake_rep(OUT uint8_t *a,
                                    OUT idx_t wlist[],
                                    IN const uint32_t weight,
                                    IN const uint32_t fake_weight,
                                    IN const uint32_t len,
                                    IN const uint32_t padded_len,
                                    IN OUT aes_ctr_prf_state_t *prf_state);

// Generate a pseudorandom r of length len with a set weight
// Using the pseudorandom ctx supplied
// Outputs also a compressed (not ordered) list of indices
OQS_STATUS generate_sparse_rep(OUT uint8_t *a,
                               OUT idx_t wlist[],
                               IN const uint32_t weight,
                               IN const uint32_t len,
                               IN const uint32_t padded_len,
                               IN OUT aes_ctr_prf_state_t *prf_state);

#endif // _SAMPLE_H_
