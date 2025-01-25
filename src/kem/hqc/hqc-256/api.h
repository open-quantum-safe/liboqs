/**
 * @file api.h
 * @brief NIST KEM API used by the HQC_KEM IND-CCA2 scheme
 */

#ifndef HQC256_API_H
#define HQC256_API_H

#define HQC256_CRYPTO_ALGNAME                      "HQC-256"

#define HQC256_CRYPTO_SECRETKEYBYTES               7317
#define HQC256_CRYPTO_PUBLICKEYBYTES               7245
#define HQC256_CRYPTO_BYTES                        64
#define HQC256_CRYPTO_CIPHERTEXTBYTES              14421

// As a technicality, the public key is appended to the secret key in order to respect the NIST API.
// Without this constraint, CRYPTO_SECRETKEYBYTES would be defined as 32

int HQC256_crypto_kem_keypair(unsigned char* pk, unsigned char* sk);
int HQC256_crypto_kem_enc(unsigned char* ct, unsigned char* ss, const unsigned char* pk);
int HQC256_crypto_kem_dec(unsigned char* ss, const unsigned char* ct, const unsigned char* sk);

#endif
