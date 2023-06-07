#ifndef PQCLEAN_KYBER1024_AARCH64_API_H
#define PQCLEAN_KYBER1024_AARCH64_API_H

#include <stdint.h>


#define PQCLEAN_KYBER1024_AARCH64_CRYPTO_SECRETKEYBYTES  3168
#define PQCLEAN_KYBER1024_AARCH64_CRYPTO_PUBLICKEYBYTES  1568
#define PQCLEAN_KYBER1024_AARCH64_CRYPTO_CIPHERTEXTBYTES 1568
#define PQCLEAN_KYBER1024_AARCH64_CRYPTO_BYTES           32
#define PQCLEAN_KYBER1024_AARCH64_CRYPTO_ALGNAME "Kyber1024"

int PQCLEAN_KYBER1024_AARCH64_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

int PQCLEAN_KYBER1024_AARCH64_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

int PQCLEAN_KYBER1024_AARCH64_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#ifdef OQS_HAZARDOUS_ENABLE_DERIVE_KEYPAIR
int PQCLEAN_KYBER1024_AARCH64_crypto_kem_derive_keypair(const uint8_t *randomness, uint8_t *pk, uint8_t *sk);
#endif

#endif
