#ifndef __OQS_KEM_FRODOKEM_H
#define __OQS_KEM_FRODOKEM_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_frodokem_640_aes

#define OQS_KEM_frodokem_640_aes_length_public_key 9616
#define OQS_KEM_frodokem_640_aes_length_secret_key 19872
#define OQS_KEM_frodokem_640_aes_length_ciphertext 9736
#define OQS_KEM_frodokem_640_aes_length_shared_secret 16

OQS_KEM *OQS_KEM_frodokem_640_aes_new();

OQS_API OQS_STATUS OQS_KEM_frodokem_640_aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_frodokem_640_aes_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_frodokem_640_aes_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_frodokem_976_aes

#define OQS_KEM_frodokem_976_aes_length_public_key 15632
#define OQS_KEM_frodokem_976_aes_length_secret_key 31272
#define OQS_KEM_frodokem_976_aes_length_ciphertext 15768
#define OQS_KEM_frodokem_976_aes_length_shared_secret 24

OQS_KEM *OQS_KEM_frodokem_976_aes_new();

OQS_API OQS_STATUS OQS_KEM_frodokem_976_aes_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_frodokem_976_aes_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_frodokem_976_aes_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_frodokem_640_cshake

#define OQS_KEM_frodokem_640_cshake_length_public_key 9616
#define OQS_KEM_frodokem_640_cshake_length_secret_key 19872
#define OQS_KEM_frodokem_640_cshake_length_ciphertext 9736
#define OQS_KEM_frodokem_640_cshake_length_shared_secret 16

OQS_KEM *OQS_KEM_frodokem_640_cshake_new();

OQS_API OQS_STATUS OQS_KEM_frodokem_640_cshake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_frodokem_640_cshake_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_frodokem_640_cshake_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_frodokem_976_cshake

#define OQS_KEM_frodokem_976_cshake_length_public_key 15632
#define OQS_KEM_frodokem_976_cshake_length_secret_key 31272
#define OQS_KEM_frodokem_976_cshake_length_ciphertext 15768
#define OQS_KEM_frodokem_976_cshake_length_shared_secret 24

OQS_KEM *OQS_KEM_frodokem_976_cshake_new();

OQS_API OQS_STATUS OQS_KEM_frodokem_976_cshake_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_frodokem_976_cshake_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_frodokem_976_cshake_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#endif
