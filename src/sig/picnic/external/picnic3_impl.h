/*! @file picnic3_impl.h
 *  @brief This is the main implementation file of the signature scheme for
 *  the Picnic3 parameter sets.
 *
 *  This file is part of the reference implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef PICNIC3_IMPL_H
#define PICNIC3_IMPL_H

#include <stdint.h>
#include <stddef.h>
#include "picnic_instances.h"

typedef struct proof2_t {
  uint16_t unOpenedIndex; // P[t], index of the party that is not opened.
  uint8_t* seedInfo; // Information required to compute the tree with seeds of of all opened parties
  size_t seedInfoLen; // Length of seedInfo buffer
  uint8_t* aux;       // Last party's correction bits; NULL if P[t] == N-1
  uint8_t* C;         // Commitment to preprocessing step of unopened party
  uint8_t* input;     // Masked input used in online execution
  uint8_t* msgs;      // Broadcast messages of unopened party P[t]
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

int impl_sign_picnic3(const picnic_instance_t* pp, const uint8_t* plaintext,
                      const uint8_t* private_key, const uint8_t* public_key, const uint8_t* msg,
                      size_t msglen, uint8_t* sig, size_t* siglen);
int impl_verify_picnic3(const picnic_instance_t* instance, const uint8_t* plaintext,
                        const uint8_t* public_key, const uint8_t* msg, size_t msglen,
                        const uint8_t* signature, size_t signature_len);

void allocateSignature2(signature2_t* sig, const picnic_instance_t* params);
void freeSignature2(signature2_t* sig, const picnic_instance_t* params);

#endif /* PICNIC3_IMPL_H */
