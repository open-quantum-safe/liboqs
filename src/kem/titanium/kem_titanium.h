#ifndef __OQS_KEM_TITANIUM_H
#define __OQS_KEM_TITANIUM_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_titanium_cca_std_kem

#define OQS_KEM_titanium_cca_std_length_public_key 16352
#define OQS_KEM_titanium_cca_std_length_secret_key 16384
#define OQS_KEM_titanium_cca_std_length_ciphertext 3552
#define OQS_KEM_titanium_cca_std_length_shared_secret 32

OQS_KEM *OQS_KEM_titanium_cca_std_new();

OQS_STATUS OQS_KEM_titanium_cca_std_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_titanium_cca_std_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_titanium_cca_std_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_titanium_cca_hi_kem

#define OQS_KEM_titanium_cca_hi_length_public_key 20512
#define OQS_KEM_titanium_cca_hi_length_secret_key 20544
#define OQS_KEM_titanium_cca_hi_length_ciphertext 6048
#define OQS_KEM_titanium_cca_hi_length_shared_secret 32

OQS_KEM *OQS_KEM_titanium_cca_hi_new();

OQS_STATUS OQS_KEM_titanium_cca_hi_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_titanium_cca_hi_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_titanium_cca_hi_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_titanium_cca_med_kem

#define OQS_KEM_titanium_cca_med_length_public_key 18272
#define OQS_KEM_titanium_cca_med_length_secret_key 18304
#define OQS_KEM_titanium_cca_med_length_ciphertext 4544
#define OQS_KEM_titanium_cca_med_length_shared_secret 32

OQS_KEM *OQS_KEM_titanium_cca_med_new();

OQS_STATUS OQS_KEM_titanium_cca_med_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_titanium_cca_med_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_titanium_cca_med_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_titanium_cca_super_kem

#define OQS_KEM_titanium_cca_super_length_public_key 26912
#define OQS_KEM_titanium_cca_super_length_secret_key 26944
#define OQS_KEM_titanium_cca_super_length_ciphertext 8352
#define OQS_KEM_titanium_cca_super_length_shared_secret 32

OQS_KEM *OQS_KEM_titanium_cca_super_new();

OQS_STATUS OQS_KEM_titanium_cca_super_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_titanium_cca_super_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_titanium_cca_super_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#endif /* __OQS_KEM_TITANIUM_H */
