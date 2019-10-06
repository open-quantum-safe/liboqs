#ifndef PQCLEAN_SABER_CLEAN_API_H
#define PQCLEAN_SABER_CLEAN_API_H

#define PQCLEAN_SABER_CLEAN_CRYPTO_ALGNAME "Saber"
#define PQCLEAN_SABER_CLEAN_CRYPTO_SECRETKEYBYTES 2304
#define PQCLEAN_SABER_CLEAN_CRYPTO_PUBLICKEYBYTES (3*320+32)
#define PQCLEAN_SABER_CLEAN_CRYPTO_BYTES 32
#define PQCLEAN_SABER_CLEAN_CRYPTO_CIPHERTEXTBYTES 1088

int PQCLEAN_SABER_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SABER_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int PQCLEAN_SABER_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif /* api_h */
