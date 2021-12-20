/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef PICNIC_IMPL_H
#define PICNIC_IMPL_H

#include "lowmc.h"
#include "picnic_instances.h"
#include "picnic.h"

typedef struct {
  mzd_local_t m_plaintext[(MAX_LOWMC_BLOCK_SIZE_BITS + 255) / 256];
  /* private key for signing, public key for verification */
  mzd_local_t m_key[(MAX_LOWMC_KEY_SIZE_BITS + 255) / 256];

  const uint8_t* plaintext;
  const uint8_t* private_key;
  const uint8_t* public_key;
  const uint8_t* msg;
  size_t msglen;
#if defined(WITH_UNRUH)
  bool unruh;
#endif
} picnic_context_t;

int picnic_impl_sign(const picnic_instance_t* pp, const picnic_context_t* context, uint8_t* sig,
                     size_t* siglen);
int picnic_impl_verify(const picnic_instance_t* pp, const picnic_context_t* context,
                       const uint8_t* sig, size_t siglen);

#if defined(PICNIC_STATIC)
void visualize_signature(FILE* out, const picnic_instance_t* pp, const picnic_context_t* ctxt,
                         const uint8_t* sig, size_t siglen);
void picnic_visualize_keys(FILE* out, const picnic_privatekey_t* private_key,
                           const picnic_publickey_t* public_key);
void picnic_visualize(FILE* out, const picnic_publickey_t* public_key, const uint8_t* msg,
                      size_t msglen, const uint8_t* sig, size_t siglen);
#endif
#endif
