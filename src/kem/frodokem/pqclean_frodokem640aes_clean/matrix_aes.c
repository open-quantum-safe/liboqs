/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: matrix arithmetic functions used by the KEM
*********************************************************************************************/

#include <stdint.h>
#include <string.h>

#include "aes.h"

#include "api.h"
#include "common.h"
#include "params.h"

int PQCLEAN_FRODOKEM640AES_CLEAN_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A) {
    // Generate-and-multiply: generate matrix A (N x N) row-wise, multiply by s on the right.
    // Inputs: s, e (N x N_BAR)
    // Output: out = A*s + e (N x N_BAR)
    int i, j, k;
    int16_t A[PARAMS_N * PARAMS_N] = {0};
    aes128ctx ctx128;

    aes128_keyexp(&ctx128, seed_A);
    for (i = 0; i < PARAMS_N; i++) {
        for (j = 0; j < PARAMS_N; j += PARAMS_STRIPE_STEP) {
            A[i * PARAMS_N + j] = (int16_t) i;                            // Loading values in the little-endian order
            A[i * PARAMS_N + j + 1] = (int16_t) j;
        }
    }
    for (i = 0; i < PARAMS_N * PARAMS_N; i++) {
        A[i] = PQCLEAN_FRODOKEM640AES_CLEAN_UINT16_TO_LE(A[i]);
    }

    aes128_ecb((uint8_t *) A, (uint8_t *) A, PARAMS_N * PARAMS_N * sizeof(int16_t) / AES_BLOCKBYTES, &ctx128);
    aes128_ctx_release(&ctx128);

    for (i = 0; i < PARAMS_N * PARAMS_N; i++) {
        A[i] = PQCLEAN_FRODOKEM640AES_CLEAN_LE_TO_UINT16(A[i]);
    }
    memcpy(out, e, PARAMS_NBAR * PARAMS_N * sizeof(uint16_t));

    for (i = 0; i < PARAMS_N; i++) {                            // Matrix multiplication-addition A*s + e
        for (k = 0; k < PARAMS_NBAR; k++) {
            uint16_t sum = 0;
            for (j = 0; j < PARAMS_N; j++) {
                sum += A[i * PARAMS_N + j] * s[k * PARAMS_N + j];
            }
            out[i * PARAMS_NBAR + k] += sum;                    // Adding e. No need to reduce modulo 2^15, extra bits are taken care of during packing later on.
        }
    }

    return 1;
}


int PQCLEAN_FRODOKEM640AES_CLEAN_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A) {
    // Generate-and-multiply: generate matrix A (N x N) column-wise, multiply by s' on the left.
    // Inputs: s', e' (N_BAR x N)
    // Output: out = s'*A + e' (N_BAR x N)
    int i, j, k;
    int16_t A[PARAMS_N * PARAMS_N] = {0};
    aes128ctx ctx128;

    aes128_keyexp(&ctx128, seed_A);
    for (i = 0; i < PARAMS_N; i++) {
        for (j = 0; j < PARAMS_N; j += PARAMS_STRIPE_STEP) {
            A[i * PARAMS_N + j] = (int16_t) i;                            // Loading values in the little-endian order
            A[i * PARAMS_N + j + 1] = (int16_t) j;
        }
    }
    for (i = 0; i < PARAMS_N * PARAMS_N; i++) {
        A[i] = PQCLEAN_FRODOKEM640AES_CLEAN_UINT16_TO_LE(A[i]);
    }

    aes128_ecb((uint8_t *) A, (uint8_t *) A, PARAMS_N * PARAMS_N * sizeof(int16_t) / AES_BLOCKBYTES, &ctx128);
    aes128_ctx_release(&ctx128);

    for (i = 0; i < PARAMS_N * PARAMS_N; i++) {
        A[i] = PQCLEAN_FRODOKEM640AES_CLEAN_LE_TO_UINT16(A[i]);
    }
    memcpy(out, e, PARAMS_NBAR * PARAMS_N * sizeof(uint16_t));

    for (i = 0; i < PARAMS_N; i++) {                            // Matrix multiplication-addition A*s + e
        for (k = 0; k < PARAMS_NBAR; k++) {
            uint16_t sum = 0;
            for (j = 0; j < PARAMS_N; j++) {
                sum += A[j * PARAMS_N + i] * s[k * PARAMS_N + j];
            }
            out[k * PARAMS_N + i] += sum;                       // Adding e. No need to reduce modulo 2^15, extra bits are taken care of during packing later on.
        }
    }

    return 1;
}
