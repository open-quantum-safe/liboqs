#include <oqs/sha3x4.h>

int frodo_mul_add_sa_plus_e_shake_portable(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
{ // Generate-and-multiply: generate matrix A (N x N) column-wise, multiply by s' on the left.
  // Inputs: s', e' (N_BAR x N)
  // Output: out = s'*A + e' (N_BAR x N)
    int i, j, kk;

    for (i = 0; i < (PARAMS_N*PARAMS_NBAR); i += 2) {
        *((uint32_t*)&out[i]) = *((uint32_t*)&e[i]);
    }

    int t=0;
    ALIGN_HEADER(32) uint16_t a_cols[4*PARAMS_N] ALIGN_FOOTER(32) = {0};

    uint8_t seed_A_separated_0[2 + BYTES_SEED_A];
    uint8_t seed_A_separated_1[2 + BYTES_SEED_A];
    uint8_t seed_A_separated_2[2 + BYTES_SEED_A];
    uint8_t seed_A_separated_3[2 + BYTES_SEED_A];
    uint16_t* seed_A_origin_0 = (uint16_t*)&seed_A_separated_0;
    uint16_t* seed_A_origin_1 = (uint16_t*)&seed_A_separated_1;
    uint16_t* seed_A_origin_2 = (uint16_t*)&seed_A_separated_2;
    uint16_t* seed_A_origin_3 = (uint16_t*)&seed_A_separated_3;
    memcpy(&seed_A_separated_0[2], seed_A, BYTES_SEED_A);
    memcpy(&seed_A_separated_1[2], seed_A, BYTES_SEED_A);
    memcpy(&seed_A_separated_2[2], seed_A, BYTES_SEED_A);
    memcpy(&seed_A_separated_3[2], seed_A, BYTES_SEED_A);

    for (kk = 0; kk < PARAMS_N; kk+=4) {
        seed_A_origin_0[0] = UINT16_TO_LE(kk + 0);
        seed_A_origin_1[0] = UINT16_TO_LE(kk + 1);
        seed_A_origin_2[0] = UINT16_TO_LE(kk + 2);
        seed_A_origin_3[0] = UINT16_TO_LE(kk + 3);
        OQS_SHA3_shake128_x4((unsigned char*)(a_cols), (unsigned char*)(a_cols + PARAMS_N), (unsigned char*)(a_cols + 2*PARAMS_N), (unsigned char*)(a_cols + 3*PARAMS_N),
        (unsigned long long)(2*PARAMS_N), seed_A_separated_0, seed_A_separated_1, seed_A_separated_2, seed_A_separated_3, 2 + BYTES_SEED_A);

        for (i = 0; i < PARAMS_NBAR; i++) {
            uint16_t sum[PARAMS_N] = {0};
            for (j = 0; j < 4; j++) {
                uint16_t sp = s[i*PARAMS_N + kk + j];
                for (int k = 0; k < PARAMS_N; k++) {                // Matrix-vector multiplication
                    sum[k] += (uint16_t) ((uint32_t) sp * (uint32_t) UINT16_TO_LE(a_cols[(t+j)*PARAMS_N + k]));
                }
             }
            for (int k = 0; k < PARAMS_N; k++){
                out[i*PARAMS_N + k] += sum[k];
            }
        }
    }

    return 1;
}
