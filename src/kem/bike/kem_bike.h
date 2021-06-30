// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_BIKE_H
#define OQS_KEM_BIKE_H

#include <oqs/oqs.h>

// Round-3 KEMs
#ifdef OQS_ENABLE_KEM_bike_l1
#define OQS_KEM_bike_l1_length_secret_key 5223
#define OQS_KEM_bike_l1_length_public_key 1541
#define OQS_KEM_bike_l1_length_ciphertext 1573
#define OQS_KEM_bike_l1_length_shared_secret 32
OQS_KEM *OQS_KEM_bike_l1_new(void);
OQS_API OQS_STATUS OQS_KEM_bike_l1_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike_l1_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike_l1_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_bike_l3
#define OQS_KEM_bike_l3_length_secret_key 10105
#define OQS_KEM_bike_l3_length_public_key 3083
#define OQS_KEM_bike_l3_length_ciphertext 3115
#define OQS_KEM_bike_l3_length_shared_secret 32
OQS_KEM *OQS_KEM_bike_l3_new(void);
OQS_API OQS_STATUS OQS_KEM_bike_l3_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike_l3_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike_l3_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif // OQS_KEM_BIKE_H
