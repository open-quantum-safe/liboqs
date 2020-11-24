#include "SABER_indcpa.h"
#include "SABER_params.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "poly.h"
#include "randombytes.h"
#include <stdint.h>
#include <string.h>

#define h1 (1 << (SABER_EQ - SABER_EP - 1))
#define h2 ((1 << (SABER_EP - 2)) - (1 << (SABER_EP - SABER_ET - 1)) + (1 << (SABER_EQ - SABER_EP - 1)))

void PQCLEAN_SABER_CLEAN_indcpa_kem_keypair(uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t sk[SABER_INDCPA_SECRETKEYBYTES]) {
    size_t i, j;

    poly A[SABER_L][SABER_L];
    poly s[SABER_L];
    poly res[SABER_L];

    uint8_t rand[SABER_NOISESEEDBYTES];
    uint8_t *seed_A = pk + SABER_POLYVECCOMPRESSEDBYTES;

    randombytes(seed_A, SABER_SEEDBYTES);
    shake128(seed_A, SABER_SEEDBYTES, seed_A, SABER_SEEDBYTES); // for not revealing system RNG state

    randombytes(rand, SABER_NOISESEEDBYTES);
    PQCLEAN_SABER_CLEAN_GenSecret(s, rand);
    PQCLEAN_SABER_CLEAN_POLVECq2BS(sk, s);

    PQCLEAN_SABER_CLEAN_GenMatrix(A, seed_A); // sample matrix A
    PQCLEAN_SABER_CLEAN_MatrixVectorMul(res, (const poly (*)[SABER_L])A, (const poly *)s, 1); // Matrix in transposed order


    // rounding
    for (i = 0; i < SABER_L; i++) {
        for (j = 0; j < SABER_N; j++) {
            res[i].coeffs[j] += h1;
            res[i].coeffs[j] >>= SABER_EQ - SABER_EP;
            res[i].coeffs[j] &= SABER_Q - 1;
        }
    }

    PQCLEAN_SABER_CLEAN_POLVECp2BS(pk, res); // pack public key
}


void PQCLEAN_SABER_CLEAN_indcpa_kem_enc(uint8_t ciphertext[SABER_BYTES_CCA_DEC], const uint8_t m[SABER_KEYBYTES], const uint8_t noiseseed[SABER_NOISESEEDBYTES], const uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES]) {
    size_t i, j;

    poly A[SABER_L][SABER_L];
    poly res[SABER_L];
    poly s[SABER_L];
    poly *temp = A[0]; // re-use stack space
    poly *vprime = &A[0][0];
    poly *message = &A[0][1];

    const uint8_t *seed_A = pk + SABER_POLYVECCOMPRESSEDBYTES;
    uint8_t *msk_c = ciphertext + SABER_POLYVECCOMPRESSEDBYTES;

    PQCLEAN_SABER_CLEAN_GenSecret(s, noiseseed);
    PQCLEAN_SABER_CLEAN_GenMatrix(A, seed_A);
    PQCLEAN_SABER_CLEAN_MatrixVectorMul(res, (const poly (*)[SABER_L])A, (const poly *)s, 0); // 0 => not transposed


    // rounding
    for (i = 0; i < SABER_L; i++) { //shift right EQ-EP bits
        for (j = 0; j < SABER_N; j++) {
            res[i].coeffs[j] += h1;
            res[i].coeffs[j] >>= SABER_EQ - SABER_EP;
            res[i].coeffs[j] &= SABER_Q - 1;
        }
    }
    PQCLEAN_SABER_CLEAN_POLVECp2BS(ciphertext, res);

    // vector-vector scalar multiplication with mod p
    PQCLEAN_SABER_CLEAN_BS2POLVECp(temp, pk);
    PQCLEAN_SABER_CLEAN_InnerProd(vprime, temp, s);
    PQCLEAN_SABER_CLEAN_BS2POLmsg(message, m);

    for (i = 0; i < SABER_N; i++) {
        vprime->coeffs[i] += h1 - (message->coeffs[i] << (SABER_EP - 1));
        vprime->coeffs[i] &= SABER_P - 1;
        vprime->coeffs[i] >>= SABER_EP - SABER_ET;
    }

    PQCLEAN_SABER_CLEAN_POLT2BS(msk_c, vprime);
}


void PQCLEAN_SABER_CLEAN_indcpa_kem_dec(uint8_t m[SABER_KEYBYTES], const uint8_t sk[SABER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[SABER_BYTES_CCA_DEC]) {
    size_t i;

    poly temp[SABER_L];
    poly s[SABER_L];

    const uint8_t *packed_cm = ciphertext + SABER_POLYVECCOMPRESSEDBYTES;
    poly *v = &temp[0];
    poly *cm = &temp[1];

    PQCLEAN_SABER_CLEAN_BS2POLVECq(s, sk);
    PQCLEAN_SABER_CLEAN_BS2POLVECp(temp, ciphertext);
    PQCLEAN_SABER_CLEAN_InnerProd(&temp[0], temp, s);

    PQCLEAN_SABER_CLEAN_BS2POLT(cm, packed_cm);

    for (i = 0; i < SABER_N; i++) {
        v->coeffs[i] += h2 - (cm->coeffs[i] << (SABER_EP - SABER_ET));
        v->coeffs[i] &= SABER_P - 1;
        v->coeffs[i] >>= SABER_EP - 1;
    }

    PQCLEAN_SABER_CLEAN_POLmsg2BS(m, v);
}
