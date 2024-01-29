// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_ML_KEM_H
#define OQS_KEM_ML_KEM_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_ml_kem_512_ipd
#define OQS_KEM_ml_kem_512_ipd_length_public_key 800
#define OQS_KEM_ml_kem_512_ipd_length_secret_key 1632
#define OQS_KEM_ml_kem_512_ipd_length_ciphertext 768
#define OQS_KEM_ml_kem_512_ipd_length_shared_secret 32
OQS_KEM *OQS_KEM_ml_kem_512_ipd_new(void);
OQS_API OQS_STATUS OQS_KEM_ml_kem_512_ipd_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ml_kem_512_ipd_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ml_kem_512_ipd_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ml_kem_768_ipd
#define OQS_KEM_ml_kem_768_ipd_length_public_key 1184
#define OQS_KEM_ml_kem_768_ipd_length_secret_key 2400
#define OQS_KEM_ml_kem_768_ipd_length_ciphertext 1088
#define OQS_KEM_ml_kem_768_ipd_length_shared_secret 32
OQS_KEM *OQS_KEM_ml_kem_768_ipd_new(void);
OQS_API OQS_STATUS OQS_KEM_ml_kem_768_ipd_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ml_kem_768_ipd_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ml_kem_768_ipd_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ml_kem_1024_ipd
#define OQS_KEM_ml_kem_1024_ipd_length_public_key 1568
#define OQS_KEM_ml_kem_1024_ipd_length_secret_key 3168
#define OQS_KEM_ml_kem_1024_ipd_length_ciphertext 1568
#define OQS_KEM_ml_kem_1024_ipd_length_shared_secret 32
OQS_KEM *OQS_KEM_ml_kem_1024_ipd_new(void);
OQS_API OQS_STATUS OQS_KEM_ml_kem_1024_ipd_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ml_kem_1024_ipd_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ml_kem_1024_ipd_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#endif

