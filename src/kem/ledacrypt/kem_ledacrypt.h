#ifndef __OQS_KEM_LEDACRYPT_H
#define __OQS_KEM_LEDACRYPT_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_ledacrypt_kem_lt_12
#define OQS_KEM_ledacrypt_kem_lt_12_length_public_key 6520
#define OQS_KEM_ledacrypt_kem_lt_12_length_secret_key 26
#define OQS_KEM_ledacrypt_kem_lt_12_length_ciphertext 6520
#define OQS_KEM_ledacrypt_kem_lt_12_length_shared_secret 32
OQS_KEM *OQS_KEM_ledacrypt_kem_lt_12_new();
OQS_API OQS_STATUS OQS_KEM_ledacrypt_kem_lt_12_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_kem_lt_12_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ledacrypt_kem_lt_12_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif
