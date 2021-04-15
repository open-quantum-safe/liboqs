int frodo_mul_add_sa_plus_e_aes_portable(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
{ // Generate-and-multiply: generate matrix A (N x N) column-wise, multiply by s' on the left.
  // Inputs: s', e' (N_BAR x N)
  // Output: out = s'*A + e' (N_BAR x N)
    int i, j, kk;

    for (i = 0; i < (PARAMS_N*PARAMS_NBAR); i += 2) {
        *((uint32_t*)&out[i]) = *((uint32_t*)&e[i]);
    }

    int k;
    uint16_t a_cols[PARAMS_N*PARAMS_STRIPE_STEP] = {0};
    ALIGN_HEADER(32) uint16_t a_cols_t[PARAMS_N*PARAMS_STRIPE_STEP] ALIGN_FOOTER(32) = {0};
    uint16_t a_cols_temp[PARAMS_N*PARAMS_STRIPE_STEP] = {0};
    uint8_t *aes_key_schedule;
    OQS_AES128_ECB_load_schedule(seed_A, (void **) &aes_key_schedule);

    for (i = 0, j = 0; i < PARAMS_N; i++, j += PARAMS_STRIPE_STEP) {
        a_cols_temp[j] = UINT16_TO_LE(i);                       // Loading values in the little-endian order
    }

    for (kk = 0; kk < PARAMS_N; kk += PARAMS_STRIPE_STEP) {     // Go through A's columns, 8 (== PARAMS_STRIPE_STEP) columns at a time.
        for (i = 0; i < (PARAMS_N*PARAMS_STRIPE_STEP); i += PARAMS_STRIPE_STEP) {
            a_cols_temp[i + 1] = UINT16_TO_LE(kk);              // Loading values in the little-endian order
        }

        OQS_AES128_ECB_enc_sch((uint8_t*)a_cols_temp, PARAMS_N*PARAMS_STRIPE_STEP*sizeof(int16_t), aes_key_schedule, (uint8_t*)a_cols);

        for (i = 0; i < PARAMS_N; i++) {                        // Transpose a_cols to have access to it in the column-major order.
            for (k = 0; k < PARAMS_STRIPE_STEP; k++) {
                a_cols_t[k*PARAMS_N + i] = LE_TO_UINT16(a_cols[i*PARAMS_STRIPE_STEP + k]);
            }
        }

        for (i = 0; i < PARAMS_NBAR; i++) {
            for (k = 0; k < PARAMS_STRIPE_STEP; k += PARAMS_PARALLEL) {
                uint16_t sum[PARAMS_PARALLEL] = {0};
                for (j = 0; j < PARAMS_N; j++) {                // Matrix-vector multiplication
                    uint16_t sp = s[i*PARAMS_N + j];
                    sum[0] += (uint16_t) ((uint32_t) sp * (uint32_t) a_cols_t[(k+0)*PARAMS_N + j]);
                    sum[1] += (uint16_t) ((uint32_t) sp * (uint32_t) a_cols_t[(k+1)*PARAMS_N + j]);
                    sum[2] += (uint16_t) ((uint32_t) sp * (uint32_t) a_cols_t[(k+2)*PARAMS_N + j]);
                    sum[3] += (uint16_t) ((uint32_t) sp * (uint32_t) a_cols_t[(k+3)*PARAMS_N + j]);
                }
                out[i*PARAMS_N + kk + k + 0] += sum[0];
                out[i*PARAMS_N + kk + k + 2] += sum[2];
                out[i*PARAMS_N + kk + k + 1] += sum[1];
                out[i*PARAMS_N + kk + k + 3] += sum[3];
            }
        }
    }
    OQS_AES128_free_schedule(aes_key_schedule);
    return 1;
}
