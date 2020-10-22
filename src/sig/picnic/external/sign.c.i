/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifdef PICNIC_INSTANCE

#include "endian_compat.h"
#include "picnic.h"

#include <string.h>
#include <assert.h>

int crypto_sign_keypair(unsigned char* pk, unsigned char* sk) {
  picnic_publickey_t ppk;
  picnic_privatekey_t psk;

  int ret = picnic_keygen(PICNIC_INSTANCE, &ppk, &psk);
  if (ret) {
    return ret;
  }

  ret = picnic_write_public_key(&ppk, pk, PICNIC_PUBLIC_KEY_SIZE(PICNIC_INSTANCE));
  if (ret < 0) {
    return ret;
  }

  ret = picnic_write_private_key(&psk, sk, PICNIC_PRIVATE_KEY_SIZE(PICNIC_INSTANCE));
  if (ret < 0) {
    return ret;
  }

  return 0;
}

int crypto_sign(unsigned char* sm, unsigned long long* smlen, const unsigned char* m,
                unsigned long long mlen, const unsigned char* sk) {
  /* the first byte encodes the parameter set and is public */
  picnic_declassify(&sk[0], sizeof(unsigned char));
  if (sk[0] != PICNIC_INSTANCE) {
    return -3;
  }

  size_t signature_len = PICNIC_SIGNATURE_SIZE(PICNIC_INSTANCE);
  uint32_t len         = 0;

#if !defined(SUPERCOP)
  assert(signature_len + sizeof(len) == CRYPTO_BYTES);
#endif

  picnic_privatekey_t psk;
  int ret = picnic_read_private_key(&psk, sk, PICNIC_PRIVATE_KEY_SIZE(PICNIC_INSTANCE));
  if (ret < 0) {
    return ret;
  }

  ret = picnic_sign(&psk, m, mlen, sm + sizeof(len) + mlen, &signature_len);
  if (ret) {
    return ret;
  }

  // Serialize signature as:
  // 4-byte signature length (little endian)
  // message
  // signature
  len    = htole32(signature_len);
  *smlen = sizeof(len) + mlen + signature_len;
  memmove(sm + sizeof(len), m, mlen);
  memcpy(sm, &len, sizeof(len));

  return 0;
}

int crypto_sign_open(unsigned char* m, unsigned long long* mlen, const unsigned char* sm,
                     unsigned long long smlen, const unsigned char* pk) {
  if (pk[0] != PICNIC_INSTANCE) {
    return -3;
  }

  uint32_t signature_len;
  memcpy(&signature_len, sm, sizeof(signature_len));
  signature_len = le32toh(signature_len);

  if (signature_len + sizeof(signature_len) > smlen) {
    return -2;
  }

  const size_t message_len = smlen - signature_len - sizeof(signature_len);
  const uint8_t* message   = sm + sizeof(signature_len);
  const uint8_t* sig       = sm + sizeof(signature_len) + message_len;

  picnic_publickey_t ppk;
  int ret = picnic_read_public_key(&ppk, pk, PICNIC_PUBLIC_KEY_SIZE(PICNIC_INSTANCE));
  if (ret < 0) {
    return ret;
  }

  ret = picnic_verify(&ppk, message, message_len, sig, signature_len);
  if (ret) {
    return ret;
  }

  memmove(m, message, message_len);
  *mlen = message_len;

  return 0;
}

#endif

// vim: ft=c
