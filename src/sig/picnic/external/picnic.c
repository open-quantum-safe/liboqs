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

#include "picnic.h"

#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "lowmc.h"
#include "picnic_impl.h"
#include "picnic2_impl.h"
#include <oqs/rand.h>

// Public and private keys are serialized as follows:
// - public key: instance || C || p
// - secret key: instance || sk || C || p

#define SK_SK(sk) &(sk)->data[1]
#define SK_C(sk) &(sk)->data[1 + input_size]
#define SK_PT(sk) &(sk)->data[1 + input_size + output_size]

#define PK_SK(pk) &(pk)->data[1]
#define PK_C(pk) &(pk)->data[1]
#define PK_PT(pk) &(pk)->data[1 + output_size]

size_t PICNIC_CALLING_CONVENTION picnic_get_lowmc_block_size(picnic_params_t param) {
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return 0;
  }

  return instance->output_size;
}

size_t PICNIC_CALLING_CONVENTION picnic_signature_size(picnic_params_t param) {
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return 0;
  }

  return instance->max_signature_size;
}

size_t PICNIC_CALLING_CONVENTION picnic_get_private_key_size(picnic_params_t param) {
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return 0;
  }

  return picnic_get_public_key_size(param) + instance->input_size;
}

size_t PICNIC_CALLING_CONVENTION picnic_get_public_key_size(picnic_params_t param) {
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return 0;
  }

  return 1 + (instance->output_size << 1);
}

int PICNIC_CALLING_CONVENTION picnic_keygen(picnic_params_t param, picnic_publickey_t* pk,
                                            picnic_privatekey_t* sk) {

  if (!pk || !sk) {
    return -1;
  }

  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return -1;
  }

  const size_t input_size  = instance->input_size;
  const size_t output_size = instance->output_size;

  uint8_t* sk_sk = SK_SK(sk);
  uint8_t* sk_pt = SK_PT(sk);
  uint8_t* sk_c  = SK_C(sk);

  // generate private key
  sk->data[0] = param;
  // random secret key
  OQS_randombytes(sk_sk, input_size);

  // random plain text
  OQS_randombytes(sk_pt, output_size);

  // encrypt plaintext under secret key
  if (picnic_sk_to_pk(sk, pk)) {
    return -1;
  }
  // copy ciphertext to secret key
  memcpy(sk_c, PK_C(pk), output_size);
  return 0;
}

int PICNIC_CALLING_CONVENTION picnic_sk_to_pk(const picnic_privatekey_t* sk,
                                              picnic_publickey_t* pk) {
  if (!sk || !pk) {
    return -1;
  }

  const picnic_params_t param       = sk->data[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return -1;
  }

  const size_t input_size  = instance->input_size;
  const size_t output_size = instance->output_size;

  const uint8_t* sk_sk = SK_SK(sk);
  uint8_t* pk_c        = PK_C(pk);
  uint8_t* pk_pt       = PK_PT(pk);
  const uint8_t* sk_pt = SK_PT(sk);

  mzd_local_t plaintext[(MAX_LOWMC_BLOCK_SIZE_BITS + 255) / 256];
  mzd_local_t privkey[(MAX_LOWMC_BLOCK_SIZE_BITS + 255) / 256];
  mzd_local_t ciphertext[(MAX_LOWMC_BLOCK_SIZE_BITS + 255) / 256];

  mzd_from_char_array(plaintext, sk_pt, output_size);
  mzd_from_char_array(privkey, sk_sk, input_size);

  // compute public key
  instance->impls.lowmc(privkey, plaintext, ciphertext);

  pk->data[0] = param;
  memcpy(pk_pt, sk_pt, output_size);
  mzd_to_char_array(pk_c, ciphertext, output_size);

  return 0;
}

int PICNIC_CALLING_CONVENTION picnic_validate_keypair(const picnic_privatekey_t* sk,
                                                      const picnic_publickey_t* pk) {
  if (!sk || !pk) {
    return -1;
  }

  const picnic_params_t param       = sk->data[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return -1;
  }

  const size_t input_size  = instance->input_size;
  const size_t output_size = instance->output_size;
  const uint8_t* sk_sk     = SK_SK(sk);
  const uint8_t* sk_pt     = SK_PT(sk);
  const uint8_t* sk_c      = SK_C(sk);
  const uint8_t* pk_pt     = PK_PT(pk);
  const uint8_t* pk_c      = PK_C(pk);

  // check param and plaintext
  if (param != pk->data[0] || memcmp(sk_pt, pk_pt, output_size) != 0 ||
      memcmp(sk_c, pk_c, output_size) != 0) {
    return -1;
  }

  mzd_local_t plaintext[(MAX_LOWMC_BLOCK_SIZE_BITS + 255) / 256];
  mzd_local_t privkey[(MAX_LOWMC_BLOCK_SIZE_BITS + 255) / 256];
  mzd_local_t ciphertext[(MAX_LOWMC_BLOCK_SIZE_BITS + 255) / 256];

  mzd_from_char_array(plaintext, sk_pt, instance->output_size);
  mzd_from_char_array(privkey, sk_sk, instance->input_size);

  // compute public key
  instance->impls.lowmc(privkey, plaintext, ciphertext);

  uint8_t buffer[MAX_LOWMC_BLOCK_SIZE];
  mzd_to_char_array(buffer, ciphertext, output_size);

  return memcmp(buffer, pk_c, output_size);
}

