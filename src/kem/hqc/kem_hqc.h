// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_HQC_H
#define OQS_KEM_HQC_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_hqc_128_1_cca2
#define OQS_KEM_hqc_128_1_cca2_length_public_key 3125
#define OQS_KEM_hqc_128_1_cca2_length_secret_key 3165
#define OQS_KEM_hqc_128_1_cca2_length_ciphertext 6234
#define OQS_KEM_hqc_128_1_cca2_length_shared_secret 64
OQS_KEM *OQS_KEM_hqc_128_1_cca2_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_128_1_cca2_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_128_1_cca2_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_128_1_cca2_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif

