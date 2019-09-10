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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "kdf_shake.h"
#include "macros.h"
#include "picnic.h"
#include "picnic2_impl.h"
#include "picnic2_simulate_mul.h"
#include "picnic2_tree.h"
#include "picnic2_types.h"
#include "picnic_impl.h"

#define LOWMC_MAX_KEY_BITS 256
#define LOWMC_MAX_AND_GATES (3 * 38 * 10 + 4) /* Rounded to nearest byte */
#define MAX_AUX_BYTES ((LOWMC_MAX_AND_GATES + LOWMC_MAX_KEY_BITS) / 8 + 1)

/* Helper functions */

ATTR_CONST
static uint32_t numBytes(uint32_t numBits) {
  return (numBits + 7) >> 3;
}

static void createRandomTapes(randomTape_t* tapes, uint8_t** seeds, uint8_t* salt, size_t t,
                              const picnic_instance_t* params) {
  hash_context_x4 ctx;

  size_t tapeSizeBytes = 2 * params->view_size + params->input_size;

  allocateRandomTape(tapes, params);
  assert(params->num_MPC_parties % 4 == 0);
  for (size_t i = 0; i < params->num_MPC_parties; i += 4) {
    hash_init_x4(&ctx, params);

    const uint8_t* seeds_ptr[4] = {seeds[i], seeds[i + 1], seeds[i + 2], seeds[i + 3]};
    hash_update_x4(&ctx, seeds_ptr, params->seed_size);
    const uint8_t* salt_ptr[4] = {salt, salt, salt, salt};
    hash_update_x4(&ctx, salt_ptr, SALT_SIZE);
    uint16_t tLE              = htole16((uint16_t)t);
    const uint8_t* tLE_ptr[4] = {(const uint8_t*)&tLE, (const uint8_t*)&tLE, (const uint8_t*)&tLE,
                                 (const uint8_t*)&tLE};
    hash_update_x4(&ctx, tLE_ptr, sizeof(uint16_t));
    uint16_t iLE0             = htole16((uint16_t)(i + 0));
    uint16_t iLE1             = htole16((uint16_t)(i + 1));
    uint16_t iLE2             = htole16((uint16_t)(i + 2));
    uint16_t iLE3             = htole16((uint16_t)(i + 3));
    const uint8_t* iLE_ptr[4] = {(const uint8_t*)&iLE0, (const uint8_t*)&iLE1,
                                 (const uint8_t*)&iLE2, (const uint8_t*)&iLE3};
    hash_update_x4(&ctx, iLE_ptr, sizeof(uint16_t));
    hash_final_x4(&ctx);

    uint8_t* out_ptr[4] = {tapes->tape[i], tapes->tape[i + 1], tapes->tape[i + 2],
                           tapes->tape[i + 3]};
    hash_squeeze_x4(&ctx, out_ptr, tapeSizeBytes);
  }
}

/* Read one bit from each tape and assemble them into a word.  The tapes form a
 * z by N matrix, we'll transpose it, then the first "count" N-bit rows forms
 * an output word.  In the current implementation N is 64 so the words are
 * uint64_t. The return value must be freed with freeShares().
 */
static void tapesToWords(shares_t* shares, randomTape_t* tapes) {
  for (size_t w = 0; w < shares->numWords; w++) {
    shares->shares[w] = tapesToWord(tapes);
  }
}

static void aux_mpc_AND_bitsliced(uint64_t mask_a, uint64_t mask_b, uint64_t mask_c, uint64_t* ab,
                                  uint64_t* bc, uint64_t* ca, randomTape_t* tapes) {

  for (int i = 0; i < 10; i++) {
    uint64_t fresh_output_maks_ab = tapesToParityOfWord(tapes, 0);
    uint64_t and_helper_ab        = tapesToParityOfWord(tapes, 1);
    uint64_t fresh_output_maks_bc = tapesToParityOfWord(tapes, 0);
    uint64_t and_helper_bc        = tapesToParityOfWord(tapes, 1);
    uint64_t fresh_output_maks_ca = tapesToParityOfWord(tapes, 0);
    uint64_t and_helper_ca        = tapesToParityOfWord(tapes, 1);

    uint64_t aux_bit_ab = (((mask_a & mask_b) >> (63 - 3 * i)) & 1) ^ and_helper_ab;
    uint64_t aux_bit_bc = (((mask_b & mask_c) >> (63 - 3 * i)) & 1) ^ and_helper_bc;
    uint64_t aux_bit_ca = (((mask_c & mask_a) >> (63 - 3 * i)) & 1) ^ and_helper_ca;

    setBit(tapes->tape[63], tapes->pos - 5, (uint8_t)aux_bit_ab);
    setBit(tapes->tape[63], tapes->pos - 3, (uint8_t)aux_bit_bc);
    setBit(tapes->tape[63], tapes->pos - 1, (uint8_t)aux_bit_ca);
    setBit(tapes->aux_bits, tapes->aux_pos++, (uint8_t)aux_bit_ab);
    setBit(tapes->aux_bits, tapes->aux_pos++, (uint8_t)aux_bit_bc);
    setBit(tapes->aux_bits, tapes->aux_pos++, (uint8_t)aux_bit_ca);

    *ab <<= 3;
    *ab |= fresh_output_maks_ab;
    *bc <<= 3;
    *bc |= fresh_output_maks_bc;
    *ca <<= 3;
    *ca |= fresh_output_maks_ca;
  }
  *ab <<= 36;
  *bc <<= 36;
  *ca <<= 36;
}

/**
 * S-box for m = 10, for Picnic2 aux computation, as bitsliced as possible
 */
void sbox_layer_10_uint64_aux(uint64_t* d, randomTape_t* tapes) {
  uint64_t in = *d;

  // a, b, c
  const uint64_t x0s = (in & MASK_X0I) << 2;
  const uint64_t x1s = (in & MASK_X1I) << 1;
  const uint64_t x2m = in & MASK_X2I;

  uint64_t ab = 0, bc = 0, ca = 0;
  aux_mpc_AND_bitsliced(x0s, x1s, x2m, &ab, &bc, &ca, tapes);

  // (b & c) ^ a
  const uint64_t t0 = (bc) ^ x0s;
  // (c & a) ^ a ^ b
  const uint64_t t1 = (ca) ^ x0s ^ x1s;
  // (a & b) ^ a ^ b ^c
  const uint64_t t2 = (ab) ^ x0s ^ x1s ^ x2m;

  *d = (in & MASK_MASK) ^ (t0 >> 2) ^ (t1 >> 1) ^ t2;
}

/* Input is the tapes for one parallel repitition; i.e., tapes[t]
 * Updates the random tapes of all players with the mask values for the output of
 * AND gates, and computes the N-th party's share such that the AND gate invariant
 * holds on the mask values.
 */
static void computeAuxTape(randomTape_t* tapes, const picnic_instance_t* params) {
  mzd_local_t* lowmc_key = mzd_local_init_ex(params->lowmc->n, 1, true);

  uint8_t temp[32] = {
      0,
  };

  // combine into key shares and calculate lowmc evaluation in plain
  for (size_t i = 0; i < params->num_MPC_parties; i++) {
    for (size_t j = 0; j < params->input_size; j++) {
      temp[j] ^= tapes->tape[i][j];
    }
  }
  mzd_from_char_array(lowmc_key, temp, params->lowmc->n / 8);
  tapes->pos = params->lowmc->n;

  lowmc_compute_aux_implementation_f lowmc_aux_impl = params->impls.lowmc_aux;
  // Perform LowMC evaluation and fix AND masks for all AND gates
  lowmc_aux_impl(lowmc_key, tapes);

  // Reset the random tape counter so that the online execution uses the
  // same random bits as when computing the aux shares
  tapes->pos = 0;

  mzd_local_free(lowmc_key);
}

static void commit(uint8_t* digest, const uint8_t* seed, const uint8_t* aux, const uint8_t* salt,
                   size_t t, size_t j, const picnic_instance_t* params) {
  /* Compute C[t][j];  as digest = H(seed||[aux]) aux is optional */
  hash_context ctx;

  hash_init(&ctx, params);
  hash_update(&ctx, seed, params->seed_size);
  if (aux != NULL) {
    size_t tapeLenBytes = params->view_size;
    hash_update(&ctx, aux, tapeLenBytes);
  }
  hash_update(&ctx, salt, SALT_SIZE);
  const uint16_t tLE = htole16((uint16_t)t);
  hash_update(&ctx, (const uint8_t*)&tLE, sizeof(uint16_t));
  const uint16_t jLE = htole16((uint16_t)j);
  hash_update(&ctx, (const uint8_t*)&jLE, sizeof(uint16_t));
  hash_final(&ctx);
  hash_squeeze(&ctx, digest, params->digest_size);
}

static void commit_x4(uint8_t** digest, const uint8_t** seed, const uint8_t* salt, size_t t,
                      size_t j, const picnic_instance_t* params) {
  /* Compute C[t][j];  as digest = H(seed||[aux]) aux is optional */
  hash_context_x4 ctx;

  hash_init_x4(&ctx, params);
  hash_update_x4(&ctx, seed, params->seed_size);
  const uint8_t* salt_ptr[4] = {salt, salt, salt, salt};
  hash_update_x4(&ctx, salt_ptr, SALT_SIZE);
  const uint16_t tLE        = htole16((uint16_t)t);
  const uint8_t* tLE_ptr[4] = {(const uint8_t*)&tLE, (const uint8_t*)&tLE, (const uint8_t*)&tLE,
                               (const uint8_t*)&tLE};
  hash_update_x4(&ctx, tLE_ptr, sizeof(uint16_t));
  const uint16_t jLE0       = htole16((uint16_t)(j + 0));
  const uint16_t jLE1       = htole16((uint16_t)(j + 1));
  const uint16_t jLE2       = htole16((uint16_t)(j + 2));
  const uint16_t jLE3       = htole16((uint16_t)(j + 3));
  const uint8_t* jLE_ptr[4] = {(const uint8_t*)&jLE0, (const uint8_t*)&jLE1, (const uint8_t*)&jLE2,
                               (const uint8_t*)&jLE3};
  hash_update_x4(&ctx, jLE_ptr, sizeof(uint16_t));
  hash_final_x4(&ctx);
  hash_squeeze_x4(&ctx, digest, params->digest_size);
}

static void commit_h(uint8_t* digest, const commitments_t* C, const picnic_instance_t* params) {
  hash_context ctx;

  hash_init(&ctx, params);
  for (size_t i = 0; i < params->num_MPC_parties; i++) {
    hash_update(&ctx, C->hashes[i], params->digest_size);
  }
  hash_final(&ctx);
  hash_squeeze(&ctx, digest, params->digest_size);
}

static void commit_h_x4(uint8_t** digest, const commitments_t* C, const picnic_instance_t* params) {
  hash_context_x4 ctx;

  hash_init_x4(&ctx, params);
  for (size_t i = 0; i < params->num_MPC_parties; i++) {
    const uint8_t* data[4] = {
        C[0].hashes[i],
        C[1].hashes[i],
        C[2].hashes[i],
        C[3].hashes[i],
    };
    hash_update_x4(&ctx, data, params->digest_size);
  }
  hash_final_x4(&ctx);
  hash_squeeze_x4(&ctx, digest, params->digest_size);
}

// Commit to the views for one parallel rep
static void commit_v(uint8_t* digest, const uint8_t* input, const msgs_t* msgs,
                     const picnic_instance_t* params) {
  hash_context ctx;

  hash_init(&ctx, params);
  hash_update(&ctx, input, params->input_size);
  for (size_t i = 0; i < params->num_MPC_parties; i++) {
    hash_update(&ctx, msgs->msgs[i], numBytes(msgs->pos));
  }
  hash_final(&ctx);
  hash_squeeze(&ctx, digest, params->digest_size);
}

