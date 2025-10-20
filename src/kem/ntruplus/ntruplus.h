// #ifndef NTRUPLUS_H
// #define NTRUPLUS_H

// #include <stdint.h>
// #include <stddef.h>

// #define NTRUPLUS_PUBLICKEYBYTES 1138
// #define NTRUPLUS_SECRETKEYBYTES 1450
// #define NTRUPLUS_CIPHERTEXTBYTES 1138
// #define NTRUPLUS_BYTES 32

// int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
// int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
// int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

// #endif

#ifndef NTRUPLUS_H
#define NTRUPLUS_H

#include <stdint.h>
#include <stddef.h>

// NTRU+ 768 파라미터
#define NTRUPLUS_N 768
#define NTRUPLUS_Q 3457

#define NTRUPLUS_SYMBYTES  32   /* 해시 및 시드의 바이트 크기 */
#define NTRUPLUS_SSBYTES   32   /* 공유 키의 바이트 크기 */
#define NTRUPLUS_POLYBYTES 1152

#define NTRUPLUS_PUBLICKEYBYTES  NTRUPLUS_POLYBYTES
#define NTRUPLUS_SECRETKEYBYTES  ((NTRUPLUS_POLYBYTES << 1) + NTRUPLUS_SYMBYTES)
#define NTRUPLUS_CIPHERTEXTBYTES  NTRUPLUS_POLYBYTES

// liboqc 호환성을 위한 매크로
#define CRYPTO_SECRETKEYBYTES  NTRUPLUS_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES  NTRUPLUS_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES NTRUPLUS_CIPHERTEXTBYTES
#define CRYPTO_BYTES           NTRUPLUS_SSBYTES

#define CRYPTO_ALGNAME "NTRU+KEM768"

// liboqc 통합용 API 함수들
int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif /* NTRUPLUS_H */
