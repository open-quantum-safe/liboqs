/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bitstream.h"
#include "compat.h"
#include "io.h"
#include "kdf_shake.h"
#include "lowmc.h"
#include "mpc_lowmc.h"
#include "picnic_impl.h"
#include <oqs/rand.h>

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#if !defined(UNUSED_PARAMETER)
#define UNUSED_PARAMETER(x) (void)(x)
#endif

/* max number of ZKB++ rounds */
#if defined(WITH_LOWMC_255_255_4) || defined(WITH_LOWMC_256_256_38)
#define MAX_NUM_ROUNDS 438
#elif defined(WITH_LOWMC_192_192_4) || defined(WITH_LOWMC_192_192_30)
#define MAX_NUM_ROUNDS 329
#elif defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_128_128_20)
#define MAX_NUM_ROUNDS 219
#endif

/* max view size per round */
#if defined(WITH_LOWMC_256_256_38)
#define MAX_VIEW_SIZE 143
#elif defined(WITH_LOWMC_255_255_4)
#define MAX_VIEW_SIZE 128
#elif defined(WITH_LOWMC_192_192_30)
#define MAX_VIEW_SIZE 113
#elif defined(WITH_LOWMC_192_192_4)
#define MAX_VIEW_SIZE 96
#elif defined(WITH_LOWMC_128_128_20)
#define MAX_VIEW_SIZE 75
#elif defined(WITH_LOWMC_129_129_4)
#define MAX_VIEW_SIZE 65
#endif

typedef struct {
  uint8_t* seeds[SC_PROOF];
  uint8_t* commitments[SC_PROOF];
  uint8_t* input_shares[SC_PROOF];
  uint8_t* communicated_bits[SC_PROOF];
  uint8_t* output_shares[SC_PROOF];
#if defined(WITH_UNRUH)
  uint8_t* gs[SC_PROOF];
#endif
} proof_round_t;

typedef struct {
  uint8_t salt[SALT_SIZE];
  uint8_t* challenge;
  proof_round_t round[];
} sig_proof_t;

typedef struct {
  proof_round_t* round;
} sorting_helper_t;

#if defined(WITH_UNRUH)
static bool is_unruh(const picnic_instance_t* pp) {
  return pp->params == Picnic_L1_UR || pp->params == Picnic_L3_UR || pp->params == Picnic_L5_UR;
}
#endif

static inline void clear_padding_bits(uint8_t* v, const unsigned int diff) {
#if defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_255_255_4)
  *v &= UINT8_C(0xff) << diff;
#else // OQS note: mark unused params explicitely to avoid warnings
  UNUSED_PARAMETER(v);
  UNUSED_PARAMETER(diff);
#endif
}

/**
 * Collapse challenge from one char per challenge to bit array.
 */
static void collapse_challenge(uint8_t* collapsed, const picnic_instance_t* pp,
                               const uint8_t* challenge) {
  bitstream_t bs;
  bs.buffer.w = collapsed;
  bs.position = 0;

  for (unsigned int i = 0; i < pp->num_rounds; ++i) {
    // flip challenge bits according to spec
    bitstream_put_bits_8(&bs, (challenge[i] >> 1) | ((challenge[i] & 1) << 1), 2);
  }
}

/**
 * Expand challenge from bit array to one char per challenge.
 */
static bool expand_challenge(uint8_t* challenge, const picnic_instance_t* pp,
                             const uint8_t* collapsed) {
  bitstream_t bs;
  bs.buffer.r = collapsed;
  bs.position = 0;

  for (unsigned int i = 0; i < pp->num_rounds; ++i) {
    const uint8_t ch = bitstream_get_bits_8(&bs, 2);
    if (ch == 3) {
      return false;
    }
    // flip challenge bits according to spec
    challenge[i] = (ch & 1) << 1 | (ch >> 1);
  }

  const size_t remaining_bits = (pp->collapsed_challenge_size << 3) - bs.position;
  if (remaining_bits && bitstream_get_bits(&bs, remaining_bits)) {
    return false;
  }

  return true;
}

static sig_proof_t* proof_new(const picnic_instance_t* pp) {
  const size_t digest_size = pp->digest_size;
  const size_t seed_size   = pp->seed_size;
  const size_t num_rounds  = pp->num_rounds;
  const size_t input_size  = pp->input_size;
  const size_t output_size = pp->output_size;
  const size_t view_size   = ALIGNU64T(pp->view_size);
#if defined(WITH_UNRUH)
  const size_t unruh_with_input_bytes_size    = pp->unruh_with_input_bytes_size;
  const size_t unruh_without_input_bytes_size = pp->unruh_without_input_bytes_size;
#endif

  sig_proof_t* prf = calloc(1, sizeof(sig_proof_t) + num_rounds * sizeof(proof_round_t));
  if (!prf) {
    return NULL;
  }

  size_t per_round_mem =
      SC_PROOF * (seed_size + digest_size + input_size + output_size + view_size);
#if defined(WITH_UNRUH)
  if (is_unruh(pp)) {
    per_round_mem += (SC_PROOF - 1) * unruh_without_input_bytes_size + unruh_with_input_bytes_size;
  }
#endif

  // in memory:
  // - challenge (aligned to uint64_t)
  // - seeds
  // - salt
  // - commitments
  // - input shares
  // - communicated bits (aligned to uint64_t)
  // - output shares
  // - Gs
  //
  // Since seeds size, commitment size, input share size and output share size are all divisible by
  // the alignment of uint64_t, this means, that up to the memory of the Gs, everything is
  // uint64_t-aligned.
  uint8_t* slab  = calloc(1, num_rounds * per_round_mem + ALIGNU64T(num_rounds) + SALT_SIZE);
  prf->challenge = slab;
  slab += ALIGNU64T(num_rounds);

  for (uint32_t r = 0; r < num_rounds; ++r) {
    for (uint32_t i = 0; i < SC_PROOF; ++i) {
      prf->round[r].seeds[i] = slab;
      slab += seed_size;
    }
  }

  for (uint32_t r = 0; r < num_rounds; ++r) {
    for (uint32_t i = 0; i < SC_PROOF; ++i) {
      prf->round[r].commitments[i] = slab;
      slab += digest_size;
    }
  }

  for (uint32_t r = 0; r < num_rounds; ++r) {
    for (uint32_t i = 0; i < SC_PROOF; ++i) {
      prf->round[r].input_shares[i] = slab;
      slab += input_size;
    }
  }

  for (uint32_t r = 0; r < num_rounds; ++r) {
    for (uint32_t i = 0; i < SC_PROOF; ++i) {
      prf->round[r].communicated_bits[i] = slab;
      slab += view_size;
    }
  }

  for (uint32_t r = 0; r < num_rounds; ++r) {
    for (uint32_t i = 0; i < SC_PROOF; ++i) {
      prf->round[r].output_shares[i] = slab;
      slab += output_size;
    }
  }

#if defined(WITH_UNRUH)
  if (is_unruh(pp)) {
    for (uint32_t r = 0; r < num_rounds; ++r) {
      for (uint32_t i = 0; i < SC_PROOF - 1; ++i) {
        prf->round[r].gs[i] = slab;
        slab += unruh_without_input_bytes_size;
      }
      prf->round[r].gs[SC_PROOF - 1] = slab;
      slab += unruh_with_input_bytes_size;
    }
  }
#endif

  return prf;
}

