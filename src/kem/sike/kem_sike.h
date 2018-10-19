#ifndef __OQS_KEM_SIKE_H
#define __OQS_KEM_SIKE_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_sike_p503

#define OQS_KEM_sike_p503_length_public_key 378
#define OQS_KEM_sike_p503_length_secret_key 434
#define OQS_KEM_sike_p503_length_ciphertext 402
#define OQS_KEM_sike_p503_length_shared_secret 16

OQS_KEM *OQS_KEM_sike_p503_new();

OQS_API OQS_STATUS OQS_KEM_sike_p503_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p503_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p503_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sike_p751

#define OQS_KEM_sike_p751_length_public_key 564
#define OQS_KEM_sike_p751_length_secret_key 644
#define OQS_KEM_sike_p751_length_ciphertext 596
#define OQS_KEM_sike_p751_length_shared_secret 24

OQS_KEM *OQS_KEM_sike_p751_new();

OQS_API OQS_STATUS OQS_KEM_sike_p751_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sike_p751_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sike_p751_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p503

#define OQS_KEM_sidh_p503_length_public_key 378
#define OQS_KEM_sidh_p503_length_secret_key 32
#define OQS_KEM_sidh_p503_length_ciphertext 378
#define OQS_KEM_sidh_p503_length_shared_secret 126

OQS_KEM *OQS_KEM_sidh_p503_new();

OQS_API OQS_STATUS OQS_KEM_sidh_p503_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p503_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p503_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_sidh_p751

#define OQS_KEM_sidh_p751_length_public_key 564
#define OQS_KEM_sidh_p751_length_secret_key 48
#define OQS_KEM_sidh_p751_length_ciphertext 564
#define OQS_KEM_sidh_p751_length_shared_secret 188

OQS_KEM *OQS_KEM_sidh_p751_new();

OQS_API OQS_STATUS OQS_KEM_sidh_p751_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p751_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_sidh_p751_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#endif
