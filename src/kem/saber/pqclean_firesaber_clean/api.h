#ifndef PQCLEAN_FIRESABER_CLEAN_API_H
#define PQCLEAN_FIRESABER_CLEAN_API_H


#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_ALGNAME "FireSaber"
#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_BYTES 32
#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_CIPHERTEXTBYTES 1472
#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_PUBLICKEYBYTES 1312
#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_SECRETKEYBYTES 3040

int PQCLEAN_FIRESABER_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_FIRESABER_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *k, const unsigned char *pk);

int PQCLEAN_FIRESABER_CLEAN_crypto_kem_dec(unsigned char *k, const unsigned char *ct, const unsigned char *sk);


#endif /* PQCLEAN_FIRESABER_CLEAN_API_H */