static sig_proof_t* proof_new_verify(const picnic_instance_t* pp, uint8_t** rslab) {
  const size_t digest_size = pp->digest_size;
  const size_t num_rounds  = pp->num_rounds;
  const size_t input_size  = pp->input_size;
  const size_t output_size = pp->output_size;
  const size_t view_size   = ALIGNU64T(pp->view_size);

  sig_proof_t* proof = calloc(1, sizeof(sig_proof_t) + num_rounds * sizeof(proof_round_t));
  if (!proof) {
    return NULL;
  }

  size_t per_round_mem = SC_VERIFY * digest_size;
#if defined(WITH_UNRUH)
  const size_t unruh_with_input_bytes_size = pp->unruh_with_input_bytes_size;
  if (is_unruh(pp)) {
    // we don't know what we actually need, so allocate more than needed
    per_round_mem += SC_VERIFY * unruh_with_input_bytes_size;
  }
#endif
  per_round_mem += SC_VERIFY * input_size + SC_PROOF * output_size + view_size;

  uint8_t* slab    = calloc(1, num_rounds * per_round_mem + ALIGNU64T(num_rounds));
  proof->challenge = slab;
  slab += ALIGNU64T(num_rounds);

  for (uint32_t r = 0; r < num_rounds; ++r) {
    for (uint32_t i = 0; i < SC_VERIFY; ++i) {
      proof->round[r].commitments[i] = slab;
      slab += digest_size;
    }
  }

  for (uint32_t r = 0; r < num_rounds; ++r) {
    proof->round[r].communicated_bits[0] = slab;
    slab += view_size;
  }

  for (uint32_t r = 0; r < num_rounds; ++r) {
    proof->round[r].output_shares[0] = slab;
    slab += output_size;
    proof->round[r].output_shares[1] = slab;
    slab += output_size;
    proof->round[r].output_shares[2] = slab;
    slab += output_size;
  }

#if defined(WITH_UNRUH)
  if (is_unruh(pp)) {
    for (uint32_t r = 0; r < num_rounds; ++r) {
      for (uint32_t i = 0; i < SC_VERIFY; ++i) {
        proof->round[r].gs[i] = slab;
        slab += unruh_with_input_bytes_size;
      }
    }
  }
#endif

  *rslab = slab;
  return proof;
}

static void proof_free(sig_proof_t* prf) {
  free(prf->challenge);
  free(prf);
}

static void kdf_init_from_seed(kdf_shake_t* kdf, const uint8_t* seed, const uint8_t* salt,
                               uint16_t round_number, uint16_t player_number,
                               bool include_input_size, const picnic_instance_t* pp) {
  const size_t digest_size = pp->digest_size;

  // Hash the seed with H_2.
  kdf_shake_init_prefix(kdf, digest_size, HASH_PREFIX_2);
  kdf_shake_update_key(kdf, seed, pp->seed_size);
  kdf_shake_finalize_key(kdf);

  uint8_t tmp[MAX_DIGEST_SIZE];
  kdf_shake_get_randomness(kdf, tmp, digest_size);
  kdf_shake_clear(kdf);

  // Initialize KDF with H_2(seed) || salt || round_number || player_number || output_size.
  kdf_shake_init(kdf, digest_size);
  kdf_shake_update_key(kdf, tmp, digest_size);
  kdf_shake_update_key(kdf, salt, SALT_SIZE);
  kdf_shake_update_key_uint16_le(kdf, round_number);
  kdf_shake_update_key_uint16_le(kdf, player_number);
  kdf_shake_update_key_uint16_le(kdf, pp->view_size + (include_input_size ? pp->input_size : 0));
  kdf_shake_finalize_key(kdf);
}

static void kdf_init_x4_from_seed(kdf_shake_x4_t* kdf, const uint8_t** seed, const uint8_t* salt,
                                  const uint16_t round_number[4], const uint16_t player_number,
                                  bool include_input_size, const picnic_instance_t* pp) {
  const size_t digest_size = pp->digest_size;

  // Hash the seed with H_2.
  kdf_shake_x4_init_prefix(kdf, digest_size, HASH_PREFIX_2);
  kdf_shake_x4_update_key(kdf, seed, pp->seed_size);
  kdf_shake_x4_finalize_key(kdf);

  uint8_t tmp[4][MAX_DIGEST_SIZE];
  kdf_shake_x4_get_randomness_4(kdf, tmp[0], tmp[1], tmp[2], tmp[3], digest_size);
  kdf_shake_x4_clear(kdf);

  // Initialize KDF with H_2(seed) || salt || round_number || player_number || output_size.
  kdf_shake_x4_init(kdf, digest_size);
  kdf_shake_x4_update_key_4(kdf, tmp[0], tmp[1], tmp[2], tmp[3], digest_size);
  kdf_shake_x4_update_key_1(kdf, salt, SALT_SIZE);
  kdf_shake_x4_update_key_uint16s_le(kdf, round_number);
  kdf_shake_x4_update_key_uint16_le(kdf, player_number);
  kdf_shake_x4_update_key_uint16_le(kdf, pp->view_size + (include_input_size ? pp->input_size : 0));
  kdf_shake_x4_finalize_key(kdf);
}

#if defined(WITH_LOWMC_128_128_20) || defined(WITH_LOWMC_192_192_30) || defined(WITH_LOWMC_256_256_38)
static void uint64_to_bitstream_10(bitstream_t* bs, const uint64_t v) {
  bitstream_put_bits(bs, v >> (64 - 30), 30);
}

static uint64_t uint64_from_bitstream_10(bitstream_t* bs) {
  return bitstream_get_bits(bs, 30) << (64 - 30);
}
#endif

static void compress_view(uint8_t* dst, const picnic_instance_t* pp, const view_t* views,
                          const unsigned int idx) {
  const size_t num_views = pp->lowmc.r;

  bitstream_t bs;
  bs.buffer.w = dst;
  bs.position = 0;

  const view_t* v = &views[0];
#if defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_192_192_4) || defined(WITH_LOWMC_255_255_4)
  if (pp->lowmc.m != 10) {
    const size_t view_round_size = pp->view_round_size;
    const size_t width           = (pp->lowmc.n + 63) / 64;

    for (size_t i = 0; i < num_views; ++i, ++v) {
      mzd_to_bitstream(&bs, &v->s[idx], width, view_round_size);
    }
    return;
  }
#endif
#if defined(WITH_LOWMC_128_128_20) || defined(WITH_LOWMC_192_192_30) || defined(WITH_LOWMC_256_256_38)
  if (pp->lowmc.m == 10) {
    for (size_t i = 0; i < num_views; ++i, ++v) {
      uint64_to_bitstream_10(&bs, v->t[idx]);
    }
  }
#endif
}

