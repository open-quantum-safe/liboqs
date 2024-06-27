// SPDX-License-Identifier: Apache-2.0

#ifndef ARITHMETIC_COMMON_H
#define ARITHMETIC_COMMON_H

#include <stdalign.h>

#ifndef MAYO_VARIANT
static void m_multiply_bins(const int m_legs, uint64_t *bins, uint64_t *out) {

    m_vec_add(m_legs, bins + 15 * m_legs * 2, bins + 12 * m_legs * 2);
    m_vec_add(m_legs, bins + 15 * m_legs * 2, bins +  3 * m_legs * 2);

    m_vec_add(m_legs, bins + 14 * m_legs * 2, bins +  8 * m_legs * 2);
    m_vec_add(m_legs, bins + 14 * m_legs * 2, bins +  6 * m_legs * 2);

    m_vec_add(m_legs, bins + 13 * m_legs * 2, bins + 10 * m_legs * 2);
    m_vec_add(m_legs, bins + 13 * m_legs * 2, bins +  7 * m_legs * 2);

    m_vec_add(m_legs, bins + 12 * m_legs * 2, bins +  8 * m_legs * 2);
    m_vec_add(m_legs, bins + 12 * m_legs * 2, bins +  4 * m_legs * 2);

    m_vec_add(m_legs, bins + 11 * m_legs * 2, bins +  9 * m_legs * 2);
    m_vec_add(m_legs, bins + 11 * m_legs * 2, bins +  2 * m_legs * 2);

    m_vec_add(m_legs, bins + 10 * m_legs * 2, bins +  8 * m_legs * 2);
    m_vec_add(m_legs, bins + 10 * m_legs * 2, bins +  2 * m_legs * 2);

    m_vec_add(m_legs, bins + 9 * m_legs * 2, bins +  8 * m_legs * 2);
    m_vec_add(m_legs, bins + 9 * m_legs * 2, bins +  1 * m_legs * 2);

    m_vec_add(m_legs, bins + 7 * m_legs * 2, bins +  4 * m_legs * 2);
    m_vec_add(m_legs, bins + 7 * m_legs * 2, bins +  3 * m_legs * 2);

    m_vec_add(m_legs, bins + 6 * m_legs * 2, bins +  4 * m_legs * 2);
    m_vec_add(m_legs, bins + 6 * m_legs * 2, bins +  2 * m_legs * 2);

    m_vec_add(m_legs, bins + 5 * m_legs * 2, bins +  4 * m_legs * 2);
    m_vec_add(m_legs, bins + 5 * m_legs * 2, bins +  1 * m_legs * 2);

    m_vec_add(m_legs, bins + 3 * m_legs * 2, bins +  2 * m_legs * 2);
    m_vec_add(m_legs, bins + 3 * m_legs * 2, bins +  1 * m_legs * 2);

    m_vec_mul_add_x(m_legs, bins + 8 * m_legs * 2, bins + 4 * m_legs * 2);
    m_vec_mul_add_x(m_legs, bins + 4 * m_legs * 2, bins + 2 * m_legs * 2);
    m_vec_mul_add_x(m_legs, bins + 2 * m_legs * 2, bins + 1 * m_legs * 2);

    m_vec_copy(m_legs, bins + 1 * m_legs * 2, out);
}
#endif

