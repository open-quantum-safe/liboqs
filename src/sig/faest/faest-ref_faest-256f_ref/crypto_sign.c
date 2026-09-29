/*
 *  SPDX-License-Identifier: MIT
 */

#include "api.h"

#include "faest_256f.h"

int crypto_sign_keypair(uint8_t* pk, uint8_t* sk) {
  return faest_256f_keygen(pk, sk);
}

int crypto_sign_signature(uint8_t* sig, size_t* siglen, const uint8_t* m, size_t mlen,
                          const uint8_t* sk) {
  size_t sigbuflen = FAEST_256F_SIGNATURE_SIZE;
  int ret = faest_256f_sign(sk, m, mlen, sig, &sigbuflen);
  if (ret == 0 && siglen != NULL) {
    *siglen = sigbuflen;
  }
  return ret;
}

int crypto_sign_verify(const uint8_t* sig, size_t siglen, const uint8_t* m, size_t mlen,
                       const uint8_t* pk) {
  return faest_256f_verify(pk, m, mlen, sig, siglen);
}
