#ifndef PQCLEAN_NTRULPR653_CLEAN_API_H
#define PQCLEAN_NTRULPR653_CLEAN_API_H



#define PQCLEAN_NTRULPR653_CLEAN_CRYPTO_ALGNAME "ntrulpr653"

#define PQCLEAN_NTRULPR653_CLEAN_CRYPTO_SECRETKEYBYTES 1125
#define PQCLEAN_NTRULPR653_CLEAN_CRYPTO_PUBLICKEYBYTES 897
#define PQCLEAN_NTRULPR653_CLEAN_CRYPTO_CIPHERTEXTBYTES 1025
#define PQCLEAN_NTRULPR653_CLEAN_CRYPTO_BYTES 32

int PQCLEAN_NTRULPR653_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_NTRULPR653_CLEAN_crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk);
int PQCLEAN_NTRULPR653_CLEAN_crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk);
#endif
