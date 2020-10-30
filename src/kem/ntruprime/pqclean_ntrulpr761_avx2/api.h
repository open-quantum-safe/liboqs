#ifndef PQCLEAN_NTRULPR761_AVX2_API_H
#define PQCLEAN_NTRULPR761_AVX2_API_H



#define PQCLEAN_NTRULPR761_AVX2_CRYPTO_ALGNAME "ntrulpr761"

#define PQCLEAN_NTRULPR761_AVX2_CRYPTO_SECRETKEYBYTES 1294
#define PQCLEAN_NTRULPR761_AVX2_CRYPTO_PUBLICKEYBYTES 1039
#define PQCLEAN_NTRULPR761_AVX2_CRYPTO_CIPHERTEXTBYTES 1167
#define PQCLEAN_NTRULPR761_AVX2_CRYPTO_BYTES 32

int PQCLEAN_NTRULPR761_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_NTRULPR761_AVX2_crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk);
int PQCLEAN_NTRULPR761_AVX2_crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk);
#endif
