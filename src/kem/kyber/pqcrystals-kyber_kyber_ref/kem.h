#ifndef KEM_H
#define KEM_H

#include <stdint.h>
#include "params.h"

#define CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define CRYPTO_BYTES           KYBER_SSBYTES

// #if   (KYBER_K == 2)
// #ifdef KYBER_90S
// #define CRYPTO_ALGNAME "Kyber512-90s"
// #else
// #define CRYPTO_ALGNAME "Kyber512"
// #endif
// #elif (KYBER_K == 3)
// #ifdef KYBER_90S
// #define CRYPTO_ALGNAME "Kyber768-90s"
// #else
// #define CRYPTO_ALGNAME "Kyber768"
// #endif
// #elif (KYBER_K == 4)
// #ifdef KYBER_90S
// #define CRYPTO_ALGNAME "Kyber1024-90s"
// #else
// #define CRYPTO_ALGNAME "Kyber1024"
// #endif
// #endif

#define crypto_kem_keypair_512 KYBER_NAMESPACE_512(keypair)
int crypto_kem_keypair_512(uint8_t *pk, uint8_t *sk);
#define crypto_kem_keypair_768 KYBER_NAMESPACE_768(keypair)
int crypto_kem_keypair_768(uint8_t *pk, uint8_t *sk);
#define crypto_kem_keypair_1024 KYBER_NAMESPACE_1024(keypair)
int crypto_kem_keypair_1024(uint8_t *pk, uint8_t *sk);


#define crypto_kem_enc_512 KYBER_NAMESPACE_512(enc)
int crypto_kem_enc_512(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
#define crypto_kem_enc_768 KYBER_NAMESPACE_768(enc)
int crypto_kem_enc_768(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
#define crypto_kem_enc_1024 KYBER_NAMESPACE_1024(enc)
int crypto_kem_enc_1024(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

#define crypto_kem_dec_512 KYBER_NAMESPACE_512(dec)
int crypto_kem_dec_512(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#define crypto_kem_dec_768 KYBER_NAMESPACE_768(dec)
int crypto_kem_dec_768(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#define crypto_kem_dec_1024 KYBER_NAMESPACE_1024(dec)
int crypto_kem_dec_1024(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);


#endif
