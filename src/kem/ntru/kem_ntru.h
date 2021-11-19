// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_NTRU_H
#define OQS_KEM_NTRU_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_ntru_hps2048509
#define OQS_KEM_ntru_hps2048509_length_public_key 699
#define OQS_KEM_ntru_hps2048509_length_secret_key 935
#define OQS_KEM_ntru_hps2048509_length_ciphertext 699
#define OQS_KEM_ntru_hps2048509_length_shared_secret 32
OQS_KEM *OQS_KEM_ntru_hps2048509_new(void);
OQS_API OQS_STATUS OQS_KEM_ntru_hps2048509_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hps2048509_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hps2048509_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntru_hps2048677
#define OQS_KEM_ntru_hps2048677_length_public_key 930
#define OQS_KEM_ntru_hps2048677_length_secret_key 1234
#define OQS_KEM_ntru_hps2048677_length_ciphertext 930
#define OQS_KEM_ntru_hps2048677_length_shared_secret 32
OQS_KEM *OQS_KEM_ntru_hps2048677_new(void);
OQS_API OQS_STATUS OQS_KEM_ntru_hps2048677_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hps2048677_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hps2048677_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntru_hps4096821
#define OQS_KEM_ntru_hps4096821_length_public_key 1230
#define OQS_KEM_ntru_hps4096821_length_secret_key 1590
#define OQS_KEM_ntru_hps4096821_length_ciphertext 1230
#define OQS_KEM_ntru_hps4096821_length_shared_secret 32
OQS_KEM *OQS_KEM_ntru_hps4096821_new(void);
OQS_API OQS_STATUS OQS_KEM_ntru_hps4096821_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hps4096821_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hps4096821_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntru_hps40961229
#define OQS_KEM_ntru_hps40961229_length_public_key 1842
#define OQS_KEM_ntru_hps40961229_length_secret_key 2366
#define OQS_KEM_ntru_hps40961229_length_ciphertext 1842
#define OQS_KEM_ntru_hps40961229_length_shared_secret 32
OQS_KEM *OQS_KEM_ntru_hps40961229_new(void);
OQS_API OQS_STATUS OQS_KEM_ntru_hps40961229_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hps40961229_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hps40961229_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntru_hrss701
#define OQS_KEM_ntru_hrss701_length_public_key 1138
#define OQS_KEM_ntru_hrss701_length_secret_key 1450
#define OQS_KEM_ntru_hrss701_length_ciphertext 1138
#define OQS_KEM_ntru_hrss701_length_shared_secret 32
OQS_KEM *OQS_KEM_ntru_hrss701_new(void);
OQS_API OQS_STATUS OQS_KEM_ntru_hrss701_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hrss701_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hrss701_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_ntru_hrss1373
#define OQS_KEM_ntru_hrss1373_length_public_key 2401
#define OQS_KEM_ntru_hrss1373_length_secret_key 2983
#define OQS_KEM_ntru_hrss1373_length_ciphertext 2401
#define OQS_KEM_ntru_hrss1373_length_shared_secret 32
OQS_KEM *OQS_KEM_ntru_hrss1373_new(void);
OQS_API OQS_STATUS OQS_KEM_ntru_hrss1373_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hrss1373_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_ntru_hrss1373_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);
#endif

#endif

