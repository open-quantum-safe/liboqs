#include <oqs/sha3x4.h>

#include <string.h>
#include <immintrin.h>

int frodo_mul_add_sa_plus_e_shake_avx2(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
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
            __m256i a, b0, b1, b2, b3, acc[PARAMS_N/16];
            b0 = _mm256_set1_epi16(s[i*PARAMS_N + kk + 0]);
            b1 = _mm256_set1_epi16(s[i*PARAMS_N + kk + 1]);
            b2 = _mm256_set1_epi16(s[i*PARAMS_N + kk + 2]);
            b3 = _mm256_set1_epi16(s[i*PARAMS_N + kk + 3]);
            for (j = 0; j < PARAMS_N; j+=16) {                  // Matrix-vector multiplication
                acc[j/16] = _mm256_load_si256((__m256i*)&out[i*PARAMS_N + j]);
                a = _mm256_load_si256((__m256i*)&a_cols[(t+0)*PARAMS_N + j]);
                a = _mm256_mullo_epi16(a, b0);
                acc[j/16] = _mm256_add_epi16(a, acc[j/16]);
                a = _mm256_load_si256((__m256i*)&a_cols[(t+1)*PARAMS_N + j]);
                a = _mm256_mullo_epi16(a, b1);
                acc[j/16] = _mm256_add_epi16(a, acc[j/16]);
                a = _mm256_load_si256((__m256i*)&a_cols[(t+2)*PARAMS_N + j]);
                a = _mm256_mullo_epi16(a, b2);
                acc[j/16] = _mm256_add_epi16(a, acc[j/16]);
                a = _mm256_load_si256((__m256i*)&a_cols[(t+3)*PARAMS_N + j]);
                a = _mm256_mullo_epi16(a, b3);
                acc[j/16] = _mm256_add_epi16(a, acc[j/16]);
            }
            for (j = 0; j < PARAMS_N/16; j++) {
                _mm256_store_si256((__m256i*)&out[i*PARAMS_N + 16*j], acc[j]);
            }
        }
    }

    return 1;
}
