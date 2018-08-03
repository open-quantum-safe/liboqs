#ifndef __OQS_KEM_TITANIUM_H
#define __OQS_KEM_TITANIUM_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_titanium_cca_std_kem

#define OQS_KEM_titanium_cca_std_length_public_key 16352
#define OQS_KEM_titanium_cca_std_length_secret_key 16384
#define OQS_KEM_titanium_cca_std_length_ciphertext 3552
#define OQS_KEM_titanium_cca_std_length_shared_secret 32

OQS_KEM *OQS_KEM_titanium_cca_std_new();

extern OQS_STATUS OQS_KEM_titanium_cca_std_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_titanium_cca_std_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_titanium_cca_std_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif
#endif
