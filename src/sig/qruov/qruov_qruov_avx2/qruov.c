#include <string.h>
#include <immintrin.h>
#include <pthread.h>
#include "qruov.h"
#include "qrop.h"
#include "prg.h"
#include "util.h"
#include "linsys.h"
#include "gf.h"
#include "emi_transform.h"
#include "helpers.h"
#include "kernel.h"
#include "qruov_simd.h"
#include "fql.h"
#include "fastop.h"
#include "qruov_common.h"

__attribute__((aligned(16))) uint8_t qruov_avx2_bextmask[1u << 16][16];
__attribute__((aligned(16))) uint8_t qruov_avx2_bdepmask[1u << 16][16];
static pthread_once_t qruov_avx2_init_once = PTHREAD_ONCE_INIT;

static void qruov_avx2_init_impl(void)
{
    for (unsigned int i = 0; i < (1u << 16); i++) {
        uint8_t extmask[16];
        uint8_t depmask[16];
        memset(extmask, 0xFF, sizeof(extmask));
        memset(depmask, 0xFF, sizeof(depmask));
        int t = 0;
        for (int j = 0; j < 16; j++) {
            if (i & (1u << j)) {
                extmask[t] = (uint8_t)j;
                depmask[j] = (uint8_t)t;
                t++;
            }
        }
        memcpy(qruov_avx2_bextmask[i], extmask, sizeof(extmask));
        memcpy(qruov_avx2_bdepmask[i], depmask, sizeof(depmask));
    }
}

void qruov_init(void)
{
    pthread_once(&qruov_avx2_init_once, qruov_avx2_init_impl);
}

// Convert SoA [L][rows][cols] to AoS [rows][cols][L].
static void fql_soa_to_aos(uint8_t *dst_aos, const uint8_t *src_soa, int rows, int cols)
{
    int n = rows * cols;
    for (int i = 0; i < n; i++) {
        for (int l = 0; l < QRUOV_L; l++) {
            dst_aos[i * QRUOV_L + l] = src_soa[l * n + i];
        }
    }
}

static void fq_minus_a_plus_b_plus_c(uint8_t *dst, const uint8_t *a, const uint8_t *b, const uint8_t *c, size_t len)
{
    const __m256i q = _mm256_set1_epi8(QRUOV_q);
    size_t i = 0;
    for (; i + 31 < len; i += 32) {
        __m256i va = _mm256_loadu_si256((const __m256i *)&a[i]);
        __m256i vb = _mm256_loadu_si256((const __m256i *)&b[i]);
        __m256i vc = _mm256_loadu_si256((const __m256i *)&c[i]);
        __m256i tmp = _mm256_add_epi8(_mm256_xor_si256(va, q), vb);
        tmp = ymm_subq_if_greater_epi8(tmp);
        tmp = _mm256_add_epi8(tmp, vc);
        tmp = ymm_subq_if_greater_epi8(tmp);
        _mm256_storeu_si256((__m256i *)&dst[i], tmp);
    }
    for (; i < len; i++) {
        dst[i] = (uint8_t)(((a[i] ^ QRUOV_q) + b[i] + c[i]) % QRUOV_q);
    }
}

static void sigmap_sym_features(uint8_t *dst, const uint8_t *products_soa,
                                int base, int len, int stride)
{
    for (int i = 0; i < len; i++) {
        int row = base + i;
        fql_soa_to_aos_permute(&dst[SYMM_COMPACT_INDEX(i, i, len) * QRUOV_L],
                               &products_soa[row * stride + row], 1, stride * stride);
        fql_soa_to_aos_permute_double(&dst[SYMM_COMPACT_INDEX(i, i + 1, len) * QRUOV_L],
                                      &products_soa[row * stride + row + 1], len - i - 1,
                                      stride * stride);
    }
}

