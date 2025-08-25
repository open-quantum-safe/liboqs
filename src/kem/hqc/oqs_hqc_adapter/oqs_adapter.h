// SPDX-License-Identifier: MIT

#ifndef OQS_ADAPTER_H_
#define OQS_ADAPTER_H_

#include <stdint.h>

#include "oqs_adapter_macros.h"

extern int OQS_MAKE_FN(KEM_PREFIX, crypto_kem_keypair)(uint8_t* ek_kem,
                                                       uint8_t* dk_kem);
extern int OQS_MAKE_FN(KEM_PREFIX, crypto_kem_enc)(uint8_t* c_kem, uint8_t* K,
                                                   const uint8_t* ek_kem);
extern int OQS_MAKE_FN(KEM_PREFIX, crypto_kem_dec)(uint8_t* K_prime,
                                                   const uint8_t* c_kem,
                                                   const uint8_t* dk_kem);

#endif