int PICNIC_CALLING_CONVENTION picnic_sign(const picnic_privatekey_t* sk, const uint8_t* message,
                                          size_t message_len, uint8_t* signature,
                                          size_t* signature_len) {
  if (!sk || !signature || !signature_len) {
    return -1;
  }

  const picnic_params_t param       = sk->data[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return -1;
  }

  const size_t output_size = instance->output_size;
  const size_t input_size  = instance->input_size;

  const uint8_t* sk_sk = SK_SK(sk);
  const uint8_t* sk_c  = SK_C(sk);
  const uint8_t* sk_pt = SK_PT(sk);

  if (param == Picnic2_L1_FS || param == Picnic2_L3_FS || param == Picnic2_L5_FS)
    return impl_sign_picnic2(instance, sk_pt, sk_sk, sk_c, message, message_len, signature,
                             signature_len);
  else
    return impl_sign(instance, sk_pt, sk_sk, sk_c, message, message_len, signature, signature_len);
}

int PICNIC_CALLING_CONVENTION picnic_verify(const picnic_publickey_t* pk, const uint8_t* message,
                                            size_t message_len, const uint8_t* signature,
                                            size_t signature_len) {
  if (!pk || !signature || !signature_len) {
    return -1;
  }

  const picnic_params_t param       = pk->data[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return -1;
  }

  const size_t output_size = instance->output_size;

  const uint8_t* pk_c  = PK_C(pk);
  const uint8_t* pk_pt = PK_PT(pk);

  if (param == Picnic2_L1_FS || param == Picnic2_L3_FS || param == Picnic2_L5_FS)
    return impl_verify_picnic2(instance, pk_pt, pk_c, message, message_len, signature,
                               signature_len);
  else
    return impl_verify(instance, pk_pt, pk_c, message, message_len, signature, signature_len);
}

const char* PICNIC_CALLING_CONVENTION picnic_get_param_name(picnic_params_t parameters) {
  switch (parameters) {
  case Picnic_L1_1_FS:
    return "Picnic_L1_1_FS";
  case Picnic_L1_1_UR:
    return "Picnic_L1_1_UR";
  case Picnic_L3_1_FS:
    return "Picnic_L3_1_FS";
  case Picnic_L3_1_UR:
    return "Picnic_L3_1_UR";
  case Picnic_L5_1_FS:
    return "Picnic_L5_1_FS";
  case Picnic_L5_1_UR:
    return "Picnic_L5_1_UR";
  case Picnic_L1_FS:
    return "Picnic_L1_FS";
  case Picnic_L1_UR:
    return "Picnic_L1_UR";
  case Picnic_L3_FS:
    return "Picnic_L3_FS";
  case Picnic_L3_UR:
    return "Picnic_L3_UR";
  case Picnic_L5_FS:
    return "Picnic_L5_FS";
  case Picnic_L5_UR:
    return "Picnic_L5_UR";
  case Picnic2_L1_FS:
    return "Picnic2_L1_FS";
  case Picnic2_L3_FS:
    return "Picnic2_L3_FS";
  case Picnic2_L5_FS:
    return "Picnic2_L5_FS";
  default:
    return "Unknown parameter set";
  }
}

int PICNIC_CALLING_CONVENTION picnic_write_public_key(const picnic_publickey_t* key, uint8_t* buf,
                                                      size_t buflen) {
  if (!key || !buf) {
    return -1;
  }

  const picnic_params_t param       = key->data[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return -1;
  }

  const size_t output_size    = instance->output_size;
  const size_t bytes_required = 1 + 2 * output_size;
  if (buflen < bytes_required) {
    return -1;
  }

  memcpy(buf, key->data, bytes_required);
  return (int)bytes_required;
}

int PICNIC_CALLING_CONVENTION picnic_read_public_key(picnic_publickey_t* key, const uint8_t* buf,
                                                     size_t buflen) {
  if (!key || !buf || buflen < 1) {
    return -1;
  }

  const picnic_params_t param       = buf[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return -1;
  }

  const size_t output_size    = instance->output_size;
  const size_t bytes_required = 1 + 2 * output_size;
  if (buflen < bytes_required) {
    return -1;
  }

  memcpy(key->data, buf, bytes_required);
  return 0;
}

int PICNIC_CALLING_CONVENTION picnic_write_private_key(const picnic_privatekey_t* key, uint8_t* buf,
                                                       size_t buflen) {
  if (!key || !buf) {
    return -1;
  }

  const picnic_params_t param       = key->data[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return -1;
  }

  const size_t input_size     = instance->input_size;
  const size_t output_size    = instance->output_size;
  const size_t bytes_required = 1 + input_size + 2 * output_size;
  if (buflen < bytes_required) {
    return -1;
  }

  memcpy(buf, &key->data, bytes_required);
  return (int)bytes_required;
}

int PICNIC_CALLING_CONVENTION picnic_read_private_key(picnic_privatekey_t* key, const uint8_t* buf,
                                                      size_t buflen) {
  if (!key || !buf || buflen < 1) {
    return -1;
  }

  const picnic_params_t param       = buf[0];
  const picnic_instance_t* instance = picnic_instance_get(param);
  if (!instance) {
    return -1;
  }

  const size_t input_size     = instance->input_size;
  const size_t output_size    = instance->output_size;
  const size_t bytes_required = 1 + input_size + 2 * output_size;
  if (buflen < bytes_required) {
    return -1;
  }

  memcpy(key->data, buf, bytes_required);
  return 0;
}

/* cropped unused picnic_visualize_keys */
