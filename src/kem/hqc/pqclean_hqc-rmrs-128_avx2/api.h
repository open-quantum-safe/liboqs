#ifndef PQCLEAN_HQCRMRS128_AVX2_API_H
#define PQCLEAN_HQCRMRS128_AVX2_API_H
/**
 * @file api.h
 * @brief NIST KEM API used by the HQC_KEM IND-CCA2 scheme
 */

#define PQCLEAN_HQCRMRS128_AVX2_CRYPTO_ALGNAME                      "HQC-RMRS-128"

#define PQCLEAN_HQCRMRS128_AVX2_CRYPTO_SECRETKEYBYTES               2289
#define PQCLEAN_HQCRMRS128_AVX2_CRYPTO_PUBLICKEYBYTES               2249
#define PQCLEAN_HQCRMRS128_AVX2_CRYPTO_BYTES                        64
#define PQCLEAN_HQCRMRS128_AVX2_CRYPTO_CIPHERTEXTBYTES              4481

// As a technicality, the public key is appended to the secret key in order to respect the NIST API.
// Without this constraint, PQCLEAN_HQCRMRS128_AVX2_CRYPTO_SECRETKEYBYTES would be defined as 32

int PQCLEAN_HQCRMRS128_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_HQCRMRS128_AVX2_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int PQCLEAN_HQCRMRS128_AVX2_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);


#endif