static void decompress_view(view_t* views, const picnic_instance_t* pp, const uint8_t* src,
                            const unsigned int idx) {
  const size_t num_views = pp->lowmc.r;

  bitstream_t bs;
  bs.buffer.r = src;
  bs.position = 0;

  view_t* v = &views[0];
#if defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_192_192_4) || defined(WITH_LOWMC_255_255_4)
  if (pp->lowmc.m != 10) {
    const size_t view_round_size = pp->view_round_size;
    const size_t width           = (pp->lowmc.n + 63) / 64;

    for (size_t i = 0; i < num_views; ++i, ++v) {
      mzd_from_bitstream(&bs, &v->s[idx], width, view_round_size);
    }
    return;
  }
#endif
#if defined(WITH_LOWMC_128_128_20) || defined(WITH_LOWMC_192_192_30) || defined(WITH_LOWMC_256_256_38)
  if (pp->lowmc.m == 10) {
    for (size_t i = 0; i < num_views; ++i, ++v) {
      v->t[idx] = uint64_from_bitstream_10(&bs);
    }
  }
#endif
}

static void decompress_random_tape(rvec_t* rvec, const picnic_instance_t* pp, const uint8_t* src,
                                   const unsigned int idx) {
  decompress_view(rvec, pp, src, idx);
}

/**
 * Compute commitment to a view.
 */
static void hash_commitment(const picnic_instance_t* pp, proof_round_t* prf_round,
                            const unsigned int vidx) {
  const size_t hashlen = pp->digest_size;

  hash_context ctx;
  // hash the seed
  hash_init_prefix(&ctx, hashlen, HASH_PREFIX_4);
  hash_update(&ctx, prf_round->seeds[vidx], pp->seed_size);
  hash_final(&ctx);
  uint8_t tmp[MAX_DIGEST_SIZE];
  hash_squeeze(&ctx, tmp, hashlen);

  // compute H_0(H_4(seed), view)
  hash_init_prefix(&ctx, hashlen, HASH_PREFIX_0);
  hash_update(&ctx, tmp, hashlen);
  // hash input share
  hash_update(&ctx, prf_round->input_shares[vidx], pp->input_size);
  // hash communicated bits
  hash_update(&ctx, prf_round->communicated_bits[vidx], pp->view_size);
  // hash output share
  hash_update(&ctx, prf_round->output_shares[vidx], pp->output_size);
  hash_final(&ctx);
  hash_squeeze(&ctx, prf_round->commitments[vidx], hashlen);
}

/**
 * Compute commitment to 4 views.
 */
static void hash_commitment_x4(const picnic_instance_t* pp, proof_round_t* prf_round,
                               const unsigned int vidx) {
  const size_t hashlen = pp->digest_size;

  hash_context_x4 ctx;
  // hash the seed
  hash_init_prefix_x4(&ctx, hashlen, HASH_PREFIX_4);
  hash_update_x4_4(&ctx, prf_round[0].seeds[vidx], prf_round[1].seeds[vidx],
                   prf_round[2].seeds[vidx], prf_round[3].seeds[vidx], pp->seed_size);
  hash_final_x4(&ctx);
  uint8_t tmp[4][MAX_DIGEST_SIZE];
  hash_squeeze_x4_4(&ctx, tmp[0], tmp[1], tmp[2], tmp[3], hashlen);

  // compute H_0(H_4(seed), view)
  hash_init_prefix_x4(&ctx, hashlen, HASH_PREFIX_0);
  hash_update_x4_4(&ctx, tmp[0], tmp[1], tmp[2], tmp[3], hashlen);
  // hash input share
  hash_update_x4_4(&ctx, prf_round[0].input_shares[vidx], prf_round[1].input_shares[vidx],
                   prf_round[2].input_shares[vidx], prf_round[3].input_shares[vidx],
                   pp->input_size);
  // hash communicated bits
  hash_update_x4_4(&ctx, prf_round[0].communicated_bits[vidx], prf_round[1].communicated_bits[vidx],
                   prf_round[2].communicated_bits[vidx], prf_round[3].communicated_bits[vidx],
                   pp->view_size);
  // hash output share
  hash_update_x4_4(&ctx, prf_round[0].output_shares[vidx], prf_round[1].output_shares[vidx],
                   prf_round[2].output_shares[vidx], prf_round[3].output_shares[vidx],
                   pp->output_size);
  hash_final_x4(&ctx);
  hash_squeeze_x4_4(&ctx, prf_round[0].commitments[vidx], prf_round[1].commitments[vidx],
                    prf_round[2].commitments[vidx], prf_round[3].commitments[vidx], hashlen);
}

/**
 * Compute commitment to 4 views, for verification
 */
static void hash_commitment_x4_verify(const picnic_instance_t* pp, const sorting_helper_t* helper,
                                      const unsigned int vidx) {
  const size_t hashlen = pp->digest_size;

  hash_context_x4 ctx;
  // hash the seed
  hash_init_prefix_x4(&ctx, hashlen, HASH_PREFIX_4);
  hash_update_x4_4(&ctx, helper[0].round->seeds[vidx], helper[1].round->seeds[vidx],
                   helper[2].round->seeds[vidx], helper[3].round->seeds[vidx], pp->seed_size);
  hash_final_x4(&ctx);
  uint8_t tmp[4][MAX_DIGEST_SIZE];
  hash_squeeze_x4_4(&ctx, tmp[0], tmp[1], tmp[2], tmp[3], hashlen);

  // compute H_0(H_4(seed), view)
  hash_init_prefix_x4(&ctx, hashlen, HASH_PREFIX_0);
  hash_update_x4_4(&ctx, tmp[0], tmp[1], tmp[2], tmp[3], hashlen);
  // hash input share
  hash_update_x4_4(&ctx, helper[0].round->input_shares[vidx], helper[1].round->input_shares[vidx],
                   helper[2].round->input_shares[vidx], helper[3].round->input_shares[vidx],
                   pp->input_size);
  // hash communicated bits
  hash_update_x4_4(&ctx, helper[0].round->communicated_bits[vidx],
                   helper[1].round->communicated_bits[vidx],
                   helper[2].round->communicated_bits[vidx],
                   helper[3].round->communicated_bits[vidx], pp->view_size);
  // hash output share
  hash_update_x4_4(&ctx, helper[0].round->output_shares[vidx], helper[1].round->output_shares[vidx],
                   helper[2].round->output_shares[vidx], helper[3].round->output_shares[vidx],
                   pp->output_size);
  hash_final_x4(&ctx);
  hash_squeeze_x4_4(&ctx, helper[0].round->commitments[vidx], helper[1].round->commitments[vidx],
                    helper[2].round->commitments[vidx], helper[3].round->commitments[vidx],
                    hashlen);
}

/**
 * Compute challenge from transform dependent hash - outputs {1,2 or 3}^t
 */
static void H3_compute(const picnic_instance_t* pp, uint8_t* hash, uint8_t* ch) {
  const size_t digest_size      = pp->digest_size;
  const size_t digest_size_bits = digest_size << 3;

  // Pick bits from hash
  uint8_t* eof   = ch + pp->num_rounds;
  size_t bit_idx = 0;
  while (ch < eof) {
    if (bit_idx >= digest_size_bits) {
      hash_context ctx;
      hash_init_prefix(&ctx, digest_size, HASH_PREFIX_1);
      hash_update(&ctx, hash, digest_size);
      hash_final(&ctx);
      hash_squeeze(&ctx, hash, digest_size);
      bit_idx = 0;
    }

    const uint8_t twobits = (hash[bit_idx >> 3] >> ((6 - (bit_idx & 0x7)))) & 0x3;
    if (twobits != 0x3) {
      *ch++ = twobits;
    }
    bit_idx += 2;
  }
}

