// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_HQC_H
#define OQS_KEM_HQC_H

#include <oqs/oqs.h>

#if defined(OQS_ENABLE_KEM_hqc_1)
#define OQS_KEM_hqc_1_length_public_key 2241
#define OQS_KEM_hqc_1_length_secret_key 2321
#define OQS_KEM_hqc_1_length_ciphertext 4433
#define OQS_KEM_hqc_1_length_shared_secret 32
#define OQS_KEM_hqc_1_length_keypair_seed 0
#define OQS_KEM_hqc_1_length_encaps_seed 0
OQS_KEM *OQS_KEM_hqc_1_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_1_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_1_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed);
OQS_API OQS_STATUS OQS_KEM_hqc_1_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_1_encaps_derand(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *seed);
OQS_API OQS_STATUS OQS_KEM_hqc_1_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#if defined(OQS_ENABLE_KEM_hqc_3)
#define OQS_KEM_hqc_3_length_public_key 4514
#define OQS_KEM_hqc_3_length_secret_key 4602
#define OQS_KEM_hqc_3_length_ciphertext 8978
#define OQS_KEM_hqc_3_length_shared_secret 32
#define OQS_KEM_hqc_3_length_keypair_seed 0
#define OQS_KEM_hqc_3_length_encaps_seed 0
OQS_KEM *OQS_KEM_hqc_3_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_3_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed);
OQS_API OQS_STATUS OQS_KEM_hqc_3_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_3_encaps_derand(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *seed);
OQS_API OQS_STATUS OQS_KEM_hqc_3_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#if defined(OQS_ENABLE_KEM_hqc_5)
#define OQS_KEM_hqc_5_length_public_key 7237
#define OQS_KEM_hqc_5_length_secret_key 7333
#define OQS_KEM_hqc_5_length_ciphertext 14421
#define OQS_KEM_hqc_5_length_shared_secret 32
#define OQS_KEM_hqc_5_length_keypair_seed 0
#define OQS_KEM_hqc_5_length_encaps_seed 0
OQS_KEM *OQS_KEM_hqc_5_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_5_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_5_keypair_derand(uint8_t *public_key, uint8_t *secret_key, const uint8_t *seed);
OQS_API OQS_STATUS OQS_KEM_hqc_5_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_5_encaps_derand(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *seed);
OQS_API OQS_STATUS OQS_KEM_hqc_5_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#endif

