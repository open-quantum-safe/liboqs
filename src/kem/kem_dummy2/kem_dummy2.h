#ifndef __OQS_KEM_DUMMY2_H
#define __OQS_KEM_DUMMY2_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_dummy2

// I am not sure how to automatically generate these from api.h without including api.h which would risk polluting the global namespace
#define OQS_KEM_dummy2_length_public_key 64
#define OQS_KEM_dummy2_length_secret_key 192
#define OQS_KEM_dummy2_length_ciphertext 128
#define OQS_KEM_dummy2_length_shared_secret 32

OQS_KEM *OQS_KEM_dummy2_new();

extern OQS_STATUS OQS_KEM_dummy2_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_dummy2_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_dummy2_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#endif