static void commit_v_x4(uint8_t** digest, const uint8_t** input, const msgs_t* msgs,
                        const picnic_instance_t* params) {
  hash_context_x4 ctx;

  hash_init_x4(&ctx, params);
  hash_update_x4(&ctx, input, params->input_size);
  for (size_t i = 0; i < params->num_MPC_parties; i++) {
    assert(msgs[0].pos == msgs[1].pos && msgs[2].pos == msgs[3].pos && msgs[0].pos == msgs[2].pos);
    const uint8_t* data[4] = {
        msgs[0].msgs[i],
        msgs[1].msgs[i],
        msgs[2].msgs[i],
        msgs[3].msgs[i],
    };
    hash_update_x4(&ctx, data, numBytes(msgs->pos));
  }
  hash_final_x4(&ctx);
  hash_squeeze_x4(&ctx, digest, params->digest_size);
}

static int contains(const uint16_t* list, size_t len, uint16_t value) {
  for (size_t i = 0; i < len; i++) {
    if (list[i] == value) {
      return 1;
    }
  }
  return 0;
}

static int indexOf(const uint16_t* list, size_t len, uint16_t value) {
  for (size_t i = 0; i < len; i++) {
    if (list[i] == value) {
      return i;
    }
  }
  assert(!"indexOf called on list where value is not found. (caller bug)");
  return -1;
}

static void setAuxBits(randomTape_t* tapes, uint8_t* input, const picnic_instance_t* params) {
  size_t firstAuxIndex = params->lowmc->n + 1;
  size_t last          = params->num_MPC_parties - 1;
  size_t pos           = 0;

  for (size_t i = 0; i < params->view_size * 2 * 8; i += 2) {
    uint8_t auxBit = getBit(input, pos);
    setBit(tapes->tape[last], firstAuxIndex + i, auxBit);
    pos++;
  }
}

static size_t bitsToChunks(size_t chunkLenBits, const uint8_t* input, size_t inputLen,
                           uint16_t* chunks) {
  if (chunkLenBits > inputLen * 8) {
    assert(!"Invalid input to bitsToChunks: not enough input");
    return 0;
  }
  size_t chunkCount = ((inputLen * 8) / chunkLenBits);

  for (size_t i = 0; i < chunkCount; i++) {
    chunks[i] = 0;
    for (size_t j = 0; j < chunkLenBits; j++) {
      chunks[i] += getBit(input, i * chunkLenBits + j) << j;
      assert(chunks[i] < (1 << chunkLenBits));
    }
    chunks[i] = le16toh(chunks[i]);
  }

  return chunkCount;
}

static size_t appendUnique(uint16_t* list, uint16_t value, size_t position) {
  if (position == 0) {
    list[position] = value;
    return position + 1;
  }

  for (size_t i = 0; i < position; i++) {
    if (list[i] == value) {
      return position;
    }
  }
  list[position] = value;
  return position + 1;
}

static void HCP(uint16_t* challengeC, uint16_t* challengeP, commitments_t* Ch, uint8_t* hCv,
                uint8_t* salt, const uint32_t* pubKey, const uint32_t* plaintext,
                const uint8_t* message, size_t messageByteLength, const picnic_instance_t* params) {
  hash_context ctx;
  uint8_t h[MAX_DIGEST_SIZE] = {0};

  assert(params->num_opened_rounds < params->num_rounds);

  hash_init(&ctx, params);
  for (size_t t = 0; t < params->num_rounds; t++) {
    hash_update(&ctx, Ch->hashes[t], params->digest_size);
  }

  hash_update(&ctx, hCv, params->digest_size);
  hash_update(&ctx, salt, SALT_SIZE);
  hash_update(&ctx, (const uint8_t*)pubKey, params->input_size);
  hash_update(&ctx, (const uint8_t*)plaintext, params->input_size);
  hash_update(&ctx, message, messageByteLength);
  hash_final(&ctx);
  hash_squeeze(&ctx, h, params->digest_size);

  // Populate C
  uint32_t bitsPerChunkC = ceil_log2(params->num_rounds);
  uint32_t bitsPerChunkP = ceil_log2(params->num_MPC_parties);
  uint16_t* chunks       = calloc(params->digest_size * 8 / MIN(bitsPerChunkP,bitsPerChunkC), sizeof(uint16_t));

  size_t countC = 0;
  while (countC < params->num_opened_rounds) {
    size_t numChunks = bitsToChunks(bitsPerChunkC, h, params->digest_size, chunks);
    for (size_t i = 0; i < numChunks; i++) {
      if (chunks[i] < params->num_rounds) {
        countC = appendUnique(challengeC, chunks[i], countC);
      }
      if (countC == params->num_opened_rounds) {
        break;
      }
    }

    hash_init_prefix(&ctx, params, HASH_PREFIX_1);
    hash_update(&ctx, h, params->digest_size);
    hash_final(&ctx);
    hash_squeeze(&ctx, h, params->digest_size);
  }

  // Note that we always compute h = H(h) after setting C
  size_t countP = 0;

  while (countP < params->num_opened_rounds) {
    size_t numChunks = bitsToChunks(bitsPerChunkP, h, params->digest_size, chunks);
    for (size_t i = 0; i < numChunks; i++) {
      if (chunks[i] < params->num_MPC_parties) {
        challengeP[countP] = chunks[i];
        countP++;
      }
      if (countP == params->num_opened_rounds) {
        break;
      }
    }

    hash_init_prefix(&ctx, params, HASH_PREFIX_1);
    hash_update(&ctx, h, params->digest_size);
    hash_final(&ctx);
    hash_squeeze(&ctx, h, params->digest_size);
  }

  free(chunks);
}

static uint16_t* getMissingLeavesList(uint16_t* challengeC, const picnic_instance_t* params) {
  size_t missingLeavesSize = params->num_rounds - params->num_opened_rounds;
  uint16_t* missingLeaves  = calloc(missingLeavesSize, sizeof(uint16_t));
  size_t pos               = 0;

  for (size_t i = 0; i < params->num_rounds; i++) {
    if (!contains(challengeC, params->num_opened_rounds, i)) {
      missingLeaves[pos] = i;
      pos++;
    }
  }

  return missingLeaves;
}

