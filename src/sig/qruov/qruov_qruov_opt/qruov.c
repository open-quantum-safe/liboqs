#include <string.h>
#include "qruov.h"
#include "qrop.h"
#include "prg.h"
#include "util.h"
#include "linsys.h"
#include "gf.h"
#include "emi_transform.h"
#include "fql.h"
#include "qruov_common.h"

void qruov_init(void)
{
}

// Convert AoS [rows][cols][L] to SoA [L][rows][cols].
static void fql_aos_to_soa(uint8_t *dst_soa, const uint8_t *src_aos, int rows, int cols)
{
    int n = rows * cols;
    for (int i = 0; i < n; i++) {
        for (int l = 0; l < QRUOV_L; l++) {
            dst_soa[l * n + i] = src_aos[i * QRUOV_L + l];
        }
    }
}

// Convert SoA [L][rows][cols] to AoS [rows][cols][L].
static void fql_soa_to_aos(uint8_t *dst_aos, const uint8_t *src_soa, int rows, int cols)
{
    int n = rows * cols;
    for (int l = 0; l < QRUOV_L; l++) {
        for (int i = 0; i < n; i++) {
            dst_aos[i * QRUOV_L + l] = src_soa[l * n + i];
        }
    }
}

// Convert AoS [rows][cols][L] to eval-domain SoA [LL][rows][cols].
static void fql_aos_to_eval(uint8_t *dst_eval, const uint8_t *src_aos, int rows, int cols)
{
    const int plane = rows * cols;
    uint8_t src_soa[QRUOV_L * plane];
    fql_aos_to_soa(src_soa, src_aos, rows, cols);
    evaluate(dst_eval, plane, src_soa, plane, plane);
}

// Transpose plane-major matrix [planes][rows][cols] -> [planes][cols][rows].
static void fq_transpose_planes(uint8_t *dst, const uint8_t *src, int planes, int rows, int cols)
{
    for (int p = 0; p < planes; p++) {
        const uint8_t *sp = &src[p * rows * cols];
        uint8_t *dp = &dst[p * rows * cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                dp[j * rows + i] = sp[i * cols + j];
            }
        }
    }
}

// C[planes][M][N] = A[planes][M][K] * B^T[planes][N][K] over F_q.
static void fq_matmul_transposed_planes(uint8_t *restrict C, const uint8_t *restrict A,
                                        const uint8_t *restrict BT, int planes, int M, int N, int K)
{
    for (int p = 0; p < planes; p++) {
        const uint8_t *Ap = &A[p * M * K];
        const uint8_t *BTp = &BT[p * N * K];
        uint8_t *Cp = &C[p * M * N];
        for (int i = 0; i < M; i++) {
            const uint8_t *Ai = &Ap[i * K];
            uint8_t *Ci = &Cp[i * N];

            int j = 0;
            for (; j + 1 < N; j += 2) {
                const uint8_t *BTj0 = &BTp[j * K];
                const uint8_t *BTj1 = BTj0 + K;
                uint32_t sum0 = 0;
                uint32_t sum1 = 0;
                for (int k = 0; k < K; k++) {
                    uint32_t aik = Ai[k];
                    sum0 += aik * BTj0[k];
                    sum1 += aik * BTj1[k];
                }
                Ci[j] = gf_reduce(sum0);
                Ci[j + 1] = gf_reduce(sum1);
            }

            if (j < N) {
                const uint8_t *BTj = &BTp[j * K];
                Ci[j] = fastop_dotprod((unsigned int)K, Ai, BTj);
            }
        }
    }
}

static void fq_minus_a_plus_b_plus_c(uint8_t *dst, const uint8_t *a, const uint8_t *b, const uint8_t *c, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        uint32_t sum = (uint32_t)(QRUOV_q - a[i]) + b[i] + c[i];
        dst[i] = gf_reduce(sum);
    }
}

static void sigmap_sym_features(uint8_t *dst, const uint8_t *vec, int len)
{
    size_t idx = 0;
    for (int i = 0; i < len; i++) {
        for (int j = i; j < len; j++) {
            uint8_t prod[QRUOV_L];
            fql_mul(prod, &vec[i * QRUOV_L], &vec[j * QRUOV_L]);
            if (i != j) {
                fql_add(prod, prod, prod);
            }
            fql_perm(&dst[idx], prod);
            idx += QRUOV_L;
        }
    }
}

