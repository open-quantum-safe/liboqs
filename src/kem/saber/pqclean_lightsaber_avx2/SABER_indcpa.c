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

void PQCLEAN_LIGHTSABER_AVX2_indcpa_kem_keypair(uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t sk[SABER_INDCPA_SECRETKEYBYTES]) {
    size_t i, j;

    poly A[SABER_L][SABER_L];
    poly *skpv1 = A[0]; // use first row of A to hold sk temporarily
    toom4_points skpv1_eval[SABER_L];
    poly res[SABER_L];

    uint8_t rand[SABER_NOISESEEDBYTES];
    uint8_t *seed_A = pk + SABER_POLYVECCOMPRESSEDBYTES;

    randombytes(seed_A, SABER_SEEDBYTES);
    shake128(seed_A, SABER_SEEDBYTES, seed_A, SABER_SEEDBYTES); // for not revealing system RNG state

    randombytes(rand, SABER_NOISESEEDBYTES);
    PQCLEAN_LIGHTSABER_AVX2_GenSecret(skpv1, rand);
    PQCLEAN_LIGHTSABER_AVX2_POLVECq2BS(sk, skpv1); // pack secret key

    for (j = 0; j < SABER_L; j++) {
        PQCLEAN_LIGHTSABER_AVX2_toom4_eval(&skpv1_eval[j], &skpv1[j]);
    }

    PQCLEAN_LIGHTSABER_AVX2_GenMatrix(A, seed_A); // sample matrix A
    PQCLEAN_LIGHTSABER_AVX2_MatrixVectorMul(res, (const poly (*)[SABER_L])A, (const toom4_points *)skpv1_eval, 1); // Matrix in transposed order

    // rounding
    for (i = 0; i < SABER_L; i++) {
        for (j = 0; j < SABER_N; j++) {
            res[i].coeffs[j] += h1;
            res[i].coeffs[j] >>= SABER_EQ - SABER_EP;
            res[i].coeffs[j] &= SABER_Q - 1;
        }
    }

    PQCLEAN_LIGHTSABER_AVX2_POLVECp2BS(pk, res); // pack public key
}


void PQCLEAN_LIGHTSABER_AVX2_indcpa_kem_enc(uint8_t ciphertext[SABER_BYTES_CCA_DEC], const uint8_t m[SABER_KEYBYTES], const uint8_t noiseseed[SABER_NOISESEEDBYTES], const uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES]) {
    size_t i, j;

    poly A[SABER_L][SABER_L];
    poly res[SABER_L];
    toom4_points skpv1_eval[SABER_L];

    poly *temp = A[0]; // re-use stack space
    poly *vprime = &A[0][0];
    poly *message = &A[0][1];

    const uint8_t *seed_A = pk + SABER_POLYVECCOMPRESSEDBYTES;
    uint8_t *msk_c = ciphertext + SABER_POLYVECCOMPRESSEDBYTES;

    PQCLEAN_LIGHTSABER_AVX2_GenSecret(temp, noiseseed);
    for (j = 0; j < SABER_L; j++) {
        PQCLEAN_LIGHTSABER_AVX2_toom4_eval(&skpv1_eval[j], &temp[j]);
    }

    PQCLEAN_LIGHTSABER_AVX2_GenMatrix(A, seed_A);
    PQCLEAN_LIGHTSABER_AVX2_MatrixVectorMul(res, (const poly (*)[SABER_L])A, (const toom4_points *)skpv1_eval, 0); // 0 => not transposed

    // rounding
    for (i = 0; i < SABER_L; i++) { //shift right EQ-EP bits
        for (j = 0; j < SABER_N; j++) {
            res[i].coeffs[j] += h1;
            res[i].coeffs[j] >>= SABER_EQ - SABER_EP;
            res[i].coeffs[j] &= SABER_Q - 1;
        }
    }
    PQCLEAN_LIGHTSABER_AVX2_POLVECp2BS(ciphertext, res);

    // vector-vector scalar multiplication with mod p
    PQCLEAN_LIGHTSABER_AVX2_BS2POLVECp(temp, pk);
    PQCLEAN_LIGHTSABER_AVX2_InnerProd(vprime, temp, skpv1_eval);
    PQCLEAN_LIGHTSABER_AVX2_BS2POLmsg(message, m);

    for (i = 0; i < SABER_N; i++) {
        vprime->coeffs[i] += h1 - (message->coeffs[i] << (SABER_EP - 1));
        vprime->coeffs[i] &= SABER_P - 1;
        vprime->coeffs[i] >>= SABER_EP - SABER_ET;
    }

    PQCLEAN_LIGHTSABER_AVX2_POLT2BS(msk_c, vprime);
}


void PQCLEAN_LIGHTSABER_AVX2_indcpa_kem_dec(uint8_t m[SABER_KEYBYTES], const uint8_t sk[SABER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[SABER_BYTES_CCA_DEC]) {
    size_t i;

    poly temp[SABER_L];
    toom4_points sksv_eval[SABER_L];

    const uint8_t *packed_cm = ciphertext + SABER_POLYVECCOMPRESSEDBYTES;
    poly *v = &temp[0];
    poly *cm = &temp[1];

    PQCLEAN_LIGHTSABER_AVX2_BS2POLVECq(temp, sk);
    for (i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_AVX2_toom4_eval(&sksv_eval[i], &temp[i]);
    }

    PQCLEAN_LIGHTSABER_AVX2_BS2POLVECp(temp, ciphertext);
    PQCLEAN_LIGHTSABER_AVX2_InnerProd(v, temp, sksv_eval);

    PQCLEAN_LIGHTSABER_AVX2_BS2POLT(cm, packed_cm);

    for (i = 0; i < SABER_N; i++) {
        v->coeffs[i] += h2 - (cm->coeffs[i] << (SABER_EP - SABER_ET));
        v->coeffs[i] &= SABER_P - 1;
        v->coeffs[i] >>= SABER_EP - 1;
    }

    PQCLEAN_LIGHTSABER_AVX2_POLmsg2BS(m, v);
}
