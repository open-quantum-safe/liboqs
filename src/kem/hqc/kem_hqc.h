// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_HQC_H
#define OQS_KEM_HQC_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_hqc_128
#define OQS_KEM_hqc_128_length_public_key 3024
#define OQS_KEM_hqc_128_length_secret_key 3064
#define OQS_KEM_hqc_128_length_ciphertext 6017
#define OQS_KEM_hqc_128_length_shared_secret 64
OQS_KEM *OQS_KEM_hqc_128_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_128_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_128_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_128_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_hqc_192
#define OQS_KEM_hqc_192_length_public_key 5690
#define OQS_KEM_hqc_192_length_secret_key 5730
#define OQS_KEM_hqc_192_length_ciphertext 11364
#define OQS_KEM_hqc_192_length_shared_secret 64
OQS_KEM *OQS_KEM_hqc_192_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_192_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_192_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_192_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_hqc_256
#define OQS_KEM_hqc_256_length_public_key 8698
#define OQS_KEM_hqc_256_length_secret_key 8738
#define OQS_KEM_hqc_256_length_ciphertext 17379
#define OQS_KEM_hqc_256_length_shared_secret 64
OQS_KEM *OQS_KEM_hqc_256_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_256_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_256_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_256_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_hqc_rmrs_128
#define OQS_KEM_hqc_rmrs_128_length_public_key 2607
#define OQS_KEM_hqc_rmrs_128_length_secret_key 2647
#define OQS_KEM_hqc_rmrs_128_length_ciphertext 5191
#define OQS_KEM_hqc_rmrs_128_length_shared_secret 64
OQS_KEM *OQS_KEM_hqc_rmrs_128_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_rmrs_128_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_rmrs_128_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_rmrs_128_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_hqc_rmrs_192
#define OQS_KEM_hqc_rmrs_192_length_public_key 4906
#define OQS_KEM_hqc_rmrs_192_length_secret_key 4946
#define OQS_KEM_hqc_rmrs_192_length_ciphertext 9794
#define OQS_KEM_hqc_rmrs_192_length_shared_secret 64
OQS_KEM *OQS_KEM_hqc_rmrs_192_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_rmrs_192_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_rmrs_192_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_rmrs_192_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_hqc_rmrs_256
#define OQS_KEM_hqc_rmrs_256_length_public_key 7535
#define OQS_KEM_hqc_rmrs_256_length_secret_key 7575
#define OQS_KEM_hqc_rmrs_256_length_ciphertext 15047
#define OQS_KEM_hqc_rmrs_256_length_shared_secret 64
OQS_KEM *OQS_KEM_hqc_rmrs_256_new(void);
OQS_API OQS_STATUS OQS_KEM_hqc_rmrs_256_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_hqc_rmrs_256_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_hqc_rmrs_256_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif

