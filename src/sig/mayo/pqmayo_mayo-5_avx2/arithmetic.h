
// SPDX-License-Identifier: Apache-2.0

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <stdint.h>
#include <mayo.h>
#include <stdint.h>

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define TARGET_BIG_ENDIAN
#endif

#if defined(MAYO_AVX) && (M_MAX == 64)
    #include <shuffle_arithmetic_64.h>
#endif
#if defined(MAYO_AVX) && (M_MAX == 96)
    #include <shuffle_arithmetic_96.h>
#endif
#if defined(MAYO_AVX) && (M_MAX == 128)
    #include <shuffle_arithmetic_128.h>
#endif
#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (M_MAX == 64))
    #include <arithmetic_64.h>
    #include <arithmetic_96.h>
#endif
#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (M_MAX == 96))
    #include <arithmetic_96.h>
    #include <arithmetic_128.h>
#endif
#if !defined(MAYO_VARIANT) || (defined(MAYO_VARIANT) && (M_MAX == 128))
    #include <arithmetic_128.h>
#endif

// Calculate P3 = O^T * (P1*O + P2) in KeyGen
#define Ot_times_P1O_P2 MAYO_NAMESPACE(Ot_times_P1O_P2)
void Ot_times_P1O_P2(const mayo_params_t* p, const uint64_t* P1, const unsigned char* O, uint64_t* P1O_P2, uint64_t* P3);

// Calculate Upper in KeyGen
#define m_upper MAYO_NAMESPACE(m_upper)
void m_upper(int m_legs, const uint64_t *in, uint64_t *out, int size);

// Calculate acc = (P1+P1^T)*O in expand_sk
#define P1P1t_times_O MAYO_NAMESPACE(P1P1t_times_O)
void P1P1t_times_O(const mayo_params_t* p, const uint64_t* P1P1t, const unsigned char* O, uint64_t* acc);

// Calculate M=V*L and Y=V*P1*V^T in Sign
#define V_times_L__V_times_P1_times_Vt MAYO_NAMESPACE(V_times_L__V_times_P1_times_Vt)
void V_times_L__V_times_P1_times_Vt(const mayo_params_t* p, const uint64_t* L, const unsigned char* V, uint64_t* M, const uint64_t* P1, uint64_t* Y);

// Sample solution in Sign
#define sample_solution MAYO_NAMESPACE(sample_solution)
int sample_solution(const mayo_params_t *p, unsigned char *A, const unsigned char *y, const unsigned char *r, unsigned char *x, int k, int o, int m, int A_cols);

// Calculate SPS = S*P*S^T in Verify
#define m_calculate_PS_SPS MAYO_NAMESPACE(m_calculate_PS_SPS)
void m_calculate_PS_SPS(const uint64_t *P1, const uint64_t *P2, const uint64_t *P3, const unsigned char *S,
                              const int m, const int v, const int o, const int k, uint64_t *SPS);

#endif

