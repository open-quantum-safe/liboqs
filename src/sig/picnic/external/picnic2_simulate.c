/*! @file picnic2_impl.c
 *  @brief This is the main file of the signature scheme for the Picnic2
 *  parameter sets.
 *
 *  This file is part of the reference implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#if !defined(_MSC_VER)
#include <stdalign.h>
#endif

#include "picnic2_simulate.h"
#include "picnic2_simulate_mul.h"
#include "io.h"

static void wordToMsgsNoTranspose(uint64_t w, msgs_t* msgs) {
  ((uint64_t*)msgs->msgs[msgs->pos % 64])[msgs->pos / 64] = w;
  msgs->pos++;
}

static void msgsTranspose(msgs_t* msgs) {
  uint64_t buffer[64] ATTR_ALIGNED(32);
  size_t pos;
  for (pos = 0; pos < msgs->pos / 64; pos++) {
    for (size_t i = 0; i < 64; i++) {
      buffer[i] = ((uint64_t*)msgs->msgs[i])[pos];
    }
    transpose_64_64(buffer, buffer);
    for (size_t i = 0; i < 64; i++) {
      ((uint64_t*)msgs->msgs[i])[pos] = buffer[i];
    }
  }
  memset(buffer, 0, 64 * sizeof(uint64_t));
  for (size_t i = 0; i < msgs->pos % 64; i++) {
    buffer[i] = ((uint64_t*)msgs->msgs[i])[pos];
  }
  transpose_64_64(buffer, buffer);
  for (size_t i = 0; i < 64; i++) {
    ((uint64_t*)msgs->msgs[i])[pos] = buffer[i];
  }
}

/* For each word in shares; write player i's share to their stream of msgs */
static void broadcast(shares_t* shares, msgs_t* msgs) {
  for (size_t w = 0; w < shares->numWords; w++) {
    wordToMsgsNoTranspose(shares->shares[w], msgs);
  }
}

/* For an input bit b = 0 or 1, return the word of all b bits, i.e.,
 * extend(1) = 0xFFFFFFFFFFFFFFFF
 * extend(0) = 0x0000000000000000
 * Assumes inputs are always 0 or 1.  If this doesn't hold, add "& 1" to the
 * input.
 */
static inline uint64_t extend(uint64_t bit) {
  return ~(bit - 1);
}

static uint8_t mpc_AND(uint8_t a, uint8_t b, uint64_t mask_a, uint64_t mask_b, randomTape_t* tapes,
                       msgs_t* msgs, uint64_t* out, uint8_t* unopened_msg) {
  uint64_t output_mask = tapesToWord(tapes); // A fresh random mask to hide the result

  *out = output_mask;
  uint64_t and_helper =
      tapesToWord(tapes); // The special mask value setup during preprocessing for each AND gate
  uint64_t s_shares = (extend(a) & mask_b) ^ (extend(b) & mask_a) ^ and_helper ^ output_mask;

  if (msgs->unopened >= 0) {
    uint8_t unopenedPartyBit = getBit(unopened_msg, msgs->pos);
    setBit((uint8_t*)&s_shares, msgs->unopened, unopenedPartyBit);
  }

  // Broadcast each share of s
  wordToMsgsNoTranspose(s_shares, msgs);

  return (uint8_t)(parity64_uint64(s_shares) ^ (a & b));
}

static void mpc_sbox(mzd_local_t* statein, shares_t* state_masks, randomTape_t* tapes, msgs_t* msgs,
                     uint8_t* unopenened_msg, const picnic_instance_t* params) {
  uint8_t state[MAX_LOWMC_BLOCK_SIZE];
  mzd_to_char_array(state, statein, params->lowmc->n / 8);
  for (size_t i = 0; i < params->lowmc->m * 3; i += 3) {
    uint8_t a       = getBit((uint8_t*)state, i + 2);
    uint64_t mask_a = state_masks->shares[i + 2];

    uint8_t b       = getBit((uint8_t*)state, i + 1);
    uint64_t mask_b = state_masks->shares[i + 1];

    uint8_t c       = getBit((uint8_t*)state, i);
    uint64_t mask_c = state_masks->shares[i];

    uint64_t bc_mask, ab_mask, ca_mask; // Fresh output masks used for the AND gate

    uint8_t ab = mpc_AND(a, b, mask_a, mask_b, tapes, msgs, &ab_mask, unopenened_msg);
    uint8_t bc = mpc_AND(b, c, mask_b, mask_c, tapes, msgs, &bc_mask, unopenened_msg);
    uint8_t ca = mpc_AND(c, a, mask_c, mask_a, tapes, msgs, &ca_mask, unopenened_msg);

    setBit((uint8_t*)state, i + 2, a ^ bc);
    state_masks->shares[i + 2] = mask_a ^ bc_mask;
    setBit((uint8_t*)state, i + 1, a ^ b ^ ca);
    state_masks->shares[i + 1] = mask_b ^ mask_a ^ ca_mask;
    setBit((uint8_t*)state, i, a ^ b ^ c ^ ab);
    state_masks->shares[i] = mask_a ^ mask_b ^ mask_c ^ ab_mask;
  }
  mzd_from_char_array(statein, state, params->lowmc->n / 8);
}