static void sigmap_rect_features(uint8_t *dst, const uint8_t *products_soa,
                                 int row_base, int rows, int col_base, int cols,
                                 int stride)
{
    for (int i = 0; i < rows; i++) {
        fql_soa_to_aos_permute(&dst[i * cols * QRUOV_L],
                               &products_soa[(row_base + i) * stride + col_base],
                               cols, stride * stride);
    }
}

static void sign_build_system(uint8_t eqn_out[QRUOV_m * QRUOV_m], uint8_t u_out[QRUOV_m], const uint8_t sd[QRUOV_tau_n2], const uint8_t y[QRUOV_V * QRUOV_L], const SEED_PK seed_pk)
{
    // AVX2-specific sign system construction with the same semantics as
    // qruov_common.c sign_build_system() used by ref/opt.
    enum { V4 = ROUND_UP4(QRUOV_V) };
    uint8_t y_soa[QRUOV_L * V4];
    uint8_t y_perm_soa[QRUOV_L * V4];
    uint8_t sdvnni[QRUOV_L * V4 * QRUOV_M];
    uint8_t p1vnni[QRUOV_L * V4 * V4] = {0};
    uint8_t p2vnni[QRUOV_L * V4 * QRUOV_M];
    uint8_t yT_p1[QRUOV_L * V4] = {0};
    uint8_t yT_p1_sd[QRUOV_L * QRUOV_M];
    uint8_t yT_p2[QRUOV_L * QRUOV_M];

    memset(y_soa, 0, sizeof(y_soa));
    fql_aos_to_soa(y_soa, y, QRUOV_V, V4);
    for (int l = 0; l < QRUOV_L; l++) {
        memcpy(&y_perm_soa[l * V4], &y_soa[QRUOV_perm(l) * V4], V4);
    }
    fql_aos_to_vnni(sdvnni, V4, QRUOV_M, sd, QRUOV_V, QRUOV_M);

    prg pk_prg;
    prg_init(&pk_prg, seed_pk);
    for (int eq = 0; eq < QRUOV_m; eq++) {
        uint8_t pi1_coeffs[QRUOV_tau_n1];
        uint8_t pi2_coeffs[QRUOV_tau_n2];
        prepare_pi1_coeffs_from_prg(pi1_coeffs, &pk_prg, (uint16_t)eq);
        fql_compact_sym_aos_to_vnni(p1vnni, V4, V4, pi1_coeffs, QRUOV_V);
        prepare_pi2_coeffs_from_prg(pi2_coeffs, &pk_prg, (uint16_t)eq);
        fql_aos_to_vnni(p2vnni, V4, QRUOV_M, pi2_coeffs, QRUOV_V, QRUOV_M);

        fql_matmul_vnni_fast(yT_p1, y_soa, p1vnni, 1, V4, V4);
        fql_matmul_vnni_fast(yT_p1_sd, yT_p1, sdvnni, 1, QRUOV_M, V4);
        fql_matmul_vnni_fast(yT_p2, y_soa, p2vnni, 1, QRUOV_M, V4);

        for (int l = 0; l < QRUOV_L; l++) {
            for (int j = 0; j < QRUOV_M; j++) {
                eqn_out[eq * QRUOV_m + j * QRUOV_L + QRUOV_perm(l)] =
                    (uint8_t)(((uint32_t)(0x2 ^ QRUOV_q) * yT_p1_sd[l * QRUOV_M + j] +
                               2u * yT_p2[l * QRUOV_M + j]) % QRUOV_q);
            }
        }
        u_out[eq] = fastop_dotprod((unsigned int)sizeof(yT_p1), yT_p1, y_perm_soa);
    }
    prg_free(&pk_prg);
    secure_zero(yT_p2, sizeof(yT_p2));
    secure_zero(yT_p1_sd, sizeof(yT_p1_sd));
    secure_zero(yT_p1, sizeof(yT_p1));
    secure_zero(sdvnni, sizeof(sdvnni));
    secure_zero(y_perm_soa, sizeof(y_perm_soa));
    secure_zero(y_soa, sizeof(y_soa));
}

