/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker, Shay Gueron and Dusan Kostic,
 * AWS Cryptographic Algorithms Group.
 */

#include <assert.h>

#include "cleanup.h"
#include "sampling.h"
#include "prf_internal.h"
#include "sampling_internal.h"
#include <oqs/rand.h>

void get_seeds(OUT seeds_t *seeds)
{
  OQS_randombytes((uint8_t *)seeds, NUM_OF_SEEDS * sizeof(seed_t));

  for(uint32_t i = 0; i < NUM_OF_SEEDS; ++i) {
    print("s: ", (uint64_t *)&seeds->seed[i], SIZEOF_BITS(seed_t));
  }
}

_INLINE_ void make_odd_weight(IN OUT r_t *r)
{
  if(((r_bits_vector_weight(r) % 2) == 1)) {
    // Already odd
    return;
  }

  r->raw[0] ^= 1;
}

// Returns an array of r pseudorandom bits.
// No restrictions exist for the top or bottom bits.
// If the generation requires an odd number, then set must_be_odd=1.
// The function uses the provided prf context.
ret_t sample_uniform_r_bits_with_fixed_prf_context(
  OUT r_t *r,
  IN OUT prf_state_t *prf_state,
  IN const must_be_odd_t      must_be_odd)
{
  // Generate random data
  GUARD(get_prf_output(r->raw, prf_state, R_BYTES));

  // Mask upper bits of the MSByte
  r->raw[R_BYTES - 1] &= MASK(R_BITS + 8 - (R_BYTES * 8));

  if(must_be_odd == MUST_BE_ODD) {
    make_odd_weight(r);
  }

  return SUCCESS;
}

ret_t sample_indices_fisher_yates(OUT idx_t *out,
                                  IN  size_t num_indices,
                                  IN  idx_t max_idx_val,
                                  IN OUT prf_state_t *prf_state) {

    for (size_t i = num_indices; i-- > 0;) {
#define CWW_RAND_BYTES 4
        uint64_t rand = 0ULL;
        GUARD(get_prf_output((uint8_t *)&rand, prf_state, CWW_RAND_BYTES));
        rand *= (max_idx_val - i);

		// new index l is such that i <= l < max_idx_val
        uint32_t l = i + (uint32_t)(rand >> (CWW_RAND_BYTES * 8));

		// Loop over (the end of) the output array to determine if l is a duplicate
        uint32_t is_dup = 0;
        for (size_t j = i + 1; j < num_indices; ++j) {
            is_dup |= secure_cmp32(l, out[j]);
        }

		// if l is a duplicate out[i] gets i else out[i] gets l
		// mask is all 1 if l is a duplicate, all 0 else
        uint32_t mask = -is_dup;
        out[i] = (mask & i) ^ (~mask & l);
    }

    return SUCCESS;
}

_INLINE_ ret_t generate_sparse_rep_for_sk(OUT pad_r_t *r,
                                          OUT idx_t *wlist,
                                          IN OUT prf_state_t *prf_state,
                                          IN sampling_ctx *ctx)
{
  idx_t wlist_temp[D] = {0};

  GUARD(sample_indices_fisher_yates(wlist_temp, D, R_BITS, prf_state));

  bike_memcpy(wlist, wlist_temp, D * sizeof(idx_t));
  ctx->secure_set_bits(r, 0, wlist, D);

  secure_clean((uint8_t *)wlist_temp, sizeof(*wlist_temp));
  return SUCCESS;
}

ret_t generate_secret_key(OUT pad_r_t *h0, OUT pad_r_t *h1,
                          OUT idx_t *h0_wlist, OUT idx_t *h1_wlist,
                          IN const seed_t *seed)
{
  // Initialize the sampling context.
  sampling_ctx ctx = {0};
  sampling_ctx_init(&ctx);

  DEFER_CLEANUP(prf_state_t prf_state = {0}, clean_prf_state);

  GUARD(init_prf_state(&prf_state, MAX_PRF_INVOCATION, seed));

  GUARD(generate_sparse_rep_for_sk(h0, h0_wlist, &prf_state, &ctx));
  GUARD(generate_sparse_rep_for_sk(h1, h1_wlist, &prf_state, &ctx));

  return SUCCESS;
}

ret_t generate_error_vector(OUT pad_e_t *e, IN const seed_t *seed)
{
  // Initialize the sampling context.
  sampling_ctx ctx;
  sampling_ctx_init(&ctx);

  DEFER_CLEANUP(prf_state_t prf_state = {0}, clean_prf_state);

  GUARD(init_prf_state(&prf_state, MAX_PRF_INVOCATION, seed));

  idx_t wlist[T];
  GUARD(sample_indices_fisher_yates(wlist, T, N_BITS, &prf_state));

  // (e0, e1) hold bits 0..R_BITS-1 and R_BITS..2*R_BITS-1 of the error, resp.
  ctx.secure_set_bits(&e->val[0], 0, wlist, T);
  ctx.secure_set_bits(&e->val[1], R_BITS, wlist, T);

  // Clean the padding of the elements.
  PE0_RAW(e)[R_BYTES - 1] &= LAST_R_BYTE_MASK;
  PE1_RAW(e)[R_BYTES - 1] &= LAST_R_BYTE_MASK;
  bike_memset(&PE0_RAW(e)[R_BYTES], 0, R_PADDED_BYTES - R_BYTES);
  bike_memset(&PE1_RAW(e)[R_BYTES], 0, R_PADDED_BYTES - R_BYTES);

  secure_clean((uint8_t *)wlist, sizeof(*wlist));

  return SUCCESS;
}

// Returns an array of r pseudorandom bits.
// No restrictions exist for the top or bottom bits.
// If the generation requires an odd number, then set must_be_odd = MUST_BE_ODD
ret_t sample_uniform_r_bits(OUT r_t *r,
                            IN const seed_t *      seed,
                            IN const must_be_odd_t must_be_odd)
{
  // For the seedexpander
  DEFER_CLEANUP(prf_state_t prf_state = {0}, clean_prf_state);

  GUARD(init_prf_state(&prf_state, MAX_PRF_INVOCATION, seed));

  GUARD(sample_uniform_r_bits_with_fixed_prf_context(r, &prf_state, must_be_odd));

  return SUCCESS;
}

