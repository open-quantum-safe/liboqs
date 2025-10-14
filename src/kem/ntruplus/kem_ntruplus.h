#ifndef OQS_KEM_NTRUPLUS_H
#define OQS_KEM_NTRUPLUS_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_ntruplus_701

#define OQS_KEM_ntruplus_701_length_public_key 1138
#define OQS_KEM_ntruplus_701_length_secret_key 1450
#define OQS_KEM_ntruplus_701_length_ciphertext 1138
#define OQS_KEM_ntruplus_701_length_shared_secret 32

OQS_KEM *OQS_KEM_ntruplus_701_new(void);

OQS_API OQS_STATUS OQS_KEM_ntruplus_701_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruplus_701_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruplus_701_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif

#endif