/**
 * Hash public key, salt and message
 */
static void H3_public_key_message(hash_context* ctx, const picnic_instance_t* pp,
                                  const uint8_t* salt, const picnic_context_t* context) {
  // hash circuit out and input (public key)
  hash_update(ctx, context->public_key, pp->output_size);
  hash_update(ctx, context->plaintext, pp->input_size);
  // hash salt
  hash_update(ctx, salt, SALT_SIZE);
  // hash message
  hash_update(ctx, context->msg, context->msglen);
}

/**
 * Re-compute challenge for verification
 */
static void H3_verify(const picnic_instance_t* pp, sig_proof_t* prf,
                      const picnic_context_t* context, uint8_t* ch) {
  const size_t digest_size = pp->digest_size;
  const size_t num_rounds  = pp->num_rounds;
  const size_t output_size = pp->output_size;

  hash_context ctx;
  hash_init_prefix(&ctx, digest_size, HASH_PREFIX_1);

  // hash output shares
  proof_round_t* round = prf->round;
  for (size_t i = 0; i < num_rounds; ++i, ++round) {
    switch (prf->challenge[i]) {
    case 0: {
      hash_update(&ctx, round->output_shares[0], output_size);
      hash_update(&ctx, round->output_shares[1], output_size);
      hash_update(&ctx, round->output_shares[2], output_size);
      break;
    }
    case 1: {
      hash_update(&ctx, round->output_shares[2], output_size);
      hash_update(&ctx, round->output_shares[0], output_size);
      hash_update(&ctx, round->output_shares[1], output_size);
      break;
    }
    default: {
      hash_update(&ctx, round->output_shares[1], output_size);
      hash_update(&ctx, round->output_shares[2], output_size);
      hash_update(&ctx, round->output_shares[0], output_size);
      break;
    }
    }
  }

  // hash commitments
  round = prf->round;
  for (size_t i = 0; i < num_rounds; ++i, ++round) {
    switch (prf->challenge[i]) {
    case 0: {
      hash_update(&ctx, round->commitments[0], digest_size);
      hash_update(&ctx, round->commitments[1], digest_size);
      hash_update(&ctx, round->commitments[2], digest_size);
      break;
    }
    case 1: {
      hash_update(&ctx, round->commitments[2], digest_size);
      hash_update(&ctx, round->commitments[0], digest_size);
      hash_update(&ctx, round->commitments[1], digest_size);
      break;
    }
    default: {
      hash_update(&ctx, round->commitments[1], digest_size);
      hash_update(&ctx, round->commitments[2], digest_size);
      hash_update(&ctx, round->commitments[0], digest_size);
      break;
    }
    }
  }

#if defined(WITH_UNRUH)
  if (is_unruh(pp)) {
    const size_t without_input_bytes_size = pp->unruh_without_input_bytes_size;
    const size_t with_input_bytes_size    = pp->unruh_with_input_bytes_size;

    // hash commitments
    round = prf->round;
    for (size_t i = 0; i < num_rounds; ++i, ++round) {
      switch (prf->challenge[i]) {
      case 0: {
        hash_update(&ctx, round->gs[0], without_input_bytes_size);
        hash_update(&ctx, round->gs[1], without_input_bytes_size);
        hash_update(&ctx, round->gs[2], with_input_bytes_size);
        break;
      }
      case 1: {
        hash_update(&ctx, round->gs[2], without_input_bytes_size);
        hash_update(&ctx, round->gs[0], without_input_bytes_size);
        hash_update(&ctx, round->gs[1], with_input_bytes_size);
        break;
      }
      default: {
        hash_update(&ctx, round->gs[1], without_input_bytes_size);
        hash_update(&ctx, round->gs[2], without_input_bytes_size);
        hash_update(&ctx, round->gs[0], with_input_bytes_size);
        break;
      }
      }
    }
  }
#endif

  // hash public key, salt, and message
  H3_public_key_message(&ctx, pp, prf->salt, context);
  hash_final(&ctx);

  uint8_t hash[MAX_DIGEST_SIZE];
  hash_squeeze(&ctx, hash, digest_size);
  H3_compute(pp, hash, ch);
}

/**
 * Compute challenge
 */
static void H3(const picnic_instance_t* pp, sig_proof_t* prf, const picnic_context_t* context) {
  const size_t num_rounds = pp->num_rounds;

  hash_context ctx;
  hash_init_prefix(&ctx, pp->digest_size, HASH_PREFIX_1);

  // hash output shares
  hash_update(&ctx, prf->round[0].output_shares[0], pp->output_size * num_rounds * SC_PROOF);
  // hash all commitments C
  hash_update(&ctx, prf->round[0].commitments[0], pp->digest_size * num_rounds * SC_PROOF);
#if defined(WITH_UNRUH)
  if (is_unruh(pp)) {
    // hash all commitments G
    hash_update(&ctx, prf->round[0].gs[0],
                num_rounds * ((SC_PROOF - 1) * pp->unruh_without_input_bytes_size +
                              pp->unruh_with_input_bytes_size));
  }
#endif
  // hash public key, salt, and message
  H3_public_key_message(&ctx, pp, prf->salt, context);
  hash_final(&ctx);

  uint8_t hash[MAX_DIGEST_SIZE];
  hash_squeeze(&ctx, hash, pp->digest_size);
  /* parts of this hash will be published as challenge so is public anyway */
  picnic_declassify(hash, MAX_DIGEST_SIZE);
  H3_compute(pp, hash, prf->challenge);
}

#if defined(WITH_UNRUH)
/*
 * G permutation for Unruh transform
 */
static void unruh_G(const picnic_instance_t* pp, proof_round_t* prf_round, unsigned int vidx,
                    bool include_is) {
  const size_t outputlen =
      include_is ? pp->unruh_with_input_bytes_size : pp->unruh_without_input_bytes_size;
  const size_t digest_size = pp->digest_size;
  const size_t seedlen     = pp->seed_size;

  // Hash the seed with H_5, store digest in output
  hash_context ctx;
  hash_init_prefix(&ctx, digest_size, HASH_PREFIX_5);
  hash_update(&ctx, prf_round->seeds[vidx], seedlen);
  hash_final(&ctx);

  uint8_t tmp[MAX_DIGEST_SIZE];
  hash_squeeze(&ctx, tmp, digest_size);

  // Hash H_5(seed), the view, and the length
  hash_init(&ctx, digest_size);
  hash_update(&ctx, tmp, digest_size);
  if (include_is) {
    hash_update(&ctx, prf_round->input_shares[vidx], pp->input_size);
  }
  hash_update(&ctx, prf_round->communicated_bits[vidx], pp->view_size);
  hash_update_uint16_le(&ctx, outputlen);
  hash_final(&ctx);
  hash_squeeze(&ctx, prf_round->gs[vidx], outputlen);
}

/*
 * 4x G permutation for Unruh transform
 */
