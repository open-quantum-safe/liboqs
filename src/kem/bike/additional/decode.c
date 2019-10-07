/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 * http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 * The license is detailed in the file LICENSE.md, and applies to this file.
 *
 * Written by Nir Drucker and Shay Gueron
 * AWS Cryptographic Algorithms Group.
 * (ndrucker@amazon.com, gueron@amazon.com)
 *
 * The optimizations are based on the description developed in the paper:
 * Drucker, Nir, and Shay Gueron. 2019. “A Toolbox for Software Optimization
 * of QC-MDPC Code-Based Cryptosystems.” Journal of Cryptographic Engineering,
 * January, 1–17. https://doi.org/10.1007/s13389-018-00200-4.
 *
 * The decoder (in decoder/decoder.c) algorithm is the Black-Gray decoder in
 * the early submission of CAKE (due to N. Sandrier and R Misoczki).
 */

#include "decode.h"
#include "converts.h"
#include "gf2x.h"
#include "utilities.h"
#include <string.h>

// Decoding (bit-flipping) parameter
#ifdef INDCPA
#if (LEVEL == 1)
#define MAX_IT 4
#elif (LEVEL == 3)
#define MAX_IT 6
#else
#error "Level can only be 1/3"
#endif
#else
#if (LEVEL == 1)
#define MAX_IT 3
#elif (LEVEL == 3)
#define MAX_IT 4
#else
#error "Level can only be 1/3"
#endif
#endif

#define MAX_DELTA 4

void split_e(OUT split_e_t *split_e_, IN const e_t *e) {
	// Copy lower bytes (e0)
	memcpy(split_e_->val[0].raw, e->raw, R_SIZE);

	// Now load second value
	for (uint32_t i = R_SIZE; i < N_SIZE; ++i) {
		split_e_->val[1].raw[i - R_SIZE] =
		    ((e->raw[i] << LAST_R_BYTE_TRAIL) | (e->raw[i - 1] >> LAST_R_BYTE_LEAD));
	}

	// Fix corner case
	if (N_SIZE < (2ULL * R_SIZE)) {
		split_e_->val[1].raw[R_SIZE - 1] = (e->raw[N_SIZE - 1] >> LAST_R_BYTE_LEAD);
	}

	// Fix last value
	split_e_->val[0].raw[R_SIZE - 1] &= LAST_R_BYTE_MASK;
	split_e_->val[1].raw[R_SIZE - 1] &= LAST_R_BYTE_MASK;
}

// Transpose a row into a column
_INLINE_ void
transpose(OUT red_r_t *col, IN const red_r_t *row) {
	col->raw[0] = row->raw[0];
	for (uint64_t i = 1; i < R_BITS; ++i) {
		col->raw[i] = row->raw[(R_BITS) -i];
	}
}

ret_t compute_syndrome(OUT syndrome_t *syndrome, IN const ct_t *ct, IN const sk_t *sk) {
	// gf2x_mod_mul requires the values to be 64bit padded and extra (dbl) space
	// for the results
	DEFER_CLEANUP(dbl_pad_syndrome_t pad_s, dbl_pad_syndrome_cleanup);
	DEFER_CLEANUP(pad_sk_t pad_sk = {0}, pad_sk_cleanup);
	pad_sk[0].val = sk->bin[0];
	pad_sk[1].val = sk->bin[1];

	DEFER_CLEANUP(pad_ct_t pad_ct = {0}, pad_ct_cleanup);
	pad_ct[0].val = ct->val[0];
	pad_ct[1].val = ct->val[1];

	// Compute s = c0*h0 + c1*h1:
	GUARD(gf2x_mod_mul((uint64_t *) &pad_s[0], (uint64_t *) &pad_ct[0],
	                   (uint64_t *) &pad_sk[0]));
	GUARD(gf2x_mod_mul((uint64_t *) &pad_s[1], (uint64_t *) &pad_ct[1],
	                   (uint64_t *) &pad_sk[1]));

	GUARD(gf2x_add(pad_s[0].val.raw, pad_s[0].val.raw, pad_s[1].val.raw, R_SIZE));

	// Converting to redunandt representation and then transposing the value.
	red_r_t s_tmp_bytes = {0};
	convert_to_redundant_rep(s_tmp_bytes.raw, pad_s[0].val.raw,
	                         sizeof(s_tmp_bytes));
	transpose(&syndrome->dup1, &s_tmp_bytes);

	syndrome->dup2 = syndrome->dup1;

	return SUCCESS;
}

_INLINE_ ret_t
recompute_syndrome(OUT syndrome_t *syndrome,
                   IN const ct_t *ct,
                   IN const sk_t *sk,
                   IN const e_t *e) {
	// Split e into e0 and e1. Initialization is done in split_e
	DEFER_CLEANUP(split_e_t splitted_e, split_e_cleanup);
	split_e(&splitted_e, e);

	ct_t tmp_ct = *ct;

	// Adapt the ciphertext
	GUARD(gf2x_add(tmp_ct.val[0].raw, tmp_ct.val[0].raw, splitted_e.val[0].raw,
	               R_SIZE));
	GUARD(gf2x_add(tmp_ct.val[1].raw, tmp_ct.val[1].raw, splitted_e.val[1].raw,
	               R_SIZE));

	// Recompute the syndrome
	GUARD(compute_syndrome(syndrome, &tmp_ct, sk));

	return SUCCESS;
}

