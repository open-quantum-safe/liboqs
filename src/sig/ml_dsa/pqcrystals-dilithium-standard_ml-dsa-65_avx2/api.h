#ifndef API_H
#define API_H

#include <stddef.h>
#include <stdint.h>

#define pqcrystals_ml_dsa_44_PUBLICKEYBYTES 1312
#define pqcrystals_ml_dsa_44_SECRETKEYBYTES 2560
#define pqcrystals_ml_dsa_44_BYTES 2420

#define pqcrystals_ml_dsa_44_avx2_PUBLICKEYBYTES pqcrystals_ml_dsa_44_PUBLICKEYBYTES
#define pqcrystals_ml_dsa_44_avx2_SECRETKEYBYTES pqcrystals_ml_dsa_44_SECRETKEYBYTES
#define pqcrystals_ml_dsa_44_avx2_BYTES pqcrystals_ml_dsa_44_BYTES

int pqcrystals_ml_dsa_44_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_ml_dsa_44_avx2_signature_ctx(uint8_t *sig, size_t *siglen,
                                          const uint8_t *m, size_t mlen,
                                          const uint8_t *ctx, size_t ctxlen,
                                          const uint8_t *sk);

int pqcrystals_ml_dsa_44_avx2_signature(uint8_t *sig, size_t *siglen,
                                       const uint8_t *m, size_t mlen,
                                       const uint8_t *sk);

int pqcrystals_ml_dsa_44_avx2_ctx(uint8_t *sm, size_t *smlen,
                                 const uint8_t *m, size_t mlen,
                                 const uint8_t *ctx, size_t ctxlen,
                                 const uint8_t *sk);

int pqcrystals_ml_dsa_44_avx2(uint8_t *sm, size_t *smlen,
                             const uint8_t *m, size_t mlen,
                             const uint8_t *sk);

int pqcrystals_ml_dsa_44_avx2_verify_ctx(const uint8_t *sig, size_t siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *ctx, size_t ctxlen,
                                        const uint8_t *pk);

int pqcrystals_ml_dsa_44_avx2_verify(const uint8_t *sig, size_t siglen,
                                    const uint8_t *m, size_t mlen,
                                    const uint8_t *pk);

int pqcrystals_ml_dsa_44_avx2_open_ctx(uint8_t *m, size_t *mlen,
                                      const uint8_t *sm, size_t smlen,
                                      const uint8_t *ctx, size_t ctxlen,
                                      const uint8_t *pk);

int pqcrystals_ml_dsa_44_avx2_open(uint8_t *m, size_t *mlen,
                                  const uint8_t *sm, size_t smlen,
                                  const uint8_t *pk);

#define pqcrystals_ml_dsa_65_PUBLICKEYBYTES 1952
#define pqcrystals_ml_dsa_65_SECRETKEYBYTES 4032
#define pqcrystals_ml_dsa_65_BYTES 3309

#define pqcrystals_ml_dsa_65_avx2_PUBLICKEYBYTES pqcrystals_ml_dsa_65_PUBLICKEYBYTES
#define pqcrystals_ml_dsa_65_avx2_SECRETKEYBYTES pqcrystals_ml_dsa_65_SECRETKEYBYTES
#define pqcrystals_ml_dsa_65_avx2_BYTES pqcrystals_ml_dsa_65_BYTES

int pqcrystals_ml_dsa_65_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_ml_dsa_65_avx2_signature_ctx(uint8_t *sig, size_t *siglen,
                                           const uint8_t *m, size_t mlen,
                                           const uint8_t *ctx, size_t ctxlen,
                                           const uint8_t *sk);

int pqcrystals_ml_dsa_65_avx2_signature(uint8_t *sig, size_t *siglen,
                                       const uint8_t *m, size_t mlen,
                                       const uint8_t *sk);

int pqcrystals_ml_dsa_65_avx2_ctx(uint8_t *sm, size_t *smlen,
                                 const uint8_t *m, size_t mlen,
                                 const uint8_t *ctx, size_t ctxlen,
                                 const uint8_t *sk);

int pqcrystals_ml_dsa_65_avx2(uint8_t *sm, size_t *smlen,
                             const uint8_t *m, size_t mlen,
                             const uint8_t *sk);

int pqcrystals_ml_dsa_65_avx2_verify_ctx(const uint8_t *sig, size_t siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *ctx, size_t ctxlen,
                                        const uint8_t *pk);

int pqcrystals_ml_dsa_65_avx2_verify(const uint8_t *sig, size_t siglen,
                                    const uint8_t *m, size_t mlen,
                                    const uint8_t *pk);

int pqcrystals_ml_dsa_65_avx2_open_ctx(uint8_t *m, size_t *mlen,
                                      const uint8_t *sm, size_t smlen,
                                      const uint8_t *ctx, size_t ctxlen,
                                      const uint8_t *pk);

int pqcrystals_ml_dsa_65_avx2_open(uint8_t *m, size_t *mlen,
                                  const uint8_t *sm, size_t smlen,
                                  const uint8_t *pk);

#define pqcrystals_ml_dsa_87_PUBLICKEYBYTES 2592
#define pqcrystals_ml_dsa_87_SECRETKEYBYTES 4896
#define pqcrystals_ml_dsa_87_BYTES 4627

#define pqcrystals_ml_dsa_87_avx2_PUBLICKEYBYTES pqcrystals_ml_dsa_87_PUBLICKEYBYTES
#define pqcrystals_ml_dsa_87_avx2_SECRETKEYBYTES pqcrystals_ml_dsa_87_SECRETKEYBYTES
#define pqcrystals_ml_dsa_87_avx2_BYTES pqcrystals_ml_dsa_87_BYTES

int pqcrystals_ml_dsa_87_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_ml_dsa_87_avx2_signature_ctx(uint8_t *sig, size_t *siglen,
                                           const uint8_t *m, size_t mlen,
                                           const uint8_t *ctx, size_t ctxlen,
                                           const uint8_t *sk);

int pqcrystals_ml_dsa_87_avx2_signature(uint8_t *sig, size_t *siglen,
                                       const uint8_t *m, size_t mlen,
                                       const uint8_t *sk);

int pqcrystals_ml_dsa_87_avx2_ctx(uint8_t *sm, size_t *smlen,
                                 const uint8_t *m, size_t mlen,
                                 const uint8_t *ctx, size_t ctxlen,
                                 const uint8_t *sk);

int pqcrystals_ml_dsa_87_avx2(uint8_t *sm, size_t *smlen,
                             const uint8_t *m, size_t mlen,
                             const uint8_t *sk);

int pqcrystals_ml_dsa_87_avx2_verify_ctx(const uint8_t *sig, size_t siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *ctx, size_t ctxlen,
                                        const uint8_t *pk);

int pqcrystals_ml_dsa_87_avx2_verify(const uint8_t *sig, size_t siglen,
                                    const uint8_t *m, size_t mlen,
                                    const uint8_t *pk);

int pqcrystals_ml_dsa_87_avx2_open_ctx(uint8_t *m, size_t *mlen,
                                      const uint8_t *sm, size_t smlen,
                                      const uint8_t *ctx, size_t ctxlen,
                                      const uint8_t *pk);

int pqcrystals_ml_dsa_87_avx2_open(uint8_t *m, size_t *mlen,
                                  const uint8_t *sm, size_t smlen,
                                  const uint8_t *pk);


#endif