static void unruh_G_x4(const picnic_instance_t* pp, proof_round_t* prf_round, unsigned int vidx,
                       bool include_is) {
  const size_t outputlen =
      include_is ? pp->unruh_with_input_bytes_size : pp->unruh_without_input_bytes_size;
  const size_t digest_size = pp->digest_size;
  const size_t seedlen     = pp->seed_size;

  // Hash the seed with H_5, store digest in output
  hash_context_x4 ctx;
  hash_init_prefix_x4(&ctx, digest_size, HASH_PREFIX_5);
  hash_update_x4_4(&ctx, prf_round[0].seeds[vidx], prf_round[1].seeds[vidx],
                   prf_round[2].seeds[vidx], prf_round[3].seeds[vidx], seedlen);
  hash_final_x4(&ctx);

  uint8_t tmp[4][MAX_DIGEST_SIZE];
  hash_squeeze_x4_4(&ctx, tmp[0], tmp[1], tmp[2], tmp[3], digest_size);

  // Hash H_5(seed), the view, and the length
  hash_init_x4(&ctx, digest_size);
  hash_update_x4_4(&ctx, tmp[0], tmp[1], tmp[2], tmp[3], digest_size);
  if (include_is) {
    hash_update_x4_4(&ctx, prf_round[0].input_shares[vidx], prf_round[1].input_shares[vidx],
                     prf_round[2].input_shares[vidx], prf_round[3].input_shares[vidx],
                     pp->input_size);
  }
  hash_update_x4_4(&ctx, prf_round[0].communicated_bits[vidx], prf_round[1].communicated_bits[vidx],
                   prf_round[2].communicated_bits[vidx], prf_round[3].communicated_bits[vidx],
                   pp->view_size);
  hash_update_x4_uint16_le(&ctx, outputlen);
  hash_final_x4(&ctx);
  hash_squeeze_x4_4(&ctx, prf_round[0].gs[vidx], prf_round[1].gs[vidx], prf_round[2].gs[vidx],
                    prf_round[3].gs[vidx], outputlen);
}

/*
 * 4x G permutation for Unruh transform, for verification
 */
static void unruh_G_x4_verify(const picnic_instance_t* pp, const sorting_helper_t* helper,
                              unsigned int vidx, bool include_is) {
  const size_t outputlen =
      include_is ? pp->unruh_with_input_bytes_size : pp->unruh_without_input_bytes_size;
  const size_t digest_size = pp->digest_size;
  const size_t seedlen     = pp->seed_size;

  // Hash the seed with H_5, store digest in output
  hash_context_x4 ctx;
  hash_init_prefix_x4(&ctx, digest_size, HASH_PREFIX_5);
  hash_update_x4_4(&ctx, helper[0].round->seeds[vidx], helper[1].round->seeds[vidx],
                   helper[2].round->seeds[vidx], helper[3].round->seeds[vidx], seedlen);
  hash_final_x4(&ctx);

  uint8_t tmp[4][MAX_DIGEST_SIZE];
  hash_squeeze_x4_4(&ctx, tmp[0], tmp[1], tmp[2], tmp[3], digest_size);

  // Hash H_5(seed), the view, and the length
  hash_init_x4(&ctx, digest_size);
  hash_update_x4_4(&ctx, tmp[0], tmp[1], tmp[2], tmp[3], digest_size);
  if (include_is) {
    hash_update_x4_4(&ctx, helper[0].round->input_shares[vidx], helper[1].round->input_shares[vidx],
                     helper[2].round->input_shares[vidx], helper[3].round->input_shares[vidx],
                     pp->input_size);
  }
  hash_update_x4_4(&ctx, helper[0].round->communicated_bits[vidx],
                   helper[1].round->communicated_bits[vidx],
                   helper[2].round->communicated_bits[vidx],
                   helper[3].round->communicated_bits[vidx], pp->view_size);
  hash_update_x4_uint16_le(&ctx, outputlen);
  hash_final_x4(&ctx);
  hash_squeeze_x4_4(&ctx, helper[0].round->gs[vidx], helper[1].round->gs[vidx],
                    helper[2].round->gs[vidx], helper[3].round->gs[vidx], outputlen);
}
#endif

// serilization helper functions
static int sig_proof_to_char_array(const picnic_instance_t* pp, const sig_proof_t* prf,
                                   uint8_t* result, size_t* siglen) {
  const uint32_t num_rounds     = pp->num_rounds;
  const uint32_t seed_size      = pp->seed_size;
  const uint32_t challenge_size = pp->collapsed_challenge_size;
  const uint32_t digest_size    = pp->digest_size;
  const size_t view_size        = pp->view_size;
  const size_t input_size       = pp->input_size;
#if defined(WITH_UNRUH)
  const size_t unruh_with_input_bytes_size    = pp->unruh_with_input_bytes_size;
  const size_t unruh_without_input_bytes_size = pp->unruh_without_input_bytes_size;
  const bool unruh                            = is_unruh(pp);
#endif

  uint8_t* tmp = result;

  // write challenge
  collapse_challenge(tmp, pp, prf->challenge);
  tmp += challenge_size;

  // write salt
  memcpy(tmp, prf->salt, SALT_SIZE);
  tmp += SALT_SIZE;

  const proof_round_t* round = prf->round;
  for (unsigned int i = 0; i < num_rounds; ++i, ++round) {
    const unsigned int a = prf->challenge[i];
    const unsigned int b = (a + 1) % 3;
    const unsigned int c = (a + 2) % 3;

    // write commitment
    memcpy(tmp, round->commitments[c], digest_size);
    tmp += digest_size;

#if defined(WITH_UNRUH)
    // write unruh G
    if (unruh) {
      const uint32_t unruh_g_size =
          a ? unruh_without_input_bytes_size : unruh_with_input_bytes_size;
      memcpy(tmp, round->gs[c], unruh_g_size);
      tmp += unruh_g_size;
    }
#endif

    // write views
    memcpy(tmp, round->communicated_bits[b], view_size);
    tmp += view_size;

    // write seeds
    memcpy(tmp, round->seeds[a], seed_size);
    tmp += seed_size;
    memcpy(tmp, round->seeds[b], seed_size);
    tmp += seed_size;

    if (a) {
      // write input share
      memcpy(tmp, round->input_shares[SC_PROOF - 1], input_size);
      tmp += input_size;
    }
  }

  *siglen = tmp - result;
  return 0;
}

