#ifndef __OQS_KEM_NTRUPRIME_H
#define __OQS_KEM_NTRUPRIME_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_ntruprime_ntrulpr653
#define OQS_KEM_ntruprime_ntrulpr653_length_public_key 897
#define OQS_KEM_ntruprime_ntrulpr653_length_secret_key 1125
#define OQS_KEM_ntruprime_ntrulpr653_length_ciphertext 1025
#define OQS_KEM_ntruprime_ntrulpr653_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_ntrulpr653_new();
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr653_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr653_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr653_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntruprime_ntrulpr761
#define OQS_KEM_ntruprime_ntrulpr761_length_public_key 1039
#define OQS_KEM_ntruprime_ntrulpr761_length_secret_key 1294
#define OQS_KEM_ntruprime_ntrulpr761_length_ciphertext 1167
#define OQS_KEM_ntruprime_ntrulpr761_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_ntrulpr761_new();
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr761_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr761_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr761_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntruprime_ntrulpr857
#define OQS_KEM_ntruprime_ntrulpr857_length_public_key 1184
#define OQS_KEM_ntruprime_ntrulpr857_length_secret_key 1463
#define OQS_KEM_ntruprime_ntrulpr857_length_ciphertext 1312
#define OQS_KEM_ntruprime_ntrulpr857_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_ntrulpr857_new();
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr857_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr857_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr857_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif
