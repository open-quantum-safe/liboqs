#include <oqs/sha3x4.h>

int frodo_mul_add_as_plus_e_actual(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
{ // Generate-and-multiply: generate matrix A (N x N) row-wise, multiply by s on the right.
  // Inputs: s, e (N x N_BAR)
  // Output: out = A*s + e (N x N_BAR)
    int i, j, k;
    ALIGN_HEADER(32) int16_t a_row[4*PARAMS_N] ALIGN_FOOTER(32) = {0};

    for (i = 0; i < (PARAMS_N*PARAMS_NBAR); i += 2) {
        *((uint32_t*)&out[i]) = *((uint32_t*)&e[i]);
    }

#if defined(USE_AES128_FOR_A)
    int16_t a_row_temp[4*PARAMS_N] = {0};                       // Take four lines of A at once
    uint8_t *aes_key_schedule;
    OQS_AES128_ECB_load_schedule(seed_A, (void **) &aes_key_schedule);

    for (j = 0; j < PARAMS_N; j += PARAMS_STRIPE_STEP) {
        a_row_temp[j + 1 + 0*PARAMS_N] = UINT16_TO_LE(j);       // Loading values in the little-endian order
        a_row_temp[j + 1 + 1*PARAMS_N] = UINT16_TO_LE(j);
        a_row_temp[j + 1 + 2*PARAMS_N] = UINT16_TO_LE(j);
        a_row_temp[j + 1 + 3*PARAMS_N] = UINT16_TO_LE(j);
    }

    for (i = 0; i < PARAMS_N; i += 4) {
        for (j = 0; j < PARAMS_N; j += PARAMS_STRIPE_STEP) {    // Go through A, four rows at a time
            a_row_temp[j + 0*PARAMS_N] = UINT16_TO_LE(i+0);     // Loading values in the little-endian order
            a_row_temp[j + 1*PARAMS_N] = UINT16_TO_LE(i+1);
            a_row_temp[j + 2*PARAMS_N] = UINT16_TO_LE(i+2);
            a_row_temp[j + 3*PARAMS_N] = UINT16_TO_LE(i+3);
        }

        OQS_AES128_ECB_enc_sch((uint8_t*)a_row_temp, 4*PARAMS_N*sizeof(int16_t), aes_key_schedule, (uint8_t*)a_row);
#elif defined (USE_SHAKE128_FOR_A)
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
    for (i = 0; i < PARAMS_N; i += 4) {
        seed_A_origin_0[0] = UINT16_TO_LE(i + 0);
        seed_A_origin_1[0] = UINT16_TO_LE(i + 1);
        seed_A_origin_2[0] = UINT16_TO_LE(i + 2);
        seed_A_origin_3[0] = UINT16_TO_LE(i + 3);
        OQS_SHA3_shake128_x4((unsigned char*)(a_row), (unsigned char*)(a_row + PARAMS_N), (unsigned char*)(a_row + 2*PARAMS_N), (unsigned char*)(a_row + 3*PARAMS_N),
                    (unsigned long long)(2*PARAMS_N), seed_A_separated_0, seed_A_separated_1, seed_A_separated_2, seed_A_separated_3, 2 + BYTES_SEED_A);
#endif /* USE_AES128_FOR_A */
        for (k = 0; k < 4 * PARAMS_N; k++) {
            a_row[k] = LE_TO_UINT16(a_row[k]);
        }
        for (k = 0; k < PARAMS_NBAR; k++) {
            uint16_t sum[4] = {0};
            for (j = 0; j < PARAMS_N; j++) {                    // Matrix-vector multiplication
                uint16_t sp = s[k*PARAMS_N + j];
                sum[0] += a_row[0*PARAMS_N + j] * sp;           // Go through four lines with same s
                sum[1] += a_row[1*PARAMS_N + j] * sp;
                sum[2] += a_row[2*PARAMS_N + j] * sp;
                sum[3] += a_row[3*PARAMS_N + j] * sp;
            }
            out[(i+0)*PARAMS_NBAR + k] += sum[0];
            out[(i+2)*PARAMS_NBAR + k] += sum[2];
            out[(i+1)*PARAMS_NBAR + k] += sum[1];
            out[(i+3)*PARAMS_NBAR + k] += sum[3];
        }
    }

#if defined(USE_AES128_FOR_A)
    OQS_AES128_free_schedule(aes_key_schedule);
#endif
    return 1;
}