static void sigmap_rect_features(uint8_t *dst, const uint8_t *a, int rows, const uint8_t *b, int cols)
{
    size_t idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uint8_t prod[QRUOV_L];
            fql_mul(prod, &a[i * QRUOV_L], &b[j * QRUOV_L]);
            fql_perm(&dst[idx], prod);
            idx += QRUOV_L;
        }
    }
}

void qruov_keygen(const SEED_PK seed_pk, const SEED_SK seed_sk, uint8_t pk_p3[P3_SERIALIZED_LEN])
{
    uint8_t sdt[QRUOV_M * QRUOV_V * QRUOV_L];
    uint8_t sdt_eval[QRUOV_LL * QRUOV_M * QRUOV_V];
    uint8_t p3_chunk[P3_CHUNK_COEFF_LEN];
    int chunk_eqs = 0;
    size_t p3_offset = 0;
    prepare_sdt_from_seed(sdt, seed_sk);
    fql_aos_to_eval(sdt_eval, sdt, QRUOV_M, QRUOV_V);
    prg pk_prg;
    prg_init(&pk_prg, seed_pk);
    for (int eq = 0; eq < QRUOV_m; eq++) {
        uint8_t pi1[QRUOV_V * QRUOV_V * QRUOV_L];
        uint8_t pi2t[QRUOV_M * QRUOV_V * QRUOV_L];
        uint8_t pi1_eval[QRUOV_LL * QRUOV_V * QRUOV_V];
        uint8_t pi2t_eval[QRUOV_LL * QRUOV_M * QRUOV_V];
        uint8_t sdt_pi1_eval[QRUOV_LL * QRUOV_M * QRUOV_V];
        uint8_t sdt_pi1_reval[QRUOV_LL * QRUOV_M * QRUOV_V];
        uint8_t sdt_pi2_eval[QRUOV_LL * QRUOV_M * QRUOV_M];
        uint8_t pi2t_sd_eval[QRUOV_LL * QRUOV_M * QRUOV_M];
        uint8_t pi3_eval[QRUOV_LL * QRUOV_M * QRUOV_M];
        uint8_t pi3_soa[QRUOV_L * QRUOV_M * QRUOV_M];
        uint8_t pi3[QRUOV_M * QRUOV_M * QRUOV_L];
        prepare_pi1_from_prg(pi1, &pk_prg, (uint16_t)eq);
        prepare_pi2t_from_prg(pi2t, &pk_prg, (uint16_t)eq);

        fql_aos_to_eval(pi1_eval, pi1, QRUOV_V, QRUOV_V);
        fql_aos_to_eval(pi2t_eval, pi2t, QRUOV_M, QRUOV_V);

        fq_matmul_transposed_planes(sdt_pi2_eval, sdt_eval, pi2t_eval, QRUOV_LL, QRUOV_M, QRUOV_M, QRUOV_V);
        fq_transpose_planes(pi2t_sd_eval, sdt_pi2_eval, QRUOV_LL, QRUOV_M, QRUOV_M);

        fq_matmul_transposed_planes(sdt_pi1_eval, sdt_eval, pi1_eval, QRUOV_LL, QRUOV_M, QRUOV_V, QRUOV_V);
        reevaluate(sdt_pi1_reval, QRUOV_M * QRUOV_V, sdt_pi1_eval, QRUOV_M * QRUOV_V, QRUOV_M * QRUOV_V);
        fq_matmul_transposed_planes(pi3_eval, sdt_pi1_reval, sdt_eval, QRUOV_LL, QRUOV_M, QRUOV_M, QRUOV_V);
        fq_minus_a_plus_b_plus_c(pi3_eval, pi3_eval, pi2t_sd_eval, sdt_pi2_eval, sizeof(pi3_eval));

        interpolate(pi3_soa, QRUOV_M * QRUOV_M, pi3_eval, QRUOV_M * QRUOV_M, QRUOV_M * QRUOV_M);
        fql_soa_to_aos(pi3, pi3_soa, QRUOV_M, QRUOV_M);
        p3_store_upper_MxM(&p3_chunk[chunk_eqs * P3_EQ_COEFF_LEN], pi3);
        chunk_eqs++;
        if (chunk_eqs == P3_CHUNK_EQS) {
            store_fq(pk_p3 + p3_offset, p3_chunk, P3_CHUNK_COEFF_LEN);
            p3_offset += BITS2BYTE(QRUOV_q_LOG * P3_CHUNK_COEFF_LEN);
            chunk_eqs = 0;
        }
    }
    if (chunk_eqs > 0) {
        store_fq(pk_p3 + p3_offset, p3_chunk, (size_t)chunk_eqs * P3_EQ_COEFF_LEN);
    }
    prg_free(&pk_prg);
    secure_zero(sdt, sizeof(sdt));
    secure_zero(sdt_eval, sizeof(sdt_eval));
}