int verify_picnic2(signature2_t* sig, const uint32_t* pubKey, const uint32_t* plaintext,
                   const uint8_t* message, size_t messageByteLength,
                   const picnic_instance_t* params) {
  commitments_t C[4]        = {0,};
  allocateCommitments2(&C[0], params, params->num_MPC_parties);
  allocateCommitments2(&C[1], params, params->num_MPC_parties);
  allocateCommitments2(&C[2], params, params->num_MPC_parties);
  allocateCommitments2(&C[3], params, params->num_MPC_parties);
  msgs_t* msgs              = allocateMsgsVerify(params);
  tree_t* treeCv            = createTree(params->num_rounds, params->digest_size);
  size_t challengeSizeBytes = params->num_opened_rounds * sizeof(uint16_t);
  uint16_t* challengeC      = malloc(challengeSizeBytes);
  uint16_t* challengeP      = malloc(challengeSizeBytes);
  tree_t** seeds            = calloc(params->num_rounds, sizeof(tree_t*));
  randomTape_t* tapes       = malloc(params->num_rounds * sizeof(randomTape_t));
  tree_t* iSeedsTree        = createTree(params->num_rounds, params->seed_size);
  int ret = reconstructSeeds(iSeedsTree, sig->challengeC, params->num_opened_rounds, sig->iSeedInfo,
                             sig->iSeedInfoLen, sig->salt, 0, params);
  const size_t last = params->num_MPC_parties - 1;
  lowmc_simulate_online_f simulateOnline = params->impls.lowmc_simulate_online;

  commitments_t Ch          = {0};
  allocateCommitments2(&Ch, params, params->num_rounds);
  commitments_t Cv          = {0};
  allocateCommitments2(&Cv, params, params->num_rounds);
  shares_t* mask_shares = allocateShares(params->lowmc->n);
  mzd_local_t* m_plaintext = mzd_local_init_ex(1, params->lowmc->n, false);
  mzd_local_t* m_maskedKey = mzd_local_init_ex(1, params->lowmc->k, false);
  mzd_from_char_array(m_plaintext, (const uint8_t*)plaintext, params->output_size);

  if (ret != 0) {
    ret = -1;
    goto Exit;
  }

  /* Populate seeds with values from the signature */
  for (size_t t = 0; t < params->num_rounds; t++) {
    if (!contains(sig->challengeC, params->num_opened_rounds, t)) {
      /* Expand iSeed[t] to seeds for each parties, using a seed tree */
      seeds[t] =
          generateSeeds(params->num_MPC_parties, getLeaf(iSeedsTree, t), sig->salt, t, params);
    } else {
      /* We don't have the initial seed for the round, but instead a seed
       * for each unopened party */
      seeds[t]       = createTree(params->num_MPC_parties, params->seed_size);
      size_t P_index = indexOf(sig->challengeC, params->num_opened_rounds, t);
      uint16_t hideList[1];
      hideList[0] = sig->challengeP[P_index];
      ret         = reconstructSeeds(seeds[t], hideList, 1, sig->proofs[t].seedInfo,
                             sig->proofs[t].seedInfoLen, sig->salt, t, params);
      if (ret != 0) {
#if !defined(NDEBUG)
        printf("Failed to reconstruct seeds for round %lu\n", t);
#endif
        ret = -1;
        goto Exit;
      }
    }
    /* Commit */

    /* Compute random tapes for all parties.  One party for each repitition
     * challengeC will have a bogus seed; but we won't use that party's
     * random tape. */
    createRandomTapes(&tapes[t], getLeaves(seeds[t]), sig->salt, t, params);


    if (!contains(sig->challengeC, params->num_opened_rounds, t)) {
      /* We're given iSeed, have expanded the seeds, compute aux from scratch so we can comnpte
       * Com[t] */
      computeAuxTape(&tapes[t], params);
      for (size_t j = 0; j < params->num_MPC_parties; j += 4) {
        const uint8_t* seed_ptr[4] = {getLeaf(seeds[t], j + 0), getLeaf(seeds[t], j + 1),
                                      getLeaf(seeds[t], j + 2), getLeaf(seeds[t], j + 3)};
        commit_x4(C[t%4].hashes + j, seed_ptr, sig->salt, t, j, params);
      }
      commit(C[t%4].hashes[last], getLeaf(seeds[t], last), tapes[t].aux_bits, sig->salt, t, last,
             params);
      /* after we have checked the tape, we do not need it anymore for this opened iteration */
      freeRandomTape(&tapes[t]);
    } else {
      /* We're given all seeds and aux bits, execpt for the unopened
       * party, we get their commitment */
      size_t unopened = sig->challengeP[indexOf(sig->challengeC, params->num_opened_rounds, t)];
      for (size_t j = 0; j < params->num_MPC_parties; j += 4) {
        const uint8_t* seed_ptr[4] = {getLeaf(seeds[t], j + 0), getLeaf(seeds[t], j + 1),
                                      getLeaf(seeds[t], j + 2), getLeaf(seeds[t], j + 3)};
        commit_x4(C[t%4].hashes + j, seed_ptr, sig->salt, t, j, params);
      }
      if (last != unopened) {
        commit(C[t%4].hashes[last], getLeaf(seeds[t], last), sig->proofs[t].aux, sig->salt, t, last,
               params);
      }

      memcpy(C[t%4].hashes[unopened], sig->proofs[t].C, params->digest_size);
    }
    /* hash commitments every four iterations if possible, for the last few do single commitments */
    if(t >= params->num_rounds / 4 * 4) {
      commit_h(Ch.hashes[t], &C[t%4], params);
    } else if ((t + 1) % 4 == 0) {
      size_t t4 = t / 4 * 4;
      commit_h_x4(&Ch.hashes[t4], &C[0], params);
    }
    freeTree(seeds[t]);

    /* Commit to the views */
    if (contains(sig->challengeC, params->num_opened_rounds, t)) {
      /* 2. When t is in C, we have everything we need to re-compute the view, as an honest signer
       * would.
       * We simulate the MPC with one fewer party; the unopned party's values are all set to zero.
       */
      size_t unopened = sig->challengeP[indexOf(sig->challengeC, params->num_opened_rounds, t)];
      size_t tapeLengthBytes = 2 * params->view_size + params->input_size;
      setAuxBits(&tapes[t], sig->proofs[t].aux, params);
      memset(tapes[t].tape[unopened], 0, tapeLengthBytes);
      memcpy(msgs->msgs[unopened], sig->proofs[t].msgs, params->view_size + params->input_size);
      msgs->pos      = 0;
      msgs->unopened = unopened;

      tapesToWords(mask_shares, &tapes[t]);
      mzd_from_char_array(m_maskedKey, sig->proofs[t].input, params->input_size);
      ret = simulateOnline(m_maskedKey, mask_shares, &tapes[t], msgs,
                           m_plaintext, pubKey, params);

      freeRandomTape(&tapes[t]);
      if (ret != 0) {
#if !defined(NDEBUG)
        printf("MPC simulation failed for round %lu, signature invalid\n", t);
#endif
        ret = -1;
        goto Exit;
      }
      commit_v(Cv.hashes[t], sig->proofs[t].input, msgs, params);
    } else {
      Cv.hashes[t] = NULL;
    }
  }

  size_t missingLeavesSize = params->num_rounds - params->num_opened_rounds;
  uint16_t* missingLeaves  = getMissingLeavesList(sig->challengeC, params);
  ret = addMerkleNodes(treeCv, missingLeaves, missingLeavesSize, sig->cvInfo, sig->cvInfoLen);
  free(missingLeaves);
  if (ret != 0) {
    ret = -1;
    goto Exit;
  }

  ret = verifyMerkleTree(treeCv, Cv.hashes, sig->salt, params);
  if (ret != 0) {
    ret = -1;
    goto Exit;
  }

  /* Compute the challenge; two lists of integers */
  HCP(challengeC, challengeP, &Ch, treeCv->nodes[0], sig->salt, pubKey, plaintext, message,
      messageByteLength, params);

  /* Compare to challenge from signature */
  if (memcmp(sig->challengeC, challengeC, challengeSizeBytes) != 0 ||
      memcmp(sig->challengeP, challengeP, challengeSizeBytes) != 0) {
#if !defined(NDEBUG)
    printf("Challenge does not match, signature invalid\n");
#endif
    ret = -1;
    goto Exit;
  }

  ret = EXIT_SUCCESS;

Exit:

  mzd_local_free(m_plaintext);
  mzd_local_free(m_maskedKey);
  freeShares(mask_shares);
  free(challengeC);
  free(challengeP);
  freeCommitments2(&C[0]);
  freeCommitments2(&C[1]);
  freeCommitments2(&C[2]);
  freeCommitments2(&C[3]);
  freeCommitments2(&Cv);
  freeCommitments2(&Ch);
  freeMsgs(msgs);
  freeTree(treeCv);
  freeTree(iSeedsTree);
  free(seeds);
  free(tapes);

  return ret;
}

