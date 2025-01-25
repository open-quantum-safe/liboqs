/**
 * @file api.h
 * @brief NIST KEM API used by the HQC_KEM IND-CCA2 scheme
 */

#ifndef HQC192_API_H
#define HQC192_API_H

#define HQC192_CRYPTO_ALGNAME                      "HQC-192"

#define HQC192_CRYPTO_SECRETKEYBYTES               4586
#define HQC192_CRYPTO_PUBLICKEYBYTES               4522
#define HQC192_CRYPTO_BYTES                        64
#define HQC192_CRYPTO_CIPHERTEXTBYTES              8978

// As a technicality, the public key is appended to the secret key in order to respect the NIST API.
// Without this constraint, CRYPTO_SECRETKEYBYTES would be defined as 32

int HQC192_crypto_kem_keypair(unsigned char* pk, unsigned char* sk);
int HQC192_crypto_kem_enc(unsigned char* ct, unsigned char* ss, const unsigned char* pk);
int HQC192_crypto_kem_dec(unsigned char* ss, const unsigned char* ct, const unsigned char* sk);

#endif
