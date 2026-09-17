#include <inttypes.h>
#include <string.h>

#include "sdith_prng.h"
#include "sdith_rsd.h"
#include "vole_private.h"

EXPORT uint64_t rsd_generate_random_instance_tmp_bytes(const vole_parameters* vole_params,  //
                                                       uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim) {
  CREQUIRE(rsd_n >= rsd_codim, "rsd_n (%" PRId64 ") is not larger than codim (%" PRId64 ")", rsd_n, rsd_codim);
  CREQUIRE(rsd_w != 0, "empty rsd_w (%" PRId64 ")", rsd_w);
  CREQUIRE(rsd_codim != 0, "empty rsd_codim (%" PRId64 ")", rsd_codim);
  uint64_t rsd_codim_limbs, rsd_codim_slice;
  compute_rsd_codim_slice(&rsd_codim_limbs, &rsd_codim_slice, vole_params->LAMBDA, rsd_codim);
  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint8_t*, h_row, 32, rsd_codim_slice);
  COMP_SPACE_RETURN();
}

/** generate a random rsd instance out of the master key entropy.
 * Constant-time in the solution it produces, except for the rejection loop of
 * the position sampler (see below). tmp_space holds one expanded H row on
 * return: the caller must scrub it. */
EXPORT void rsd_generate_random_instance_ref(            //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim,  // rsd dimensions
    const vole_parameters* vole_params,                  // prng + matrix-prng family
    bitvec_t* y, uint64_t y_bytes,                       // out: syndrome (ceil(rsd_codim/8) bytes are set)
    uint32_t* solution,                                  // out: rsd_w integers in [0,n/w-1]
    const seed_t* sk_seed,                               // in: secret key seed (lambda bits)
    const seed_t* pk_seed,                               // in: pubkey seed (lambda bits)
    uint8_t* tmp_space                                   // scratch space
) {
  const uint64_t col_bytes = (rsd_codim + 7) >> 3;
  CREQUIRE(rsd_n != 0, "empty rsd_n (%" PRId64 ")", rsd_n);
  CREQUIRE(rsd_w != 0, "empty rsd_w (%" PRId64 ")", rsd_w);
  CREQUIRE(rsd_codim != 0, "empty rsd_codim (%" PRId64 ")", rsd_codim);
  CREQUIRE(rsd_n >= rsd_codim, "rsd_n (%" PRId64 ") is not larger than codim (%" PRId64 ")", rsd_n, rsd_codim);
  CREQUIRE(y_bytes >= col_bytes, "y_bytes too small (%" PRId64 " vs %" PRId64 ")", y_bytes, col_bytes);
  CREQUIRE(rsd_n % rsd_w == 0, "rsd_n (%" PRId64 ") is not a multiple of rsd_w (%" PRId64 ")", rsd_n, rsd_w);
  uint64_t rsd_codim_limbs, rsd_codim_slice;
  compute_rsd_codim_slice(&rsd_codim_limbs, &rsd_codim_slice, vole_params->LAMBDA, rsd_codim);
  CASSERT(col_bytes <= rsd_codim_slice, "invalid rsd_codim_slice");
  // one H row at a time: the matrix prng draws any row independently, so the
  // rsd_w rows y needs cost rsd_w cipher calls instead of the whole matrix.
  TMP_SPACE_MAP_ALIGNED(uint8_t*, h_row, 32, rsd_codim_slice);
  // sample the solution positions from the keygen prng (the ctr keystream of the
  // secret-key seed, read as little-endian uint32s).
  keygen_rng_ctx sk_rng;
  vole_params->keygen_rng.init(&sk_rng, sk_seed);
  // generate the solution vector
  const uint32_t npw = rsd_n / rsd_w;
  // hit and miss version (the rejection loop is not ct, but a rejected draw is
  // discarded and independent of the accepted one, so its count says nothing
  // about the solution). The reduction of the accepted draw is ct: it is the
  // secret itself.
  const uint32_t npw_max = UINT32_MAX - (UINT32_MAX % npw);
  for (uint64_t i = 0; i < rsd_w; i++) {
    uint32_t pos;
    do {
      pos = vole_params->keygen_rng.next_u32(&sk_rng);
    } while (pos >= npw_max);
    uint32_t quo;
    vole_params->ct.div_rem_u32(&quo, &solution[i], pos, npw);
  }
  // prepare the pk_rng to draw rows of h (row-wise matrix prng)
  matrix_rng_t pk_rng;
  vole_params->matrix_prg.matrix_rng_init(&pk_rng, pk_seed, rsd_codim);
  // Compute y. The plain form of the loop body is:
  //   if (real_index < rsd_codim) {
  //     yy ^= single_bit_at(real_index)          // the identity block of H
  //   } else {
  //     yy ^= get_pk_row(real_index - rsd_codim) // the random block of H
  //   }
  // but real_index is secret, so the test may not be branched on: both arms run
  // for every i, and a mask keeps the one that counts. The extra cost is one
  // cipher row per identity-block hit -- a handful.
  memset(y, 0, y_bytes);
  for (uint64_t i = 0; i < rsd_w; i++) {
    const uint64_t real_index = i * npw + solution[i];
    const uint64_t in_id = vole_params->ct.mask_lt(real_index, rsd_codim);  // ~0 on the identity block
    const uint64_t in_rnd = ~in_id;                                         // ~0 on the random block
    // else-arm: draw the row unconditionally (index clamped to 0 when unused,
    // so the draw itself reveals nothing) and xor it under the mask
    vole_params->matrix_prg.matrix_rng_get_row(&pk_rng, h_row, (real_index - rsd_codim) & in_rnd);
    vole_params->ct.bitvec_xor_to_masked(y, col_bytes, h_row, in_rnd);
    // then-arm: flip bit real_index, at a secret position (clamped to 0 when
    // unused, the same way as the else-arm: the xor is a no-op under a zero
    // mask, but the position must stay inside y for the debug range check)
    vole_params->ct.bitvec_xorbit(y, col_bytes, real_index & in_id, in_id);
  }
}