static sig_proof_t* sig_proof_from_char_array(const picnic_instance_t* pp, const uint8_t* data,
                                              size_t len) {
  const size_t digest_size            = pp->digest_size;
  const size_t seed_size              = pp->seed_size;
  const size_t num_rounds             = pp->num_rounds;
  const size_t challenge_size         = pp->collapsed_challenge_size;
  const size_t input_size             = pp->input_size;
  const size_t view_size              = pp->view_size;
  const unsigned int view_diff        = pp->view_size * 8 - pp->view_round_size * pp->lowmc.r;
  const unsigned int input_share_diff = pp->input_size * 8 - pp->lowmc.k;
#if defined(WITH_UNRUH)
  const size_t without_input_bytes_size = pp->unruh_without_input_bytes_size;
  const size_t with_input_bytes_size    = pp->unruh_with_input_bytes_size;
  const bool unruh                      = is_unruh(pp);
#endif

  uint8_t* slab      = NULL;
  sig_proof_t* proof = proof_new_verify(pp, &slab);
  if (!proof) {
    return NULL;
  }

  size_t remaining_len = len;
  const uint8_t* tmp   = data;

  // read and process challenge
  if (sub_overflow_size_t(remaining_len, challenge_size, &remaining_len)) {
    goto err;
  }
  if (!expand_challenge(proof->challenge, pp, tmp)) {
    goto err;
  }
  tmp += challenge_size;

  // read salt
  if (sub_overflow_size_t(remaining_len, SALT_SIZE, &remaining_len)) {
    goto err;
  }
  memcpy(proof->salt, tmp, SALT_SIZE);
  tmp += SALT_SIZE;

  const size_t base_size = digest_size + view_size + 2 * seed_size;
  proof_round_t* round   = proof->round;
  for (unsigned int i = 0; i < num_rounds; ++i, ++round) {
    const unsigned char ch = proof->challenge[i];
    size_t requested_size  = base_size + (ch ? input_size : 0);
#if defined(WITH_UNRUH)
    const size_t unruh_g_len = ch ? without_input_bytes_size : with_input_bytes_size;
    requested_size += unruh_g_len;
#endif

    if (sub_overflow_size_t(remaining_len, requested_size, &remaining_len)) {
      goto err;
    }

    // read commitments
    round->commitments[2] = (uint8_t*)tmp;
    tmp += digest_size;

#if defined(WITH_UNRUH)
    // read unruh G
    if (unruh) {
      round->gs[2] = (uint8_t*)tmp;
      tmp += unruh_g_len;
    }
#endif

    // read view
    round->communicated_bits[1] = (uint8_t*)tmp;
    if (check_padding_bits(round->communicated_bits[1][view_size - 1], view_diff)) {
      goto err;
    }
    tmp += view_size;

    // read seeds
    round->seeds[0] = (uint8_t*)tmp;
    tmp += seed_size;
    round->seeds[1] = (uint8_t*)tmp;
    tmp += seed_size;

    // read input shares
    switch (ch) {
    case 0:
      round->input_shares[0] = slab;
      slab += input_size;
      round->input_shares[1] = slab;
      slab += input_size;
      break;
    case 1:
      round->input_shares[0] = slab;
      slab += input_size;
      round->input_shares[1] = (uint8_t*)tmp;
      if (check_padding_bits(round->input_shares[1][input_size - 1], input_share_diff)) {
        goto err;
      }
      tmp += input_size;
      break;
    default:
      round->input_shares[0] = (uint8_t*)tmp;
      if (check_padding_bits(round->input_shares[0][input_size - 1], input_share_diff)) {
        goto err;
      }
      tmp += input_size;
      round->input_shares[1] = slab;
      slab += input_size;
    }
  }

  if (remaining_len) {
    goto err;
  }

  return proof;

err:
  proof_free(proof);
  return NULL;
}

static void generate_seeds(const picnic_instance_t* pp, const picnic_context_t* context,
                           uint8_t* seeds, uint8_t* salt) {
  const size_t seed_size   = pp->seed_size;
  const size_t num_rounds  = pp->num_rounds;
  const size_t input_size  = pp->input_size;
  const size_t output_size = pp->output_size;
  const size_t lowmc_n     = pp->lowmc.n;

  kdf_shake_t ctx;
  kdf_shake_init(&ctx, pp->digest_size);
  // sk || m || C || p
  kdf_shake_update_key(&ctx, context->private_key, input_size);
  kdf_shake_update_key(&ctx, context->msg, context->msglen);
  kdf_shake_update_key(&ctx, context->public_key, output_size);
  kdf_shake_update_key(&ctx, context->plaintext, output_size);
  // N as 16 bit LE integer
  kdf_shake_update_key_uint16_le(&ctx, lowmc_n);
#if defined(WITH_EXTRA_RANDOMNESS)
  // Add extra random bytes for fault attack mitigation
  unsigned char buffer[2 * MAX_DIGEST_SIZE];
  OQS_randombytes(buffer, 2 * seed_size);
  kdf_shake_update_key(&ctx, buffer, 2 * seed_size);
#endif
  kdf_shake_finalize_key(&ctx);

  // Generate seeds and salt
  kdf_shake_get_randomness(&ctx, seeds, seed_size * num_rounds * SC_PROOF);
  kdf_shake_get_randomness(&ctx, salt, SALT_SIZE);
  kdf_shake_clear(&ctx);
}