_INLINE_ uint32_t
get_threshold(IN const red_r_t *s) {
	const uint32_t syndrome_weight = count_ones(s->raw, R_BITS);

// The equations below are defined in BIKE's specification:
// https://bikesuite.org/files/round2/spec/BIKE-Spec-Round2.2019.03.30.pdf
// Page 20 Section 2.4.2
#if LEVEL == 1
	const uint32_t threshold = (13.530 + 0.0069721 * (syndrome_weight));
#elif LEVEL == 3
	const uint32_t threshold = (15.932 + 0.0052936 * (syndrome_weight));
#elif LEVEL == 5
	const uint32_t threshold = (17.489 + 0.0043536 * (syndrome_weight));
#endif

	DMSG("    Thresold: %d\n", threshold);
	return threshold;
}

ret_t decode(OUT e_t *e,
             IN OUT syndrome_t *original_s,
             IN const ct_t *ct,
             IN const sk_t *sk) {
	e_t black_e = {0};
	e_t gray_e = {0};
	uint32_t threshold;
	syndrome_t s;

// Count the number of unsatisfied parity-checks:
#ifdef AVX512
	ALIGN(16)
	uint8_t upc[N_QDQWORDS_BITS] = {0};
#else
	ALIGN(16)
	uint8_t upc[N_DDQWORDS_BITS] = {0};
#endif

	ALIGN(16)
	DEFER_CLEANUP(compressed_idx_dv_ar_t inv_h_compressed = {0},
	              compressed_idx_dv_ar_cleanup);
	for (uint64_t i = 0; i < FAKE_DV; i++) {
		if ((sk->wlist[0].val[i].val > R_BITS) || (sk->wlist[1].val[i].val > R_BITS)) {
			BIKE_ERROR(E_DECODING_FAILURE);
		}

		inv_h_compressed[0].val[i].val = R_BITS - sk->wlist[0].val[i].val;
		inv_h_compressed[1].val[i].val = R_BITS - sk->wlist[1].val[i].val;

		inv_h_compressed[0].val[i].used = sk->wlist[0].val[i].used;
		inv_h_compressed[1].val[i].used = sk->wlist[1].val[i].used;
	}

	// Reset the error
	memset(e, 0, sizeof(*e));
	s.dup1 = original_s->dup1;

	// Reset the syndrome. Copying dup1 twice is faster than copying dup1+dup2.
	s.dup1 = original_s->dup1;
	s.dup2 = original_s->dup1;

	for (uint32_t iter = 0; iter < MAX_IT; iter++) {
		DMSG("    Iteration: %d\n", iter);
		DMSG("    Weight of e: %lu\n", count_ones(e->raw, sizeof(*e)));
		DMSG("    Weight of syndrome: %lu\n", count_ones(s.dup1.raw, sizeof(s.dup1)));

		// Step 1: standrad BitFlip iteration
		calc_upc(upc, (uint8_t *) &s, &inv_h_compressed[0], &inv_h_compressed[1]);

		threshold = get_threshold(&s.dup1);
		find_error1(e, &black_e, &gray_e, upc, threshold, threshold - MAX_DELTA + 1);
		GUARD(recompute_syndrome(&s, ct, sk, e));

		DMSG("    Weight of e: %lu\n", count_ones(e->raw, sizeof(*e)));
		DMSG("    Weight of syndrome: %lu\n", count_ones(s.dup1.raw, sizeof(s.dup1)));

		// Step II: Unflip positions that still have high number of UPC associated
		calc_upc(upc, (uint8_t *) &s, &inv_h_compressed[0], &inv_h_compressed[1]);
		find_error2(e, &black_e, upc, ((DV + 1) / 2) + 1);
		GUARD(recompute_syndrome(&s, ct, sk, e));

		DMSG("    Weight of e: %lu\n", count_ones(e->raw, sizeof(*e)));
		DMSG("    Weight of syndrome: %lu\n", count_ones(s.dup1.raw, sizeof(s.dup1)));

		// Step III: Flip all gray positions associated to high number of UPC
		calc_upc(upc, (uint8_t *) &s, &inv_h_compressed[0], &inv_h_compressed[1]);
		find_error2(e, &gray_e, upc, ((DV + 1) / 2) + 1);
		GUARD(recompute_syndrome(&s, ct, sk, e));
	}

	if (count_ones(s.dup1.raw, sizeof(s.dup1)) > 0) {
		BIKE_ERROR(E_DECODING_FAILURE);
	}

	return SUCCESS;
}
