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

#define frodo_mul_add_as_plus_e_actual frodo_mul_add_as_plus_e_portable
#include "frodo_macrify_as_plus_e.c"

int frodo_mul_add_as_plus_e_avx2(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
int frodo_mul_add_sa_plus_e_aes_avx2(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
int frodo_mul_add_sa_plus_e_aes_portable(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
int frodo_mul_add_sa_plus_e_shake_avx2(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
int frodo_mul_add_sa_plus_e_shake_portable(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);

int frodo_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
{ // Generate-and-multiply: generate matrix A (N x N) row-wise, multiply by s on the right.
  // Inputs: s, e (N x N_BAR)
  // Output: out = A*s + e (N x N_BAR)
#if defined(OQS_DIST_X86_64_BUILD)
    if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
      return frodo_mul_add_as_plus_e_avx2(out, s, e, seed_A);
    } else {
      return frodo_mul_add_as_plus_e_portable(out, s, e, seed_A);
    }
#elif defined(OQS_USE_AVX2_INSTRUCTIONS)
  return frodo_mul_add_as_plus_e_avx2(out, s, e, seed_A);
#else
  return frodo_mul_add_as_plus_e_portable(out, s, e, seed_A);
#endif
}

int frodo_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
{ // Generate-and-multiply: generate matrix A (N x N) column-wise, multiply by s' on the left.
  // Inputs: s', e' (N_BAR x N)
  // Output: out = s'*A + e' (N_BAR x N)
#if defined(USE_AES128_FOR_A)
  #if defined(OQS_DIST_X86_64_BUILD)
    if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
        return frodo_mul_add_sa_plus_e_aes_avx2(out, s, e, seed_A);
    } else {
        return frodo_mul_add_sa_plus_e_aes_portable(out, s, e, seed_A);
    }
  #elif defined(OQS_USE_AVX2_INSTRUCTIONS)
    return frodo_mul_add_sa_plus_e_aes_avx2(out, s, e, seed_A);
  #else
    return frodo_mul_add_sa_plus_e_aes_portable(out, s, e, seed_A);
  #endif
#elif defined(USE_SHAKE128_FOR_A)
  #if defined(OQS_DIST_X86_64_BUILD)
    if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
        return frodo_mul_add_sa_plus_e_shake_avx2(out, s, e, seed_A);
    } else {
        return frodo_mul_add_sa_plus_e_shake_portable(out, s, e, seed_A);
    }
  #elif defined(OQS_USE_AVX2_INSTRUCTIONS)
    return frodo_mul_add_sa_plus_e_shake_avx2(out, s, e, seed_A);
  #else
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
        for(j = 0; j < PARAMS_EXTRACTED_BITS; j++) {
            temp |= ((uint64_t)((uint8_t*)in)[i*PARAMS_EXTRACTED_BITS + j]) << (8*j);
        }
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
        for (j = 0; j < PARAMS_EXTRACTED_BITS; j++) {
            pos[i*PARAMS_EXTRACTED_BITS + j] = (templong >> (8*j)) & 0xFF;
        }
    }
}