int impl_sign(const picnic_instance_t* pp, const picnic_context_t* context, uint8_t* sig,
              size_t* siglen) {
  const size_t num_rounds  = pp->num_rounds;
  const size_t input_size  = pp->input_size;
  const size_t output_size = pp->output_size;
  const size_t lowmc_r     = pp->lowmc.r;
  const size_t view_size   = pp->view_size;
  const size_t aview_size  = ALIGNU64T(view_size);
  const unsigned int diff  = input_size * 8 - pp->lowmc.n;
#if defined(WITH_UNRUH)
  const bool unruh = is_unruh(pp);
#endif

  const zkbpp_lowmc_implementation_f lowmc_impl       = pp->impls.zkbpp_lowmc;
  const lowmc_store_implementation_f lowmc_store_impl = pp->impls.lowmc_store;
  const zkbpp_share_implementation_f mzd_share        = pp->impls.mzd_share;

  // Perform LowMC evaluation and record state before AND gates
  recorded_state_t* recorded_state = aligned_alloc(32, sizeof(recorded_state_t) * (lowmc_r + 1));
  lowmc_store_impl(context->m_key, context->m_plaintext, recorded_state);

  sig_proof_t* prf = proof_new(pp);
  view_t* views    = aligned_alloc(32, sizeof(view_t) * lowmc_r);

  in_out_shares_t in_out_shares[2];

  // Generate seeds
  generate_seeds(pp, context, prf->round[0].seeds[0], prf->salt);

  rvec_t* rvec = aligned_alloc(32, sizeof(rvec_t) * lowmc_r); // random tapes for AND-gates
  uint8_t* tape_bytes_x4 = aligned_alloc(sizeof(uint64_t), SC_PROOF * 4 * aview_size);

  proof_round_t* round = prf->round;
  unsigned int i       = 0;
  for (; i < (num_rounds / 4) * 4; i += 4, round += 4) {
    // use 4 parallel instances of keccak for speedup
    {
      kdf_shake_x4_t kdfs[SC_PROOF];
      for (unsigned int j = 0; j < SC_PROOF; ++j) {
        const bool include_input_size   = (j != SC_PROOF - 1);
        const uint8_t* seeds[4]         = {round[0].seeds[j], round[1].seeds[j], round[2].seeds[j],
                                   round[3].seeds[j]};
        const uint16_t round_numbers[4] = {i, i + 1, i + 2, i + 3};
        kdf_init_x4_from_seed(&kdfs[j], seeds, prf->salt, round_numbers, j, include_input_size, pp);
      }

      // compute sharing
      for (unsigned int j = 0; j < SC_PROOF - 1; ++j) {
        kdf_shake_x4_get_randomness_4(&kdfs[j], round[0].input_shares[j], round[1].input_shares[j],
                                      round[2].input_shares[j], round[3].input_shares[j],
                                      input_size);
      }
      // compute random tapes
      for (unsigned int j = 0; j < SC_PROOF; ++j) {
        kdf_shake_x4_get_randomness_4(&kdfs[j], &tape_bytes_x4[(j * 4 + 0) * aview_size],
                                      &tape_bytes_x4[(j * 4 + 1) * aview_size],
                                      &tape_bytes_x4[(j * 4 + 2) * aview_size],
                                      &tape_bytes_x4[(j * 4 + 3) * aview_size], view_size);
        kdf_shake_x4_clear(&kdfs[j]);
      }
    }

    for (unsigned int round_offset = 0; round_offset < 4; round_offset++) {
      for (unsigned int j = 0; j < SC_PROOF - 1; ++j) {
        clear_padding_bits(&round[round_offset].input_shares[j][input_size - 1], diff);
        mzd_from_char_array(in_out_shares[0].s[j], round[round_offset].input_shares[j], input_size);
      }
      mzd_share(in_out_shares[0].s[2], in_out_shares[0].s[0], in_out_shares[0].s[1],
                context->m_key);
      mzd_to_char_array(round[round_offset].input_shares[SC_PROOF - 1],
                        in_out_shares[0].s[SC_PROOF - 1], input_size);

      for (unsigned int j = 0; j < SC_PROOF; ++j) {
        decompress_random_tape(rvec, pp, &tape_bytes_x4[(j * 4 + round_offset) * aview_size], j);
      }

      // perform ZKB++ LowMC evaluation
      lowmc_impl(context->m_plaintext, views, in_out_shares, rvec, recorded_state);

      for (unsigned int j = 0; j < SC_PROOF; ++j) {
        mzd_to_char_array(round[round_offset].output_shares[j], in_out_shares[1].s[j], output_size);
        compress_view(round[round_offset].communicated_bits[j], pp, views, j);
      }
    }

    // commitments
    for (unsigned int j = 0; j < SC_PROOF; ++j) {
      hash_commitment_x4(pp, round, j);
    }

#if defined(WITH_UNRUH)
    // unruh G
    if (unruh) {
      for (unsigned int j = 0; j < SC_PROOF; ++j) {
        unruh_G_x4(pp, round, j, j == SC_PROOF - 1);
      }
    }
#endif
  }
  for (; i < num_rounds; ++i, ++round) {
    {
      kdf_shake_t kdfs[SC_PROOF];
      for (unsigned int j = 0; j < SC_PROOF; ++j) {
        const bool include_input_size = (j != SC_PROOF - 1);
        kdf_init_from_seed(&kdfs[j], round->seeds[j], prf->salt, i, j, include_input_size, pp);
      }

      // compute sharing
      for (unsigned int j = 0; j < SC_PROOF - 1; ++j) {
        kdf_shake_get_randomness(&kdfs[j], round->input_shares[j], input_size);
        clear_padding_bits(&round->input_shares[j][input_size - 1], diff);
        mzd_from_char_array(in_out_shares[0].s[j], round->input_shares[j], input_size);
      }
      mzd_share(in_out_shares[0].s[2], in_out_shares[0].s[0], in_out_shares[0].s[1],
                context->m_key);
      mzd_to_char_array(round->input_shares[SC_PROOF - 1], in_out_shares[0].s[SC_PROOF - 1],
                        input_size);

      // compute random tapes
      for (unsigned int j = 0; j < SC_PROOF; ++j) {
        assert(view_size <= MAX_VIEW_SIZE);
        uint8_t tape_bytes[MAX_VIEW_SIZE];
        kdf_shake_get_randomness(&kdfs[j], tape_bytes, view_size);
        decompress_random_tape(rvec, pp, tape_bytes, j);
      }

      for (unsigned int j = 0; j < SC_PROOF; ++j) {
        kdf_shake_clear(&kdfs[j]);
      }
    }

    // perform ZKB++ LowMC evaluation
    lowmc_impl(context->m_plaintext, views, in_out_shares, rvec, recorded_state);

    // commitments
    for (unsigned int j = 0; j < SC_PROOF; ++j) {
      mzd_to_char_array(round->output_shares[j], in_out_shares[1].s[j], output_size);
      compress_view(round->communicated_bits[j], pp, views, j);
      hash_commitment(pp, round, j);
    }

#if defined(WITH_UNRUH)
    // unruh G
    if (unruh) {
      for (unsigned int j = 0; j < SC_PROOF; ++j) {
        unruh_G(pp, round, j, j == SC_PROOF - 1);
      }
    }
#endif
  }
  H3(pp, prf, context);

  const int ret = sig_proof_to_char_array(pp, prf, sig, siglen);

  // clean up
  aligned_free(tape_bytes_x4);
  aligned_free(rvec);
  aligned_free(views);
  proof_free(prf);
  aligned_free(recorded_state);
  return ret;
}

