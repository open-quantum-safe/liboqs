#ifndef API_H
#define API_H

#include <stddef.h>
#include <stdint.h>

#define pqcrystals_dilithium2_PUBLICKEYBYTES 1184
#define pqcrystals_dilithium2_SECRETKEYBYTES 2800
#define pqcrystals_dilithium2_BYTES 2044

#define pqcrystals_dilithium2_ref_PUBLICKEYBYTES \
        pqcrystals_dilithium2_PUBLICKEYBYTES
#define pqcrystals_dilithium2_ref_SECRETKEYBYTES \
        pqcrystals_dilithium2_SECRETKEYBYTES
#define pqcrystals_dilithium2_ref_BYTES \
        pqcrystals_dilithium2_BYTES

int pqcrystals_dilithium2_ref_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium2_ref_signature(uint8_t *sig, size_t *siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *sk);

int pqcrystals_dilithium2_ref(uint8_t *sm, size_t *smlen,
                              const uint8_t *m, size_t mlen,
                              const uint8_t *sk);

int pqcrystals_dilithium2_ref_verify(const uint8_t *sig, size_t siglen,
                                     const uint8_t *m, size_t mlen,
                                     const uint8_t *pk);

int pqcrystals_dilithium2_ref_open(uint8_t *m, size_t *mlen,
                                   const uint8_t *sm, size_t smlen,
                                   const uint8_t *pk);

#define pqcrystals_dilithium2aes_ref_PUBLICKEYBYTES \
        pqcrystals_dilithium2_ref_PUBLICKEYBYTES
#define pqcrystals_dilithium2aes_ref_SECRETKEYBYTES \
        pqcrystals_dilithium2_ref_SECRETKEYBYTES
#define pqcrystals_dilithium2aes_ref_BYTES \
        pqcrystals_dilithium2_ref_BYTES

int pqcrystals_dilithium2aes_ref_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium2aes_ref_signature(uint8_t *sig, size_t *siglen,
                                           const uint8_t *m, size_t mlen,
                                           const uint8_t *sk);

int pqcrystals_dilithium2aes_ref(uint8_t *sm, size_t *smlen,
                                 const uint8_t *m, size_t mlen,
                                 const uint8_t *sk);

int pqcrystals_dilithium2aes_ref_verify(const uint8_t *sig, size_t siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *pk);

int pqcrystals_dilithium2aes_ref_open(uint8_t *m, size_t *mlen,
                                      const uint8_t *sm, size_t smlen,
                                      const uint8_t *pk);

#define pqcrystals_dilithium3_PUBLICKEYBYTES 1472
#define pqcrystals_dilithium3_SECRETKEYBYTES 3504
#define pqcrystals_dilithium3_BYTES 2701

#define pqcrystals_dilithium3_ref_PUBLICKEYBYTES \
        pqcrystals_dilithium3_PUBLICKEYBYTES
#define pqcrystals_dilithium3_ref_SECRETKEYBYTES \
        pqcrystals_dilithium3_SECRETKEYBYTES
#define pqcrystals_dilithium3_ref_BYTES \
        pqcrystals_dilithium3_BYTES

int pqcrystals_dilithium3_ref_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium3_ref_signature(uint8_t *sig, size_t *siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *sk);

int pqcrystals_dilithium3_ref(uint8_t *sm, size_t *smlen,
                              const uint8_t *m, size_t mlen,
                              const uint8_t *sk);

int pqcrystals_dilithium3_ref_verify(const uint8_t *sig, size_t siglen,
                                     const uint8_t *m, size_t mlen,
                                     const uint8_t *pk);

int pqcrystals_dilithium3_ref_open(uint8_t *m, size_t *mlen,
                                   const uint8_t *sm, size_t smlen,
                                   const uint8_t *pk);

#define pqcrystals_dilithium3aes_ref_PUBLICKEYBYTES \
        pqcrystals_dilithium3_ref_PUBLICKEYBYTES
#define pqcrystals_dilithium3aes_ref_SECRETKEYBYTES \
        pqcrystals_dilithium3_ref_SECRETKEYBYTES
#define pqcrystals_dilithium3aes_ref_BYTES \
        pqcrystals_dilithium3_ref_BYTES

int pqcrystals_dilithium3aes_ref_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium3aes_ref_signature(uint8_t *sig, size_t *siglen,
                                           const uint8_t *m, size_t mlen,
                                           const uint8_t *sk);

int pqcrystals_dilithium3aes_ref(uint8_t *sm, size_t *smlen,
                                 const uint8_t *m, size_t mlen,
                                 const uint8_t *sk);

int pqcrystals_dilithium3aes_ref_verify(const uint8_t *sig, size_t siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *pk);

int pqcrystals_dilithium3aes_ref_open(uint8_t *m, size_t *mlen,
                                      const uint8_t *sm, size_t smlen,
                                      const uint8_t *pk);

#define pqcrystals_dilithium4_PUBLICKEYBYTES 1760
#define pqcrystals_dilithium4_SECRETKEYBYTES 3856
#define pqcrystals_dilithium4_BYTES 3366

#define pqcrystals_dilithium4_ref_PUBLICKEYBYTES \
        pqcrystals_dilithium4_PUBLICKEYBYTES
#define pqcrystals_dilithium4_ref_SECRETKEYBYTES \
        pqcrystals_dilithium4_SECRETKEYBYTES
#define pqcrystals_dilithium4_ref_BYTES \
        pqcrystals_dilithium4_BYTES

int pqcrystals_dilithium4_ref_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium4_ref_signature(uint8_t *sig, size_t *siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *sk);

int pqcrystals_dilithium4_ref(uint8_t *sm, size_t *smlen,
                              const uint8_t *m, size_t mlen,
                              const uint8_t *sk);

int pqcrystals_dilithium4_ref_verify(const uint8_t *sig, size_t siglen,
                                     const uint8_t *m, size_t mlen,
                                     const uint8_t *pk);

int pqcrystals_dilithium4_ref_open(uint8_t *m, size_t *mlen,
                                   const uint8_t *sm, size_t smlen,
                                   const uint8_t *pk);

#define pqcrystals_dilithium4aes_ref_PUBLICKEYBYTES \
        pqcrystals_dilithium4_ref_PUBLICKEYBYTES
#define pqcrystals_dilithium4aes_ref_SECRETKEYBYTES \
        pqcrystals_dilithium4_ref_SECRETKEYBYTES
#define pqcrystals_dilithium4aes_ref_BYTES \
        pqcrystals_dilithium4_ref_BYTES

int pqcrystals_dilithium4aes_ref_keypair(uint8_t *pk, uint8_t *sk);

int pqcrystals_dilithium4aes_ref_signature(uint8_t *sig, size_t *siglen,
                                           const uint8_t *m, size_t mlen,
                                           const uint8_t *sk);

int pqcrystals_dilithium4aes_ref(uint8_t *sm, size_t *smlen,
                                 const uint8_t *m, size_t mlen,
                                 const uint8_t *sk);

int pqcrystals_dilithium4aes_ref_verify(const uint8_t *sig, size_t siglen,
                                        const uint8_t *m, size_t mlen,
                                        const uint8_t *pk);

int pqcrystals_dilithium4aes_ref_open(uint8_t *m, size_t *mlen,
                                      const uint8_t *sm, size_t smlen,
                                      const uint8_t *pk);


#endif
