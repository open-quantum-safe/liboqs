#include "H_Q_matrices_generation.h"
#include "bf_decoding.h"
#include "dfr_test.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "niederreiter.h"
#include "qc_ldpc_parameters.h"
#include "randombytes.h"
#include "rng.h"

#include <string.h>

void PQCLEAN_LEDAKEMLT52_LEAKTIME_niederreiter_keygen(publicKeyNiederreiter_t *pk, privateKeyNiederreiter_t *sk) {

    AES_XOF_struct keys_expander;
    POSITION_T HPosOnes[N0][DV];
    POSITION_T QPosOnes[N0][M];
    POSITION_T LPosOnes[N0][DV * M];
    POSITION_T auxPosOnes[DV * M];
    unsigned char processedQOnes[N0];
    DIGIT Ln0dense[NUM_DIGITS_GF2X_ELEMENT] = {0};
    DIGIT Ln0Inv[NUM_DIGITS_GF2X_ELEMENT] = {0};
    int is_L_full;
    int isDFRok = 0;

    memset(&keys_expander, 0x00, sizeof(AES_XOF_struct));
    randombytes(sk->prng_seed, TRNG_BYTE_LENGTH);
    PQCLEAN_LEDAKEMLT52_LEAKTIME_seedexpander_from_trng(&keys_expander, sk->prng_seed);

    sk->rejections = (uint8_t) 0;
    do {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_generateHPosOnes(HPosOnes, &keys_expander);
        PQCLEAN_LEDAKEMLT52_LEAKTIME_generateQPosOnes(QPosOnes, &keys_expander);
        for (int i = 0; i < N0; i++) {
            for (int j = 0; j < DV * M; j++) {
                LPosOnes[i][j] = INVALID_POS_VALUE;
            }
        }

        memset(processedQOnes, 0x00, sizeof(processedQOnes));
        for (int colQ = 0; colQ < N0; colQ++) {
            for (int i = 0; i < N0; i++) {
                PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_mul_sparse(DV * M, auxPosOnes,
                        DV, HPosOnes[i],
                        qBlockWeights[i][colQ], QPosOnes[i] + processedQOnes[i]);
                PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_add_sparse(DV * M, LPosOnes[colQ],
                        DV * M, LPosOnes[colQ],
                        DV * M, auxPosOnes);
                processedQOnes[i] += qBlockWeights[i][colQ];
            }
        }
        is_L_full = 1;
        for (size_t i = 0; i < N0; i++) {
            is_L_full = is_L_full && (LPosOnes[i][DV * M - 1] != INVALID_POS_VALUE);
        }
        sk->rejections = sk->rejections + 1;
        if (is_L_full) {
            isDFRok = PQCLEAN_LEDAKEMLT52_LEAKTIME_DFR_test(LPosOnes, &(sk->secondIterThreshold));
        }
    } while (!is_L_full || !isDFRok);
    sk->rejections = sk->rejections - 1;

    PQCLEAN_LEDAKEMLT52_LEAKTIME_seedexpander(&keys_expander,
            sk->decryption_failure_secret,
            (unsigned long)TRNG_BYTE_LENGTH);

    for (size_t j = 0; j < DV * M; j++) {
        if (LPosOnes[N0 - 1][j] != INVALID_POS_VALUE) {
            PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_set_coeff(Ln0dense, LPosOnes[N0 - 1][j], 1);
        }
    }

    PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_inverse(Ln0Inv, Ln0dense);
    for (size_t i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_mul_dense_to_sparse(pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT,
                Ln0Inv,
                LPosOnes[i],
                DV * M);
    }

    for (size_t i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_transpose_in_place(pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT);
    }
}

void PQCLEAN_LEDAKEMLT52_LEAKTIME_niederreiter_encrypt(DIGIT syndrome[],
        const publicKeyNiederreiter_t *pk,
        const DIGIT err[]) {

    DIGIT saux[NUM_DIGITS_GF2X_ELEMENT];

    memset(syndrome, 0x00, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    for (size_t i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_mul(saux,
                pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT,
                err + i * NUM_DIGITS_GF2X_ELEMENT);
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_add(syndrome, syndrome, saux);

    }
    PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_add(syndrome, syndrome, err + (N0 - 1)*NUM_DIGITS_GF2X_ELEMENT);
}

int PQCLEAN_LEDAKEMLT52_LEAKTIME_niederreiter_decrypt(DIGIT *err, const privateKeyNiederreiter_t *sk, const DIGIT *syndrome) {
    AES_XOF_struct niederreiter_decrypt_expander;
    POSITION_T HPosOnes[N0][DV];
    POSITION_T QPosOnes[N0][M];
    POSITION_T LPosOnes[N0][DV * M];
    POSITION_T auxPosOnes[DV * M];
    POSITION_T HtrPosOnes[N0][DV];
    POSITION_T QtrPosOnes[N0][M];
    POSITION_T auxSparse[DV * M];
    POSITION_T Ln0trSparse[DV * M];
    DIGIT err_computed[N0 * NUM_DIGITS_GF2X_ELEMENT] = {0};
    DIGIT err_mockup[N0 * NUM_DIGITS_GF2X_ELEMENT];
    DIGIT privateSyndrome[NUM_DIGITS_GF2X_ELEMENT];
    uint8_t processedQOnes[N0];
    int rejections = sk->rejections;
    int decrypt_ok = 0;
    int err_weight;

    PQCLEAN_LEDAKEMLT52_LEAKTIME_seedexpander_from_trng(&niederreiter_decrypt_expander, sk->prng_seed);
    do {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_generateHPosOnes(HPosOnes, &niederreiter_decrypt_expander);
        PQCLEAN_LEDAKEMLT52_LEAKTIME_generateQPosOnes(QPosOnes, &niederreiter_decrypt_expander);

        for (size_t i = 0; i < N0; i++) {
            for (size_t j = 0; j < DV * M; j++) {
                LPosOnes[i][j] = INVALID_POS_VALUE;
            }
        }

        memset(processedQOnes, 0x00, sizeof(processedQOnes));
        for (size_t colQ = 0; colQ < N0; colQ++) {
            for (size_t i = 0; i < N0; i++) {
                PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_mul_sparse(DV * M, auxPosOnes,
                        DV, HPosOnes[i],
                        qBlockWeights[i][colQ], QPosOnes[i] + processedQOnes[i]);
                PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_add_sparse(DV * M, LPosOnes[colQ],
                        DV * M, LPosOnes[colQ],
                        DV * M, auxPosOnes);
                processedQOnes[i] += qBlockWeights[i][colQ];
            }
        }
        rejections--;
    } while (rejections >= 0);

    PQCLEAN_LEDAKEMLT52_LEAKTIME_transposeHPosOnes(HtrPosOnes, HPosOnes);
    PQCLEAN_LEDAKEMLT52_LEAKTIME_transposeQPosOnes(QtrPosOnes, QPosOnes);

    for (size_t i = 0; i < DV * M; i++) {
        Ln0trSparse[i] = INVALID_POS_VALUE;
        auxSparse[i] = INVALID_POS_VALUE;
    }

    for (size_t i = 0; i < N0; i++) {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_mul_sparse(DV * M, auxSparse,
                DV, HPosOnes[i],
                qBlockWeights[i][N0 - 1], &QPosOnes[i][M - qBlockWeights[i][N0 - 1]]);
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_add_sparse(DV * M, Ln0trSparse,
                DV * M, Ln0trSparse,
                DV * M, auxSparse);
    }
    PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_transpose_in_place_sparse(DV * M, Ln0trSparse);

    PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mod_mul_dense_to_sparse(privateSyndrome,
            syndrome,
            Ln0trSparse,
            DV * M);

    decrypt_ok = PQCLEAN_LEDAKEMLT52_LEAKTIME_bf_decoding(err_computed,
                 (const POSITION_T (*)[DV]) HtrPosOnes,
                 (const POSITION_T (*)[M])  QtrPosOnes,
                 privateSyndrome, sk->secondIterThreshold);

    err_weight = 0;
    for (size_t i = 0 ; i < N0; i++) {
        err_weight += PQCLEAN_LEDAKEMLT52_LEAKTIME_population_count(err_computed + (NUM_DIGITS_GF2X_ELEMENT * i));
    }
    decrypt_ok = decrypt_ok && (err_weight == NUM_ERRORS_T);

    /* prepare mockup error vector in case a decoding failure occurs */
    memcpy(err_mockup, syndrome, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    memcpy(err_mockup + NUM_DIGITS_GF2X_ELEMENT, sk->decryption_failure_secret, TRNG_BYTE_LENGTH);
    memset(((unsigned char *) err_mockup) + (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B) + TRNG_BYTE_LENGTH, 0x00,
           (N0 - 1)*NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B - TRNG_BYTE_LENGTH);

    memcpy(err, err_computed, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);

    // Overwrite on decryption failure
    PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_cmov(err, err_mockup, N0 * NUM_DIGITS_GF2X_ELEMENT, !decrypt_ok);

    return decrypt_ok;
}
