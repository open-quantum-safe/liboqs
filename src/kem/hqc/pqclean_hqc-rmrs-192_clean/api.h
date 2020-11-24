#ifndef PQCLEAN_HQCRMRS192_CLEAN_API_H
#define PQCLEAN_HQCRMRS192_CLEAN_API_H
/**
 * @file api.h
 * @brief NIST KEM API used by the HQC_KEM IND-CCA2 scheme
 */

#define PQCLEAN_HQCRMRS192_CLEAN_CRYPTO_ALGNAME                      "HQC-RMRS-192"

#define PQCLEAN_HQCRMRS192_CLEAN_CRYPTO_SECRETKEYBYTES               4562
#define PQCLEAN_HQCRMRS192_CLEAN_CRYPTO_PUBLICKEYBYTES               4522
#define PQCLEAN_HQCRMRS192_CLEAN_CRYPTO_BYTES                        64
#define PQCLEAN_HQCRMRS192_CLEAN_CRYPTO_CIPHERTEXTBYTES              9026

// As a technicality, the public key is appended to the secret key in order to respect the NIST API.
// Without this constraint, PQCLEAN_HQCRMRS192_CLEAN_CRYPTO_SECRETKEYBYTES would be defined as 32

int PQCLEAN_HQCRMRS192_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_HQCRMRS192_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int PQCLEAN_HQCRMRS192_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);


#endif
