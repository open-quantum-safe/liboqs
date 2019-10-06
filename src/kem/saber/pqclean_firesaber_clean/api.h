#ifndef PQCLEAN_FIRESABER_CLEAN_API_H
#define PQCLEAN_FIRESABER_CLEAN_API_H

#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_ALGNAME "FireSaber"
#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_SECRETKEYBYTES 3040
#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_PUBLICKEYBYTES (4*320+32)
#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_BYTES 32
#define PQCLEAN_FIRESABER_CLEAN_CRYPTO_CIPHERTEXTBYTES 1472

int PQCLEAN_FIRESABER_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_FIRESABER_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int PQCLEAN_FIRESABER_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif /* api_h */
