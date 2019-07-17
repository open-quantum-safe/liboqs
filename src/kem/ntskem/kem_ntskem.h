#ifndef __OQS_KEM_NTSKEM_H
#define __OQS_KEM_NTSKEM_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_ntskem_12_64
#define OQS_KEM_ntskem_12_64_length_public_key 319488
#define OQS_KEM_ntskem_12_64_length_secret_key 9248
#define OQS_KEM_ntskem_12_64_length_ciphertext 128
#define OQS_KEM_ntskem_12_64_length_shared_secret 32
OQS_KEM *OQS_KEM_ntskem_12_64_new();
OQS_API OQS_STATUS OQS_KEM_ntskem_12_64_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntskem_12_64_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntskem_12_64_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif
