#ifndef __OQS_KEM_KYBER_H
#define __OQS_KEM_KYBER_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_kyber_512_cca_kem

#define OQS_KEM_kyber_512_cca_kem_length_secret_key 1632
#define OQS_KEM_kyber_512_cca_kem_length_public_key 736
#define OQS_KEM_kyber_512_cca_kem_length_ciphertext 800
#define OQS_KEM_kyber_512_cca_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_kyber_512_cca_kem_new();

OQS_API OQS_STATUS OQS_KEM_kyber_512_cca_kem_keypair(uint8_t *public_key,
                                                     uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_kyber_512_cca_kem_encaps(uint8_t *ciphertext,
                                                    uint8_t *shared_secret,
                                                    const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_kyber_512_cca_kem_decaps(uint8_t *shared_secret,
                                                    const unsigned char *ciphertext,
                                                    const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_kyber_768_cca_kem

#define OQS_KEM_kyber_768_cca_kem_length_secret_key 2400
#define OQS_KEM_kyber_768_cca_kem_length_public_key 1088
#define OQS_KEM_kyber_768_cca_kem_length_ciphertext 1152
#define OQS_KEM_kyber_768_cca_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_kyber_768_cca_kem_new();

OQS_API OQS_STATUS OQS_KEM_kyber_768_cca_kem_keypair(uint8_t *public_key,
                                                     uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_kyber_768_cca_kem_encaps(uint8_t *ciphertext,
                                                    uint8_t *shared_secret,
                                                    const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_kyber_768_cca_kem_decaps(uint8_t *shared_secret,
                                                    const unsigned char *ciphertext,
                                                    const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_kyber_1024_cca_kem

#define OQS_KEM_kyber_1024_cca_kem_length_secret_key 3168
#define OQS_KEM_kyber_1024_cca_kem_length_public_key 1440
#define OQS_KEM_kyber_1024_cca_kem_length_ciphertext 1504
#define OQS_KEM_kyber_1024_cca_kem_length_shared_secret 32

OQS_KEM *OQS_KEM_kyber_1024_cca_kem_new();

OQS_API OQS_STATUS OQS_KEM_kyber_1024_cca_kem_keypair(uint8_t *public_key,
                                                      uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_kyber_1024_cca_kem_encaps(uint8_t *ciphertext,
                                                     uint8_t *shared_secret,
                                                     const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_kyber_1024_cca_kem_decaps(uint8_t *shared_secret,
                                                     const unsigned char *ciphertext,
                                                     const uint8_t *secret_key);
#endif

#endif
