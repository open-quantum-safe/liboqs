#ifndef PQCLEAN_SNTRUP653_AVX2_API_H
#define PQCLEAN_SNTRUP653_AVX2_API_H



#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_ALGNAME "sntrup653"

#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_SECRETKEYBYTES 1518
#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_PUBLICKEYBYTES 994
#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_CIPHERTEXTBYTES 897
#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_BYTES 32

int PQCLEAN_SNTRUP653_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SNTRUP653_AVX2_crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk);
int PQCLEAN_SNTRUP653_AVX2_crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk);
#endif