#if defined(REDUCED_ROUND_KEY_COMPUTATION)
static void mpc_xor_masks_nl(shares_t* out, const shares_t* a, const shares_t* b, size_t index,
                             size_t num) {
  for (size_t i = 0; i < num; i++) {
    out->shares[i] = a->shares[i] ^ b->shares[index + num - 1 - i];
  }
}
#endif

#if defined(OPTIMIZED_LINEAR_LAYER_EVALUATION)
static void mpc_shuffle(mzd_local_t* state, shares_t* mask_shares, uint64_t r_mask) {
  if (mask_shares->numWords == 128) {
    mzd_shuffle_128_30(state, r_mask);
  }
  else if (mask_shares->numWords == 192) {
    mzd_shuffle_192_30(state, r_mask);
  }
  else if (mask_shares->numWords == 256) {
    mzd_shuffle_256_30(state, r_mask);
  } else {
    assert(false && "invalid state size");
  }
  for (int i = 63; i >= 0 && r_mask != UINT64_C(0xFFFFFFFC00000000); i--) {
    if (!((r_mask >> i) & 1)) { // bit is not set
      // find next 1 and swap all entries until then
      for (int j = i - 1; j >= 0; j--) {
        if ((r_mask >> j) & 1) {
          for (int k = j; k < i; k++) {
            uint64_t t                      = mask_shares->shares[63 - k];
            mask_shares->shares[63 - k]     = mask_shares->shares[63 - k - 1];
            mask_shares->shares[63 - k - 1] = t;
          }
          r_mask |= (UINT64_C(1) << i);  // set bit i
          r_mask &= ~(UINT64_C(1) << j); // clear bit j
          break;
        }
      }
    }
  }
}
#endif

#if !defined(REDUCED_ROUND_KEY_COMPUTATION) || defined(OPTIMIZED_LINEAR_LAYER_EVALUATION)
static void mpc_xor_masks(shares_t* out, const shares_t* a, const shares_t* b) {
  assert(out->numWords == a->numWords && a->numWords == b->numWords);

  for (size_t i = 0; i < out->numWords; i++) {
    out->shares[i] = a->shares[i] ^ b->shares[i];
  }
}
#endif

#if !defined(NO_UINT64_FALLBACK)
/* PICNIC2_L1_FS */
#define XOR mzd_xor_uint64_128
#define MPC_MUL mpc_matrix_mul_uint64_128
#define MPC_MUL_MC mpc_matrix_mul_nl_part_uint64_128
#define MPC_ADDMUL_R mpc_matrix_addmul_r_uint64_128
#define MPC_MUL_Z mpc_matrix_mul_z_uint64_128

#define LOWMC_N LOWMC_L1_N
#define LOWMC_R LOWMC_L1_R
#if defined(WITH_LOWMC_128_128_20)
#include "lowmc_128_128_20.h"
#define LOWMC_INSTANCE lowmc_128_128_20
#define SIM_ONLINE lowmc_simulate_online_uint64_128_10
#include "picnic2_simulate.c.i"
#endif
#undef XOR
#undef MPC_MUL
#undef MPC_MUL_MC
#undef MPC_ADDMUL_R
#undef MPC_MUL_Z
#undef LOWMC_N
#undef LOWMC_R
#undef LOWMC_INSTANCE
#undef SIM_ONLINE

/* PICNIC2_L3_FS */
#define XOR mzd_xor_uint64_192
#define MPC_MUL mpc_matrix_mul_uint64_192
#define MPC_MUL_MC mpc_matrix_mul_nl_part_uint64_192
#define MPC_ADDMUL_R mpc_matrix_addmul_r_uint64_192
#define MPC_MUL_Z mpc_matrix_mul_z_uint64_192

