#include <immintrin.h>

int frodo_mul_add_sa_plus_e_aes_avx2(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
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
                ALIGN_HEADER(32) uint32_t sum[8 * PARAMS_PARALLEL] ALIGN_FOOTER(32);
                __m256i a[PARAMS_PARALLEL], b, acc[PARAMS_PARALLEL];
                acc[0] = _mm256_setzero_si256();
                acc[1] = _mm256_setzero_si256();
                acc[2] = _mm256_setzero_si256();
                acc[3] = _mm256_setzero_si256();
                for (j = 0; j < PARAMS_N; j += 16) {            // Matrix-vector multiplication
                    b = _mm256_load_si256((__m256i*)&s[i*PARAMS_N + j]);
                    a[0] = _mm256_load_si256((__m256i*)&a_cols_t[(k+0)*PARAMS_N + j]);
                    a[0] = _mm256_madd_epi16(a[0], b);
                    acc[0] = _mm256_add_epi16(a[0], acc[0]);
                    a[1] = _mm256_load_si256((__m256i*)&a_cols_t[(k+1)*PARAMS_N + j]);
                    a[1] = _mm256_madd_epi16(a[1], b);
                    acc[1] = _mm256_add_epi16(a[1], acc[1]);
                    a[2] = _mm256_load_si256((__m256i*)&a_cols_t[(k+2)*PARAMS_N + j]);
                    a[2] = _mm256_madd_epi16(a[2], b);
                    acc[2] = _mm256_add_epi16(a[2], acc[2]);
                    a[3] = _mm256_load_si256((__m256i*)&a_cols_t[(k+3)*PARAMS_N + j]);
                    a[3] = _mm256_madd_epi16(a[3], b);
                    acc[3] = _mm256_add_epi16(a[3], acc[3]);
                }
                _mm256_store_si256((__m256i*)(sum + (8*0)), acc[0]);
                out[i*PARAMS_N + kk + k + 0] += sum[8*0 + 0] + sum[8*0 + 1] + sum[8*0 + 2] + sum[8*0 + 3] + sum[8*0 + 4] + sum[8*0 + 5] + sum[8*0 + 6] + sum[8*0 + 7];
                _mm256_store_si256((__m256i*)(sum + (8*1)), acc[1]);
                out[i*PARAMS_N + kk + k + 1] += sum[8*1 + 0] + sum[8*1 + 1] + sum[8*1 + 2] + sum[8*1 + 3] + sum[8*1 + 4] + sum[8*1 + 5] + sum[8*1 + 6] + sum[8*1 + 7];
                _mm256_store_si256((__m256i*)(sum + (8*2)), acc[2]);
                out[i*PARAMS_N + kk + k + 2] += sum[8*2 + 0] + sum[8*2 + 1] + sum[8*2 + 2] + sum[8*2 + 3] + sum[8*2 + 4] + sum[8*2 + 5] + sum[8*2 + 6] + sum[8*2 + 7];
                _mm256_store_si256((__m256i*)(sum + (8*3)), acc[3]);
                out[i*PARAMS_N + kk + k + 3] += sum[8*3 + 0] + sum[8*3 + 1] + sum[8*3 + 2] + sum[8*3 + 3] + sum[8*3 + 4] + sum[8*3 + 5] + sum[8*3 + 6] + sum[8*3 + 7];
            }
        }
    }
    OQS_AES128_free_schedule(aes_key_schedule);
    return 1;
}
