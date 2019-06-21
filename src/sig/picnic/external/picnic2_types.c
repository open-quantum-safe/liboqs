/*! @file picnic_types.c
 *  @brief Functions to allocate/free data types used in the Picnic signature
 *  scheme implementation.
 *
 *  This file is part of the reference implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "picnic2_types.h"
#include "compat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

shares_t* allocateShares(size_t count) {
  shares_t* shares = malloc(sizeof(shares_t));

  shares->shares = aligned_alloc(32, count * sizeof(uint64_t));
  memset(shares->shares, 0, count * sizeof(uint64_t));
  shares->numWords = count;
  return shares;
}
void freeShares(shares_t* shares) {
  aligned_free(shares->shares);
  free(shares);
}

void allocateRandomTape(randomTape_t* tape, const picnic_instance_t* params) {
  tape->nTapes         = params->num_MPC_parties;
  tape->tape           = malloc(tape->nTapes * sizeof(uint8_t*));
  size_t tapeSizeBytes = 2 * params->view_size + params->input_size;
  tapeSizeBytes        = ((tapeSizeBytes + 7) / 8) * 8;
  uint8_t* slab        = calloc(1, tape->nTapes * tapeSizeBytes);
  for (uint8_t i = 0; i < tape->nTapes; i++) {
    tape->tape[i] = slab;
    slab += tapeSizeBytes;
  }
  tape->pos = 0;
}

void freeRandomTape(randomTape_t* tape) {
  if (tape != NULL) {
    free(tape->tape[0]);
    free(tape->tape);
  }
}

void allocateProof2(proof2_t* proof, const picnic_instance_t* params) {
  memset(proof, 0, sizeof(proof2_t));

  proof->unOpenedIndex = 0;
  proof->seedInfo      = NULL; // Sign/verify code sets it
  proof->seedInfoLen   = 0;
  proof->C             = malloc(params->digest_size);
  proof->input         = malloc(params->input_size);
  proof->aux           = malloc(params->view_size);
  proof->msgs          = malloc(params->view_size + params->input_size);
}
void freeProof2(proof2_t* proof) {
  free(proof->seedInfo);
  free(proof->C);
  free(proof->input);
  free(proof->aux);
  free(proof->msgs);
}

void allocateSignature2(signature2_t* sig, const picnic_instance_t* params) {
  sig->iSeedInfo    = NULL;
  sig->iSeedInfoLen = 0;
  sig->cvInfo       = NULL; // Sign/verify code sets it
  sig->cvInfoLen    = 0;
  sig->challengeC   = (uint16_t*)malloc(params->num_opened_rounds * sizeof(uint16_t));
  sig->challengeP   = (uint16_t*)malloc(params->num_opened_rounds * sizeof(uint16_t));
  sig->proofs       = calloc(params->num_rounds, sizeof(proof2_t));
  // Individual proofs are allocated during signature generation, only for rounds when neeeded
}

void freeSignature2(signature2_t* sig, const picnic_instance_t* params) {
  free(sig->iSeedInfo);
  free(sig->cvInfo);
  free(sig->challengeC);
  free(sig->challengeP);
  for (size_t i = 0; i < params->num_rounds; i++) {
    freeProof2(&sig->proofs[i]);
  }
  free(sig->proofs);
}

commitments_t* allocateCommitments(const picnic_instance_t* params, size_t numCommitments) {
  commitments_t* commitments = malloc(params->num_rounds * sizeof(commitments_t));

  commitments->nCommitments = (numCommitments) ? numCommitments : params->num_MPC_parties;

  uint8_t* slab = malloc(params->num_rounds * (commitments->nCommitments * params->digest_size +
                                               commitments->nCommitments * sizeof(uint8_t*)));

  for (uint32_t i = 0; i < params->num_rounds; i++) {
    commitments[i].hashes = (uint8_t**)slab;
    slab += commitments->nCommitments * sizeof(uint8_t*);

    for (uint32_t j = 0; j < commitments->nCommitments; j++) {
      commitments[i].hashes[j] = slab;
      slab += params->digest_size;
    }
  }

  return commitments;
}

void freeCommitments(commitments_t* commitments) {
  free(commitments[0].hashes);
  free(commitments);
}

/* Allocate one commitments_t object with capacity for numCommitments values */
void allocateCommitments2(commitments_t* commitments, const picnic_instance_t* params,
                          size_t numCommitments) {
  commitments->nCommitments = numCommitments;

  uint8_t* slab = malloc(numCommitments * params->digest_size + numCommitments * sizeof(uint8_t*));

  commitments->hashes = (uint8_t**)slab;
  slab += numCommitments * sizeof(uint8_t*);

  for (size_t i = 0; i < numCommitments; i++) {
    commitments->hashes[i] = slab;
    slab += params->digest_size;
  }
}

void freeCommitments2(commitments_t* commitments) {
  if (commitments != NULL) {
    free(commitments->hashes);
  }
}

inputs_t allocateInputs(const picnic_instance_t* params) {
  uint8_t* slab = calloc(1, params->num_rounds * (params->input_size + sizeof(uint8_t*)));

  inputs_t inputs = (uint8_t**)slab;

  slab += params->num_rounds * sizeof(uint8_t*);

  for (uint32_t i = 0; i < params->num_rounds; i++) {
    inputs[i] = (uint8_t*)slab;
    slab += params->input_size;
  }

  return inputs;
}

void freeInputs(inputs_t inputs) {
  free(inputs);
}

msgs_t* allocateMsgs(const picnic_instance_t* params) {
  msgs_t* msgs = malloc(params->num_rounds * sizeof(msgs_t));

  uint8_t* slab =
      calloc(1, params->num_rounds * (params->num_MPC_parties *
                                          ((params->view_size + params->input_size + 7) / 8 * 8) +
                                      params->num_MPC_parties * sizeof(uint8_t*)));

  for (uint32_t i = 0; i < params->num_rounds; i++) {
    msgs[i].pos      = 0;
    msgs[i].unopened = -1;
    msgs[i].msgs     = (uint8_t**)slab;
    slab += params->num_MPC_parties * sizeof(uint8_t*);

    for (uint32_t j = 0; j < params->num_MPC_parties; j++) {
      msgs[i].msgs[j] = slab;
      slab += (params->view_size + params->input_size + 7) / 8 * 8;
    }
  }

  return msgs;
}

void freeMsgs(msgs_t* msgs) {
  free(msgs[0].msgs);
  free(msgs);
}
