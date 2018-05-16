#ifndef __OQS_KEM_SABER_H
#define __OQS_KEM_SABER_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_saber_light_saber_kem

#define OQS_KEM_saber_light_saber_kem_length_public_key 672
#define OQS_KEM_saber_light_saber_kem_length_secret_key 1568
#define OQS_KEM_saber_light_saber_kem_length_ciphertext 736
#define OQS_KEM_saber_light_saber_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_saber_light_saber_kem_new();

extern OQS_STATUS OQS_KEM_saber_light_saber_kem_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_saber_light_saber_kem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_saber_light_saber_kem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_saber_saber_kem

#define OQS_KEM_saber_saber_kem_length_public_key 992
#define OQS_KEM_saber_saber_kem_length_secret_key 2304
#define OQS_KEM_saber_saber_kem_length_ciphertext 1088
#define OQS_KEM_saber_saber_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_saber_saber_kem_new();

extern OQS_STATUS OQS_KEM_saber_saber_kem_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_saber_saber_kem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_saber_saber_kem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_saber_fire_saber_kem

#define OQS_KEM_saber_fire_saber_kem_length_public_key 1312
#define OQS_KEM_saber_fire_saber_kem_length_secret_key 3040
#define OQS_KEM_saber_fire_saber_kem_length_ciphertext 1472
#define OQS_KEM_saber_fire_saber_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_saber_fire_saber_kem_new();

extern OQS_STATUS OQS_KEM_saber_fire_saber_kem_keypair(uint8_t *public_key, uint8_t *secret_key);
extern OQS_STATUS OQS_KEM_saber_fire_saber_kem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
extern OQS_STATUS OQS_KEM_saber_fire_saber_kem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#endif
