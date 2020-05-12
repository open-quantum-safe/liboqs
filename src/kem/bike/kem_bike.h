// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_BIKE_H
#define OQS_KEM_BIKE_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_bike1_l1_cpa
#define OQS_KEM_bike1_l1_cpa_length_secret_key 3110
#define OQS_KEM_bike1_l1_cpa_length_public_key 2542
#define OQS_KEM_bike1_l1_cpa_length_ciphertext 2542
#define OQS_KEM_bike1_l1_cpa_length_shared_secret 32
OQS_KEM *OQS_KEM_bike1_l1_cpa_new(void);
OQS_API OQS_STATUS OQS_KEM_bike1_l1_cpa_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l1_cpa_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l1_cpa_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_bike1_l3_cpa
#define OQS_KEM_bike1_l3_cpa_length_secret_key 5788
#define OQS_KEM_bike1_l3_cpa_length_public_key 4964
#define OQS_KEM_bike1_l3_cpa_length_ciphertext 4964
#define OQS_KEM_bike1_l3_cpa_length_shared_secret 32
OQS_KEM *OQS_KEM_bike1_l3_cpa_new(void);
OQS_API OQS_STATUS OQS_KEM_bike1_l3_cpa_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l3_cpa_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l3_cpa_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_bike1_l1_fo
#define OQS_KEM_bike1_l1_fo_length_secret_key 6460
#define OQS_KEM_bike1_l1_fo_length_public_key 2946
#define OQS_KEM_bike1_l1_fo_length_ciphertext 2946
#define OQS_KEM_bike1_l1_fo_length_shared_secret 32
OQS_KEM *OQS_KEM_bike1_l1_fo_new(void);
OQS_API OQS_STATUS OQS_KEM_bike1_l1_fo_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l1_fo_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l1_fo_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_bike1_l3_fo
#define OQS_KEM_bike1_l3_fo_length_secret_key 13236
#define OQS_KEM_bike1_l3_fo_length_public_key 6206
#define OQS_KEM_bike1_l3_fo_length_ciphertext 6206
#define OQS_KEM_bike1_l3_fo_length_shared_secret 32
OQS_KEM *OQS_KEM_bike1_l3_fo_new(void);
OQS_API OQS_STATUS OQS_KEM_bike1_l3_fo_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l3_fo_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_bike1_l3_fo_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif // OQS_KEM_BIKE_H
