#ifndef __OQS_KEM_THREEBEARS_H
#define __OQS_KEM_THREEBEARS_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_babybear_cca

#define OQS_KEM_babybear_cca_length_public_key 804
#define OQS_KEM_babybear_cca_length_secret_key 40
#define OQS_KEM_babybear_cca_length_ciphertext 917
#define OQS_KEM_babybear_cca_length_shared_secret 32

OQS_KEM *OQS_KEM_babybear_cca_new();

OQS_STATUS OQS_KEM_babybear_cca_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_babybear_cca_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_babybear_cca_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_mamabear_cca

#define OQS_KEM_mamabear_cca_length_public_key 1194
#define OQS_KEM_mamabear_cca_length_secret_key 40
#define OQS_KEM_mamabear_cca_length_ciphertext 1307
#define OQS_KEM_mamabear_cca_length_shared_secret 32

OQS_KEM *OQS_KEM_mamabear_cca_new();

OQS_STATUS OQS_KEM_mamabear_cca_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_mamabear_cca_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_mamabear_cca_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_babybear_cca

#define OQS_KEM_papabear_cca_length_public_key 1584
#define OQS_KEM_papabear_cca_length_secret_key 40
#define OQS_KEM_papabear_cca_length_ciphertext 1697
#define OQS_KEM_papabear_cca_length_shared_secret 32

OQS_KEM *OQS_KEM_papabear_cca_new();

OQS_STATUS OQS_KEM_papabear_cca_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_papabear_cca_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_papabear_cca_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_babybear_ephem_cpa

#define OQS_KEM_babybear_ephem_cpa_length_public_key 804
#define OQS_KEM_babybear_ephem_cpa_length_secret_key 40
#define OQS_KEM_babybear_ephem_cpa_length_ciphertext 917
#define OQS_KEM_babybear_ephem_cpa_length_shared_secret 32

OQS_KEM *OQS_KEM_babybear_ephem_cpa_new();

OQS_STATUS OQS_KEM_babybear_ephem_cpa_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_babybear_ephem_cpa_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_babybear_ephem_cpa_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_mamabear_ephem_cpa

#define OQS_KEM_mamabear_ephem_cpa_length_public_key 1194
#define OQS_KEM_mamabear_ephem_cpa_length_secret_key 40
#define OQS_KEM_mamabear_ephem_cpa_length_ciphertext 1307
#define OQS_KEM_mamabear_ephem_cpa_length_shared_secret 32

OQS_KEM *OQS_KEM_mamabear_ephem_cpa_new();

OQS_STATUS OQS_KEM_mamabear_ephem_cpa_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_mamabear_ephem_cpa_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_mamabear_ephem_cpa_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#ifdef OQS_ENABLE_KEM_babybear_ephem_cpa

#define OQS_KEM_papabear_ephem_cpa_length_public_key 1584
#define OQS_KEM_papabear_ephem_cpa_length_secret_key 40
#define OQS_KEM_papabear_ephem_cpa_length_ciphertext 1697
#define OQS_KEM_papabear_ephem_cpa_length_shared_secret 32

OQS_KEM *OQS_KEM_papabear_ephem_cpa_new();

OQS_STATUS OQS_KEM_papabear_ephem_cpa_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_STATUS OQS_KEM_papabear_ephem_cpa_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_STATUS OQS_KEM_papabear_ephem_cpa_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

#endif

#endif
