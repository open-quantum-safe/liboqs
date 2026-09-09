// SPDX-License-Identifier: Apache-2.0

#ifndef GENERIC_ARITHMETIC_H
#define GENERIC_ARITHMETIC_H

#include <simple_arithmetic.h>

#ifdef ENABLE_PARAMS_DYNAMIC
#include <arithmetic_dynamic.h>
#else
#include <arithmetic_fixed.h>
#endif

// multiplies m (possibly upper triangular) matrices with a single matrix and adds result to acc
static inline
void mul_add_m_upper_triangular_mat_x_mat(const int m_vec_limbs, const uint64_t *bs_mat, const unsigned char *mat, uint64_t *acc,
                                          const int bs_mat_rows, const int bs_mat_cols, const int mat_cols, const int triangular) {

    int bs_mat_entries_used = 0;
    for (int r = 0; r < bs_mat_rows; r++) {
        for (int c = triangular * r; c < bs_mat_cols; c++) {
            for (int k = 0; k < mat_cols; k += 1) {
                m_vec_mul_add(m_vec_limbs, bs_mat + m_vec_limbs * bs_mat_entries_used, mat[c * mat_cols + k], acc + m_vec_limbs * (r * mat_cols + k));
            }
            bs_mat_entries_used += 1;
        }
    }
}

// multiplies m (possibly upper triangular) matrices with the transpose of a single matrix and adds result to acc
static inline
void mul_add_m_upper_triangular_mat_x_mat_trans(const int m_vec_limbs, const uint64_t *bs_mat, const unsigned char *mat, uint64_t *acc,
                                                const int bs_mat_rows, const int bs_mat_cols, const int mat_rows, const int triangular) {
    int bs_mat_entries_used = 0;
    for (int r = 0; r < bs_mat_rows; r++) {
        for (int c = triangular * r; c < bs_mat_cols; c++) {
            for (int k = 0; k < mat_rows; k += 1) {
                m_vec_mul_add(m_vec_limbs, bs_mat + m_vec_limbs * bs_mat_entries_used, mat[k * bs_mat_cols + c], acc + m_vec_limbs * (r * mat_rows + k));
            }
            bs_mat_entries_used += 1;
        }
    }
}

// multiplies the transpose of a single matrix with m matrices and adds result to acc
static inline
void mul_add_mat_trans_x_m_mat(const int m_vec_limbs, const unsigned char *mat, const uint64_t *bs_mat, uint64_t *acc,
                               const int mat_rows, const int mat_cols, const int bs_mat_cols) {

    for (int r = 0; r < mat_cols; r++) {
        for (int c = 0; c < mat_rows; c++) {
            for (int k = 0; k < bs_mat_cols; k += 1) {
                m_vec_mul_add(m_vec_limbs, bs_mat + m_vec_limbs * (c * bs_mat_cols + k), mat[c * mat_cols + r], acc + m_vec_limbs * (r * bs_mat_cols + k));
            }
        }
    }
}

// multiplies a single matrix with m matrices and adds result to acc
static inline
void mul_add_mat_x_m_mat(const int m_vec_limbs, const unsigned char *mat, const uint64_t *bs_mat, uint64_t *acc,
                         const int mat_rows, const int mat_cols, const int bs_mat_cols) {

    for (int r = 0; r < mat_rows; r++) {
        for (int c = 0; c < mat_cols; c++) {
            for (int k = 0; k < bs_mat_cols; k += 1) {
                m_vec_mul_add(m_vec_limbs, bs_mat + m_vec_limbs * (c * bs_mat_cols + k), mat[r * mat_cols + c], acc + m_vec_limbs * (r * bs_mat_cols + k));
            }
        }
    }
}

static inline
void P1_times_O(const mayo_params_t* p, const uint64_t* P1, const unsigned char* O, uint64_t* acc){
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif
    mul_add_m_upper_triangular_mat_x_mat(PARAM_m_vec_limbs(p), P1, O, acc, PARAM_v(p), PARAM_v(p), PARAM_o(p), 1);
}

static inline
void P1_times_Vt(const mayo_params_t* p, const uint64_t* P1, const unsigned char* V, uint64_t* acc){
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif
    mul_add_m_upper_triangular_mat_x_mat_trans(PARAM_m_vec_limbs(p), P1, V, acc, PARAM_v(p), PARAM_v(p), PARAM_k(p), 1);
}

#if defined(HAVE_STACKEFFICIENT) || defined(PQM4)
// compute P * S^t = [ P1  P2 ] * [S1] = [P1*S1 + P2*S2]
//                   [  0  P3 ]   [S2]   [        P3*S2]
// compute S * PS  = [ S1 S2 ] * [ P1*S1 + P2*S2 = P1 ] = [ S1*P1 + S2*P2 ]
//                               [         P3*S2 = P2 ]
static inline void mayo_generic_m_calculate_PS_SPS(const uint64_t *P1, const uint64_t *P2, const uint64_t *P3, const unsigned char *S,
                                                   const int m, const int v, const int o, const int k, const uint64_t *lambda, uint64_t *SPS) {

    const int n = o + v;
    const int m_vec_limbs = (m + 15)/16;

    uint64_t PS[(N_MAX + K_MAX) * M_VEC_LIMBS_MAX] = { 0 };
    uint64_t accumulator[16 * ((M_MAX+15)/16) * N_MAX] = {0};
    int P1_used;
    int P3_used;
    
    for (int col = 0; col < k; col++) {
        for(unsigned int i = 0; i < sizeof(accumulator)/8; i++) {
            accumulator[i] = 0;
        }
        P1_used = 0;
        for (int row = 0; row < v; row++) {
            for (int j = row; j < v; j++) {
                m_vec_add(m_vec_limbs, P1 + (P1_used * m_vec_limbs), accumulator + ( row * 16 + S[col * n + j] )*m_vec_limbs);

                P1_used ++;
            }

            for (int j = 0; j < o; j++) {
                m_vec_add(m_vec_limbs, P2 + (row * o + j)*m_vec_limbs, accumulator + ( row * 16 + S[(col * n) + j + v] )* m_vec_limbs);
            }
        }

        P3_used = 0;
        for (int row = v; row < n; row++) {
            for (int j = row; j < n; j++) {
                m_vec_add(m_vec_limbs, P3 + P3_used * m_vec_limbs, accumulator + ( row * 16 + S[col * n + j] )* m_vec_limbs );
                P3_used ++;
            }
        }

        for (int row = 0; row < n; row++) {
            m_vec_multiply_bins(m_vec_limbs, accumulator + row * 16 * m_vec_limbs, PS + (row + col) * m_vec_limbs);
        }

        // lambda lives in column 0 of PS. The S * PS pass right below applies
        // S[row][j] to it, so SPS[row][0] picks up Lambda(s_row) for free.
        if (col == 0) {
            for (int row = 0; row < n; row++) {
                m_vec_add(m_vec_limbs, lambda + row * m_vec_limbs, PS + row * m_vec_limbs);
            }
        }

        for (int row = 0; row < k; row++) {
            for (unsigned int i = 0; i < 16*((M_MAX+15)/16); ++i)
                accumulator[i] = 0;
            for (int j = 0; j < n; j++) {
                m_vec_add(m_vec_limbs, PS + (j + col) * m_vec_limbs, accumulator + S[row * n + j]*m_vec_limbs);
            }
            m_vec_multiply_bins(m_vec_limbs, accumulator, SPS + (row * k + col) * m_vec_limbs);
        }

    }

}

#else

// compute P * S^t = [ P1  P2 ] * [S1] = [P1*S1 + P2*S2]
//                   [  0  P3 ]   [S2]   [        P3*S2]
static inline void mayo_generic_m_calculate_PS(const uint64_t *P1, const uint64_t *P2, const uint64_t *P3, const unsigned char *S,
                                               const int m, const int v, const int o, const int k, uint64_t *PS) {

    const int n = o + v;
    const int m_vec_limbs = (m + 15)/16;

    uint64_t accumulator[16 * ((M_MAX+15)/16) * K_MAX * N_MAX] = {0};
    int P1_used = 0;
    for (int row = 0; row < v; row++) {
        for (int j = row; j < v; j++) {
            for (int col = 0; col < k; col++) {
                m_vec_add(m_vec_limbs, P1 + (P1_used * m_vec_limbs), accumulator + ( (row * k + col) * 16 + S[col * n + j] )*m_vec_limbs);
            }
            P1_used ++;
        }

        for (int j = 0; j < o; j++) {
            for (int col = 0; col < k; col++) {
                m_vec_add(m_vec_limbs, P2 + (row * o + j)*m_vec_limbs, accumulator + ( (row * k + col) * 16 + S[(col * n) + j + v] )* m_vec_limbs);
            }
        }
    }

    int P3_used = 0;
    for (int row = v; row < n; row++) {
        for (int j = row; j < n; j++) {
            for (int col = 0; col < k; col++) {
                m_vec_add(m_vec_limbs, P3 + P3_used * m_vec_limbs, accumulator + ( (row * k + col) * 16 + S[col * n + j] )* m_vec_limbs );
            }
            P3_used ++;
        }
    }

    // multiply stuff according to the bins of the accumulator and add to PS.
    int i = 0;
    while (i < n * k) {
        m_vec_multiply_bins(m_vec_limbs, accumulator + i * 16 * m_vec_limbs, PS + i * m_vec_limbs);
        i++;
    }

}

// compute S * PS  = [ S1 S2 ] * [ P1*S1 + P2*S2 = P1 ] = [ S1*P1 + S2*P2 ]
//                               [         P3*S2 = P2 ]
static inline void mayo_generic_m_calculate_SPS(const uint64_t *PS, const unsigned char *S, int m, int k, int  n, uint64_t *SPS){
    uint64_t accumulator[16*((M_MAX+15)/16)*K_MAX*K_MAX] = {0};
    const int m_vec_limbs = (m + 15)/ 16;
    for (int row = 0; row < k; row++) {
        for (int j = 0; j < n; j++) {
            for (int col = 0; col < k; col += 1) {
                    m_vec_add(m_vec_limbs, PS + (j * k + col) * m_vec_limbs, accumulator + ( (row * k + col) * 16 + S[row * n + j] )*m_vec_limbs);
            }
        }
    }

    // multiply stuff according to the bins of the accumulator and add to PS.
    int i = 0;
    while (i < k*k) {
        m_vec_multiply_bins(m_vec_limbs, accumulator + i * 16 * m_vec_limbs, SPS + i * m_vec_limbs);
        i++;
    }
}

#endif


static inline
void P1P1t_times_O(const mayo_params_t* p, const uint64_t* P1, const unsigned char* O, uint64_t* acc){
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif
    const int param_o = PARAM_o(p);
    const int param_v = PARAM_v(p);
    const int m_vec_limbs = PARAM_m_vec_limbs(p);

    int bs_mat_entries_used = 0;
    for (int r = 0; r < param_v; r++) {
        for (int c = r; c < param_v; c++) {
            if(c==r) {
                bs_mat_entries_used += 1;
                continue;
            }
            for (int k = 0; k < param_o; k += 1) {
                m_vec_mul_add(m_vec_limbs, P1 + m_vec_limbs * bs_mat_entries_used, O[c * param_o + k], acc + m_vec_limbs * (r * param_o + k));
                m_vec_mul_add(m_vec_limbs, P1 + m_vec_limbs * bs_mat_entries_used, O[r * param_o + k], acc + m_vec_limbs * (c * param_o + k));
            }
            bs_mat_entries_used += 1;
        }
    }
}


static inline
void compute_M_and_VPV(const mayo_params_t* p, const unsigned char* Vdec, const uint64_t *L, const uint64_t *P1, uint64_t *VL, uint64_t *VP1V){

    const int param_k = PARAM_k(p);
    const int param_v = PARAM_v(p);
    const int param_o = PARAM_o(p);

    //VL
    mul_add_mat_x_m_mat(PARAM_m_vec_limbs(p), Vdec, L, VL, param_k, param_v, param_o);

    //VP1V
    uint64_t Pv[V_MAX * K_MAX * M_VEC_LIMBS_MAX] = {0};
    P1_times_Vt(p, P1, Vdec, Pv);
    mul_add_mat_x_m_mat(PARAM_m_vec_limbs(p), Vdec, Pv, VP1V, param_k, param_v, param_k);
}

// M = ((P1 + P1^t) * V^t)^t * O = V * (P1 + P1^t) * O; W is v x k, O is v x o, M is k x o
static inline
void Wt_times_O(const mayo_params_t* p, const uint64_t* W, const unsigned char* O, uint64_t* M){
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif
    const int param_v = PARAM_v(p);
    const int param_k = PARAM_k(p);
    const int param_o = PARAM_o(p);
    const int m_vec_limbs = PARAM_m_vec_limbs(p);

    for (int r = 0; r < param_v; r++) {
        for (int j = 0; j < param_k; j++) {
            for (int l = 0; l < param_o; l++) {
                m_vec_mul_add(m_vec_limbs, W + m_vec_limbs * (r * param_k + j), O[r * param_o + l], M + m_vec_limbs * (j * param_o + l));
            }
        }
    }
}

// Like compute_M_and_VPV, but without a materialized L: reuses P1 * V^t to form
// M = V * ((P1 + P1^t) * O + P2) on the fly, avoiding the O(v^2 * o) L step.
static inline
void compute_M_and_VPV_implicit(const mayo_params_t* p, const unsigned char* Vdec, const uint64_t *P2, const uint64_t *P1, const unsigned char *O, const uint64_t *lambda, uint64_t *M, uint64_t *VP1V){

    const int param_k = PARAM_k(p);
    const int param_v = PARAM_v(p);
    const int param_o = PARAM_o(p);
    const int m_vec_limbs = PARAM_m_vec_limbs(p);

    uint64_t Pv[V_MAX * K_MAX * M_VEC_LIMBS_MAX] = {0}; // P1 * V^t
    uint64_t Pt[V_MAX * K_MAX * M_VEC_LIMBS_MAX] = {0}; // P1^t * V^t

    // single pass over upper-triangular P1: build P1 * V^t and P1^t * V^t together
    int bs_mat_entries_used = 0;
    for (int r = 0; r < param_v; r++) {
        for (int c = r; c < param_v; c++) {
            for (int j = 0; j < param_k; j++) {
                m_vec_mul_add(m_vec_limbs, P1 + m_vec_limbs * bs_mat_entries_used, Vdec[j * param_v + c], Pv + m_vec_limbs * (r * param_k + j));
                m_vec_mul_add(m_vec_limbs, P1 + m_vec_limbs * bs_mat_entries_used, Vdec[j * param_v + r], Pt + m_vec_limbs * (c * param_k + j));
            }
            bs_mat_entries_used += 1;
        }
    }

    // lambda goes into column 0 of P1 * V^t, and both of its contributions then
    // ride along on products that are computed anyway: V * (P1 * V^t) turns it into
    // Lambda(v_i) in VP1V[i][0], and because column 0 survives the Pv ^= Pt fold,
    // Wt_times_O turns it into (O^t lambda)[c] in block 0 of M. v XORs, no multiplies.
    for (int r = 0; r < param_v; r++) {
        m_vec_add(m_vec_limbs, lambda + (size_t) r * m_vec_limbs, Pv + (size_t) r * param_k * m_vec_limbs);
    }

    mul_add_mat_x_m_mat(m_vec_limbs, Vdec, Pv, VP1V, param_k, param_v, param_k); // VP1V = V * (P1 * V^t)

    // Pv ^= Pt  ->  (P1 + P1^t) * V^t  (the diagonal cancels in characteristic two)
    for (int i = 0; i < param_v * param_k * m_vec_limbs; i++) {
        Pv[i] ^= Pt[i];
    }

    Wt_times_O(p, Pv, O, M);                                                     // M = V * (P1 + P1^t) * O
    mul_add_mat_x_m_mat(m_vec_limbs, Vdec, P2, M, param_k, param_v, param_o);    // M += V * P2
}

static inline
void compute_P3(const mayo_params_t* p, const uint64_t* P1, uint64_t *P2, const unsigned char *O, uint64_t *P3){

    const int m_vec_limbs = PARAM_m_vec_limbs(p);
    const int param_v = PARAM_v(p);
    const int param_o = PARAM_o(p);

    // compute P1*O + P2
    P1_times_O(p, P1, O, P2);

    // compute P3 = O^t * (P1*O + P2)
    mul_add_mat_trans_x_m_mat(m_vec_limbs, O, P2, P3, param_v, param_o, param_o);
}

// compute P * S^t = [ P1  P2 ] * [S1] = [P1*S1 + P2*S2]
//                   [  0  P3 ]   [S2]   [        P3*S2]
// compute S * PS  = [ S1 S2 ] * [ P1*S1 + P2*S2 = P1 ] = [ S1*P1 + S2*P2 ]
//                               [         P3*S2 = P2 ]
static inline void m_calculate_PS_SPS(const mayo_params_t *p, const uint64_t *P1, const uint64_t *P2, const uint64_t *P3, const unsigned char *s,
                                      const uint64_t *lambda, uint64_t *SPS) {
    // compute P * S^t = {(P1, P2), (0, P3)} * S^t = {(P1*S1 + P2*S2), (P3 * S2)}
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif
    #if defined(HAVE_STACKEFFICIENT) || defined(PQM4)
    mayo_generic_m_calculate_PS_SPS(P1, P2, P3, s, PARAM_m(p), PARAM_v(p), PARAM_o(p), PARAM_k(p), lambda, SPS);
    #else
    uint64_t PS[N_MAX * K_MAX * M_VEC_LIMBS_MAX] = { 0 };
    mayo_generic_m_calculate_PS(P1, P2, P3, s, PARAM_m(p), PARAM_v(p), PARAM_o(p), PARAM_k(p), PS);

    // lambda into column 0 of PS; the S * PS pass below applies s[i][r] to it, so
    // SPS[i][0] picks up Lambda(s_i) at the cost of n m-vector XORs and no multiplies.
    for (int r = 0; r < PARAM_n(p); r++) {
        m_vec_add(PARAM_m_vec_limbs(p), lambda + (size_t) r * PARAM_m_vec_limbs(p),
                  PS + (size_t) r * PARAM_k(p) * PARAM_m_vec_limbs(p));
    }

    // compute S * P * S = S* (P*S)
    mayo_generic_m_calculate_SPS(PS, s, PARAM_m(p), PARAM_k(p), PARAM_n(p), SPS);
    #endif
}

#endif
