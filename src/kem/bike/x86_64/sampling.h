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

#include "../functions_renaming.h"
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

enum _seeds_purpose {
	KEYGEN_SEEDS = 0,
	ENCAPS_SEEDS = 1
};

typedef enum _seeds_purpose seeds_purpose_t;

_INLINE_ void get_seeds(OUT double_seed_t *seeds,
                        IN const seeds_purpose_t seeds_type) {
#ifdef FIXED_SEED
	if (seeds_type == KEYGEN_SEEDS) {
		const double_seed_t ds = {.u.v.s1 = {.qwords = {0x0, 0x0, 0x0, 0x0}},
		                          .u.v.s2 = {.qwords = {0xffffffffffffffff, 0xffffffffffffffff,
		                                                0xffffffffffffffff, 0xffffffffffffffff}}};
		*seeds = ds;
	} else if (seeds_type == ENCAPS_SEEDS) {
		const double_seed_t ds = {.u.v.s1 = {.qwords = {0x00000000000000ff, 0x0, 0x0, 0x0}},
		                          .u.v.s2 = {.qwords = {0x000000000000ffff, 0x0, 0x0, 0x0}}};
		*seeds = ds;
	}
#else
	BIKE_UNUSED(seeds_type);
	OQS_randombytes(seeds->u.v.s1.u.raw, sizeof(double_seed_t));

	EDMSG("s1: ");
	print(seeds->u.v.s1.u.qwords, sizeof(seed_t) * 8);
	EDMSG("s2: ");
	print(seeds->u.v.s2.u.qwords, sizeof(seed_t) * 8);
#endif
}

//Return's an array of r pseudorandom bits
//No restrictions exist for the top or bottom bits -
//in case an odd number is  requried then set must_be_odd=1
status_t sample_uniform_r_bits(OUT uint8_t *r,
                               IN const seed_t *seed,
                               IN const must_be_odd_t must_be_odd);

//Generate a pseudorandom r of length len with a set weight
//Using the pseudorandom ctx supplied.
//Outputs also a compressed (not ordered) list of indices.
status_t generate_sparse_fake_rep(OUT uint8_t *a,
                                  OUT idx_t wlist[],
                                  IN const uint32_t weight,
                                  IN const uint32_t fake_weight,
                                  IN const uint32_t len,
                                  IN const uint32_t padded_len,
                                  IN OUT aes_ctr_prf_state_t *prf_state);

//Generate a pseudorandom r of length len with a set weight
//Using the pseudorandom ctx supplied.
//Outputs also a compressed (not ordered) list of indices.
status_t generate_sparse_rep(OUT uint8_t *a,
                             OUT idx_t wlist[],
                             IN const uint32_t weight,
                             IN const uint32_t len,
                             IN const uint32_t padded_len,
                             IN OUT aes_ctr_prf_state_t *prf_state);

#endif //_SAMPLE_H_
