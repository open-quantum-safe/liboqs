#ifndef OQS_KEM_THREEBEARS_H
#define OQS_KEM_THREEBEARS_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_threebears_babybear
#define OQS_KEM_threebears_babybear_length_public_key 804
#define OQS_KEM_threebears_babybear_length_secret_key 40
#define OQS_KEM_threebears_babybear_length_ciphertext 917
#define OQS_KEM_threebears_babybear_length_shared_secret 32
OQS_KEM *OQS_KEM_threebears_babybear_new(void);
OQS_API OQS_STATUS OQS_KEM_threebears_babybear_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_threebears_babybear_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_threebears_babybear_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_threebears_babybear_ephem
#define OQS_KEM_threebears_babybear_ephem_length_public_key 804
#define OQS_KEM_threebears_babybear_ephem_length_secret_key 40
#define OQS_KEM_threebears_babybear_ephem_length_ciphertext 917
#define OQS_KEM_threebears_babybear_ephem_length_shared_secret 32
OQS_KEM *OQS_KEM_threebears_babybear_ephem_new(void);
OQS_API OQS_STATUS OQS_KEM_threebears_babybear_ephem_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_threebears_babybear_ephem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_threebears_babybear_ephem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_threebears_mamabear
#define OQS_KEM_threebears_mamabear_length_public_key 1194
#define OQS_KEM_threebears_mamabear_length_secret_key 40
#define OQS_KEM_threebears_mamabear_length_ciphertext 1307
#define OQS_KEM_threebears_mamabear_length_shared_secret 32
OQS_KEM *OQS_KEM_threebears_mamabear_new(void);
OQS_API OQS_STATUS OQS_KEM_threebears_mamabear_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_threebears_mamabear_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_threebears_mamabear_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_threebears_mamabear_ephem
#define OQS_KEM_threebears_mamabear_ephem_length_public_key 1194
#define OQS_KEM_threebears_mamabear_ephem_length_secret_key 40
#define OQS_KEM_threebears_mamabear_ephem_length_ciphertext 1307
#define OQS_KEM_threebears_mamabear_ephem_length_shared_secret 32
OQS_KEM *OQS_KEM_threebears_mamabear_ephem_new(void);
OQS_API OQS_STATUS OQS_KEM_threebears_mamabear_ephem_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_threebears_mamabear_ephem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_threebears_mamabear_ephem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_threebears_papabear
#define OQS_KEM_threebears_papabear_length_public_key 1584
#define OQS_KEM_threebears_papabear_length_secret_key 40
#define OQS_KEM_threebears_papabear_length_ciphertext 1697
#define OQS_KEM_threebears_papabear_length_shared_secret 32
OQS_KEM *OQS_KEM_threebears_papabear_new(void);
OQS_API OQS_STATUS OQS_KEM_threebears_papabear_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_threebears_papabear_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_threebears_papabear_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_threebears_papabear_ephem
#define OQS_KEM_threebears_papabear_ephem_length_public_key 1584
#define OQS_KEM_threebears_papabear_ephem_length_secret_key 40
#define OQS_KEM_threebears_papabear_ephem_length_ciphertext 1697
#define OQS_KEM_threebears_papabear_ephem_length_shared_secret 32
OQS_KEM *OQS_KEM_threebears_papabear_ephem_new(void);
OQS_API OQS_STATUS OQS_KEM_threebears_papabear_ephem_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_threebears_papabear_ephem_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_threebears_papabear_ephem_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif

