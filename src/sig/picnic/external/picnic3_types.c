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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "compat.h"
#include "picnic3_types.h"

void allocateRandomTape(randomTape_t* tape, const picnic_instance_t* params) {
  tape->aux_bits       = calloc(1, params->view_size);
  size_t tapeSizeBytes = 2 * params->view_size;
  tape->parity_tapes   = calloc(1, tapeSizeBytes);
  uint8_t* slab        = calloc(params->num_MPC_parties, tapeSizeBytes);
  for (uint8_t i = 0; i < params->num_MPC_parties; i++) {
    tape->tape[i] = slab;
    slab += tapeSizeBytes;
  }
  tape->pos     = 0;
  tape->aux_pos = 0;
}

void freeRandomTape(randomTape_t* tape) {
  if (tape != NULL) {
    free(tape->tape[0]);
    free(tape->parity_tapes);
    free(tape->aux_bits);
  }
}

void allocateProof2(proof2_t* proof, const picnic_instance_t* params) {
  proof->unOpenedIndex = 0;
  proof->seedInfo      = NULL; // Sign/verify code sets it
  proof->seedInfoLen   = 0;
  proof->C             = malloc(params->digest_size);
  proof->input         = malloc(params->input_output_size);
  proof->aux           = malloc(params->view_size);
  proof->msgs          = malloc(params->view_size);
}

static void freeProof2(proof2_t* proof) {
  if (proof) {
    free(proof->seedInfo);
    free(proof->C);
    free(proof->input);
    free(proof->aux);
    free(proof->msgs);
  }
}

bool allocateSignature2(signature2_t* sig, const picnic_instance_t* params) {
  sig->iSeedInfo    = NULL;
  sig->iSeedInfoLen = 0;
  sig->cvInfo       = NULL; // Sign/verify code sets it
  sig->cvInfoLen    = 0;
  sig->challenge    = malloc(params->digest_size);
  sig->challengeC   = malloc(params->num_opened_rounds * sizeof(uint16_t));
  sig->challengeP   = malloc(params->num_opened_rounds * sizeof(uint16_t));
  sig->proofs       = calloc(params->num_rounds, sizeof(proof2_t));
  // Individual proofs are allocated during signature generation, only for rounds when neeeded
  return sig->challenge && sig->challengeC && sig->challengeP && sig->proofs;
}

void freeSignature2(signature2_t* sig, const picnic_instance_t* params) {
  if (sig) {
    free(sig->iSeedInfo);
    free(sig->cvInfo);
    for (size_t i = 0; i < params->num_rounds; i++) {
      freeProof2(&sig->proofs[i]);
    }
    free(sig->proofs);
    free(sig->challengeP);
    free(sig->challengeC);
    free(sig->challenge);
  }
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
  uint8_t* slab = calloc(1, params->num_rounds * (params->input_output_size + sizeof(uint8_t*)));

  inputs_t inputs = (uint8_t**)slab;

  slab += params->num_rounds * sizeof(uint8_t*);

  for (uint32_t i = 0; i < params->num_rounds; i++) {
    inputs[i] = (uint8_t*)slab;
    slab += params->input_output_size;
  }

  return inputs;
}

void freeInputs(inputs_t inputs) {
  free(inputs);
}

msgs_t* allocateMsgs(const picnic_instance_t* params) {
  msgs_t* msgs = malloc(params->num_rounds * sizeof(msgs_t));
  if (!msgs) {
    return NULL;
  }

  uint8_t* slab =
      calloc(1, params->num_rounds * params->num_MPC_parties * ((params->view_size + 7) / 8 * 8));
  if (!slab) {
    free(msgs);
    return NULL;
  }

  for (uint32_t i = 0; i < params->num_rounds; i++) {
    msgs[i].pos      = 0;
    msgs[i].unopened = -1;

    for (uint32_t j = 0; j < params->num_MPC_parties; j++) {
      msgs[i].msgs[j] = slab;
      slab += (params->view_size + 7) / 8 * 8;
    }
  }

  return msgs;
}

msgs_t* allocateMsgsVerify(const picnic_instance_t* params) {
  msgs_t* msgs = malloc(sizeof(msgs_t));
  if (!msgs) {
    return NULL;
  }

  uint8_t* slab = calloc(1, params->num_MPC_parties * ((params->view_size + 7) / 8 * 8));
  if (!slab) {
    free(msgs);
    return NULL;
  }

  msgs->pos      = 0;
  msgs->unopened = -1;

  for (uint32_t j = 0; j < params->num_MPC_parties; j++) {
    msgs->msgs[j] = slab;
    slab += (params->view_size + 7) / 8 * 8;
  }

  return msgs;
}

void freeMsgs(msgs_t* msgs) {
  if (msgs) {
    free(msgs[0].msgs[0]);
    free(msgs);
  }
}

commitments_t* allocateCommitments(const picnic_instance_t* params, size_t numCommitments) {
  commitments_t* commitments = malloc(params->num_rounds * sizeof(commitments_t));

  commitments->nCommitments = numCommitments ? numCommitments : params->num_MPC_parties;

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
  if (commitments) {
    free(commitments[0].hashes);
    free(commitments);
  }
}