// compute P * S^t = [ P1  P2 ] * [S1] = [P1*S1 + P2*S2]
//                   [  0  P3 ]   [S2]   [        P3*S2]
static inline void mayo_generic_m_calculate_PS(const uint64_t *P1, const uint64_t *P2, const uint64_t *P3, const unsigned char *S,
                              const int m, const int v, const int o, const int k, uint64_t *PS) {

    const int n = o + v;
#if defined(MAYO_VARIANT) && ((M_MAX == 64) || (M_MAX == 96) || M_MAX == 128)
    (void)m;
#else
    const int m_legs = m / 32;
#endif

    /* Old approach which is constant time but doesn't have to be
    unsigned char S1[V_MAX*K_MAX];
    unsigned char S2[O_MAX*K_MAX];
    unsigned char *s1_write = S1;
    unsigned char *s2_write = S2;
    for (int r=0; r < k; r++)
    {
        for (int c = 0; c < n; c++)
        {
            if(c < v){
                *(s1_write++) = S[r*n + c];
            } else {
                *(s2_write++) = S[r*n + c];
            }
        }
    }

    mul_add_m_upper_triangular_mat_x_mat_trans(m_legs, P1, S1, PS, v, v, k, 1); // P1 * S1
    mul_add_m_upper_triangular_mat_x_mat_trans(m_legs, P2, S2, PS, v, o, k, 0); // P2 * S2
    mul_add_m_upper_triangular_mat_x_mat_trans(m_legs, P3, S2, PS + v*k*m_legs*4, o, o, k, 1); // P3 * S2.
    */

    // use more stack efficient version for MAYO_3 and MAYO_5
    #if (defined(HAVE_STACKEFFICIENT) || defined(PQM4)) && N_MAX > 78
    uint64_t accumulator[M_MAX * N_MAX] = {0};
    int P1_used;
    int P3_used;
    for (int col = 0; col < k; col++) {
        for(unsigned int i = 0; i < sizeof(accumulator)/8; i++) {
            accumulator[i] = 0;
        }
        P1_used = 0;
        for (int row = 0; row < v; row++) {
            for (int j = row; j < v; j++) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
                vec_add_64(P1 + P1_used * 4, accumulator + ( row * 16 + S[col * n + j] ) * 4 );
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
                vec_add_96(P1 + P1_used * 6, accumulator + ( row * 16 + S[col * n + j] ) * 6);
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
                vec_add_128(P1 + P1_used * 8, accumulator + ( row * 16 + S[col * n + j] ) * 8);
#else
                bitsliced_m_vec_add(m_legs, P1 + P1_used * m_legs * 2, accumulator + ( row * 16 + S[col * n + j] )*m_legs * 2 );
#endif
                P1_used ++;
            }

            for (int j = 0; j < o; j++) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
                vec_add_64(P2 + (row * o + j)*4, accumulator + ( row * 16 + S[(col * n) + j + v] )*4 );
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
                vec_add_96(P2 + (row * o + j)*6, accumulator + ( row * 16 + S[(col * n) + j + v] )*6);
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
                vec_add_128(P2 + (row * o + j)*8, accumulator + ( row * 16 + S[(col * n) + j + v] )*8 );
#else
                bitsliced_m_vec_add(m_legs, P2 + (row * o + j)*m_legs * 2, accumulator + ( row * 16 + S[(col * n) + j + v] )*m_legs * 2 );
#endif
            }
        }

        P3_used = 0;
        for (int row = v; row < n; row++) {
            for (int j = row; j < n; j++) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
                vec_add_64(P3 + P3_used * 4, accumulator + ( row * 16 + S[col * n + j] )*4);
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
                vec_add_96(P3 + P3_used * 6, accumulator + ( row * 16 + S[col * n + j] )*6);
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
                vec_add_128(P3 + P3_used * 8, accumulator + ( row * 16 + S[col * n + j] )*8);
#else
                bitsliced_m_vec_add(m_legs, P3 + P3_used * m_legs * 2, accumulator + ( row * 16 + S[col * n + j] )*m_legs * 2 );
#endif
                P3_used ++;
            }
        }

        for (int row = 0; row < n; row++) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
           multiply_bins_64(accumulator + row * 16 * 4, PS + (row * k + col) * 4);
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
           multiply_bins_96(accumulator + row * 16 * 6, PS + (row * k + col) * 6);
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
           multiply_bins_128(accumulator + row * 16 * 8, PS + (row * k + col) * 8);
#else
           bitsliced_m_multiply_bins(m_legs, accumulator + row * 16 * m_legs * 2, PS + (row * k + col) * m_legs * 2);