/** expand the public key from the pubkey seed */
EXPORT void rsd_expand_public_key_ref(                   //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim,  // rsd dimensions
    const vole_parameters* vole_params,                  // matrix-prng family
    bitmat_t* h, uint64_t h_slice_bytes,                 // out: (rsd_n - rsd_codim) x rsd_n matrix
    const seed_t* h_seed                                 // in: pubkey seed (lambda bits)
) {
  const uint64_t lambda = vole_params->LAMBDA;
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  uint64_t rsd_codim_limbs, rsd_codim_slice;
  compute_rsd_codim_slice(&rsd_codim_limbs, &rsd_codim_slice, lambda, rsd_codim);
  const uint64_t row_out_bytes = rsd_codim_limbs * lambda_bytes;  // matrix_prg row size
  CREQUIRE(rsd_n >= rsd_codim, "rsd_n (%" PRId64 ") is not larger than codim (%" PRId64 ")", rsd_n, rsd_codim);
  CREQUIRE(rsd_w != 0, "empty rsd_w (%" PRId64 ")", rsd_w);
  CREQUIRE(rsd_codim != 0, "empty rsd_codim (%" PRId64 ")", rsd_codim);
  CREQUIRE(h_slice_bytes >= row_out_bytes,  //
           "h_slice_bytes too small (%" PRId64 " vs %" PRId64 ")", h_slice_bytes, row_out_bytes);
  const uint64_t n_minus_k = rsd_n - rsd_codim;
  memset(h, 0, n_minus_k * h_slice_bytes);
  // Each H row is one matrix_prg row (rsd_codim bits, bits beyond that masked to
  // zero, padded to rsd_codim_limbs limbs). The rows are independent, so we
  // write each directly into its (block-aligned) slice of h.
  matrix_rng_t h_rng;
  vole_params->matrix_prg.matrix_rng_init(&h_rng, h_seed, rsd_codim);
  uint8_t* const hh = (uint8_t*)h;
  if (h_slice_bytes == rsd_codim_slice) {
    // the prng rows tile h exactly: the whole matrix is a single batch, written
    // in place (h holds the expanded matrix anyway, so this costs no extra RAM).
    vole_params->matrix_prg.matrix_rng_get_rows(&h_rng, hh, 0, n_minus_k);
  } else {
    // h is laid out with wider slices than the prng row size: one row at a time
    // so the extra padding bytes of each slice stay zero.
    for (uint64_t i = 0; i < n_minus_k; i++) {
      vole_params->matrix_prg.matrix_rng_get_row(&h_rng, hh + h_slice_bytes * i, i);
    }
  }
}

