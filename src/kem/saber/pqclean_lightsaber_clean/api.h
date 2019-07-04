#ifndef PQCLEAN_LIGHTSABER_CLEAN_API_H
#define PQCLEAN_LIGHTSABER_CLEAN_API_H

#define PQCLEAN_LIGHTSABER_CLEAN_CRYPTO_ALGNAME "LightSaber"
#define PQCLEAN_LIGHTSABER_CLEAN_CRYPTO_SECRETKEYBYTES 1568
#define PQCLEAN_LIGHTSABER_CLEAN_CRYPTO_PUBLICKEYBYTES (2*320+32)
#define PQCLEAN_LIGHTSABER_CLEAN_CRYPTO_BYTES 32
#define PQCLEAN_LIGHTSABER_CLEAN_CRYPTO_CIPHERTEXTBYTES 736

int PQCLEAN_LIGHTSABER_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_LIGHTSABER_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int PQCLEAN_LIGHTSABER_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif /* api_h */