#endif
        }
    }

    #else

    alignas (32) uint64_t accumulator[M_MAX * K_MAX * N_MAX] = {0};
    int P1_used = 0;
    for (int row = 0; row < v; row++) {
        for (int j = row; j < v; j++) {
#if defined(MAYO_VARIANT) && (M_MAX == 64) && (K_MAX == 9)
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 0) * 16 + S[0 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 1) * 16 + S[1 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 2) * 16 + S[2 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 3) * 16 + S[3 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 4) * 16 + S[4 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 5) * 16 + S[5 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 6) * 16 + S[6 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 7) * 16 + S[7 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 8) * 16 + S[8 * n + j] ) * 4 );
#elif defined(MAYO_VARIANT) && (M_MAX == 64) && (K_MAX == 4)
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 0) * 16 + S[0 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 1) * 16 + S[1 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 2) * 16 + S[2 * n + j] ) * 4 );
            vec_add_64(P1 + P1_used * 4, accumulator + ( (row * k + 3) * 16 + S[3 * n + j] ) * 4 );
#elif defined(MAYO_VARIANT) && (M_MAX == 96) && (K_MAX == 11)
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 0) * 16 + S[0 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 1) * 16 + S[1 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 2) * 16 + S[2 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 3) * 16 + S[3 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 4) * 16 + S[4 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 5) * 16 + S[5 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 6) * 16 + S[6 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 7) * 16 + S[7 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 8) * 16 + S[8 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 9) * 16 + S[9 * n + j] ) * 6 );
            vec_add_96(P1 + P1_used * 6, accumulator + ( (row * k + 10) * 16 + S[10 * n + j] ) * 6 );
#elif defined(MAYO_VARIANT) && (M_MAX == 128) && (K_MAX == 12)
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 0) * 16 + S[0 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 1) * 16 + S[1 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 2) * 16 + S[2 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 3) * 16 + S[3 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 4) * 16 + S[4 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 5) * 16 + S[5 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 6) * 16 + S[6 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 7) * 16 + S[7 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 8) * 16 + S[8 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 9) * 16 + S[9 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 10) * 16 + S[10 * n + j] ) * 8 );
            vec_add_128(P1 + P1_used * 8, accumulator + ( (row * k + 11) * 16 + S[11 * n + j] ) * 8 );
#else
            for (int col = 0; col < k; col++) {
                m_vec_add(m_legs, P1 + P1_used * m_legs * 2, accumulator + ( (row * k + col) * 16 + S[col * n + j] )*m_legs * 2 );
            }
