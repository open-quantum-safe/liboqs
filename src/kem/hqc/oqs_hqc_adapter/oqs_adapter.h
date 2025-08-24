// SPDX-License-Identifier: MIT

#ifndef OQS_ADAPTER_H_
#define OQS_ADAPTER_H_

#include <stdint.h>

#include "oqs_adapter_macros.h"

int OQS_MAKE_FN(REF_HQC_1, crypto_kem_keypair)(uint8_t* ek_kem,
                                               uint8_t* dk_kem);
int OQS_MAKE_FN(REF_HQC_1, crypto_kem_enc)(uint8_t* c_kem, uint8_t* K,
                                           const uint8_t* ek_kem);
int OQS_MAKE_FN(REF_HQC_1, crypto_kem_dec)(uint8_t* K_prime,
                                           const uint8_t* c_kem,
                                           const uint8_t* dk_kem);

int OQS_MAKE_FN(REF_HQC_3, crypto_kem_keypair)(uint8_t* ek_kem,
                                               uint8_t* dk_kem);
int OQS_MAKE_FN(REF_HQC_3, crypto_kem_enc)(uint8_t* c_kem, uint8_t* K,
                                           const uint8_t* ek_kem);
int OQS_MAKE_FN(REF_HQC_3, crypto_kem_dec)(uint8_t* K_prime,
                                           const uint8_t* c_kem,
                                           const uint8_t* dk_kem);

int OQS_MAKE_FN(REF_HQC_5, crypto_kem_keypair)(uint8_t* ek_kem,
                                               uint8_t* dk_kem);
int OQS_MAKE_FN(REF_HQC_5, crypto_kem_enc)(uint8_t* c_kem, uint8_t* K,
                                           const uint8_t* ek_kem);
int OQS_MAKE_FN(REF_HQC_5, crypto_kem_dec)(uint8_t* K_prime,
                                           const uint8_t* c_kem,
                                           const uint8_t* dk_kem);
#endif
