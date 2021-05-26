// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_SABER_H
#define OQS_KEM_SABER_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_saber_lightsaber
#define OQS_KEM_saber_lightsaber_length_public_key 672
#define OQS_KEM_saber_lightsaber_length_secret_key 1568
#define OQS_KEM_saber_lightsaber_length_ciphertext 736
#define OQS_KEM_saber_lightsaber_length_shared_secret 32
OQS_KEM *OQS_KEM_saber_lightsaber_new(void);
OQS_API OQS_STATUS OQS_KEM_saber_lightsaber_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_saber_lightsaber_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_saber_lightsaber_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_saber_saber
#define OQS_KEM_saber_saber_length_public_key 992
#define OQS_KEM_saber_saber_length_secret_key 2304
#define OQS_KEM_saber_saber_length_ciphertext 1088
#define OQS_KEM_saber_saber_length_shared_secret 32
OQS_KEM *OQS_KEM_saber_saber_new(void);
OQS_API OQS_STATUS OQS_KEM_saber_saber_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_saber_saber_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_saber_saber_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_saber_firesaber
#define OQS_KEM_saber_firesaber_length_public_key 1312
#define OQS_KEM_saber_firesaber_length_secret_key 3040
#define OQS_KEM_saber_firesaber_length_ciphertext 1472
#define OQS_KEM_saber_firesaber_length_shared_secret 32
OQS_KEM *OQS_KEM_saber_firesaber_new(void);
OQS_API OQS_STATUS OQS_KEM_saber_firesaber_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_saber_firesaber_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_saber_firesaber_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#endif

