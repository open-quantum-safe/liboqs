#ifndef API_H
#define API_H

#include <stddef.h>
#include <stdint.h>

#define pqcrystals_ml_dsa_44_PUBLICKEYBYTES 1312
#define pqcrystals_ml_dsa_44_SECRETKEYBYTES 2560
#define pqcrystals_ml_dsa_44_BYTES 2420

#define pqcrystals_ml_dsa_44_ref_PUBLICKEYBYTES pqcrystals_ml_dsa_44_PUBLICKEYBYTES
#define pqcrystals_ml_dsa_44_ref_SECRETKEYBYTES pqcrystals_ml_dsa_44_SECRETKEYBYTES
#define pqcrystals_ml_dsa_44_ref_BYTES pqcrystals_ml_dsa_44_BYTES

int pqcrystals_ml_dsa_44_ref_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_ml_dsa_44_ref_signature_ctx(uint8_t *sig, size_t *siglen,
                                          const uint8_t *m, size_t mlen,
                                          const uint8_t *ctx, size_t ctxlen,
                                          const uint8_t *sk);

int pqcrystals_ml_dsa_44_ref_signature(uint8_t *sig, size_t *siglen,
                                       const uint8_t *m, size_t mlen,
                                       const uint8_t *sk);

int pqcrystals_ml_dsa_44_ref_ctx(uint8_t *sm, size_t *smlen,
                                 const uint8_t *m, size_t mlen,
                                 const uint8_t *ctx, size_t ctxlen,
                                 const uint8_t *sk);

int pqcrystals_ml_dsa_44_ref(uint8_t *sm, size_t *smlen,
                             const uint8_t *m, size_t mlen,
                             const uint8_t *sk);

int pqcrystals_ml_dsa_44_ref_verify_ctx(const uint8_t *sig, size_t siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *ctx, size_t ctxlen,
                                        const uint8_t *pk);

int pqcrystals_ml_dsa_44_ref_verify(const uint8_t *sig, size_t siglen,
                                    const uint8_t *m, size_t mlen,
                                    const uint8_t *pk);

int pqcrystals_ml_dsa_44_ref_open_ctx(uint8_t *m, size_t *mlen,
                                      const uint8_t *sm, size_t smlen,
                                      const uint8_t *ctx, size_t ctxlen,
                                      const uint8_t *pk);

int pqcrystals_ml_dsa_44_ref_open(uint8_t *m, size_t *mlen,
                                  const uint8_t *sm, size_t smlen,
                                  const uint8_t *pk);

#define pqcrystals_ml_dsa_65_PUBLICKEYBYTES 1952
#define pqcrystals_ml_dsa_65_SECRETKEYBYTES 4032
#define pqcrystals_ml_dsa_65_BYTES 3309

#define pqcrystals_ml_dsa_65_ref_PUBLICKEYBYTES pqcrystals_ml_dsa_65_PUBLICKEYBYTES
#define pqcrystals_ml_dsa_65_ref_SECRETKEYBYTES pqcrystals_ml_dsa_65_SECRETKEYBYTES
#define pqcrystals_ml_dsa_65_ref_BYTES pqcrystals_ml_dsa_65_BYTES

int pqcrystals_ml_dsa_65_ref_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_ml_dsa_65_ref_signature_ctx(uint8_t *sig, size_t *siglen,
                                           const uint8_t *m, size_t mlen,
                                           const uint8_t *ctx, size_t ctxlen,
                                           const uint8_t *sk);

int pqcrystals_ml_dsa_65_ref_signature(uint8_t *sig, size_t *siglen,
                                       const uint8_t *m, size_t mlen,
                                       const uint8_t *sk);

int pqcrystals_ml_dsa_65_ref_ctx(uint8_t *sm, size_t *smlen,
                                 const uint8_t *m, size_t mlen,
                                 const uint8_t *ctx, size_t ctxlen,
                                 const uint8_t *sk);

int pqcrystals_ml_dsa_65_ref(uint8_t *sm, size_t *smlen,
                             const uint8_t *m, size_t mlen,
                             const uint8_t *sk);

int pqcrystals_ml_dsa_65_ref_verify_ctx(const uint8_t *sig, size_t siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *ctx, size_t ctxlen,
                                        const uint8_t *pk);

int pqcrystals_ml_dsa_65_ref_verify(const uint8_t *sig, size_t siglen,
                                    const uint8_t *m, size_t mlen,
                                    const uint8_t *pk);

int pqcrystals_ml_dsa_65_ref_open_ctx(uint8_t *m, size_t *mlen,
                                      const uint8_t *sm, size_t smlen,
                                      const uint8_t *ctx, size_t ctxlen,
                                      const uint8_t *pk);

int pqcrystals_ml_dsa_65_ref_open(uint8_t *m, size_t *mlen,
                                  const uint8_t *sm, size_t smlen,
                                  const uint8_t *pk);

#define pqcrystals_ml_dsa_87_PUBLICKEYBYTES 2592
#define pqcrystals_ml_dsa_87_SECRETKEYBYTES 4896
#define pqcrystals_ml_dsa_87_BYTES 4627

#define pqcrystals_ml_dsa_87_ref_PUBLICKEYBYTES pqcrystals_ml_dsa_87_PUBLICKEYBYTES
#define pqcrystals_ml_dsa_87_ref_SECRETKEYBYTES pqcrystals_ml_dsa_87_SECRETKEYBYTES
#define pqcrystals_ml_dsa_87_ref_BYTES pqcrystals_ml_dsa_87_BYTES

int pqcrystals_ml_dsa_87_ref_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_ml_dsa_87_ref_signature_ctx(uint8_t *sig, size_t *siglen,
                                           const uint8_t *m, size_t mlen,
                                           const uint8_t *ctx, size_t ctxlen,
                                           const uint8_t *sk);

int pqcrystals_ml_dsa_87_ref_signature(uint8_t *sig, size_t *siglen,
                                       const uint8_t *m, size_t mlen,
                                       const uint8_t *sk);

int pqcrystals_ml_dsa_87_ref_ctx(uint8_t *sm, size_t *smlen,
                                 const uint8_t *m, size_t mlen,
                                 const uint8_t *ctx, size_t ctxlen,
                                 const uint8_t *sk);

int pqcrystals_ml_dsa_87_ref(uint8_t *sm, size_t *smlen,
                             const uint8_t *m, size_t mlen,
                             const uint8_t *sk);

int pqcrystals_ml_dsa_87_ref_verify_ctx(const uint8_t *sig, size_t siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *ctx, size_t ctxlen,
                                        const uint8_t *pk);

int pqcrystals_ml_dsa_87_ref_verify(const uint8_t *sig, size_t siglen,
                                    const uint8_t *m, size_t mlen,
                                    const uint8_t *pk);

int pqcrystals_ml_dsa_87_ref_open_ctx(uint8_t *m, size_t *mlen,
                                      const uint8_t *sm, size_t smlen,
                                      const uint8_t *ctx, size_t ctxlen,
                                      const uint8_t *pk);

int pqcrystals_ml_dsa_87_ref_open(uint8_t *m, size_t *mlen,
                                  const uint8_t *sm, size_t smlen,
                                  const uint8_t *pk);


#endif