#define LOWMC_N LOWMC_L3_N
#define LOWMC_R LOWMC_L3_R
#if defined(WITH_LOWMC_192_192_30)
#include "lowmc_192_192_30.h"
#define LOWMC_INSTANCE lowmc_192_192_30
#define SIM_ONLINE lowmc_simulate_online_uint64_192_10
#include "picnic2_simulate.c.i"
#endif
#undef XOR
#undef MPC_MUL
#undef MPC_MUL_MC
#undef MPC_ADDMUL_R
#undef MPC_MUL_Z
#undef LOWMC_N
#undef LOWMC_R
#undef LOWMC_INSTANCE
#undef SIM_ONLINE

/* PICNIC2_L5_FS */
#define XOR mzd_xor_uint64_256
#define MPC_MUL mpc_matrix_mul_uint64_256
#define MPC_MUL_MC mpc_matrix_mul_nl_part_uint64_256
#define MPC_ADDMUL_R mpc_matrix_addmul_r_uint64_256
#define MPC_MUL_Z mpc_matrix_mul_z_uint64_256

#define LOWMC_N LOWMC_L5_N
#define LOWMC_R LOWMC_L5_R
#if defined(WITH_LOWMC_256_256_38)
#include "lowmc_256_256_38.h"
#define LOWMC_INSTANCE lowmc_256_256_38
#define SIM_ONLINE lowmc_simulate_online_uint64_256_10
#include "picnic2_simulate.c.i"
#endif
#undef XOR
#undef MPC_MUL
#undef MPC_MUL_MC
#undef MPC_ADDMUL_R
#undef MPC_MUL_Z
#undef LOWMC_N
#undef LOWMC_R
#undef LOWMC_INSTANCE
#undef SIM_ONLINE
#endif

#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
#if defined(WITH_SSE2)
#define FN_ATTR ATTR_TARGET_SSE2
#endif
/* PICNIC2_L1_FS */
#define XOR mzd_xor_s128_128
#define MPC_MUL mpc_matrix_mul_s128_128
#define MPC_MUL_MC mpc_matrix_mul_nl_part_s128_128
#define MPC_ADDMUL_R mpc_matrix_addmul_r_s128_128
#define MPC_MUL_Z mpc_matrix_mul_z_s128_128

#define LOWMC_N LOWMC_L1_N
#define LOWMC_R LOWMC_L1_R
#if defined(WITH_LOWMC_128_128_20)
#include "lowmc_128_128_20.h"
#define LOWMC_INSTANCE lowmc_128_128_20
#define SIM_ONLINE lowmc_simulate_online_s128_128_10
#include "picnic2_simulate.c.i"
#endif
#undef XOR
#undef MPC_MUL
#undef MPC_MUL_MC
#undef MPC_ADDMUL_R
#undef MPC_MUL_Z
#undef LOWMC_N
#undef LOWMC_R
#undef LOWMC_INSTANCE
#undef SIM_ONLINE

/* PICNIC2_L3_FS */
#define XOR mzd_xor_s128_256
#define MPC_MUL mpc_matrix_mul_s128_192
#define MPC_MUL_MC mpc_matrix_mul_nl_part_s128_192
#define MPC_ADDMUL_R mpc_matrix_addmul_r_s128_192
#define MPC_MUL_Z mpc_matrix_mul_z_s128_192

#define LOWMC_N LOWMC_L3_N
#define LOWMC_R LOWMC_L3_R
#if defined(WITH_LOWMC_192_192_30)
#include "lowmc_192_192_30.h"
#define LOWMC_INSTANCE lowmc_192_192_30
#define SIM_ONLINE lowmc_simulate_online_s128_192_10
#include "picnic2_simulate.c.i"
#endif
#undef XOR
#undef MPC_MUL
#undef MPC_MUL_MC
#undef MPC_ADDMUL_R
#undef MPC_MUL_Z
#undef LOWMC_N
#undef LOWMC_R
#undef LOWMC_INSTANCE
#undef SIM_ONLINE

/* PICNIC2_L5_FS */
#define XOR mzd_xor_s128_256
#define MPC_MUL mpc_matrix_mul_s128_256
#define MPC_MUL_MC mpc_matrix_mul_nl_part_s128_256
#define MPC_ADDMUL_R mpc_matrix_addmul_r_s128_256
#define MPC_MUL_Z mpc_matrix_mul_z_s128_256

#define LOWMC_N LOWMC_L5_N
#define LOWMC_R LOWMC_L5_R
#if defined(WITH_LOWMC_256_256_38)
#include "lowmc_256_256_38.h"
#define LOWMC_INSTANCE lowmc_256_256_38
#define SIM_ONLINE lowmc_simulate_online_s128_256_10
#include "picnic2_simulate.c.i"
#endif
#undef XOR
#undef MPC_MUL
#undef MPC_MUL_MC
#undef MPC_ADDMUL_R
#undef MPC_MUL_Z
#undef LOWMC_N
#undef LOWMC_R
#undef LOWMC_INSTANCE
#undef SIM_ONLINE
#undef FN_ATTR

