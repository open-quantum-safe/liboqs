#ifndef API_H
#define API_H

#include <stddef.h>
#include <stdint.h>

#define pqcrystals_dilithium2_avx2_PUBLICKEYBYTES 1184
#define pqcrystals_dilithium2_avx2_SECRETKEYBYTES 2800
#define pqcrystals_dilithium2_avx2_BYTES 2044

int pqcrystals_dilithium2_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium2_avx2_signature(uint8_t *sig, size_t *siglen,
                                         const uint8_t *m, size_t mlen,
                                         const uint8_t *sk);

int pqcrystals_dilithium2_avx2(uint8_t *sm, size_t *smlen,
                               const uint8_t *m, size_t mlen,
                               const uint8_t *sk);

int pqcrystals_dilithium2_avx2_verify(const uint8_t *sig, size_t siglen,
                                      const uint8_t *m, size_t mlen,
                                      const uint8_t *pk);

int pqcrystals_dilithium2_avx2_open(uint8_t *m, size_t *mlen,
                                    const uint8_t *sm, size_t smlen,
                                    const uint8_t *pk);

#define pqcrystals_dilithium2aes_avx2_PUBLICKEYBYTES \
        pqcrystals_dilithium2_avx2_PUBLICKEYBYTES
#define pqcrystals_dilithium2aes_avx2_SECRETKEYBYTES \
        pqcrystals_dilithium2_avx2_SECRETKEYBYTES
#define pqcrystals_dilithium2aes_avx2_BYTES \
        pqcrystals_dilithium2_avx2_BYTES

int pqcrystals_dilithium2aes_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium2aes_avx2_signature(uint8_t *sig, size_t *siglen,
                                            const uint8_t *m, size_t mlen,
                                            const uint8_t *sk);

int pqcrystals_dilithium2aes_avx2(uint8_t *sm, size_t *smlen,
                                  const uint8_t *m, size_t mlen,
                                  const uint8_t *sk);

int pqcrystals_dilithium2aes_avx2_verify(const uint8_t *sig, size_t siglen,
                                         const uint8_t *m, size_t mlen,
                                         const uint8_t *pk);

int pqcrystals_dilithium2aes_avx2_open(uint8_t *m, size_t *mlen,
                                       const uint8_t *sm, size_t smlen,
                                       const uint8_t *pk);

#define pqcrystals_dilithium3_avx2_PUBLICKEYBYTES 1472
#define pqcrystals_dilithium3_avx2_SECRETKEYBYTES 3504
#define pqcrystals_dilithium3_avx2_BYTES 2701

int pqcrystals_dilithium3_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium3_avx2_signature(uint8_t *sig, size_t *siglen,
                                         const uint8_t *m, size_t mlen,
                                         const uint8_t *sk);

int pqcrystals_dilithium3_avx2(uint8_t *sm, size_t *smlen,
                               const uint8_t *m, size_t mlen,
                               const uint8_t *sk);

int pqcrystals_dilithium3_avx2_verify(const uint8_t *sig, size_t siglen,
                                      const uint8_t *m, size_t mlen,
                                      const uint8_t *pk);

int pqcrystals_dilithium3_avx2_open(uint8_t *m, size_t *mlen,
                                    const uint8_t *sm, size_t smlen,
                                    const uint8_t *pk);

#define pqcrystals_dilithium3aes_avx2_PUBLICKEYBYTES \
        pqcrystals_dilithium3_avx2_PUBLICKEYBYTES
#define pqcrystals_dilithium3aes_avx2_SECRETKEYBYTES \
        pqcrystals_dilithium3_avx2_SECRETKEYBYTES
#define pqcrystals_dilithium3aes_avx2_BYTES \
        pqcrystals_dilithium3_avx2_BYTES

int pqcrystals_dilithium3aes_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium3aes_avx2_signature(uint8_t *sig, size_t *siglen,
                                            const uint8_t *m, size_t mlen,
                                            const uint8_t *sk);

int pqcrystals_dilithium3aes_avx2(uint8_t *sm, size_t *smlen,
                                  const uint8_t *m, size_t mlen,
                                  const uint8_t *sk);

int pqcrystals_dilithium3aes_avx2_verify(const uint8_t *sig, size_t siglen,
                                         const uint8_t *m, size_t mlen,
                                         const uint8_t *pk);

int pqcrystals_dilithium3aes_avx2_open(uint8_t *m, size_t *mlen,
                                       const uint8_t *sm, size_t smlen,
                                       const uint8_t *pk);

#define pqcrystals_dilithium4_avx2_PUBLICKEYBYTES 1760
#define pqcrystals_dilithium4_avx2_SECRETKEYBYTES 3856
#define pqcrystals_dilithium4_avx2_BYTES 3366

int pqcrystals_dilithium4_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium4_avx2_signature(uint8_t *sig, size_t *siglen,
                                         const uint8_t *m, size_t mlen,
                                         const uint8_t *sk);

int pqcrystals_dilithium4_avx2(uint8_t *sm, size_t *smlen,
                               const uint8_t *m, size_t mlen,
                               const uint8_t *sk);

int pqcrystals_dilithium4_avx2_verify(const uint8_t *sig, size_t siglen,
                                      const uint8_t *m, size_t mlen,
                                      const uint8_t *pk);

int pqcrystals_dilithium4_avx2_open(uint8_t *m, size_t *mlen,
                                    const uint8_t *sm, size_t smlen,
                                    const uint8_t *pk);

#define pqcrystals_dilithium4aes_avx2_PUBLICKEYBYTES \
        pqcrystals_dilithium4_avx2_PUBLICKEYBYTES
#define pqcrystals_dilithium4aes_avx2_SECRETKEYBYTES \
        pqcrystals_dilithium4_avx2_SECRETKEYBYTES
#define pqcrystals_dilithium4aes_avx2_BYTES \
        pqcrystals_dilithium4_avx2_BYTES

int pqcrystals_dilithium4aes_avx2_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium4aes_avx2_signature(uint8_t *sig, size_t *siglen,
                                            const uint8_t *m, size_t mlen,
                                            const uint8_t *sk);

int pqcrystals_dilithium4aes_avx2(uint8_t *sm, size_t *smlen,
                                  const uint8_t *m, size_t mlen,
                                  const uint8_t *sk);

int pqcrystals_dilithium4aes_avx2_verify(const uint8_t *sig, size_t siglen,
                                         const uint8_t *m, size_t mlen,
                                         const uint8_t *pk);

int pqcrystals_dilithium4aes_avx2_open(uint8_t *m, size_t *mlen,
                                       const uint8_t *sm, size_t smlen,
                                       const uint8_t *pk);


#endif