static void computeSaltAndRootSeed(uint8_t* saltAndRoot, size_t saltAndRootLength,
                                   uint32_t* privateKey, uint32_t* pubKey, uint32_t* plaintext,
                                   const uint8_t* message, size_t messageByteLength,
                                   const picnic_instance_t* params) {
  hash_context ctx;

  hash_init(&ctx, params);
  hash_update(&ctx, (const uint8_t*)privateKey, params->input_size);
  hash_update(&ctx, message, messageByteLength);
  hash_update(&ctx, (const uint8_t*)pubKey, params->input_size);
  hash_update(&ctx, (const uint8_t*)plaintext, params->input_size);
  const uint16_t stateSizeLE = htole16((uint16_t)params->lowmc->n);
  hash_update(&ctx, (const uint8_t*)&stateSizeLE, sizeof(uint16_t));
  hash_final(&ctx);
  hash_squeeze(&ctx, saltAndRoot, saltAndRootLength);
}

int sign_picnic2(uint32_t* privateKey, uint32_t* pubKey, uint32_t* plaintext,
                 const uint8_t* message, size_t messageByteLength, signature2_t* sig,
                 const picnic_instance_t* params) {
  int ret              = 0;
  uint8_t* saltAndRoot = malloc(params->seed_size + SALT_SIZE);

  computeSaltAndRootSeed(saltAndRoot, params->seed_size + SALT_SIZE, privateKey, pubKey, plaintext,
                         message, messageByteLength, params);
  memcpy(sig->salt, saltAndRoot, SALT_SIZE);
  tree_t* iSeedsTree =
      generateSeeds(params->num_rounds, saltAndRoot + SALT_SIZE, sig->salt, 0, params);
  uint8_t** iSeeds = getLeaves(iSeedsTree);
  free(saltAndRoot);

  randomTape_t* tapes = malloc(params->num_rounds * sizeof(randomTape_t));
  tree_t** seeds      = malloc(params->num_rounds * sizeof(tree_t*));
  commitments_t C[4];
  allocateCommitments2(&C[0], params, params->num_MPC_parties);
  allocateCommitments2(&C[1], params, params->num_MPC_parties);
  allocateCommitments2(&C[2], params, params->num_MPC_parties);
  allocateCommitments2(&C[3], params, params->num_MPC_parties);

  lowmc_simulate_online_f simulateOnline = params->impls.lowmc_simulate_online;
  inputs_t inputs                        = allocateInputs(params);
  msgs_t* msgs                           = allocateMsgs(params);
  shares_t* mask_shares                  = allocateShares(params->lowmc->n);

  /* Commitments to the commitments and views */
  commitments_t Ch;
  allocateCommitments2(&Ch, params, params->num_rounds);
  commitments_t Cv;
  allocateCommitments2(&Cv, params, params->num_rounds);

  mzd_local_t* m_plaintext = mzd_local_init_ex(1, params->lowmc->n, false);
  mzd_local_t* m_maskedKey = mzd_local_init_ex(1, params->lowmc->k, false);

  mzd_from_char_array(m_plaintext, (const uint8_t*)plaintext, params->output_size);


  for (size_t t = 0; t < params->num_rounds; t++) {
    seeds[t] = generateSeeds(params->num_MPC_parties, iSeeds[t], sig->salt, t, params);
    createRandomTapes(&tapes[t], getLeaves(seeds[t]), sig->salt, t, params);
    /* Preprocessing; compute aux tape for the N-th player, for each parallel rep */
    computeAuxTape(&tapes[t], params);
    /* Commit to seeds and aux bits */
    assert(params->num_MPC_parties % 4 == 0);
    for (size_t j = 0; j < params->num_MPC_parties; j += 4) {
      const uint8_t* seed_ptr[4] = {getLeaf(seeds[t], j + 0), getLeaf(seeds[t], j + 1),
                                    getLeaf(seeds[t], j + 2), getLeaf(seeds[t], j + 3)};
      commit_x4(C[t%4].hashes + j, seed_ptr, sig->salt, t, j, params);
    }
    const size_t last = params->num_MPC_parties - 1;
    commit(C[t%4].hashes[last], getLeaf(seeds[t], last), tapes[t].aux_bits, sig->salt, t, last,
           params);

    /* Simulate the online phase of the MPC */
    uint32_t* maskedKey = (uint32_t*)inputs[t];

    tapesToWords(mask_shares, &tapes[t]);
    reconstructShares(maskedKey, mask_shares); // maskedKey = masks
    xor_word_array(maskedKey, maskedKey, privateKey,
                   (params->input_size / 4)); // maskedKey += privateKey
    mzd_from_char_array(m_maskedKey, (const uint8_t*)maskedKey, params->input_size);

    int rv = simulateOnline(m_maskedKey, mask_shares, &tapes[t], &msgs[t], m_plaintext, pubKey, params);
    if (rv != 0) {
#if !defined(NDEBUG)
      printf("MPC simulation failed, aborting signature\n");
#endif
      ret = -1;
    }
    /* free the expanded random tape and associated buffers to reduce memory usage, 
	   however, we are keeping the calculated aux bits for later (hence partial) */
    partialFreeRandomTape(&tapes[t]);
    /* hash commitments every four iterations if possible, for the last few do single commitments */
    if(t >= params->num_rounds / 4 * 4) {
      commit_h(Ch.hashes[t], &C[t%4], params);
      commit_v(Cv.hashes[t], inputs[t], &msgs[t], params);
    } else if ((t + 1) % 4 == 0) {
      size_t t4 = t / 4 * 4;
      commit_h_x4(&Ch.hashes[t4], &C[0], params);
      commit_v_x4(&Cv.hashes[t4], (const uint8_t**)&inputs[t4], &msgs[t4], params);
    }

  }
  freeShares(mask_shares);
  mzd_local_free(m_maskedKey);
  mzd_local_free(m_plaintext);

  /* Create a Merkle tree with Cv as the leaves */
  tree_t* treeCv = createTree(params->num_rounds, params->digest_size);
  buildMerkleTree(treeCv, Cv.hashes, sig->salt, params);

  /* Compute the challenge; two lists of integers */
  uint16_t* challengeC = sig->challengeC;
  uint16_t* challengeP = sig->challengeP;
  HCP(challengeC, challengeP, &Ch, treeCv->nodes[0], sig->salt, pubKey, plaintext, message,
      messageByteLength, params);

  /* Send information required for checking commitments with Merkle tree.
   * The commitments the verifier will be missing are those not in challengeC. */
  size_t missingLeavesSize = params->num_rounds - params->num_opened_rounds;
  uint16_t* missingLeaves  = getMissingLeavesList(challengeC, params);
  size_t cvInfoLen         = 0;
  uint8_t* cvInfo          = openMerkleTree(treeCv, missingLeaves, missingLeavesSize, &cvInfoLen);
  sig->cvInfo              = cvInfo;
  sig->cvInfoLen           = cvInfoLen;
  free(missingLeaves);

  /* Reveal iSeeds for unopned rounds, those in {0..T-1} \ ChallengeC. */
  sig->iSeedInfo    = malloc(params->num_rounds * params->seed_size);
  sig->iSeedInfoLen = revealSeeds(iSeedsTree, challengeC, params->num_opened_rounds, sig->iSeedInfo,
                                  params->num_rounds * params->seed_size, params);
  sig->iSeedInfo    = realloc(sig->iSeedInfo, sig->iSeedInfoLen);

  /* Assemble the proof */
  proof2_t* proofs = sig->proofs;
  for (size_t t = 0; t < params->num_rounds; t++) {
    if (contains(challengeC, params->num_opened_rounds, t)) {
      allocateProof2(&proofs[t], params);
      size_t P_index          = indexOf(challengeC, params->num_opened_rounds, t);
      proofs[t].unOpenedIndex = challengeP[P_index];

      uint16_t hideList[1];
      hideList[0]           = challengeP[P_index];
      proofs[t].seedInfo    = malloc(params->num_MPC_parties * params->seed_size);
      proofs[t].seedInfoLen = revealSeeds(seeds[t], hideList, 1, proofs[t].seedInfo,
                                          params->num_MPC_parties * params->seed_size, params);
      proofs[t].seedInfo    = realloc(proofs[t].seedInfo, proofs[t].seedInfoLen);

      size_t last = params->num_MPC_parties - 1;
      if (challengeP[P_index] != last) {
        memcpy(proofs[t].aux, tapes[t].aux_bits, params->view_size);
      }

      memcpy(proofs[t].input, inputs[t], params->input_size);
      memcpy(proofs[t].msgs, msgs[t].msgs[challengeP[P_index]],
             params->view_size + params->input_size);

      /* recompute commitment of unopened party since we did not store it for memory optimization */
      if (proofs[t].unOpenedIndex == params->num_MPC_parties - 1) {
        commit(proofs[t].C, getLeaf(seeds[t], proofs[t].unOpenedIndex),
               tapes[t].aux_bits, sig->salt, t, proofs[t].unOpenedIndex, params);
      } else {
        commit(proofs[t].C, getLeaf(seeds[t], proofs[t].unOpenedIndex),
               NULL, sig->salt, t, proofs[t].unOpenedIndex, params);
	  }
    }
  }

  sig->proofs = proofs;

  for (size_t t = 0; t < params->num_rounds; t++) {
    finalFreeRandomTape(&tapes[t]);
    freeTree(seeds[t]);
  }
  free(tapes);
  free(seeds);
  freeTree(iSeedsTree);
  freeTree(treeCv);

  freeCommitments2(&Ch);
  freeCommitments2(&Cv);
  freeCommitments2(&C[0]);
  freeCommitments2(&C[1]);
  freeCommitments2(&C[2]);
  freeCommitments2(&C[3]);
  freeInputs(inputs);
  freeMsgs(msgs);

  return ret;
}