#endif
            P1_used ++;
        }


        for (int j = 0; j < o; j++) {
#if defined(MAYO_VARIANT) && (M_MAX == 64) && (K_MAX == 9)
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 0) * 16 + S[(0 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 1) * 16 + S[(1 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 2) * 16 + S[(2 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 3) * 16 + S[(3 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 4) * 16 + S[(4 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 5) * 16 + S[(5 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 6) * 16 + S[(6 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 7) * 16 + S[(7 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 8) * 16 + S[(8 * n) + j + v] ) * 4 );
#elif defined(MAYO_VARIANT) && (M_MAX == 64) && (K_MAX == 4)
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 0) * 16 + S[(0 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 1) * 16 + S[(1 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 2) * 16 + S[(2 * n) + j + v] ) * 4 );
            vec_add_64(P2 + (row * o + j) * 4, accumulator + ( (row * k + 3) * 16 + S[(3 * n) + j + v] ) * 4 );
#elif defined(MAYO_VARIANT) && (M_MAX == 96) && (K_MAX == 11)
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 0) * 16 + S[(0 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 1) * 16 + S[(1 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 2) * 16 + S[(2 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 3) * 16 + S[(3 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 4) * 16 + S[(4 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 5) * 16 + S[(5 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 6) * 16 + S[(6 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 7) * 16 + S[(7 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 8) * 16 + S[(8 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 9) * 16 + S[(9 * n) + j + v] ) * 6 );
            vec_add_96(P2 + (row * o + j) * 6, accumulator + ( (row * k + 10) * 16 + S[(10 * n) + j + v] ) * 6 );
#elif defined(MAYO_VARIANT) && (M_MAX == 128) && (K_MAX == 12)
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 0) * 16 + S[(0 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 1) * 16 + S[(1 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 2) * 16 + S[(2 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 3) * 16 + S[(3 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 4) * 16 + S[(4 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 5) * 16 + S[(5 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 6) * 16 + S[(6 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 7) * 16 + S[(7 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 8) * 16 + S[(8 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 9) * 16 + S[(9 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 10) * 16 + S[(10 * n) + j + v] ) * 8 );
            vec_add_128(P2 + (row * o + j) * 8, accumulator + ( (row * k + 11) * 16 + S[(11 * n) + j + v] ) * 8 );
#else
            for (int col = 0; col < k; col++) {
                m_vec_add(m_legs, P2 + (row * o + j)*m_legs * 2, accumulator + ( (row * k + col) * 16 + S[(col * n) + j + v] )*m_legs * 2 );
            }
#endif
        }
    }

    int P3_used = 0;
    for (int row = v; row < n; row++) {
        for (int j = row; j < n; j++) {
#if defined(MAYO_VARIANT) && (M_MAX == 64) && (K_MAX == 9)
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 0) * 16 + S[0 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 1) * 16 + S[1 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 2) * 16 + S[2 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 3) * 16 + S[3 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 4) * 16 + S[4 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 5) * 16 + S[5 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 6) * 16 + S[6 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 7) * 16 + S[7 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 8) * 16 + S[8 * n + j] ) * 4 );
#elif defined(MAYO_VARIANT) && (M_MAX == 64) && (K_MAX == 4)
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 0) * 16 + S[0 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 1) * 16 + S[1 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 2) * 16 + S[2 * n + j] ) * 4 );
            vec_add_64(P3 + P3_used * 4, accumulator + ( (row * k + 3) * 16 + S[3 * n + j] ) * 4 );
#elif defined(MAYO_VARIANT) && (M_MAX == 96) && (K_MAX == 11)
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 0) * 16 + S[0 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 1) * 16 + S[1 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 2) * 16 + S[2 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 3) * 16 + S[3 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 4) * 16 + S[4 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 5) * 16 + S[5 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 6) * 16 + S[6 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 7) * 16 + S[7 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 8) * 16 + S[8 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 9) * 16 + S[9 * n + j] ) * 6 );
            vec_add_96(P3 + P3_used * 6, accumulator + ( (row * k + 10) * 16 + S[10 * n + j] ) * 6 );
#elif defined(MAYO_VARIANT) && (M_MAX == 128) && (K_MAX == 12)
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 0) * 16 + S[0 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 1) * 16 + S[1 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 2) * 16 + S[2 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 3) * 16 + S[3 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 4) * 16 + S[4 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 5) * 16 + S[5 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 6) * 16 + S[6 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 7) * 16 + S[7 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 8) * 16 + S[8 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 9) * 16 + S[9 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 10) * 16 + S[10 * n + j] ) * 8 );
            vec_add_128(P3 + P3_used * 8, accumulator + ( (row * k + 11) * 16 + S[11 * n + j] ) * 8 );
#else
            for (int col = 0; col < k; col++) {
                m_vec_add(m_legs, P3 + P3_used * m_legs * 2, accumulator + ( (row * k + col) * 16 + S[col * n + j] )*m_legs * 2 );
            }
#endif
            P3_used ++;
        }
    }

    // multiply stuff according to the bins of the accumulator and add to PS.
    int i = 0;
    while (i < n * k) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
        multiply_bins_64(accumulator + i * 16 * 4, PS + i * 4);
        i++;
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
        multiply_bins_96(accumulator + i * 16 * 6, PS + i * 6);
        i++;
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
        multiply_bins_128(accumulator + i * 16 * 8, PS + i * 8);
        i++;
#else
        m_multiply_bins(m/32, accumulator + i * 16 * (m/32) * 2, PS + i * (m/32) * 2);
        i++;
#endif
    }

    #endif
}


static inline void mayo_generic_m_calculate_SPS(const uint64_t *PS, const unsigned char *S, int m, int k, int  n, uint64_t *SPS){
    alignas (32) uint64_t accumulator[M_MAX*K_MAX*K_MAX] = {0};
    #if !defined(MAYO_VARIANT)
    const int m_legs = m/32;
    #else
    (void) m;
    #endif
    for (int row = 0; row < k; row++) {
        for (int j = 0; j < n; j++) {
            for (int col = 0; col < k; col += 1) {
                #if defined(MAYO_VARIANT) && (M_MAX == 64)
                    vec_add_64(PS + (j * k + col) * 4, accumulator + ( (row * k + col) * 16 + S[row * n + j] ) * 4 );
                #elif defined(MAYO_VARIANT) && (M_MAX == 96)
                    vec_add_96(PS + (j * k + col) * 6, accumulator + ( (row * k + col) * 16 + S[row * n + j] ) * 6 );
                #elif defined(MAYO_VARIANT) && (M_MAX == 128)
                    vec_add_128(PS + (j * k + col) * 8, accumulator + ( (row * k + col) * 16 + S[row * n + j] ) * 8 );
                #else
                    m_vec_add(m_legs, PS + (j * k + col) * m_legs * 2, accumulator + ( (row * k + col) * 16 + S[row * n + j] )*m_legs * 2 );
                #endif
            }
        }
    }

    // multiply stuff according to the bins of the accumulator and add to PS.
    int i = 0;
    while (i < k*k) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
        multiply_bins_64(accumulator + i * 16 * 4, SPS + i * 4);
        i++;
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
        multiply_bins_96(accumulator + i * 16 * 6, SPS + i * 6);
        i++;
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
        multiply_bins_128(accumulator + i * 16 * 8, SPS + i * 8);
        i++;
#else
        m_multiply_bins(m_legs, accumulator + i * 16 * m_legs * 2, SPS + i * m_legs * 2);
        i++;
#endif
    }
}


// multiplies m (possibly upper triangular) matrices with a single matrix and adds result to acc
static inline void mul_add_m_upper_triangular_mat_x_mat(int m_legs, const uint64_t *bs_mat, const unsigned char *mat, uint64_t *acc, int bs_mat_rows, int bs_mat_cols, int mat_cols, int triangular) {

    int bs_mat_entries_used = 0;
    for (int r = 0; r < bs_mat_rows; r++) {
        for (int c = triangular * r; c < bs_mat_cols; c++) {
            for (int k = 0; k < mat_cols; k += 1) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
                (void) m_legs;
                vec_mul_add_64(bs_mat + 4 * bs_mat_entries_used, mat[c * mat_cols + k], acc + 4 * (r * mat_cols + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
                (void) m_legs;
                vec_mul_add_96(bs_mat + 6 * bs_mat_entries_used, mat[c * mat_cols + k], acc + 6 * (r * mat_cols + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
                (void) m_legs;
                vec_mul_add_128(bs_mat + 8 * bs_mat_entries_used, mat[c * mat_cols + k], acc + 8 * (r * mat_cols + k));
#else
                m_vec_mul_add(m_legs, bs_mat + m_legs * 2 * bs_mat_entries_used, mat[c * mat_cols + k], acc + m_legs * 2 * (r * mat_cols + k));
#endif
            }
            bs_mat_entries_used += 1;
        }
    }
}

// multiplies m (possibly upper triangular) matrices with the transpose of a single matrix and adds result to acc
static inline void mul_add_m_upper_triangular_mat_x_mat_trans(int m_legs, const uint64_t *bs_mat, const unsigned char *mat, uint64_t *acc, int bs_mat_rows, int bs_mat_cols, int mat_rows, int triangular) {

    int bs_mat_entries_used = 0;
    for (int r = 0; r < bs_mat_rows; r++) {
        for (int c = triangular * r; c < bs_mat_cols; c++) {
            for (int k = 0; k < mat_rows; k += 1) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
                (void) m_legs;
                vec_mul_add_64(bs_mat + 4 * bs_mat_entries_used, mat[k * bs_mat_cols + c], acc + 4 * (r * mat_rows + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
                (void) m_legs;
                vec_mul_add_96(bs_mat + 6 * bs_mat_entries_used, mat[k * bs_mat_cols + c], acc + 6 * (r * mat_rows + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
                (void) m_legs;
                vec_mul_add_128(bs_mat + 8 * bs_mat_entries_used, mat[k * bs_mat_cols + c], acc + 8 * (r * mat_rows + k));
#else
                m_vec_mul_add(m_legs, bs_mat + m_legs * 2 * bs_mat_entries_used, mat[k * bs_mat_cols + c], acc + m_legs * 2 * (r * mat_rows + k));
#endif
            }
            bs_mat_entries_used += 1;
        }
    }
}

// multiplies the transpose of a single matrix with m matrices and adds result to acc
static inline void mul_add_mat_trans_x_m_mat(int m_legs, const unsigned char *mat, const uint64_t *bs_mat, uint64_t *acc, int mat_rows, int mat_cols, int bs_mat_cols) {

    for (int r = 0; r < mat_cols; r++) {
        for (int c = 0; c < mat_rows; c++) {
            for (int k = 0; k < bs_mat_cols; k += 1) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
                (void) m_legs;
                vec_mul_add_64(bs_mat + 4 * (c * bs_mat_cols + k), mat[c * mat_cols + r], acc + 4 * (r * bs_mat_cols + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
                (void) m_legs;
                vec_mul_add_96(bs_mat + 6 * (c * bs_mat_cols + k), mat[c * mat_cols + r], acc + 6 * (r * bs_mat_cols + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
                (void) m_legs;
                vec_mul_add_128(bs_mat + 8 * (c * bs_mat_cols + k), mat[c * mat_cols + r], acc + 8 * (r * bs_mat_cols + k));
#else
                m_vec_mul_add(m_legs, bs_mat + m_legs * 2 * (c * bs_mat_cols + k), mat[c * mat_cols + r], acc + m_legs * 2 * (r * bs_mat_cols + k));
#endif
            }
        }
    }
}

// multiplies a single matrix with m matrices and adds result to acc
static inline void mul_add_mat_x_m_mat(int m_legs, const unsigned char *mat, const uint64_t *bs_mat, uint64_t *acc, int mat_rows, int mat_cols, int bs_mat_cols) {

    for (int r = 0; r < mat_rows; r++) {
        for (int c = 0; c < mat_cols; c++) {
            for (int k = 0; k < bs_mat_cols; k += 1) {
#if defined(MAYO_VARIANT) && (M_MAX == 64)
                (void) m_legs;
                vec_mul_add_64(bs_mat + 4 * (c * bs_mat_cols + k), mat[r * mat_cols + c], acc + 4 * (r * bs_mat_cols + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
                (void) m_legs;
                vec_mul_add_96(bs_mat + 6 * (c * bs_mat_cols + k), mat[r * mat_cols + c], acc + 6 * (r * bs_mat_cols + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
                (void) m_legs;
                vec_mul_add_128(bs_mat + 8 * (c * bs_mat_cols + k), mat[r * mat_cols + c], acc + 8 * (r * bs_mat_cols + k));
#else
                m_vec_mul_add(m_legs, bs_mat + m_legs * 2 * (c * bs_mat_cols + k), mat[r * mat_cols + c], acc + m_legs * 2 * (r * bs_mat_cols + k));
#endif
            }
        }
    }
}
#endif

