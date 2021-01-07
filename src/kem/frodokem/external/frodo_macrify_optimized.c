/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: matrix arithmetic functions used by the KEM
*********************************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <oqs/oqs.h>

#include <oqs/aes.h>

#include "frodo_internal.h"

int frodo_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
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
    OQS_AES128_ECB_load_schedule(seed_A, (void **) &aes_key_schedule, 1);

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
        OQS_SHA3_shake128_4x((unsigned char*)(a_row), (unsigned char*)(a_row + PARAMS_N), (unsigned char*)(a_row + 2*PARAMS_N), (unsigned char*)(a_row + 3*PARAMS_N),
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


#if defined(USE_AES128_FOR_A)
 #if defined(OQS_PORTABLE_BUILD) || !(defined(OQS_USE_AES_INSTRUCTIONS) && defined(OQS_USE_AVX2_INSTRUCTIONS))
  #include "frodo_mul_add_sa_plus_e_aes_portable.c"
 #endif
 #if defined(OQS_USE_AES_INSTRUCTIONS) && defined(OQS_USE_AVX2_INSTRUCTIONS)
  #include "frodo_mul_add_sa_plus_e_aes_avx2.c"
 #endif
#else // USE_SHAKE128_FOR_A
 #if defined(OQS_PORTABLE_BUILD) || !defined(OQS_USE_AVX2_INSTRUCTIONS)
  #include "frodo_mul_add_sa_plus_e_shake_portable.c"
 #endif
 #if defined(OQS_USE_AVX2_INSTRUCTIONS)
  #include "frodo_mul_add_sa_plus_e_shake_avx2.c"
 #endif
#endif

int frodo_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
{ // Generate-and-multiply: generate matrix A (N x N) column-wise, multiply by s' on the left.
  // Inputs: s', e' (N_BAR x N)
  // Output: out = s'*A + e' (N_BAR x N)
#if defined(USE_AES128_FOR_A)
 #if defined(OQS_USE_AES_INSTRUCTIONS) && defined(OQS_USE_AVX2_INSTRUCTIONS)
  #if defined(OQS_PORTABLE_BUILD)
    OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
    if (available_cpu_extensions.AES_ENABLED && available_cpu_extensions.AVX2_ENABLED) {
        return frodo_mul_add_sa_plus_e_aes_avx2(out, s, e, seed_A);
    } else {
        return frodo_mul_add_sa_plus_e_aes_portable(out, s, e, seed_A);
    }
  #else // OQS_USE_AES_INSTRUCTIONS && OQS_USE_AVX2_INSTRUCTIONS && !(OQS_PORTABLE_BUILD)
    return frodo_mul_add_sa_plus_e_aes_avx2(out, s, e, seed_A);
  #endif
 #else // !(OQS_USE_AES_INSTRUCTIONS && OQS_USE_AVX2_INSTRUCTIONS)
    return frodo_mul_add_sa_plus_e_aes_portable(out, s, e, seed_A);
 #endif
#else // USE_SHAKE128_FOR_A
 #if defined(OQS_USE_AVX2_INSTRUCTIONS)
  #if defined(OQS_PORTABLE_BUILD)
   OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
   if (available_cpu_extensions.AVX2_ENABLED) {
       return frodo_mul_add_sa_plus_e_shake_avx2(out, s, e, seed_A);
   } else {
       return frodo_mul_add_sa_plus_e_shake_portable(out, s, e, seed_A);
   }
  #else // OQS_USE_AVX2_INSTRUCTIONS && !(OQS_PORTABLE_BUILD)
   return frodo_mul_add_sa_plus_e_shake_avx2(out, s, e, seed_A);
  #endif
 #else // !(OQS_USE_AVX2_INSTRUCTIONS)
   return frodo_mul_add_sa_plus_e_shake_portable(out, s, e, seed_A);
 #endif
#endif
}

void frodo_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s)
{ // Multiply by s on the right
  // Inputs: b (N_BAR x N), s (N x N_BAR)
  // Output: out = b*s (N_BAR x N_BAR)
    int i, j, k;

    for (i = 0; i < PARAMS_NBAR; i++) {
        for (j = 0; j < PARAMS_NBAR; j++) {
            out[i*PARAMS_NBAR + j] = 0;
            for (k = 0; k < PARAMS_N; k++) {
                out[i*PARAMS_NBAR + j] += (uint16_t) ((uint32_t) b[i*PARAMS_N + k] * (uint32_t) s[j*PARAMS_N + k]);
            }
            out[i*PARAMS_NBAR + j] = (uint32_t)(out[i*PARAMS_NBAR + j]) & ((1<<PARAMS_LOGQ)-1);
        }
    }
}


void frodo_mul_add_sb_plus_e(uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e)
{ // Multiply by s on the left
  // Inputs: b (N x N_BAR), s (N_BAR x N), e (N_BAR x N_BAR)
  // Output: out = s*b + e (N_BAR x N_BAR)
    int i, j, k;

    for (k = 0; k < PARAMS_NBAR; k++) {
        for (i = 0; i < PARAMS_NBAR; i++) {
            out[k*PARAMS_NBAR + i] = e[k*PARAMS_NBAR + i];
            for (j = 0; j < PARAMS_N; j++) {
                out[k*PARAMS_NBAR + i] += (uint16_t) ((uint32_t) s[k*PARAMS_N + j] * (uint32_t) b[j*PARAMS_NBAR + i]);
            }
            out[k*PARAMS_NBAR + i] = (uint32_t)(out[k*PARAMS_NBAR + i]) & ((1<<PARAMS_LOGQ)-1);
        }
    }
}


void frodo_add(uint16_t *out, const uint16_t *a, const uint16_t *b)
{ // Add a and b
  // Inputs: a, b (N_BAR x N_BAR)
  // Output: c = a + b

    for (int i = 0; i < (PARAMS_NBAR*PARAMS_NBAR); i++) {
        out[i] = (a[i] + b[i]) & ((1<<PARAMS_LOGQ)-1);
    }
}


void frodo_sub(uint16_t *out, const uint16_t *a, const uint16_t *b)
{ // Subtract a and b
  // Inputs: a, b (N_BAR x N_BAR)
  // Output: c = a - b

    for (int i = 0; i < (PARAMS_NBAR*PARAMS_NBAR); i++) {
        out[i] = (a[i] - b[i]) & ((1<<PARAMS_LOGQ)-1);
    }
}


void frodo_key_encode(uint16_t *out, const uint16_t *in)
{ // Encoding
    unsigned int i, j, npieces_word = 8;
    unsigned int nwords = (PARAMS_NBAR*PARAMS_NBAR)/8;
    uint64_t temp, mask = ((uint64_t)1 << PARAMS_EXTRACTED_BITS) - 1;
    uint16_t* pos = out;

    for (i = 0; i < nwords; i++) {
        temp = 0;
        for(j = 0; j < PARAMS_EXTRACTED_BITS; j++)
            temp |= ((uint64_t)((uint8_t*)in)[i*PARAMS_EXTRACTED_BITS + j]) << (8*j);
        for (j = 0; j < npieces_word; j++) {
            *pos = (uint16_t)((temp & mask) << (PARAMS_LOGQ - PARAMS_EXTRACTED_BITS));
            temp >>= PARAMS_EXTRACTED_BITS;
            pos++;
        }
    }
}


void frodo_key_decode(uint16_t *out, const uint16_t *in)
{ // Decoding
    unsigned int i, j, index = 0, npieces_word = 8;
    unsigned int nwords = (PARAMS_NBAR * PARAMS_NBAR) / 8;
    uint16_t temp, maskex=((uint16_t)1 << PARAMS_EXTRACTED_BITS) -1, maskq =((uint16_t)1 << PARAMS_LOGQ) -1;
    uint8_t  *pos = (uint8_t*)out;
    uint64_t templong;

    for (i = 0; i < nwords; i++) {
        templong = 0;
        for (j = 0; j < npieces_word; j++) {  // temp = floor(in*2^{-11}+0.5)
            temp = ((in[index] & maskq) + (1 << (PARAMS_LOGQ - PARAMS_EXTRACTED_BITS - 1))) >> (PARAMS_LOGQ - PARAMS_EXTRACTED_BITS);
            templong |= ((uint64_t)(temp & maskex)) << (PARAMS_EXTRACTED_BITS * j);
            index++;
        }
	for(j = 0; j < PARAMS_EXTRACTED_BITS; j++)
	    pos[i*PARAMS_EXTRACTED_BITS + j] = (templong >> (8*j)) & 0xFF;
    }
}
