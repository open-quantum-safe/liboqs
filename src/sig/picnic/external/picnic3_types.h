/*! @file picnic_types.h
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

#ifndef PICNIC_TYPES_H
#define PICNIC_TYPES_H

#include "picnic3_impl.h"

/* Type definitions */
typedef struct randomTape_t {
  uint8_t* tape[MAX_KKW_MPC_PARTIES];
  uint8_t* aux_bits;
  uint8_t* parity_tapes;
  uint32_t pos;
  uint32_t aux_pos;
} randomTape_t;

typedef struct commitments_t {
  uint8_t** hashes;
  size_t nCommitments;
} commitments_t;

typedef uint8_t** inputs_t;

typedef struct msgs_t {
  uint8_t* msgs[MAX_KKW_MPC_PARTIES]; // One for each player
  size_t pos;
  int unopened; // Index of the unopened party, or -1 if all parties opened (when signing)
} msgs_t;

typedef struct proof2_t {
  uint8_t* seedInfo; // Information required to compute the tree with seeds of of all opened parties
  size_t seedInfoLen;     // Length of seedInfo buffer
  uint8_t* aux;           // Last party's correction bits; NULL if P[t] == N-1
  uint8_t* C;             // Commitment to preprocessing step of unopened party
  uint8_t* input;         // Masked input used in online execution
  uint8_t* msgs;          // Broadcast messages of unopened party P[t]
  uint16_t unOpenedIndex; // P[t], index of the party that is not opened.
} proof2_t;

typedef struct signature2_t {
  uint8_t salt[SALT_SIZE];
  uint8_t* iSeedInfo; // Info required to recompute the tree of all initial seeds
  size_t iSeedInfoLen;
  uint8_t* cvInfo; // Info required to check commitments to views (reconstruct Merkle tree)
  size_t cvInfoLen;
  uint8_t* challenge; // output of HCP
  uint16_t* challengeC;
  uint16_t* challengeP;
  proof2_t* proofs; // One proof for each online execution the verifier checks
} signature2_t;

void allocateRandomTape(randomTape_t* tape, const picnic_instance_t* params);
void freeRandomTape(randomTape_t* tape);

void allocateProof2(proof2_t* proof, const picnic_instance_t* params);

commitments_t* allocateCommitments(const picnic_instance_t* params, size_t nCommitments);
void freeCommitments(commitments_t* commitments);

void allocateCommitments2(commitments_t* commitments, const picnic_instance_t* params,
                          size_t nCommitments);
void freeCommitments2(commitments_t* commitments);

inputs_t allocateInputs(const picnic_instance_t* params);
void freeInputs(inputs_t inputs);

msgs_t* allocateMsgs(const picnic_instance_t* params);
msgs_t* allocateMsgsVerify(const picnic_instance_t* params);
void freeMsgs(msgs_t* msgs);

bool allocateSignature2(signature2_t* sig, const picnic_instance_t* params);
void freeSignature2(signature2_t* sig, const picnic_instance_t* params);

#endif /* PICNIC_TYPES_H */
