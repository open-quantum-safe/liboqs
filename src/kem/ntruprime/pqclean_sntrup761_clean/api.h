#ifndef PQCLEAN_SNTRUP761_CLEAN_API_H
#define PQCLEAN_SNTRUP761_CLEAN_API_H



#define PQCLEAN_SNTRUP761_CLEAN_CRYPTO_ALGNAME "sntrup761"

#define PQCLEAN_SNTRUP761_CLEAN_CRYPTO_SECRETKEYBYTES 1763
#define PQCLEAN_SNTRUP761_CLEAN_CRYPTO_PUBLICKEYBYTES 1158
#define PQCLEAN_SNTRUP761_CLEAN_CRYPTO_CIPHERTEXTBYTES 1039
#define PQCLEAN_SNTRUP761_CLEAN_CRYPTO_BYTES 32

int PQCLEAN_SNTRUP761_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_SNTRUP761_CLEAN_crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk);
int PQCLEAN_SNTRUP761_CLEAN_crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk);
#endif