void qruov_keygen(const SEED_PK seed_pk, const SEED_SK seed_sk, uint8_t pk_p3[P3_SERIALIZED_LEN])
{
    enum { V4 = ROUND_UP4(QRUOV_V) };
    uint8_t sdbuf[QRUOV_tau_n2];
    prepare_sd_from_seed(sdbuf, seed_sk);

    uint8_t sdvnni[QRUOV_LL * V4 * QRUOV_M];
    uint8_t sdt_eval[QRUOV_LL * QRUOV_M * V4];
    uint8_t sdt_p1_eval[QRUOV_LL * QRUOV_M * V4];
    uint8_t sdt_p1_reval[QRUOV_LL * QRUOV_M * V4];
    uint8_t p2t_sd_eval[QRUOV_LL * QRUOV_M * QRUOV_M];
    uint8_t sdt_p2_eval[QRUOV_LL * QRUOV_M * QRUOV_M];
    uint8_t p1vnni[QRUOV_LL * V4 * V4];
    uint8_t p2vnni[QRUOV_LL * V4 * QRUOV_M];
    uint8_t p3_chunk[P3_CHUNK_COEFF_LEN];
    int chunk_eqs = 0;
    size_t p3_offset = 0;

    fql_aos_to_eval_vnni(sdvnni, V4, QRUOV_M, sdbuf, QRUOV_V, QRUOV_M);
    fql_aos_to_eval_transposed_soa(sdt_eval, QRUOV_M, V4, sdbuf, QRUOV_V, QRUOV_M);

    prg pk_prg;
    prg_init(&pk_prg, seed_pk);
    for (int eq = 0; eq < QRUOV_m; eq++) {
        uint8_t pi1_coeffs[QRUOV_tau_n1];
        uint8_t pi2_coeffs[QRUOV_tau_n2];
        uint8_t pi3_eval[QRUOV_LL * QRUOV_M * QRUOV_M];
        uint8_t pi3_soa[QRUOV_L * QRUOV_M * QRUOV_M];
        uint8_t pi3[QRUOV_M * QRUOV_M * QRUOV_L];

        prepare_pi1_coeffs_from_prg(pi1_coeffs, &pk_prg, (uint16_t)eq);
        fql_compact_sym_aos_to_eval_vnni(p1vnni, V4, V4, pi1_coeffs, QRUOV_V);

        prepare_pi2_coeffs_from_prg(pi2_coeffs, &pk_prg, (uint16_t)eq);
        fql_aos_to_eval_vnni(p2vnni, V4, QRUOV_M, pi2_coeffs, QRUOV_V, QRUOV_M);

        fql_matmul_eval_vnni_fast(sdt_p2_eval, sdt_eval, p2vnni, QRUOV_M, QRUOV_M, V4);
        fq_eval_transpose_planes(p2t_sd_eval, sdt_p2_eval, QRUOV_M * QRUOV_M, QRUOV_M * QRUOV_M,
                                 QRUOV_M, QRUOV_M, QRUOV_M, QRUOV_M);
        fql_matmul_eval_vnni_fast(sdt_p1_eval, sdt_eval, p1vnni, QRUOV_M, V4, V4);
        reevaluate(sdt_p1_reval, QRUOV_M * V4, sdt_p1_eval, QRUOV_M * V4, QRUOV_M * V4);
        fql_matmul_eval_vnni_fast(pi3_eval, sdt_p1_reval, sdvnni, QRUOV_M, QRUOV_M, V4);
        fq_minus_a_plus_b_plus_c(pi3_eval, pi3_eval, p2t_sd_eval, sdt_p2_eval, sizeof(pi3_eval));

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
    secure_zero(sdt_p2_eval, sizeof(sdt_p2_eval));
    secure_zero(p2t_sd_eval, sizeof(p2t_sd_eval));
    secure_zero(sdt_p1_reval, sizeof(sdt_p1_reval));
    secure_zero(sdt_p1_eval, sizeof(sdt_p1_eval));
    secure_zero(sdt_eval, sizeof(sdt_eval));
    secure_zero(sdvnni, sizeof(sdvnni));
    secure_zero(sdbuf, sizeof(sdbuf));
}

int qruov_sign(const SEED_PK seed_pk, const SEED_SK seed_sk, const SEED_SK seed_y, const SEED_SK seed_r, const SEED_SK seed_sol, const uint8_t msg[], size_t msg_len, SALT sig_r, SIG_S sig_s)
{
    uint8_t sd[QRUOV_tau_n2];
    uint8_t y[QRUOV_V * QRUOV_L];   // vinegar
    uint8_t eqn[QRUOV_m * QRUOV_m]; // L
    uint8_t u[QRUOV_m];
    prepare_sd_from_seed(sd, seed_sk);
    prepare_y_from_seed(y, seed_y);
    sign_build_system(eqn, u, sd, y, seed_pk);
    uint8_t oil[QRUOV_m];
    int ret = qruov_common_sign_solve_oil(oil, sig_r, eqn, u, seed_pk, seed_r,
                                          seed_sol, msg, msg_len);
    if (ret == 0) {
        qruov_common_sig_s(sig_s, sd, y, oil);
    }
    secure_zero(oil, sizeof(oil));
    secure_zero(y, sizeof(y));
    secure_zero(u, sizeof(u));
    secure_zero(sd, sizeof(sd));
    secure_zero(eqn, sizeof(eqn));
    return ret;
}

int qruov_verify(const SEED_PK seed_pk, const uint8_t pk_p3[P3_SERIALIZED_LEN], const uint8_t msg[], size_t msg_len, const SALT sig_r, const SIG_S sig_s)
{
    uint8_t t[QRUOV_m];
    enum { N16 = ROUND_UP16(QRUOV_N) };
    uint8_t s_soa[QRUOV_L * N16];
    uint8_t ss_soa[QRUOV_L * N16 * N16];
    uint8_t s11[QRUOV_n1 + 32];
    uint8_t s12[QRUOV_n2 + 32];
    uint8_t s22[P3_EQ_COEFF_LEN];
    uint8_t p3_chunk[P3_CHUNK_COEFF_LEN];
    prg pk_prg;
    if (!qruov_common_is_canonical_s(sig_s)) return 0;
    compute_mu_and_hash(t, seed_pk, msg, msg_len, sig_r);
    memset(s_soa, 0, sizeof(s_soa));
    fql_aos_to_soa(s_soa, sig_s, QRUOV_N, N16);
    matmulfql_symmetric_upper(ss_soa, s_soa, N16);
    sigmap_sym_features(s11, ss_soa, 0, QRUOV_V, N16);
    sigmap_rect_features(s12, ss_soa, 0, QRUOV_V, QRUOV_V, QRUOV_M, N16);
    sigmap_sym_features(s22, ss_soa, QRUOV_V, QRUOV_M, N16);
    memset(&s11[QRUOV_n1], 0, 32);
    memset(&s12[QRUOV_n2], 0, 32);
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
            prepare_pi2_raw_from_prg(pi2_raw, &pk_prg, (uint16_t)eq);
            sum += fastop_rejection_dotprod(QRUOV_n1, QRUOV_tau_n1, pi1_raw, s11);
            sum += 2u * fastop_rejection_dotprod(QRUOV_n2, QRUOV_tau_n2, pi2_raw, s12);
            sum += fastop_dotprod(P3_EQ_COEFF_LEN, s22, &p3_chunk[chunk_eq * P3_EQ_COEFF_LEN]);
            ok &= (gf_reduce(sum) == t[eq]);
        }
    }
    prg_free(&pk_prg);
    return ok;
}
