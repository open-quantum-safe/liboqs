#ifndef __OQS_KEM_KYBER_H
#define __OQS_KEM_KYBER_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_kyber512

#define OQS_KEM_kyber512_length_public_key 736
#define OQS_KEM_kyber512_length_secret_key 1632
#define OQS_KEM_kyber512_length_ciphertext 800
#define OQS_KEM_kyber512_length_shared_secret 32

OQS_KEM *OQS_KEM_kyber512_new();

OQS_STATUS OQS_KEM_kyber512_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_kyber512_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_kyber512_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_kyber768

#define OQS_KEM_kyber768_length_public_key 1088
#define OQS_KEM_kyber768_length_secret_key 2400
#define OQS_KEM_kyber768_length_ciphertext 1152
#define OQS_KEM_kyber768_length_shared_secret 32

OQS_KEM *OQS_KEM_kyber768_new();

OQS_STATUS OQS_KEM_kyber768_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_kyber768_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_kyber768_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_kyber1024

#define OQS_KEM_kyber1024_length_public_key 1440
#define OQS_KEM_kyber1024_length_secret_key 3168
#define OQS_KEM_kyber1024_length_ciphertext 1504
#define OQS_KEM_kyber1024_length_shared_secret 32

OQS_KEM *OQS_KEM_kyber1024_new();

OQS_STATUS OQS_KEM_kyber1024_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_kyber1024_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_kyber1024_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#endif
