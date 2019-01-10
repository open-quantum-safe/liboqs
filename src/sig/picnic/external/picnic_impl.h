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
#include "mpc_lowmc.h"
#include "picnic.h"

#define MAX_DIGEST_SIZE 64
#define MAX_NUM_ROUNDS 438

typedef enum { TRANSFORM_FS, TRANSFORM_UR } transform_t;

typedef struct {
  const lowmc_t* lowmc;

  uint32_t digest_size; /* bytes */
  uint32_t seed_size;   /* bytes */
  uint32_t num_rounds;

  uint32_t input_size;      /* bytes */
  uint32_t output_size;     /* bytes */
  uint32_t view_size;       /* bytes */
  uint32_t view_round_size; /* bits */

  uint32_t collapsed_challenge_size;       /* bytes */
  uint32_t unruh_without_input_bytes_size; /* bytes */
  uint32_t unruh_with_input_bytes_size;    /* bytes */
  uint32_t max_signature_size;             /* bytes */

  picnic_params_t params;
  transform_t transform;

  struct {
    lowmc_implementation_f lowmc;
    lowmc_store_implementation_f lowmc_store;
    zkbpp_lowmc_implementation_f zkbpp_lowmc;
    zkbpp_lowmc_verify_implementation_f zkbpp_lowmc_verify;
    zkbpp_share_implementation_f mzd_share;
  } impls;
} picnic_instance_t;

const picnic_instance_t* picnic_instance_get(picnic_params_t param);

int impl_sign(const picnic_instance_t* pp, const uint8_t* plaintext, const uint8_t* private_key,
              const uint8_t* public_key, const uint8_t* msg, size_t msglen, uint8_t* sig,
              size_t* siglen);

int impl_verify(const picnic_instance_t* pp, const uint8_t* plaintext, const uint8_t* public_key,
                const uint8_t* msg, size_t msglen, const uint8_t* sig, size_t siglen);

PICNIC_EXPORT size_t PICNIC_CALLING_CONVENTION picnic_get_lowmc_block_size(picnic_params_t param);
PICNIC_EXPORT size_t PICNIC_CALLING_CONVENTION picnic_get_private_key_size(picnic_params_t param);
PICNIC_EXPORT size_t PICNIC_CALLING_CONVENTION picnic_get_public_key_size(picnic_params_t param);
/**
 * Compute public key from secret key.
 *
 * @param[in] sk The secret key
 * @param[out] pk The public key to be populated
 * @return Returns 0 on success, or a nonzero value indicating an error.
 **/
PICNIC_EXPORT int PICNIC_CALLING_CONVENTION picnic_sk_to_pk(const picnic_privatekey_t* sk,
                                                            picnic_publickey_t* pk);

#if defined(PICNIC_STATIC)
void visualize_signature(FILE* out, const picnic_instance_t* pp, const uint8_t* msg, size_t msglen,
                         const uint8_t* sig, size_t siglen);
void picnic_visualize_keys(FILE* out, const picnic_privatekey_t* private_key,
                           const picnic_publickey_t* public_key);
void picnic_visualize(FILE* out, const picnic_publickey_t* public_key, const uint8_t* msg,
                      size_t msglen, const uint8_t* sig, size_t siglen);
#endif

#endif
