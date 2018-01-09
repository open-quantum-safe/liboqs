#ifndef API_H
#define API_H

#include "params.h"

#define CRYPTO_SECRETKEYBYTES  NEWHOPE_CCAKEM_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES  NEWHOPE_CCAKEM_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES NEWHOPE_CCAKEM_CIPHERTEXTBYTES
#define CRYPTO_BYTES           NEWHOPE_SYMBYTES

#if   (NEWHOPE_N == 512)
#define CRYPTO_ALGNAME "NewHope512-CCAKEM"
#elif (NEWHOPE_N == 1024)
#define CRYPTO_ALGNAME "NewHope1024-CCAKEM"
#else
#error "NEWHOPE_N must be either 512 or 1024"
#endif

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
