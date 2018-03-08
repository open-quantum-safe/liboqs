#ifndef __OQS_KEM_BIG_QUAKE_H
#define __OQS_KEM_BIG_QUAKE_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_BIG_QUAKE_1

#define OQS_KEM_BIG_QUAKE_1_length_public_key 25482
#define OQS_KEM_BIG_QUAKE_1_length_secret_key 14772
#define OQS_KEM_BIG_QUAKE_1_length_ciphertext 201
#define OQS_KEM_BIG_QUAKE_1_length_shared_secret 32

OQS_KEM *OQS_KEM_BIG_QUAKE_1_new();

extern OQS_STATUS OQS_KEM_BIG_QUAKE_1_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_BIG_QUAKE_1_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_BIG_QUAKE_1_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif
#endif