#endif // SSE/NEON

#if defined(WITH_AVX2)
#define FN_ATTR ATTR_TARGET_AVX2
/* PICNIC2_L1_FS */
#define XOR mzd_xor_s256_128
#define MPC_MUL mpc_matrix_mul_s256_128
#define MPC_MUL_MC mpc_matrix_mul_nl_part_s256_128
#define MPC_ADDMUL_R mpc_matrix_addmul_r_s256_128
#define MPC_MUL_Z mpc_matrix_mul_z_s256_128

#define LOWMC_N LOWMC_L1_N
#define LOWMC_R LOWMC_L1_R
#if defined(WITH_LOWMC_128_128_20)
#include "lowmc_128_128_20.h"
#define LOWMC_INSTANCE lowmc_128_128_20
#define SIM_ONLINE lowmc_simulate_online_s256_128_10
#include "picnic2_simulate.c.i"
#endif
#undef XOR
#undef MPC_MUL
#undef MPC_MUL_MC
#undef MPC_ADDMUL_R
#undef MPC_MUL_Z
#undef LOWMC_N
#undef LOWMC_R
#undef LOWMC_INSTANCE
#undef SIM_ONLINE

/* PICNIC2_L3_FS */
#define XOR mzd_xor_s256_256
#define MPC_MUL mpc_matrix_mul_s256_192
#define MPC_MUL_MC mpc_matrix_mul_nl_part_s256_192
#define MPC_ADDMUL_R mpc_matrix_addmul_r_s256_192
#define MPC_MUL_Z mpc_matrix_mul_z_s256_192

#define LOWMC_N LOWMC_L3_N
#define LOWMC_R LOWMC_L3_R
#if defined(WITH_LOWMC_192_192_30)
#include "lowmc_192_192_30.h"
#define LOWMC_INSTANCE lowmc_192_192_30
#define SIM_ONLINE lowmc_simulate_online_s256_192_10
#include "picnic2_simulate.c.i"
#endif
#undef XOR
#undef MPC_MUL
#undef MPC_MUL_MC
#undef MPC_ADDMUL_R
#undef MPC_MUL_Z
#undef LOWMC_N
#undef LOWMC_R
#undef LOWMC_INSTANCE
#undef SIM_ONLINE

/* PICNIC2_L5_FS */
#define XOR mzd_xor_s256_256
#define MPC_MUL mpc_matrix_mul_s256_256
#define MPC_MUL_MC mpc_matrix_mul_nl_part_s256_256
#define MPC_ADDMUL_R mpc_matrix_addmul_r_s256_256
#define MPC_MUL_Z mpc_matrix_mul_z_s256_256

#define LOWMC_N LOWMC_L5_N
#define LOWMC_R LOWMC_L5_R
#if defined(WITH_LOWMC_256_256_38)
#include "lowmc_256_256_38.h"
#define LOWMC_INSTANCE lowmc_256_256_38
#define SIM_ONLINE lowmc_simulate_online_s256_256_10
#include "picnic2_simulate.c.i"
#endif
#undef XOR
#undef MPC_MUL
#undef MPC_MUL_MC
#undef MPC_ADDMUL_R
#undef MPC_MUL_Z
#undef LOWMC_N
#undef LOWMC_R
#undef LOWMC_INSTANCE
#undef SIM_ONLINE
#undef FN_ATTR

#endif // AVX2
#endif // WITH_OPT

lowmc_simulate_online_f lowmc_simulate_online_get_implementation(const lowmc_t* lowmc) {
  ASSUME(lowmc->m == 10);
  ASSUME(lowmc->n == 128 || lowmc->n == 192 || lowmc->n == 256);

#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  if (CPU_SUPPORTS_AVX2) {
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_simulate_online_s256_128_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_simulate_online_s256_192_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_simulate_online_s256_256_10;
#endif
      }
    }
  }
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_simulate_online_s128_128_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_simulate_online_s128_192_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_simulate_online_s128_256_10;
#endif
      }
    }
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
  if (lowmc->m == 10) {
    switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
    case 128:
      return lowmc_simulate_online_uint64_128_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case 192:
      return lowmc_simulate_online_uint64_192_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case 256:
      return lowmc_simulate_online_uint64_256_10;
#endif
    }
  }
#endif

  return NULL;
}