EXPORT uint64_t rsd_public_key_times_challenge_tmp_bytes(const vole_parameters* vole_params,  //
                                                         uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim) {
  CREQUIRE(rsd_n >= rsd_codim, "rsd_n (%" PRId64 ") is not larger than codim (%" PRId64 ")", rsd_n, rsd_codim);
  CREQUIRE(rsd_w != 0, "empty rsd_w (%" PRId64 ")", rsd_w);
  CREQUIRE(rsd_codim != 0, "empty rsd_codim (%" PRId64 ")", rsd_codim);
  const uint64_t lambda = vole_params->LAMBDA;
  uint64_t rsd_codim_limbs, rsd_codim_slice;
  compute_rsd_codim_slice(&rsd_codim_limbs, &rsd_codim_slice, lambda, rsd_codim);
  // the scratch holds the pre-processed challenge, which y_full aliases, so it
  // must also be wide enough for one slice.
  const uint64_t scratch_bytes = matrix_rng_prep_chall_nelems(rsd_codim) * vole_params->lambda_bytes;
  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint8_t*, scratch, 32, scratch_bytes < rsd_codim_slice ? rsd_codim_slice : scratch_bytes);
  COMP_SPACE_RETURN();
}

/** expand and multiply the public key with challenge points */
EXPORT void rsd_public_key_times_challenge_ref(          //
    const vole_parameters* vole_params,                  //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim,  // rsd dimensions
    flambda_t* chall_a_H,                                // out: n elements
    flambda_t* chall_a_y,                                // out: 1 element
    const flambda_t* chall_a,                            // in: rsd_codim_limbs elements
    const seed_t* pk_seed,                               // in: pubkey seed (lambda bits)
    const bitvec_t* y,                                   // in: pubkey y (rsd_codim bits padded with zeros)
    uint8_t* tmp_space                                   // scratch space
) {
  static const uint64_t TWO[] = {2, 0, 0, 0};
  CASSERT(rsd_n >= rsd_codim, "rsd_n (%" PRId64 ") is not larger than codim (%" PRId64 ")", rsd_n, rsd_codim);
  CASSERT(rsd_w != 0, "empty rsd_w (%" PRId64 ")", rsd_w);
  CASSERT(rsd_codim != 0, "empty rsd_codim (%" PRId64 ")", rsd_codim);
  const uint64_t lambda = vole_params->LAMBDA;
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  uint64_t rsd_codim_limbs, rsd_codim_slice;
  compute_rsd_codim_slice(&rsd_codim_limbs, &rsd_codim_slice, lambda, rsd_codim);
  const uint64_t rsd_codim_bytes = (rsd_codim + 7) >> 3;
  const uint64_t rsd_padding_bytes = rsd_codim_limbs * lambda_bytes - rsd_codim_bytes;
  const uint64_t n_minus_k = rsd_n - rsd_codim;
  const uint8_t h_last_mask = 0xFF >> ((-rsd_codim) & 7);
  const uint64_t scratch_bytes = matrix_rng_prep_chall_nelems(rsd_codim) * lambda_bytes;
  TMP_SPACE_MAP_ALIGNED(uint8_t*, scratch, 32,
                        scratch_bytes < rsd_codim_slice ? rsd_codim_slice : scratch_bytes);
  uint8_t* y_full = scratch;  // alias (y_full fits in rsd_codim_slice by definition)

#ifndef NDEBUG
  uint64_t leftover_bits = rsd_codim & 7;
  uint8_t* y_u8 = (uint8_t*)y;
  if (leftover_bits) {
    uint64_t mask = ((1 << (8 - leftover_bits)) - 1) << leftover_bits;
    uint64_t idx = ((rsd_codim + 7) >> 3) - 1;
    CASSERT((y_u8[idx] & mask) == 0, "empty bits of y are not zero-ed out");
  }
#endif  // NDEBUG

  // deal with y (chall_a_y = <y, chall_a>)
  memcpy(y_full, y, rsd_codim_bytes);
  y_full[rsd_codim_bytes - 1] &= h_last_mask;
  memset(y_full + rsd_codim_bytes, 0, rsd_padding_bytes);
  vole_params->flambda_dot_product(chall_a_y, y_full, chall_a, rsd_codim_limbs);
  // deal with the id block of H
  uint8_t* dest = chall_a_H;
  {
    uint64_t s = rsd_codim;
    for (uint64_t i = 0; i < rsd_codim_limbs; i++) {
      uint64_t block_s = lambda < s ? lambda : s;
      vole_params->flambda_set(dest, chall_a + i * lambda_bytes);
      dest += lambda_bytes;
      for (uint64_t j = 1; j < block_s; j++) {
        vole_params->flambda_product(dest, dest - lambda_bytes, TWO);
        dest += lambda_bytes;
      }
      s -= block_s;
    }
    CASSERT(dest == chall_a_H + rsd_codim * lambda_bytes, "bug!");
    CASSERT(s == 0, "bug!");
  }
  // deal with the regular rows of H. H is never materialised: the whole block is
  // one fused call that generates each row and multiplies it by the challenge
  // without the row ever reaching memory.
  matrix_rng_t h_rng;
  vole_params->matrix_prg.matrix_rng_init(&h_rng, pk_seed, rsd_codim);
  vole_params->matrix_prg.matrix_rng_preprocess_chall(&h_rng, scratch, chall_a);
  vole_params->matrix_prg.matrix_rng_rows_times_chall(&h_rng, dest, scratch, 0, n_minus_k);
  dest += n_minus_k * lambda_bytes;
  CASSERT(dest == chall_a_H + rsd_n * lambda_bytes, "bug!");
}

