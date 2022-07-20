#ifndef API_H
#define API_H

#include <stdint.h>

#define pqcrystals_kyber512_SECRETKEYBYTES 1632
#define pqcrystals_kyber512_PUBLICKEYBYTES 800
#define pqcrystals_kyber512_CIPHERTEXTBYTES 768
#define pqcrystals_kyber512_BYTES 32

#define pqcrystals_kyber512_ref_SECRETKEYBYTES pqcrystals_kyber512_SECRETKEYBYTES
#define pqcrystals_kyber512_ref_PUBLICKEYBYTES pqcrystals_kyber512_PUBLICKEYBYTES
#define pqcrystals_kyber512_ref_CIPHERTEXTBYTES pqcrystals_kyber512_CIPHERTEXTBYTES
#define pqcrystals_kyber512_ref_BYTES pqcrystals_kyber512_BYTES

int pqcrystals_kyber512_ref_keypair(uint8_t *pk, uint8_t *sk);
int pqcrystals_kyber512_ref_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int pqcrystals_kyber512_ref_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#define pqcrystals_kyber512_90s_ref_SECRETKEYBYTES pqcrystals_kyber512_SECRETKEYBYTES
#define pqcrystals_kyber512_90s_ref_PUBLICKEYBYTES pqcrystals_kyber512_PUBLICKEYBYTES
#define pqcrystals_kyber512_90s_ref_CIPHERTEXTBYTES pqcrystals_kyber512_CIPHERTEXTBYTES
#define pqcrystals_kyber512_90s_ref_BYTES pqcrystals_kyber512_BYTES

int pqcrystals_kyber512_90s_ref_keypair(uint8_t *pk, uint8_t *sk);
int pqcrystals_kyber512_90s_ref_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int pqcrystals_kyber512_90s_ref_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#define pqcrystals_kyber768_SECRETKEYBYTES 2400
#define pqcrystals_kyber768_PUBLICKEYBYTES 1184
#define pqcrystals_kyber768_CIPHERTEXTBYTES 1088
#define pqcrystals_kyber768_BYTES 32

#define pqcrystals_kyber768_ref_SECRETKEYBYTES pqcrystals_kyber768_SECRETKEYBYTES
#define pqcrystals_kyber768_ref_PUBLICKEYBYTES pqcrystals_kyber768_PUBLICKEYBYTES
#define pqcrystals_kyber768_ref_CIPHERTEXTBYTES pqcrystals_kyber768_CIPHERTEXTBYTES
#define pqcrystals_kyber768_ref_BYTES pqcrystals_kyber768_BYTES

int pqcrystals_kyber768_ref_keypair(uint8_t *pk, uint8_t *sk);
int pqcrystals_kyber768_ref_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int pqcrystals_kyber768_ref_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#define pqcrystals_kyber768_90s_ref_SECRETKEYBYTES pqcrystals_kyber768_SECRETKEYBYTES
#define pqcrystals_kyber768_90s_ref_PUBLICKEYBYTES pqcrystals_kyber768_PUBLICKEYBYTES
#define pqcrystals_kyber768_90s_ref_CIPHERTEXTBYTES pqcrystals_kyber768_CIPHERTEXTBYTES
#define pqcrystals_kyber768_90s_ref_BYTES pqcrystals_kyber768_BYTES

int pqcrystals_kyber768_90s_ref_keypair(uint8_t *pk, uint8_t *sk);
int pqcrystals_kyber768_90s_ref_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int pqcrystals_kyber768_90s_ref_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#define pqcrystals_kyber1024_SECRETKEYBYTES 3168
#define pqcrystals_kyber1024_PUBLICKEYBYTES 1568
#define pqcrystals_kyber1024_CIPHERTEXTBYTES 1568
#define pqcrystals_kyber1024_BYTES 32

#define pqcrystals_kyber1024_ref_SECRETKEYBYTES pqcrystals_kyber1024_SECRETKEYBYTES
#define pqcrystals_kyber1024_ref_PUBLICKEYBYTES pqcrystals_kyber1024_PUBLICKEYBYTES
#define pqcrystals_kyber1024_ref_CIPHERTEXTBYTES pqcrystals_kyber1024_CIPHERTEXTBYTES
#define pqcrystals_kyber1024_ref_BYTES pqcrystals_kyber1024_BYTES

int pqcrystals_kyber1024_ref_keypair(uint8_t *pk, uint8_t *sk);
int pqcrystals_kyber1024_ref_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int pqcrystals_kyber1024_ref_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#define pqcrystals_kyber1024_90s_ref_SECRETKEYBYTES pqcrystals_kyber1024_SECRETKEYBYTES
#define pqcrystals_kyber1024_90s_ref_PUBLICKEYBYTES pqcrystals_kyber1024_PUBLICKEYBYTES
#define pqcrystals_kyber1024_90s_ref_CIPHERTEXTBYTES pqcrystals_kyber1024_CIPHERTEXTBYTES
#define pqcrystals_kyber1024_90s_ref_BYTES pqcrystals_kyber1024_BYTES

int pqcrystals_kyber1024_90s_ref_keypair(uint8_t *pk, uint8_t *sk);
int pqcrystals_kyber1024_90s_ref_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int pqcrystals_kyber1024_90s_ref_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