static int inRange(uint16_t* list, size_t len, size_t low, size_t high) {
  for (size_t i = 0; i < len; i++) {
    if (list[i] > high || list[i] < low) {
      return 0;
    }
  }
  return 1;
}

static int unique(uint16_t* list, size_t len) {
  for (size_t i = 0; i < len; i++) {
    for (size_t j = 0; j < len; j++) {
      if (j != i && list[i] == list[j]) {
        return 0;
      }
    }
  }
  return 1;
}

static int arePaddingBitsZero(uint8_t* data, size_t byteLength, size_t bitLength) {
  for (size_t i = bitLength; i < byteLength * 8; i++) {
    uint8_t bit_i = getBit(data, i);
    if (bit_i != 0) {
      return 0;
    }
  }
  return 1;
}

static void deserialize_u16(uint16_t* dst, const uint8_t* src, size_t size) {
#if defined(PICNIC_IS_LITTLE_ENDIAN)
  memcpy(dst, src, sizeof(uint16_t) * size);
#else
  for (size_t s = size; s; --s, ++dst, src += 2) {
    uint16_t t;
    memcpy(&t, src, sizeof(t));
    *dst = le16toh(t);
  }
#endif
}

static int deserializeSignature2(signature2_t* sig, const uint8_t* sigBytes, size_t sigBytesLen,
                                 const picnic_instance_t* params) {
  /* Read the challenge and salt */
  size_t bytesRequired = 4 * params->num_opened_rounds + SALT_SIZE;

  if (sigBytesLen < bytesRequired) {
    return EXIT_FAILURE;
  }

  deserialize_u16(sig->challengeC, sigBytes, params->num_opened_rounds);
  sigBytes += 2 * params->num_opened_rounds;
  deserialize_u16(sig->challengeP, sigBytes, params->num_opened_rounds);
  sigBytes += 2 * params->num_opened_rounds;
  memcpy(sig->salt, sigBytes, SALT_SIZE);
  sigBytes += SALT_SIZE;

  if (!inRange(sig->challengeC, params->num_opened_rounds, 0, params->num_rounds - 1)) {
    return EXIT_FAILURE;
  }
  if (!unique(sig->challengeC, params->num_opened_rounds)) {
    return EXIT_FAILURE;
  }
  if (!inRange(sig->challengeP, params->num_opened_rounds, 0, params->num_MPC_parties - 1)) {
    return EXIT_FAILURE;
  }

  /* Add size of iSeeds tree data */
  sig->iSeedInfoLen =
      revealSeedsSize(params->num_rounds, sig->challengeC, params->num_opened_rounds, params);
  bytesRequired += sig->iSeedInfoLen;

  /* Add the size of the Cv Merkle tree data */
  size_t missingLeavesSize = params->num_rounds - params->num_opened_rounds;
  uint16_t* missingLeaves  = getMissingLeavesList(sig->challengeC, params);
  sig->cvInfoLen = openMerkleTreeSize(params->num_rounds, missingLeaves, missingLeavesSize, params);
  bytesRequired += sig->cvInfoLen;
  free(missingLeaves);

  /* Compute the number of bytes required for the proofs */
  uint16_t hideList[1] = {0};
  size_t seedInfoLen   = revealSeedsSize(params->num_MPC_parties, hideList, 1, params);
  for (size_t t = 0; t < params->num_rounds; t++) {
    if (contains(sig->challengeC, params->num_opened_rounds, t)) {
      size_t P_t = sig->challengeP[indexOf(sig->challengeC, params->num_opened_rounds, t)];
      if (P_t != (params->num_MPC_parties - 1)) {
        bytesRequired += params->view_size;
      }
      bytesRequired += params->digest_size;
      bytesRequired += params->input_size;
      bytesRequired += params->input_size + params->view_size;
      bytesRequired += seedInfoLen;
    }
  }

  /* Fail if the signature does not have the exact number of bytes we expect */
  if (sigBytesLen != bytesRequired) {
#if !defined(NDEBUG)
    printf("%s: sigBytesLen = %lu, expected bytesRequired = %lu\n", __func__, sigBytesLen,
           bytesRequired);
#endif
    return EXIT_FAILURE;
  }

  sig->iSeedInfo = malloc(sig->iSeedInfoLen);
  memcpy(sig->iSeedInfo, sigBytes, sig->iSeedInfoLen);
  sigBytes += sig->iSeedInfoLen;

  sig->cvInfo = malloc(sig->cvInfoLen);
  memcpy(sig->cvInfo, sigBytes, sig->cvInfoLen);
  sigBytes += sig->cvInfoLen;

  /* Read the proofs */
  for (size_t t = 0; t < params->num_rounds; t++) {
    if (contains(sig->challengeC, params->num_opened_rounds, t)) {
      allocateProof2(&sig->proofs[t], params);
      sig->proofs[t].seedInfoLen = seedInfoLen;
      sig->proofs[t].seedInfo    = malloc(sig->proofs[t].seedInfoLen);
      memcpy(sig->proofs[t].seedInfo, sigBytes, sig->proofs[t].seedInfoLen);
      sigBytes += sig->proofs[t].seedInfoLen;

      size_t P_t = sig->challengeP[indexOf(sig->challengeC, params->num_opened_rounds, t)];
      if (P_t != (params->num_MPC_parties - 1)) {
        memcpy(sig->proofs[t].aux, sigBytes, params->view_size);
        sigBytes += params->view_size;
        if (!arePaddingBitsZero(sig->proofs[t].aux, params->view_size,
                                3 * params->lowmc->r * params->lowmc->m)) {
#if !defined(NDEBUG)
          printf("%s: failed while deserializing aux bits\n", __func__);
#endif
          return -1;
        }
      }

      memcpy(sig->proofs[t].input, sigBytes, params->seed_size);
      sigBytes += params->input_size;

      size_t msgsByteLength = params->input_size + params->view_size;
      memcpy(sig->proofs[t].msgs, sigBytes, msgsByteLength);
      sigBytes += msgsByteLength;
      size_t msgsBitLength = params->lowmc->n + 3 * params->lowmc->r * params->lowmc->m;
      if (!arePaddingBitsZero(sig->proofs[t].msgs, msgsByteLength, msgsBitLength)) {
#if !defined(NDEBUG)
        printf("%s: failed while deserializing msgs bits\n", __func__);
#endif
        return -1;
      }

      memcpy(sig->proofs[t].C, sigBytes, params->digest_size);
      sigBytes += params->digest_size;
    }
  }

  return EXIT_SUCCESS;
}