int qruov_sign(const SEED_PK seed_pk, const SEED_SK seed_sk, const SEED_SK seed_y, const SEED_SK seed_r, const SEED_SK seed_sol, const uint8_t msg[], size_t msg_len, SALT sig_r, SIG_S sig_s)
{
    return qruov_common_sign_scalar(seed_pk, seed_sk, seed_y, seed_r, seed_sol,
                                    msg, msg_len, sig_r, sig_s);
}

int qruov_verify(const SEED_PK seed_pk, const uint8_t pk_p3[P3_SERIALIZED_LEN], const uint8_t msg[], size_t msg_len, const SALT sig_r, const SIG_S sig_s)
{
    uint8_t t[QRUOV_m];
    const uint8_t *vinegar = sig_s;
    const uint8_t *oil = sig_s + QRUOV_V * QRUOV_L;
    uint8_t s11[QRUOV_n1];
    uint8_t s12[QRUOV_n2];
    uint8_t s22[P3_EQ_COEFF_LEN];
    uint8_t p3_chunk[P3_CHUNK_COEFF_LEN];
    prg pk_prg;
    if (!qruov_common_is_canonical_s(sig_s)) return 0;
    compute_mu_and_hash(t, seed_pk, msg, msg_len, sig_r);
    sigmap_sym_features(s11, vinegar, QRUOV_V);
    sigmap_rect_features(s12, vinegar, QRUOV_V, oil, QRUOV_M);
    sigmap_sym_features(s22, oil, QRUOV_M);
    prg_init(&pk_prg, seed_pk);
    int ok = 1;
    size_t p3_offset = 0;
    for (int eq_base = 0; eq_base < QRUOV_m; eq_base += P3_CHUNK_EQS) {
        int chunk_eqs = QRUOV_m - eq_base;
        if (chunk_eqs > P3_CHUNK_EQS) chunk_eqs = P3_CHUNK_EQS;
        size_t chunk_coeffs = (size_t)chunk_eqs * P3_EQ_COEFF_LEN;
        if (!load_fq_checked(p3_chunk, pk_p3 + p3_offset, chunk_coeffs)) {
            ok = 0;
            memset(p3_chunk, 0, chunk_coeffs);
        }
        p3_offset += BITS2BYTE(QRUOV_q_LOG * chunk_coeffs);
        for (int chunk_eq = 0; chunk_eq < chunk_eqs; chunk_eq++) {
            int eq = eq_base + chunk_eq;
            uint8_t pi1_raw[QRUOV_tau_n1];
            uint8_t pi2_raw[QRUOV_tau_n2];
            uint32_t sum = 0;
            prepare_pi1_raw_from_prg(pi1_raw, &pk_prg, (uint16_t)eq);
            sum += fastop_rejection_dotprod(QRUOV_n1, QRUOV_tau_n1, pi1_raw, s11);
            prepare_pi2_raw_from_prg(pi2_raw, &pk_prg, (uint16_t)eq);
            sum += 2u * fastop_rejection_dotprod(QRUOV_n2, QRUOV_tau_n2, pi2_raw, s12);
            sum += fastop_dotprod(P3_EQ_COEFF_LEN, s22, &p3_chunk[chunk_eq * P3_EQ_COEFF_LEN]);
            ok &= (gf_reduce(sum) == t[eq]);
        }
    }
    prg_free(&pk_prg);
    return ok;
}