int impl_verify(const picnic_instance_t* pp, const picnic_context_t* context, const uint8_t* sig,
                size_t siglen) {
  const size_t num_rounds  = pp->num_rounds;
  const size_t input_size  = pp->input_size;
  const size_t output_size = pp->output_size;
  const size_t lowmc_r     = pp->lowmc.r;
  const size_t view_size   = pp->view_size;
  const size_t aview_size  = ALIGNU64T(view_size);
  const unsigned int diff  = input_size * 8 - pp->lowmc.n;
#if defined(WITH_UNRUH)
  const bool unruh = is_unruh(pp);
#endif

  const zkbpp_lowmc_verify_implementation_f lowmc_verify_impl = pp->impls.zkbpp_lowmc_verify;
  const zkbpp_share_implementation_f mzd_share                = pp->impls.mzd_share;

  sig_proof_t* prf = sig_proof_from_char_array(pp, sig, siglen);
  if (!prf) {
    return -1;
  }

  in_out_shares_t in_out_shares[2];
  view_t* views = aligned_alloc(32, sizeof(view_t) * lowmc_r);
  rvec_t* rvec  = aligned_alloc(32, sizeof(rvec_t) * lowmc_r); // random tapes for and-gates

  // sort the different challenge rounds based on their H3 index, so we can use the 4x Keccak when
  // verifying since all of this is public information, there is no leakage
  sorting_helper_t* sorted_rounds = malloc(sizeof(sorting_helper_t) * num_rounds);

  uint8_t* tape_bytes_x4 = aligned_alloc(sizeof(uint64_t), SC_VERIFY * 4 * aview_size);
  for (unsigned int current_chal = 0; current_chal < 3; current_chal++) {
    unsigned int num_current_rounds = 0;
    for (unsigned int r = 0; r < num_rounds; r++) {
      if (prf->challenge[r] == current_chal) {
        sorted_rounds[num_current_rounds].round = &prf->round[r];
        num_current_rounds++;
      }
    }
    unsigned int i                 = 0;
    const sorting_helper_t* helper = sorted_rounds;
    for (; i < (num_current_rounds / 4) * 4; i += 4, helper += 4) {
      const unsigned int a_i = current_chal;
      const unsigned int b_i = (a_i + 1) % 3;
      const unsigned int c_i = (a_i + 2) % 3;

      {
        kdf_shake_x4_t kdfs[SC_VERIFY];
        const uint16_t round_numbers[4] = {
            helper[0].round - prf->round, helper[1].round - prf->round,
            helper[2].round - prf->round, helper[3].round - prf->round};
        for (unsigned int j = 0; j < SC_VERIFY; ++j) {
          const bool include_input_size    = (j == 0 && b_i) || (j == 1 && c_i);
          const unsigned int player_number = (j == 0) ? a_i : b_i;
          const uint8_t* seeds[4]          = {helper[0].round->seeds[j], helper[1].round->seeds[j],
                                     helper[2].round->seeds[j], helper[3].round->seeds[j]};
          kdf_init_x4_from_seed(&kdfs[j], seeds, prf->salt, round_numbers, player_number,
                                include_input_size, pp);
        }

        // compute input shares if necessary
        if (b_i) {
          kdf_shake_x4_get_randomness_4(
              &kdfs[0], helper[0].round->input_shares[0], helper[1].round->input_shares[0],
              helper[2].round->input_shares[0], helper[3].round->input_shares[0], input_size);
        }
        if (c_i) {
          kdf_shake_x4_get_randomness_4(
              &kdfs[1], helper[0].round->input_shares[1], helper[1].round->input_shares[1],
              helper[2].round->input_shares[1], helper[3].round->input_shares[1], input_size);
        }
        // compute random tapes
        for (unsigned int j = 0; j < SC_VERIFY; ++j) {
          kdf_shake_x4_get_randomness_4(&kdfs[j], &tape_bytes_x4[(j * 4 + 0) * aview_size],
                                        &tape_bytes_x4[(j * 4 + 1) * aview_size],
                                        &tape_bytes_x4[(j * 4 + 2) * aview_size],
                                        &tape_bytes_x4[(j * 4 + 3) * aview_size], view_size);
          kdf_shake_clear(&kdfs[j]);
        }
      }

      for (unsigned int round_offset = 0; round_offset < 4; round_offset++) {
        if (b_i) {
          clear_padding_bits(&helper[round_offset].round->input_shares[0][input_size - 1], diff);
        }
        mzd_from_char_array(in_out_shares[0].s[0], helper[round_offset].round->input_shares[0],
                            input_size);
        if (c_i) {
          clear_padding_bits(&helper[round_offset].round->input_shares[1][input_size - 1], diff);
        }
        mzd_from_char_array(in_out_shares[0].s[1], helper[round_offset].round->input_shares[1],
                            input_size);

        for (unsigned int j = 0; j < SC_VERIFY; ++j) {
          decompress_random_tape(rvec, pp, &tape_bytes_x4[(j * 4 + round_offset) * aview_size], j);
        }

        decompress_view(views, pp, helper[round_offset].round->communicated_bits[1], 1);
        // perform ZKB++ LowMC evaluation
        lowmc_verify_impl(context->m_plaintext, views, in_out_shares, rvec, a_i);
        compress_view(helper[round_offset].round->communicated_bits[0], pp, views, 0);

        mzd_share(in_out_shares[1].s[2], in_out_shares[1].s[0], in_out_shares[1].s[1],
                  context->m_key);
        // recompute commitments
        for (unsigned int j = 0; j < SC_VERIFY; ++j) {
          mzd_to_char_array(helper[round_offset].round->output_shares[j], in_out_shares[1].s[j],
                            output_size);
        }
        mzd_to_char_array(helper[round_offset].round->output_shares[SC_VERIFY],
                          in_out_shares[1].s[SC_VERIFY], output_size);
      }
      for (unsigned int j = 0; j < SC_VERIFY; ++j) {
        hash_commitment_x4_verify(pp, helper, j);
      }
#if defined(WITH_UNRUH)
      if (unruh) {
        // apply Unruh G permutation
        for (unsigned int j = 0; j < SC_VERIFY; ++j) {
          unruh_G_x4_verify(pp, helper, j, (a_i == 1 && j == 1) || (a_i == 2 && j == 0));
        }
      }
#endif
    }
    for (; i < num_current_rounds; ++i, ++helper) {
      const unsigned int a_i = current_chal;
      const unsigned int b_i = (a_i + 1) % 3;
      const unsigned int c_i = (a_i + 2) % 3;

      {
        kdf_shake_t kdfs[SC_VERIFY];
        const uint16_t round_number = helper->round - prf->round;
        for (unsigned int j = 0; j < SC_VERIFY; ++j) {
          const bool include_input_size    = (j == 0 && b_i) || (j == 1 && c_i);
          const unsigned int player_number = (j == 0) ? a_i : b_i;
          kdf_init_from_seed(&kdfs[j], helper->round->seeds[j], prf->salt, round_number,
                             player_number, include_input_size, pp);
        }

        // compute input shares if necessary
        if (b_i) {
          kdf_shake_get_randomness(&kdfs[0], helper->round->input_shares[0], input_size);
          clear_padding_bits(&helper->round->input_shares[0][input_size - 1], diff);
        }
        if (c_i) {
          kdf_shake_get_randomness(&kdfs[1], helper->round->input_shares[1], input_size);
          clear_padding_bits(&helper->round->input_shares[1][input_size - 1], diff);
        }

        mzd_from_char_array(in_out_shares[0].s[0], helper->round->input_shares[0], input_size);
        mzd_from_char_array(in_out_shares[0].s[1], helper->round->input_shares[1], input_size);

        // compute random tapes
        for (unsigned int j = 0; j < SC_VERIFY; ++j) {
          assert(view_size <= MAX_VIEW_SIZE);
          uint8_t tape_bytes[MAX_VIEW_SIZE];
          kdf_shake_get_randomness(&kdfs[j], tape_bytes, view_size);
          decompress_random_tape(rvec, pp, tape_bytes, j);
        }

        for (unsigned int j = 0; j < SC_VERIFY; ++j) {
          kdf_shake_clear(&kdfs[j]);
        }
      }

      decompress_view(views, pp, helper->round->communicated_bits[1], 1);
      // perform ZKB++ LowMC evaluation
      lowmc_verify_impl(context->m_plaintext, views, in_out_shares, rvec, a_i);
      compress_view(helper->round->communicated_bits[0], pp, views, 0);

      mzd_share(in_out_shares[1].s[2], in_out_shares[1].s[0], in_out_shares[1].s[1],
                context->m_key);
      // recompute commitments
      for (unsigned int j = 0; j < SC_VERIFY; ++j) {
        mzd_to_char_array(helper->round->output_shares[j], in_out_shares[1].s[j], output_size);
        hash_commitment(pp, helper->round, j);
      }
      mzd_to_char_array(helper->round->output_shares[SC_VERIFY], in_out_shares[1].s[SC_VERIFY],
                        output_size);

#if defined(WITH_UNRUH)
      if (unruh) {
        // apply Unruh G permutation
        for (unsigned int j = 0; j < SC_VERIFY; ++j) {
          unruh_G(pp, helper->round, j, (a_i == 1 && j == 1) || (a_i == 2 && j == 0));
        }
      }
#endif
    }
  }

  assert(pp->num_rounds <= MAX_NUM_ROUNDS);
  unsigned char challenge[MAX_NUM_ROUNDS] = {0};
  H3_verify(pp, prf, context, challenge);
  const int success_status = memcmp(challenge, prf->challenge, pp->num_rounds);

  // clean up
  aligned_free(tape_bytes_x4);
  free(sorted_rounds);
  aligned_free(rvec);
  aligned_free(views);

  proof_free(prf);

  return success_status;
}

/* OQS note: cropped unused visualization functions */
