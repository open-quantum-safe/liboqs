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

#include "picnic2_impl.h"

/* Type definitions */
typedef struct randomTape_t {
  uint8_t** tape;
  uint8_t* aux_bits;
  uint32_t pos;
  uint32_t aux_pos;
  size_t nTapes;
  uint64_t* buffer;
} randomTape_t;

typedef struct commitments_t {
  uint8_t** hashes;
  size_t nCommitments;
} commitments_t;

typedef uint8_t** inputs_t;

typedef struct msgs_t {
  uint8_t** msgs; // One for each player
  size_t pos;
  int unopened; // Index of the unopened party, or -1 if all parties opened (when signing)
} msgs_t;

typedef struct shares_t {
  uint64_t* shares;
  size_t numWords;
} shares_t;

#define UNUSED_PARAMETER(x) (void)(x)

void allocateRandomTape(randomTape_t* tape, const picnic_instance_t* params);
void freeRandomTape(randomTape_t* tape);
void partialFreeRandomTape(randomTape_t* tape);
void finalFreeRandomTape(randomTape_t* tape);

void allocateProof2(proof2_t* proof, const picnic_instance_t* params);
void freeProof2(proof2_t* proof);

void allocateCommitments2(commitments_t* commitments, const picnic_instance_t* params,
                          size_t nCommitments);
void freeCommitments2(commitments_t* commitments);

inputs_t allocateInputs(const picnic_instance_t* params);
void freeInputs(inputs_t inputs);

msgs_t* allocateMsgs(const picnic_instance_t* params);
msgs_t* allocateMsgsVerify(const picnic_instance_t* params);
void freeMsgs(msgs_t* msgs);

shares_t* allocateShares(size_t count);
void freeShares(shares_t* shares);

#endif /* PICNIC_TYPES_H */