/** unary encoding of the solution (for mux circuits). Constant-time in the
 * solution: the dimensions and the arities are public, only the coefficients
 * are not. */
EXPORT void rsd_encode_solution_ref(                              //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim,           // rsd dimensions
    const vole_parameters* vole_params,                           // ct helpers
    uint64_t mux_depth, const uint64_t* mux_arities,              // mux depth and arities
    bitvec_t* encoded_solution, uint64_t encoded_solution_bytes,  // encoded solution
    const uint32_t* solution                                      // in: solution coefficients
) {
  CREQUIRE(rsd_n != 0, "empty rsd_n (%" PRId64 ")", rsd_n);
  CREQUIRE(rsd_w != 0, "empty rsd_w (%" PRId64 ")", rsd_w);
  CREQUIRE(rsd_codim != 0, "empty rsd_codim (%" PRId64 ")", rsd_codim);
  CREQUIRE(mux_depth != 0, "empty mux_depth (%" PRId64 ")", mux_depth);
  CREQUIRE(rsd_n % rsd_w == 0, "rsd_n (%" PRId64 ") is not a multiple of rsd_w (%" PRId64 ")", rsd_n, rsd_w);
  const uint64_t npw = rsd_n / rsd_w;
  uint64_t mux_size = 1;
  uint64_t mux_inputs = 0;
  for (uint64_t j = 0; j < mux_depth; j++) {
    const uint64_t arj = mux_arities[j];
    CREQUIRE(arj >= 2, "mux_arity[%" PRId64 "]=%" PRId64 " too small", j, arj);
    // the one-hot field of a level must fit in the uint32 bitvec_xoru32 takes
    CREQUIRE(arj < 32, "mux_arity[%" PRId64 "]=%" PRId64 " too large", j, arj);
    mux_inputs += arj - 1;
    mux_size *= arj;
  }
  CREQUIRE(mux_size >= npw, "mux arities too small (max_tree_size: %" PRId64 " vs. %" PRId64 ")", mux_size, npw);
  uint64_t encoded_solution_actual_bytes = (mux_inputs * rsd_w + 7) >> 3;
  CREQUIRE(encoded_solution_bytes >= encoded_solution_actual_bytes,                      //
           "encoded_solutions too small (size: %" PRId64 " vs. min size: %" PRId64 ")",  //
           encoded_solution_bytes, encoded_solution_actual_bytes);
  uint8_t* const sol = encoded_solution;
  memset(sol, 0, encoded_solution_bytes);
  // Every (element, mux level) owns a field of arj-1 bits at a PUBLIC offset
  // bitpos, since bitpos only ever advances by the public arj-1. Inside that
  // field the plain form of the body is:
  //   if (sij != 0) { sol ^= single_bit_at(bitpos + sij - 1) }
  // with sij secret. Rather than address that bit, we build the one-hot field
  // as a value and xor the whole field in at its public offset: the address is
  // public, only the bits are secret.
  uint64_t bitpos = 0;
  for (uint64_t i = 0; i < rsd_w; i++) {
    uint32_t si = solution[i];
    for (uint64_t j = 0; j < mux_depth; j++) {
      const uint64_t arj = mux_arities[j];
      uint32_t sij;
      vole_params->ct.div_rem_u32(&si, &sij, si, (uint32_t)arj);
      // (sij == 0) ? 0 : 1 << (sij - 1), with no test: the >> 1 turns the sij=0
      // case into 0 by itself. arj < 32 keeps the shift in range for every sij.
      // The shift amount is secret, which is fine: a shift by a register count
      // is data-independent on every target here (x86-64 SHL/SHR, aarch64 LSL).
      const uint32_t one_hot = (UINT32_C(1) << sij) >> 1;
      vole_params->ct.bitvec_xoru32(sol, encoded_solution_bytes, bitpos, arj - 1, one_hot);
      bitpos += arj - 1;
    }
    CASSERT(si == 0, "bug");
  }
  CASSERT(bitpos == rsd_w * mux_inputs, "bug");
}