static void serialize_u16(uint8_t* dst, const uint16_t* src, size_t size) {
#if defined(PICNIC_IS_LITTLE_ENDIAN)
  memcpy(dst, src, sizeof(uint16_t) * size);
#else
  for (size_t s = size; s; --s, ++src, dst += 2) {
    const uint16_t t = htole16(*src);
    memcpy(dst, &t, sizeof(t));
  }
#endif
}

static int serializeSignature2(const signature2_t* sig, uint8_t* sigBytes, size_t sigBytesLen,
                               const picnic_instance_t* params) {
  uint8_t* sigBytesBase = sigBytes;

  /* Compute the number of bytes required for the signature */
  size_t bytesRequired = 4 * params->num_opened_rounds + SALT_SIZE; /* challenge and salt */

  bytesRequired +=
      sig->iSeedInfoLen; /* Encode only iSeedInfo, the length will be recomputed by deserialize */
  bytesRequired += sig->cvInfoLen;

  for (size_t t = 0; t < params->num_rounds; t++) { /* proofs */
    if (contains(sig->challengeC, params->num_opened_rounds, t)) {
      size_t P_t = sig->challengeP[indexOf(sig->challengeC, params->num_opened_rounds, t)];
      bytesRequired += sig->proofs[t].seedInfoLen;
      if (P_t != (params->num_MPC_parties - 1)) {
        bytesRequired += params->view_size;
      }
      bytesRequired += params->digest_size;
      bytesRequired += params->input_size;
      bytesRequired += params->input_size + params->view_size;
    }
  }

  if (sigBytesLen < bytesRequired) {
    return -1;
  }

  serialize_u16(sigBytes, sig->challengeC, params->num_opened_rounds);
  sigBytes += 2 * params->num_opened_rounds;
  serialize_u16(sigBytes, sig->challengeP, params->num_opened_rounds);
  sigBytes += 2 * params->num_opened_rounds;

  memcpy(sigBytes, sig->salt, SALT_SIZE);
  sigBytes += SALT_SIZE;

  memcpy(sigBytes, sig->iSeedInfo, sig->iSeedInfoLen);
  sigBytes += sig->iSeedInfoLen;
  memcpy(sigBytes, sig->cvInfo, sig->cvInfoLen);
  sigBytes += sig->cvInfoLen;

  /* Write the proofs */
  for (size_t t = 0; t < params->num_rounds; t++) {
    if (contains(sig->challengeC, params->num_opened_rounds, t)) {
      memcpy(sigBytes, sig->proofs[t].seedInfo, sig->proofs[t].seedInfoLen);
      sigBytes += sig->proofs[t].seedInfoLen;

      size_t P_t = sig->challengeP[indexOf(sig->challengeC, params->num_opened_rounds, t)];

      if (P_t != (params->num_MPC_parties - 1)) {
        memcpy(sigBytes, sig->proofs[t].aux, params->view_size);
        sigBytes += params->view_size;
      }

      memcpy(sigBytes, sig->proofs[t].input, params->input_size);
      sigBytes += params->input_size;

      memcpy(sigBytes, sig->proofs[t].msgs, params->input_size + params->view_size);
      sigBytes += params->input_size + params->view_size;

      memcpy(sigBytes, sig->proofs[t].C, params->digest_size);
      sigBytes += params->digest_size;
    }
  }

  return (int)(sigBytes - sigBytesBase);
}

