// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_NTRUPRIME_H
#define OQS_KEM_NTRUPRIME_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_ntruprime_ntrulpr653
#define OQS_KEM_ntruprime_ntrulpr653_length_public_key 897
#define OQS_KEM_ntruprime_ntrulpr653_length_secret_key 1125
#define OQS_KEM_ntruprime_ntrulpr653_length_ciphertext 1025
#define OQS_KEM_ntruprime_ntrulpr653_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_ntrulpr653_new(void);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr653_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr653_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr653_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntruprime_ntrulpr761
#define OQS_KEM_ntruprime_ntrulpr761_length_public_key 1039
#define OQS_KEM_ntruprime_ntrulpr761_length_secret_key 1294
#define OQS_KEM_ntruprime_ntrulpr761_length_ciphertext 1167
#define OQS_KEM_ntruprime_ntrulpr761_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_ntrulpr761_new(void);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr761_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr761_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr761_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntruprime_ntrulpr857
#define OQS_KEM_ntruprime_ntrulpr857_length_public_key 1184
#define OQS_KEM_ntruprime_ntrulpr857_length_secret_key 1463
#define OQS_KEM_ntruprime_ntrulpr857_length_ciphertext 1312
#define OQS_KEM_ntruprime_ntrulpr857_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_ntrulpr857_new(void);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr857_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr857_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr857_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntruprime_ntrulpr1277
#define OQS_KEM_ntruprime_ntrulpr1277_length_public_key 1847
#define OQS_KEM_ntruprime_ntrulpr1277_length_secret_key 2231
#define OQS_KEM_ntruprime_ntrulpr1277_length_ciphertext 1975
#define OQS_KEM_ntruprime_ntrulpr1277_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_ntrulpr1277_new(void);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr1277_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr1277_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_ntrulpr1277_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntruprime_sntrup653
#define OQS_KEM_ntruprime_sntrup653_length_public_key 994
#define OQS_KEM_ntruprime_sntrup653_length_secret_key 1518
#define OQS_KEM_ntruprime_sntrup653_length_ciphertext 897
#define OQS_KEM_ntruprime_sntrup653_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_sntrup653_new(void);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup653_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup653_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup653_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntruprime_sntrup761
#define OQS_KEM_ntruprime_sntrup761_length_public_key 1158
#define OQS_KEM_ntruprime_sntrup761_length_secret_key 1763
#define OQS_KEM_ntruprime_sntrup761_length_ciphertext 1039
#define OQS_KEM_ntruprime_sntrup761_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_sntrup761_new(void);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup761_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup761_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup761_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntruprime_sntrup857
#define OQS_KEM_ntruprime_sntrup857_length_public_key 1322
#define OQS_KEM_ntruprime_sntrup857_length_secret_key 1999
#define OQS_KEM_ntruprime_sntrup857_length_ciphertext 1184
#define OQS_KEM_ntruprime_sntrup857_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_sntrup857_new(void);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup857_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup857_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup857_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntruprime_sntrup1277
#define OQS_KEM_ntruprime_sntrup1277_length_public_key 2067
#define OQS_KEM_ntruprime_sntrup1277_length_secret_key 3059
#define OQS_KEM_ntruprime_sntrup1277_length_ciphertext 1847
#define OQS_KEM_ntruprime_sntrup1277_length_shared_secret 32
OQS_KEM *OQS_KEM_ntruprime_sntrup1277_new(void);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup1277_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup1277_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntruprime_sntrup1277_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#endif

