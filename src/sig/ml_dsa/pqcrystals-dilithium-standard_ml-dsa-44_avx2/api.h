#ifndef API_H
#define API_H

#include <stddef.h>
#include <stdint.h>

#define pqcrystals_dilithium2_PUBLICKEYBYTES 1312
#define pqcrystals_dilithium2_SECRETKEYBYTES 2560
#define pqcrystals_dilithium2_BYTES 2420

#define pqcrystals_dilithium2_avx2_PUBLICKEYBYTES pqcrystals_dilithium2_PUBLICKEYBYTES
#define pqcrystals_dilithium2_avx2_SECRETKEYBYTES pqcrystals_dilithium2_SECRETKEYBYTES
#define pqcrystals_dilithium2_avx2_BYTES pqcrystals_dilithium2_BYTES

int pqcrystals_dilithium2_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium2_avx2_signature(uint8_t *sig, size_t *siglen,
                                         const uint8_t *m, size_t mlen,
                                         const uint8_t *ctx, size_t ctxlen,
                                         const uint8_t *sk);

int pqcrystals_dilithium2_avx2(uint8_t *sm, size_t *smlen,
                               const uint8_t *m, size_t mlen,
                               const uint8_t *ctx, size_t ctxlen,
                               const uint8_t *sk);

int pqcrystals_dilithium2_avx2_verify(const uint8_t *sig, size_t siglen,
                                      const uint8_t *m, size_t mlen,
                                      const uint8_t *ctx, size_t ctxlen,
                                      const uint8_t *pk);

int pqcrystals_dilithium2_avx2_open(uint8_t *m, size_t *mlen,
                                    const uint8_t *sm, size_t smlen,
                                    const uint8_t *ctx, size_t ctxlen,
                                    const uint8_t *pk);


#define pqcrystals_dilithium3_PUBLICKEYBYTES 1952
#define pqcrystals_dilithium3_SECRETKEYBYTES 4032
#define pqcrystals_dilithium3_BYTES 3309

#define pqcrystals_dilithium3_avx2_PUBLICKEYBYTES pqcrystals_dilithium3_PUBLICKEYBYTES
#define pqcrystals_dilithium3_avx2_SECRETKEYBYTES pqcrystals_dilithium3_SECRETKEYBYTES
#define pqcrystals_dilithium3_avx2_BYTES pqcrystals_dilithium3_BYTES

int pqcrystals_dilithium3_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium3_avx2_signature(uint8_t *sig, size_t *siglen,
                                         const uint8_t *m, size_t mlen,
                                         const uint8_t *ctx, size_t ctxlen,
                                         const uint8_t *sk);

int pqcrystals_dilithium3_avx2(uint8_t *sm, size_t *smlen,
                               const uint8_t *m, size_t mlen,
                               const uint8_t *ctx, size_t ctxlen,
                               const uint8_t *sk);

int pqcrystals_dilithium3_avx2_verify(const uint8_t *sig, size_t siglen,
                                      const uint8_t *m, size_t mlen,
                                      const uint8_t *ctx, size_t ctxlen,
                                      const uint8_t *pk);

int pqcrystals_dilithium3_avx2_open(uint8_t *m, size_t *mlen,
                                    const uint8_t *sm, size_t smlen,
                                    const uint8_t *ctx, size_t ctxlen,
                                    const uint8_t *pk);


#define pqcrystals_dilithium5_PUBLICKEYBYTES 2592
#define pqcrystals_dilithium5_SECRETKEYBYTES 4896
#define pqcrystals_dilithium5_BYTES 4627

#define pqcrystals_dilithium5_avx2_PUBLICKEYBYTES pqcrystals_dilithium5_PUBLICKEYBYTES
#define pqcrystals_dilithium5_avx2_SECRETKEYBYTES pqcrystals_dilithium5_SECRETKEYBYTES
#define pqcrystals_dilithium5_avx2_BYTES pqcrystals_dilithium5_BYTES

int pqcrystals_dilithium5_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium5_avx2_signature(uint8_t *sig, size_t *siglen,
                                         const uint8_t *m, size_t mlen,
                                         const uint8_t *ctx, size_t ctxlen,
                                         const uint8_t *sk);

int pqcrystals_dilithium5_avx2(uint8_t *sm, size_t *smlen,
                               const uint8_t *m, size_t mlen,
                               const uint8_t *ctx, size_t ctxlen,
                               const uint8_t *sk);

int pqcrystals_dilithium5_avx2_verify(const uint8_t *sig, size_t siglen,
                                      const uint8_t *m, size_t mlen,
                                      const uint8_t *ctx, size_t ctxlen,
                                      const uint8_t *pk);

int pqcrystals_dilithium5_avx2_open(uint8_t *m, size_t *mlen,
                                    const uint8_t *sm, size_t smlen,
                                    const uint8_t *ctx, size_t ctxlen,
                                    const uint8_t *pk);


#endif
