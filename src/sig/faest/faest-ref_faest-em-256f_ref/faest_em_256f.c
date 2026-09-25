/*
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "faest_em_256f.h"
#include "aes.h"
#include "compat.h"
#include "faest_impl.h"
#include "instances.h"
#include "owf.h"
#include "parameters.h"
#include "randomness.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

// memory layout of the public key: OWF input || OWF output
#define PK_INPUT(pk) (pk)
#define PK_OUTPUT(pk) (&pk[32])

// memory layout of the secret key: OWF input || OWF key
#define SK_INPUT(sk) (sk)
#define SK_KEY(sk) (&sk[32])

static_assert(64 == 32 + 32, "invalid public key size");
static_assert(64 == 32 + 256 / 8, "invalid secret key size");

int FAEST_CALLING_CONVENTION faest_em_256f_keygen(uint8_t* pk, uint8_t* sk) {
  if (!pk || !sk) {
    return -1;
  }

  bool done = false;
  while (!done) {
    rand_bytes(SK_KEY(sk), 256  / 8);;
    done = (ptr_get_bit(SK_KEY(sk), 0) & ptr_get_bit(SK_KEY(sk), 1)) == 0;
    faest_declassify(&done, sizeof(done));
  }
  rand_bytes(SK_INPUT(sk), 32);
  // declassify OWF input
  faest_declassify(SK_INPUT(sk), 32);
  faest_em_256f_owf(SK_KEY(sk), SK_INPUT(sk), PK_OUTPUT(pk));
  memcpy(PK_INPUT(pk), SK_INPUT(sk), 32);

  // declassify public key
  faest_declassify(pk, 64);
  return 0;
}

int FAEST_CALLING_CONVENTION faest_em_256f_unpack_private_key(faest_em_256f_unpacked_private_key_t* unpacked_sk, const uint8_t* sk) {
  if (!unpacked_sk || !sk) {
    return -1;
  }

  memcpy(unpacked_sk->owf_input, SK_INPUT(sk), sizeof(unpacked_sk->owf_input));
  memcpy(unpacked_sk->owf_key, SK_KEY(sk), sizeof(unpacked_sk->owf_key));

  faest_em_256f_owf(SK_KEY(sk), SK_INPUT(sk), unpacked_sk->owf_output);
  // declassify OWF output
  faest_declassify(unpacked_sk->owf_output, sizeof(unpacked_sk->owf_output));

  const faest_paramset_t* params = faest_get_paramset(FAEST_EM_256F);
  aes_extend_witness_7(unpacked_sk->witness, unpacked_sk->owf_key, unpacked_sk->owf_input, params);

  return 0;
}

int FAEST_CALLING_CONVENTION faest_em_256f_validate_keypair(const uint8_t* pk, const uint8_t* sk) {
  if (!sk || !pk) {
    return -1;
  }

  if ((ptr_get_bit(SK_KEY(sk), 0) & ptr_get_bit(SK_KEY(sk), 1)) != 0) {
    return 1;
  }

  uint8_t pk_check[64];
  faest_em_256f_owf(SK_KEY(sk), SK_INPUT(sk), PK_OUTPUT(pk_check));
  memcpy(PK_INPUT(pk_check), SK_INPUT(sk), 32);

  return faest_timingsafe_bcmp(pk_check, pk, sizeof(pk_check)) == 0 ? 0 : 2;
}

int FAEST_CALLING_CONVENTION faest_em_256f_sign_with_randomness(const uint8_t* sk, const uint8_t* message, size_t message_len, const uint8_t* rho, size_t rho_len, uint8_t* signature, size_t* signature_len) {
  if (!sk || !signature || !signature_len || *signature_len < FAEST_EM_256F_SIGNATURE_SIZE || (!rho && rho_len) || (!message && message_len)) {
    return -1;
  }

  faest_em_256f_unpacked_private_key_t unpacked_sk;
  int ret = faest_em_256f_unpack_private_key(&unpacked_sk, sk);
  if (ret) {
    faest_em_256f_clear_unpacked_private_key(&unpacked_sk);
    return ret;
  }

  ret = faest_em_256f_unpacked_sign_with_randomness(&unpacked_sk, message, message_len, rho, rho_len, signature, signature_len);
  faest_em_256f_clear_unpacked_private_key(&unpacked_sk);
  return ret;
}

int FAEST_CALLING_CONVENTION faest_em_256f_sign(const uint8_t* sk, const uint8_t* message, size_t message_len, uint8_t* signature, size_t* signature_len) {
  if (!sk || !signature || !signature_len || *signature_len < FAEST_EM_256F_SIGNATURE_SIZE || (!message && message_len)) {
    return -1;
  }

  uint8_t rho[FAEST_EM_256F_LAMBDA / 8];
  rand_bytes(rho, sizeof(rho));

  return faest_em_256f_sign_with_randomness(sk, message, message_len, rho, sizeof(rho), signature, signature_len);
}

int FAEST_CALLING_CONVENTION faest_em_256f_unpacked_sign(const faest_em_256f_unpacked_private_key_t* sk, const uint8_t* message, size_t message_len, uint8_t* signature, size_t* signature_len) {
  if (!sk || !signature || !signature_len || *signature_len < FAEST_EM_256F_SIGNATURE_SIZE || (!message && message_len)) {
    return -1;
  }

  uint8_t rho[FAEST_EM_256F_LAMBDA / 8];
  rand_bytes(rho, sizeof(rho));

  return faest_em_256f_unpacked_sign_with_randomness(sk, message, message_len, rho, sizeof(rho), signature, signature_len);
}

int FAEST_CALLING_CONVENTION faest_em_256f_unpacked_sign_with_randomness(const faest_em_256f_unpacked_private_key_t* sk, const uint8_t* message, size_t message_len, const uint8_t* rho, size_t rho_len, uint8_t* signature, size_t* signature_len) {
  if (!sk || !signature || !signature_len || *signature_len < FAEST_EM_256F_SIGNATURE_SIZE || (!message && message_len)) {
    return -1;
  }

  const faest_paramset_t* params = faest_get_paramset(FAEST_EM_256F);
  faest_sign(signature, message, message_len, sk->owf_key, sk->owf_input, sk->owf_output, sk->witness, rho, rho_len, params);
  *signature_len = FAEST_EM_256F_SIGNATURE_SIZE;

  return 0;
}

int FAEST_CALLING_CONVENTION faest_em_256f_verify(const uint8_t* pk, const uint8_t* message, size_t message_len, const uint8_t* signature, size_t signature_len) {
  if (!pk || !signature || signature_len != FAEST_EM_256F_SIGNATURE_SIZE || (!message && message_len)) {
    return -1;
  }

  const faest_paramset_t* params = faest_get_paramset(FAEST_EM_256F);
  return faest_verify(message, message_len, signature, PK_INPUT(pk), PK_OUTPUT(pk), params);
}

void FAEST_CALLING_CONVENTION faest_em_256f_clear_private_key(uint8_t* key) {
  faest_explicit_bzero(key, FAEST_EM_256F_PRIVATE_KEY_SIZE);
}

void FAEST_CALLING_CONVENTION faest_em_256f_clear_unpacked_private_key(faest_em_256f_unpacked_private_key_t* key) {
  faest_explicit_bzero(key, sizeof(*key));
}

// vim: ft=c
