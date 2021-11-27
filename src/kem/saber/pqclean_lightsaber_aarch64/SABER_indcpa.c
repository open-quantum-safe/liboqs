#include "NTT.h"
#include "SABER_indcpa.h"
#include "SABER_params.h"
#include "cbd.h"
#include "fips202.h"
#include "fips202x2.h"
#include "pack_unpack.h"
#include "randombytes.h"
#include <arm_neon.h>
#include <string.h>

#define h1 (1 << (SABER_EQ - SABER_EP - 1))
#define h2 ((1 << (SABER_EP - 2)) - (1 << (SABER_EP - SABER_ET - 1)) + (1 << (SABER_EQ - SABER_EP - 1)))

extern void PQCLEAN_LIGHTSABER_AARCH64_asm_round(uint16_t des[SABER_N], uint32_t src[SABER_N]);
extern void PQCLEAN_LIGHTSABER_AARCH64_asm_enc_add_msg(uint16_t cipher[SABER_N], uint32_t src[SABER_N], uint16_t msg[SABER_N], int const_h1);
extern void PQCLEAN_LIGHTSABER_AARCH64_asm_dec_get_msg(uint16_t msg[SABER_N], uint32_t src[SABER_N], uint16_t cipher[SABER_N], int const_h2);

void indcpa_kem_keypair(uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t sk[SABER_INDCPA_SECRETKEYBYTES]) {

    uint32_t A_NTT[SABER_L][SABER_L][SABER_N];
    uint32_t s_NTT[SABER_L][SABER_N];
    uint32_t s_NTT_asymmetric[SABER_L][SABER_N];

    uint16_t s[SABER_L][SABER_N];
    uint16_t b[SABER_L][SABER_N] = {0};

    uint8_t seed_A[SABER_SEEDBYTES];
    uint8_t seed_s[SABER_NOISE_SEEDBYTES];

    uint8_t shake_A_buf[SABER_L * SABER_L * SABER_POLYBYTES];
    uint8_t shake_s_buf[SABER_L * SABER_POLYCOINBYTES];

    randombytes(seed_A, SABER_SEEDBYTES);
    shake128(seed_A, SABER_SEEDBYTES, seed_A, SABER_SEEDBYTES); // for not revealing system RNG state
    randombytes(seed_s, SABER_NOISE_SEEDBYTES);

    shake128(shake_A_buf, sizeof(shake_A_buf), seed_A, SABER_SEEDBYTES);
    shake128(shake_s_buf, sizeof(shake_s_buf), seed_s, SABER_NOISE_SEEDBYTES);

    for (int i = 0; i < SABER_L; i++) {
        for (int j = 0; j < SABER_L; j++) {
            PQCLEAN_LIGHTSABER_AARCH64_asm_13_to_32(&(A_NTT[j][i][0]), shake_A_buf + (i * SABER_L + j) * SABER_POLYBYTES);
        }
    }

    for (int i = 0; i < SABER_L; i++) {
        cbd(s[i], shake_s_buf + i * SABER_POLYCOINBYTES);
        PQCLEAN_LIGHTSABER_AARCH64_asm_16_to_32(&(s_NTT[i][0]), &(s[i][0]));
    }

    for (int i = 0; i < SABER_L; i++) {
        NTT_heavy(&(s_NTT_asymmetric[i][0]), &(s_NTT[i][0]));
    }

    for (int i = 0; i < SABER_L; i++) {
        for (int j = 0; j < SABER_L; j++) {
            NTT(&(A_NTT[i][j][0]));
        }
    }

    for (int i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_AARCH64_asm_asymmetric_mul(&(A_NTT[i][0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);
    }

    for (int i = 0; i < SABER_L; i++) {
        iNTT(&(A_NTT[i][0][0]));
    }

    for (int i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_AARCH64_asm_round(b[i], A_NTT[i][0]);
    }

    POLVECq2BS(sk, s);
    POLVECp2BS(pk, b);
    memcpy(pk + SABER_POLYVECCOMPRESSEDBYTES, seed_A, sizeof(seed_A));
}

void indcpa_kem_enc(const uint8_t m[SABER_KEYBYTES], const uint8_t seed_sp[SABER_NOISE_SEEDBYTES], const uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t ciphertext[SABER_BYTES_CCA_DEC]) {

    uint32_t A_NTT[SABER_L][SABER_L][SABER_N];
    uint32_t s_NTT[SABER_L][SABER_N];
    uint32_t s_NTT_asymmetric[SABER_L][SABER_N];

    uint32_t b_NTT[SABER_L][SABER_N];

    uint16_t sp[SABER_L][SABER_N];
    uint16_t bp[SABER_L][SABER_N] = {0};
    uint16_t vp[SABER_N] = {0};
    uint16_t mp[SABER_N];
    uint16_t b[SABER_L][SABER_N];
    const uint8_t *seed_A = pk + SABER_POLYVECCOMPRESSEDBYTES;

    uint8_t shake_A_buf[SABER_L * SABER_L * SABER_POLYBYTES];
    uint8_t shake_s_buf[SABER_L * SABER_POLYCOINBYTES];

    shake128(shake_A_buf, sizeof(shake_A_buf), seed_A, SABER_SEEDBYTES);
    shake128(shake_s_buf, sizeof(shake_s_buf), seed_sp, SABER_NOISE_SEEDBYTES);

    for (int i = 0; i < SABER_L; i++) {
        for (int j = 0; j < SABER_L; j++) {
            PQCLEAN_LIGHTSABER_AARCH64_asm_13_to_32(&(A_NTT[i][j][0]), shake_A_buf + (i * SABER_L + j) * SABER_POLYBYTES);
        }
    }

    for (int i = 0; i < SABER_L; i++) {
        cbd(sp[i], shake_s_buf + i * SABER_POLYCOINBYTES);
        PQCLEAN_LIGHTSABER_AARCH64_asm_16_to_32(&(s_NTT[i][0]), &(sp[i][0]));
    }

    for (int i = 0; i < SABER_L; i++) {
        NTT_heavy(&(s_NTT_asymmetric[i][0]), &(s_NTT[i][0]));
    }

    for (int i = 0; i < SABER_L; i++) {
        for (int j = 0; j < SABER_L; j++) {
            NTT(&(A_NTT[i][j][0]));
        }
    }

    for (int i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_AARCH64_asm_asymmetric_mul(&(A_NTT[i][0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);
    }

    for (int i = 0; i < SABER_L; i++) {
        iNTT(&(A_NTT[i][0][0]));
    }

    for (int i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_AARCH64_asm_round(bp[i], A_NTT[i][0]);
    }


    BS2POLVECp(pk, b);
    BS2POLmsg(m, mp);

    for (int i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_AARCH64_asm_16_to_32(&(b_NTT[i][0]), &(b[i][0]));
    }

    for (int i = 0; i < SABER_L; i++) {
        NTT(&(b_NTT[i][0]));
    }

    PQCLEAN_LIGHTSABER_AARCH64_asm_asymmetric_mul(&(b_NTT[0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);

    iNTT(&(b_NTT[0][0]));

    PQCLEAN_LIGHTSABER_AARCH64_asm_enc_add_msg(vp, b_NTT[0], mp, h1);

    POLVECp2BS(ciphertext, bp);
    POLT2BS(ciphertext + SABER_POLYVECCOMPRESSEDBYTES, vp);


}

void indcpa_kem_dec(const uint8_t sk[SABER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[SABER_BYTES_CCA_DEC], uint8_t m[SABER_KEYBYTES]) {

    uint32_t b_NTT[SABER_L][SABER_N];
    uint32_t s_NTT[SABER_L][SABER_N];
    uint32_t s_NTT_asymmetric[SABER_L][SABER_N];

    uint16_t v[SABER_N] = {0};
    uint16_t cm[SABER_N];

    BS2POLT(ciphertext + SABER_POLYVECCOMPRESSEDBYTES, cm);

    for (int i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_AARCH64_asm_13_to_32(&(s_NTT[i][0]), sk + i * SABER_POLYBYTES);
    }

    for (int i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_AARCH64_asm_10_to_32(&(b_NTT[i][0]), ciphertext + i * (SABER_EP * SABER_N / 8));
    }

    for (int i = 0; i < SABER_L; i++) {
        NTT_heavy(&(s_NTT_asymmetric[i][0]), &(s_NTT[i][0]));
    }

    for (int i = 0; i < SABER_L; i++) {
        NTT(&(b_NTT[i][0]));
    }

    PQCLEAN_LIGHTSABER_AARCH64_asm_asymmetric_mul(&(b_NTT[0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);

    iNTT(&(b_NTT[0][0]));

    PQCLEAN_LIGHTSABER_AARCH64_asm_dec_get_msg(v, b_NTT[0], cm, h2);

    POLmsg2BS(m, v);
}





