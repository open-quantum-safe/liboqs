// SPDX-License-Identifier: Apache-2.0

#include <arithmetic.h>
#include <simple_arithmetic.h>
#include <arithmetic_common.h>
#include <mem.h>
#include <echelon_form.h>
#include <stdalign.h>
#ifdef ENABLE_CT_TESTING
#include <valgrind/memcheck.h>
#endif

void m_upper(int m_legs, const uint64_t *in, uint64_t *out, int size) {
#if defined(MAYO_VARIANT) && MAYO_AVX && (M_MAX == 64)
    mayo12_m_upper(m_legs, in, out, size);
#else
    int m_vecs_stored = 0;
    for (int r = 0; r < size; r++) {
        for (int c = r; c < size; c++) {
            m_vec_copy(m_legs, in + m_legs * 2 * (r * size + c), out + m_legs * 2 * m_vecs_stored );
            if (r != c) {
                m_vec_add(m_legs, in + m_legs * 2 * (c * size + r), out + m_legs * 2 * m_vecs_stored );
            }
            m_vecs_stored ++;
        }
    }
#endif
}

void P1P1t_times_O(const mayo_params_t* p, const uint64_t* P1, const unsigned char* O, uint64_t* acc){
#if MAYO_AVX && defined(MAYO_VARIANT) && M_MAX == 64
    (void) p;
    mayo_12_P1P1t_times_O(P1, O, acc);
#elif MAYO_AVX && defined(MAYO_VARIANT) && M_MAX == 96
    (void) p;   
    mayo_3_P1P1t_times_O(P1, O, acc);
#elif MAYO_AVX && defined(MAYO_VARIANT) && M_MAX == 128
    (void) p;   
    mayo_5_P1P1t_times_O(P1, O, acc);
#else
    #ifndef MAYO_VARIANT
    const int m_legs = PARAM_m(p) / 32;
    #else
    (void) p;
    #endif
    const int param_o = PARAM_o(p);
    const int param_v = PARAM_n(p) - PARAM_o(p);;

    int 
    bs_mat_entries_used = 0;
    for (int r = 0; r < param_v; r++) {
        for (int c = r; c < param_v; c++) {
            if(c==r) {
                bs_mat_entries_used += 1;
                continue;
            }
            for (int k = 0; k < param_o; k += 1) {
                
#if defined(MAYO_VARIANT) && (M_MAX == 64)
                vec_mul_add_64(P1 + 4 * bs_mat_entries_used, O[c * param_o + k], acc + 4 * (r * param_o + k));
                vec_mul_add_64( P1 + 4 * bs_mat_entries_used, O[r * param_o + k],  acc + 4 * (c * param_o + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 96)
                vec_mul_add_96( P1 + 6 * bs_mat_entries_used, O[c * param_o + k],  acc + 6 * (r * param_o + k));
                vec_mul_add_96( P1 + 6 * bs_mat_entries_used, O[r * param_o + k],  acc + 6 * (c * param_o + k));
#elif defined(MAYO_VARIANT) && (M_MAX == 128)
                vec_mul_add_128( P1 + 8 * bs_mat_entries_used, O[c * param_o + k],  acc + 8 * (r * param_o + k));
                vec_mul_add_128( P1 + 8 * bs_mat_entries_used, O[r * param_o + k],  acc + 8 * (c * param_o + k));
#else
                m_vec_mul_add(m_legs, P1 + m_legs * 2 * bs_mat_entries_used, O[c * param_o + k], acc + m_legs * 2 * (r * param_o + k));
                m_vec_mul_add(m_legs, P1 + m_legs * 2 * bs_mat_entries_used, O[r * param_o + k], acc + m_legs * 2 * (c * param_o + k));
#endif

            }
            bs_mat_entries_used += 1;
        }
    }
#endif
}

void V_times_L__V_times_P1_times_Vt(const mayo_params_t* p, const uint64_t* L, const unsigned char* V, uint64_t* M, const uint64_t* P1, uint64_t* Y) {
    (void) p;
#if MAYO_AVX && defined(MAYO_VARIANT) && M_MAX == 64
    __m256i V_multabs[(K_MAX+1)/2*V_MAX];
    alignas (32) uint64_t Pv[N_MINUS_O_MAX * K_MAX * M_MAX / 16] = {0};
    mayo_V_multabs_avx2(V, V_multabs);
    mayo_12_Vt_times_L_avx2(L, V_multabs, M);
    mayo_12_P1_times_Vt_avx2(P1, V_multabs, Pv);
    mayo_12_Vt_times_Pv_avx2(Pv, V_multabs, Y);
#elif MAYO_AVX && defined(MAYO_VARIANT) && M_MAX == 96
    __m256i V_multabs[(K_MAX+1)/2*V_MAX];
    alignas (32) uint64_t Pv[N_MINUS_O_MAX * K_MAX * M_MAX / 16] = {0};
    mayo_V_multabs_avx2(V, V_multabs);
    mayo_3_Vt_times_L_avx2(L, V_multabs, M);
    mayo_3_P1_times_Vt_avx2(P1, V_multabs, Pv);
    mayo_3_Vt_times_Pv_avx2(Pv, V_multabs, Y);
#elif MAYO_AVX && defined(MAYO_VARIANT) && M_MAX == 128
    __m256i V_multabs[(K_MAX+1)/2*V_MAX];
    alignas (32) uint64_t Pv[N_MINUS_O_MAX * K_MAX * M_MAX / 16] = {0};
    mayo_V_multabs_avx2(V, V_multabs);
    mayo_5_Vt_times_L_avx2(L, V_multabs, M);
    mayo_5_P1_times_Vt_avx2(P1, V_multabs, Pv);
    mayo_5_Vt_times_Pv_avx2(Pv, V_multabs, Y);
#else
    #ifdef MAYO_VARIANT
    (void) p;
    #endif
    const int param_m = PARAM_m(p);
    const int param_n = PARAM_n(p);
    const int param_o = PARAM_o(p);
    const int param_k = PARAM_k(p);

    alignas (32) uint64_t Pv[N_MINUS_O_MAX * K_MAX * M_MAX / 16] = {0};
    mul_add_mat_x_m_mat(param_m / 32, V, L, M,
        param_k, param_n - param_o, param_o);

    mul_add_m_upper_triangular_mat_x_mat_trans(param_m / 32, P1, V, Pv, param_n - param_o, param_n - param_o, param_k, 1);

    mul_add_mat_x_m_mat(param_m / 32, V, Pv,
        Y, param_k, param_n - param_o,
        param_k);
#endif
}

void Ot_times_P1O_P2(const mayo_params_t* p, const uint64_t* P1, const unsigned char* O, uint64_t* P1O_P2, uint64_t* P3) {
    (void) p;
#if MAYO_AVX && defined(MAYO_VARIANT) && M_MAX == 64
    __m256i O_multabs[O_MAX/2*V_MAX];
    mayo_O_multabs_avx2(O, O_multabs);
    mayo_12_P1_times_O_avx2(P1, O_multabs, P1O_P2);
    mayo_12_Ot_times_P1O_P2_avx2(P1O_P2, O_multabs, P3);
#elif MAYO_AVX && defined(MAYO_VARIANT) && M_MAX == 96
    __m256i O_multabs[O_MAX/2*V_MAX];
    mayo_O_multabs_avx2(O, O_multabs);
    mayo_3_P1_times_O_avx2(P1, O_multabs, P1O_P2);
    mayo_3_Ot_times_P1O_P2_avx2(P1O_P2, O_multabs, P3);
#elif MAYO_AVX && defined(MAYO_VARIANT) && M_MAX == 128
    __m256i O_multabs[O_MAX/2*V_MAX];
    mayo_O_multabs_avx2(O, O_multabs);
    mayo_5_P1_times_O_avx2(P1, O_multabs, P1O_P2);
    mayo_5_Ot_times_P1O_P2_avx2(P1O_P2, O_multabs, P3);
#else
    #ifdef MAYO_VARIANT
    (void) p;
    #endif
    const int param_v = PARAM_v(p);
    const int param_o = PARAM_o(p);
    const int param_m = PARAM_m(p);
    const int param_n = PARAM_n(p);
    const int m_legs = PARAM_m(p) / 32;
    mul_add_m_upper_triangular_mat_x_mat(param_m/32, P1, O, P1O_P2, param_n - param_o, param_n - param_o, param_o, 1);
    mul_add_mat_trans_x_m_mat(m_legs, O, P1O_P2, P3, param_v, param_o, param_o);
#endif
}


// compute P * S^t = [ P1  P2 ] * [S1] = [P1*S1 + P2*S2]
//                   [  0  P3 ]   [S2]   [        P3*S2]
// compute S * PS  = [ S1 S2 ] * [ P1*S1 + P2*S2 = P1 ] = [ S1*P1 + S2*P2 ]
//                               [         P3*S2 = P2 ]
void m_calculate_PS_SPS(const uint64_t *P1, const uint64_t *P2, const uint64_t *P3, const unsigned char *S,
                              const int m, const int v, const int o, const int k, uint64_t *SPS) {
    (void) m;
#if MAYO_AVX
    const int n = o + v;

    /* Old approach which is constant time but doesn't have to be */
    unsigned char S1[V_MAX*K_MAX] = { 0 }; // == N-O, K
    unsigned char S2[O_MAX*K_MAX] = { 0 }; // == O, K
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

    alignas (32) uint64_t PS[N_MAX * K_MAX * M_MAX / 16] = { 0 };

#if M_MAX == 64
    __m256i S1_multabs[(K_MAX+1)/2*V_MAX];    
    __m256i S2_multabs[(K_MAX+1)/2*O_MAX];
    mayo_S1_multabs_avx2(S1, S1_multabs);
    mayo_S2_multabs_avx2(S2, S2_multabs);

    mayo_12_P1_times_S1_plus_P2_times_S2_avx2(P1, P2, S1_multabs, S2_multabs, PS);
    mayo_12_P3_times_S2_avx2(P3, S2_multabs, PS + V_MAX*K_MAX*M_MAX/16); // upper triangular

    // S^T * PS = S1^t*PS1 + S2^t*PS2
    mayo_12_S1t_times_PS1_avx2(PS, S1_multabs, SPS);
    mayo_12_S2t_times_PS2_avx2(PS + V_MAX*K_MAX*M_MAX/16, S2_multabs, SPS);
#elif M_MAX == 96
    __m256i S1_multabs[(K_MAX+1)/2*V_MAX];    
    __m256i S2_multabs[(K_MAX+1)/2*O_MAX];

    mayo_S1_multabs_avx2(S1, S1_multabs);
    mayo_S2_multabs_avx2(S2, S2_multabs);

    mayo_3_P1_times_S1_plus_P2_times_S2_avx2(P1, P2, S1_multabs, S2_multabs, PS);
    mayo_3_P3_times_S2_avx2(P3, S2_multabs, PS + V_MAX*K_MAX*M_MAX/16); // upper triangular

    // S^T * PS = S1^t*PS1 + S2^t*PS2
    mayo_3_S1t_times_PS1_avx2(PS, S1_multabs, SPS);
    mayo_3_S2t_times_PS2_avx2(PS + V_MAX*K_MAX*M_MAX/16, S2_multabs, SPS);
#elif M_MAX == 128
    __m256i S1_multabs[(K_MAX+1)/2*V_MAX];    
    __m256i S2_multabs[(K_MAX+1)/2*O_MAX];
    mayo_S1_multabs_avx2(S1, S1_multabs);
    mayo_S2_multabs_avx2(S2, S2_multabs);
    mayo_5_P1_times_S1_plus_P2_times_S2_avx2(P1, P2, S1_multabs, S2_multabs, PS);
    mayo_5_P3_times_S2_avx2(P3, S2_multabs, PS + V_MAX*K_MAX*M_MAX/16); // upper triangular

    // S^T * PS = S1^t*PS1 + S2^t*PS2
    //m_calculate_SPS(PS, S, M_MAX, K_MAX, N_MAX, SPS);
    mayo_5_S1t_times_PS1_avx2(PS, S1_multabs, SPS);
    mayo_5_S2t_times_PS2_avx2(PS + V_MAX*K_MAX*M_MAX/16, S2_multabs, SPS);
#else 
    NOT IMPLEMENTED
#endif
#else
    const int n = o + v;
    alignas (32) uint64_t PS[N_MAX * K_MAX * M_MAX / 16] = { 0 };
    mayo_generic_m_calculate_PS(P1, P2, P3, S, m, v, o, k, PS);
    mayo_generic_m_calculate_SPS(PS, S, m, k, n, SPS);
#endif
}


// sample a solution x to Ax = y, with r used as randomness
// require:
// - A is a matrix with m rows and k*o+1 collumns (values in the last collum are
// not important, they will be overwritten by y) in row major order
// - y is a vector with m elements
// - r and x are k*o bytes long
// return: 1 on success, 0 on failure
int sample_solution(const mayo_params_t *p, unsigned char *A,
                           const unsigned char *y, const unsigned char *r,
                           unsigned char *x, int k, int o, int m, int A_cols) {
    #ifdef MAYO_VARIANT
    (void) p;
    #endif
    unsigned char finished;
    int col_upper_bound;
    unsigned char correct_column;

    // x <- r
    for (int i = 0; i < k * o; i++) {
        x[i] = r[i];
    }

    // compute Ar;
    unsigned char Ar[M_MAX];
    for (int i = 0; i < m; i++) {
        A[k * o + i * (k * o + 1)] = 0; // clear last col of A
    }
    mat_mul(A, r, Ar, k * o + 1, m, 1);

    // move y - Ar to last column of matrix A
    for (int i = 0; i < m; i++) {
        A[k * o + i * (k * o + 1)] = sub_f(y[i], Ar[i]);
    }

    EF(A, m, k * o + 1);

    // check if last row of A (excluding the last entry of y) is zero
    unsigned char full_rank = 0;
    for (int i = 0; i < A_cols - 1; i++) {
        full_rank |= A[(m - 1) * A_cols + i];
    }

// It is okay to leak if we need to restart or not
#ifdef ENABLE_CT_TESTING
    VALGRIND_MAKE_MEM_DEFINED(&full_rank, 1);
#endif

    if (full_rank == 0) {
        return 0;
    }

    // back substitution in constant time
    // the index of the first nonzero entry in each row is secret, which makes
    // things less efficient

    for (int row = m - 1; row >= 0; row--) {
        finished = 0;
        col_upper_bound = MAYO_MIN(row + (32/(m-row)), k*o);
        // the first nonzero entry in row r is between r and col_upper_bound with probability at least ~1-q^{-32}

        for (int col = row; col <= col_upper_bound; col++) {

            // Compare two chars in constant time.
            // Returns 0x00 if the byte arrays are equal, 0xff otherwise.
            correct_column = ct_compare_8((A[row * A_cols + col]), 0) & ~finished;

            unsigned char u = correct_column & A[row * A_cols + A_cols - 1];
            x[col] ^= u;

            for (int i = 0; i < row; i += 8) {
                uint64_t tmp = ( (uint64_t) A[ i    * A_cols + col] <<  0) ^ ( (uint64_t) A[(i+1) * A_cols + col] <<  8)
                             ^ ( (uint64_t) A[(i+2) * A_cols + col] << 16) ^ ( (uint64_t) A[(i+3) * A_cols + col] << 24)
                             ^ ( (uint64_t) A[(i+4) * A_cols + col] << 32) ^ ( (uint64_t) A[(i+5) * A_cols + col] << 40)
                             ^ ( (uint64_t) A[(i+6) * A_cols + col] << 48) ^ ( (uint64_t) A[(i+7) * A_cols + col] << 56);

                tmp = mul_fx8(u, tmp);

                A[ i    * A_cols + A_cols - 1] ^= (tmp      ) & 0xf;
                A[(i+1) * A_cols + A_cols - 1] ^= (tmp >> 8 ) & 0xf;
                A[(i+2) * A_cols + A_cols - 1] ^= (tmp >> 16) & 0xf;
                A[(i+3) * A_cols + A_cols - 1] ^= (tmp >> 24) & 0xf;
                A[(i+4) * A_cols + A_cols - 1] ^= (tmp >> 32) & 0xf;
                A[(i+5) * A_cols + A_cols - 1] ^= (tmp >> 40) & 0xf;
                A[(i+6) * A_cols + A_cols - 1] ^= (tmp >> 48) & 0xf;
                A[(i+7) * A_cols + A_cols - 1] ^= (tmp >> 56) & 0xf;
            }

            finished = finished | correct_column;
        }
    }
    return 1;
}

