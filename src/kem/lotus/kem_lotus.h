#ifndef __OQS_KEM_LOTUS_H
#define __OQS_KEM_LOTUS_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_LOTUS192KEM

#define OQS_KEM_LOTUS192KEM_length_public_key 1025024
#define OQS_KEM_LOTUS192KEM_length_secret_key 1126400
#define OQS_KEM_LOTUS192KEM_length_ciphertext 1480
#define OQS_KEM_LOTUS192KEM_length_shared_secret 24

OQS_KEM *OQS_KEM_LOTUS192KEM_new();

OQS_STATUS OQS_KEM_LOTUS192KEM_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_LOTUS192KEM_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_LOTUS192KEM_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_LOTUS256KEM

#define OQS_KEM_LOTUS256KEM_length_public_key 1470976
#define OQS_KEM_LOTUS256KEM_length_secret_key 1630720
#define OQS_KEM_LOTUS256KEM_length_ciphertext 1800
#define OQS_KEM_LOTUS256KEM_length_shared_secret 32

OQS_KEM *OQS_KEM_LOTUS256KEM_new();

OQS_STATUS OQS_KEM_LOTUS256KEM_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_LOTUS256KEM_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_LOTUS256KEM_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_LOTUS128KEM

#define OQS_KEM_LOTUS128KEM_length_public_key 658944
#define OQS_KEM_LOTUS128KEM_length_secret_key 714240
#define OQS_KEM_LOTUS128KEM_length_ciphertext 1160
#define OQS_KEM_LOTUS128KEM_length_shared_secret 16

OQS_KEM *OQS_KEM_LOTUS128KEM_new();

OQS_STATUS OQS_KEM_LOTUS128KEM_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_LOTUS128KEM_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_LOTUS128KEM_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#endif /* __OQS_KEM_LOTUS_H */
