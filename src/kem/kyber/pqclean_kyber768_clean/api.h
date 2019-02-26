#ifndef API_H
#define API_H

#include "params.h"

#define CRYPTO_SECRETKEYBYTES KYBER_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES KYBER_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define CRYPTO_BYTES KYBER_SYMBYTES

#define CRYPTO_ALGNAME "Kyber768"

int PQCLEAN_KYBER768_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_KYBER768_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int PQCLEAN_KYBER768_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
