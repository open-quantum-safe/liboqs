#ifndef PQCLEAN_NTRULPR857_CLEAN_API_H
#define PQCLEAN_NTRULPR857_CLEAN_API_H



#define PQCLEAN_NTRULPR857_CLEAN_CRYPTO_ALGNAME "ntrulpr857"

#define PQCLEAN_NTRULPR857_CLEAN_CRYPTO_SECRETKEYBYTES 1463
#define PQCLEAN_NTRULPR857_CLEAN_CRYPTO_PUBLICKEYBYTES 1184
#define PQCLEAN_NTRULPR857_CLEAN_CRYPTO_CIPHERTEXTBYTES 1312
#define PQCLEAN_NTRULPR857_CLEAN_CRYPTO_BYTES 32

int PQCLEAN_NTRULPR857_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_NTRULPR857_CLEAN_crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk);
int PQCLEAN_NTRULPR857_CLEAN_crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk);
#endif