int impl_sign_picnic2(const picnic_instance_t* instance, const uint8_t* plaintext,
                      const uint8_t* private_key, const uint8_t* public_key, const uint8_t* msg,
                      size_t msglen, uint8_t* signature, size_t* signature_len) {
  int ret;
  signature2_t* sig = (signature2_t*)malloc(sizeof(signature2_t));
  allocateSignature2(sig, instance);
  if (sig == NULL) {
    return -1;
  }
  ret = sign_picnic2((uint32_t*)private_key, (uint32_t*)public_key, (uint32_t*)plaintext, msg,
                     msglen, sig, instance);
  if (ret != EXIT_SUCCESS) {
#if !defined(NDEBUG)
    fprintf(stderr, "Failed to create signature\n");
    fflush(stderr);
#endif
    freeSignature2(sig, instance);
    free(sig);
    return -1;
  }
  ret = serializeSignature2(sig, signature, *signature_len, instance);
  if (ret == -1) {
#if !defined(NDEBUG)
    fprintf(stderr, "Failed to serialize signature\n");
    fflush(stderr);
#endif
    freeSignature2(sig, instance);
    free(sig);
    return -1;
  }
  *signature_len = ret;

  freeSignature2(sig, instance);
  free(sig);
  return 0;
}

int impl_verify_picnic2(const picnic_instance_t* instance, const uint8_t* plaintext,
                        const uint8_t* public_key, const uint8_t* msg, size_t msglen,
                        const uint8_t* signature, size_t signature_len) {
  int ret;
  signature2_t* sig = (signature2_t*)malloc(sizeof(signature2_t));
  allocateSignature2(sig, instance);
  if (sig == NULL) {
    return -1;
  }

  ret = deserializeSignature2(sig, signature, signature_len, instance);
  if (ret != EXIT_SUCCESS) {
#if !defined(NDEBUG)
    fprintf(stderr, "Failed to deserialize signature\n");
    fflush(stderr);
#endif
    freeSignature2(sig, instance);
    free(sig);
    return -1;
  }

  ret = verify_picnic2(sig, (uint32_t*)public_key, (uint32_t*)plaintext, msg, msglen, instance);
  if (ret != EXIT_SUCCESS) {
    /* Signature is invalid, or verify function failed */
    freeSignature2(sig, instance);
    free(sig);
    return -1;
  }

  freeSignature2(sig, instance);
  free(sig);
  return 0;
}
