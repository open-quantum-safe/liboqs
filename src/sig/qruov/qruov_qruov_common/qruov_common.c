#include "qruov_common.h"

#include <string.h>

#include "fql.h"
#include "gf.h"
#include "linsys.h"
#include "qrop.h"
#include "util.h"

static void fql_sub_V(uint8_t *dst, const uint8_t *x_V, const uint8_t *y_V)
{
    for (int i = 0; i < QRUOV_V; i++) {
        fql_sub(&dst[i * QRUOV_L], &x_V[i * QRUOV_L], &y_V[i * QRUOV_L]);
    }
}

int qruov_common_is_canonical_s(const SIG_S sig_s)
{
    for (int i = 0; i < SIG_S_COEFF_LEN; i++) {
        uint8_t a = sig_s[i];
        if (a != 0) {
            return a <= (QRUOV_q - 1) / 2;
        }
    }
    return 1;
}

void qruov_common_canonicalize_s(SIG_S sig_s)
{
    if (qruov_common_is_canonical_s(sig_s)) return;
    for (int i = 0; i < SIG_S_COEFF_LEN; i++) {
        if (sig_s[i] != 0) {
            sig_s[i] = (uint8_t)(QRUOV_q - sig_s[i]);
        }
    }
}

static void sign_build_system(uint8_t eqn_out[QRUOV_m * QRUOV_m],
                              uint8_t u_out[QRUOV_m],
                              const uint8_t sdt[QRUOV_M * QRUOV_V * QRUOV_L],
                              const uint8_t y[QRUOV_V * QRUOV_L],
                              const SEED_PK seed_pk)
{
    const int perm0 = QRUOV_perm(0);
    prg pk_prg;
    uint8_t pi1_y[QRUOV_V * QRUOV_L];
    uint8_t y_pi1_y[QRUOV_L];
    uint8_t y_pi1_sd[QRUOV_M * QRUOV_L];
    uint8_t y_pi2[QRUOV_M * QRUOV_L];
    uint8_t y_fi2[QRUOV_M * QRUOV_L];
    prg_init(&pk_prg, seed_pk);
    for (int eq = 0; eq < QRUOV_m; eq++) {
        uint8_t pi1[QRUOV_V * QRUOV_V * QRUOV_L];
        uint8_t pi2t[QRUOV_M * QRUOV_V * QRUOV_L];
        prepare_pi1_from_prg(pi1, &pk_prg, (uint16_t)eq);
        prepare_pi2t_from_prg(pi2t, &pk_prg, (uint16_t)eq);
        fql_mul_VxV_V(pi1_y, pi1, y);
        fql_dot_V(y_pi1_y, y, pi1_y);
        fql_mul_MxV_V(y_pi1_sd, sdt, pi1_y);
        fql_mul_MxV_V(y_pi2, pi2t, y);
        fql_sub_M(y_fi2, y_pi2, y_pi1_sd);
        fql_add_M(y_fi2, y_fi2, y_fi2);
        fql_perm_M(&eqn_out[eq * QRUOV_m], y_fi2);
        u_out[eq] = y_pi1_y[perm0];
    }
    prg_free(&pk_prg);
    secure_zero(y_fi2, sizeof(y_fi2));
    secure_zero(y_pi2, sizeof(y_pi2));
    secure_zero(y_pi1_sd, sizeof(y_pi1_sd));
    secure_zero(y_pi1_y, sizeof(y_pi1_y));
    secure_zero(pi1_y, sizeof(pi1_y));
}

void qruov_common_sig_s(SIG_S sig_s,
                        const uint8_t sd[QRUOV_tau_n2],
                        const uint8_t y[QRUOV_V * QRUOV_L],
                        const uint8_t oil[QRUOV_m])
{
    uint8_t sd_o[QRUOV_V * QRUOV_L];
    fql_mul_VxM_M(sd_o, sd, oil);
    fql_sub_V(sig_s, y, sd_o);
    memcpy(&sig_s[QRUOV_V * QRUOV_L], oil, QRUOV_M * QRUOV_L);
    qruov_common_canonicalize_s(sig_s);
    secure_zero(sd_o, sizeof(sd_o));
}

int qruov_common_sign_solve_oil(uint8_t oil_out[QRUOV_m], SALT sig_r_out,
                                uint8_t eqn[QRUOV_m * QRUOV_m],
                                const uint8_t u[QRUOV_m],
                                const SEED_PK seed_pk, const SEED_SK seed_r,
                                const SEED_SK seed_sol, const uint8_t *msg,
                                size_t msg_len)
{
    int ret = 1;
    uint8_t mu[64];
    compute_mu(mu, seed_pk, msg, msg_len);
    uint8_t free_oil[QRUOV_m];
    prepare_sol_from_seed(free_oil, seed_sol);
    uint8_t t[QRUOV_m];
    uint8_t rhs[QRUOV_m];
    uint8_t consistency_rhs[QRUOV_m];
    linsys_echelon echelon;
    linsys_lu_decompose(eqn, &echelon);
    int rank = echelon.rank;
    if (rank < QRUOV_m - QRUOV_delta) {
        goto cleanup;
    }
    if (rank < QRUOV_m) {
        linsys_prepare_consistency(&echelon, u, consistency_rhs);
    }
    shake256 r_stream;
    shake256_init(&r_stream);
    shake256_update(&r_stream, seed_r, SEED_SK_LEN);
    for (;;) {
        shake256_squeeze(&r_stream, sig_r_out, SALT_LEN);
        compute_hash(t, mu, sig_r_out);
        if (rank == QRUOV_m || linsys_check_prepared_consistency(&echelon, consistency_rhs, t)) {
            for (int i = 0; i < QRUOV_m; i++) rhs[i] = gf_sub(t[i], u[i]);
            linsys_sample_solution(&echelon, rhs, free_oil, oil_out);
            ret = 0;
            break;
        }
    }
    shake256_free(&r_stream);
cleanup:
    secure_zero(consistency_rhs, sizeof(consistency_rhs));
    secure_zero(rhs, sizeof(rhs));
    secure_zero(t, sizeof(t));
    secure_zero(&echelon, sizeof(echelon));
    secure_zero(free_oil, sizeof(free_oil));
    secure_zero(mu, sizeof(mu));
    return ret;
}

int qruov_common_sign_scalar(const SEED_PK seed_pk, const SEED_SK seed_sk,
                             const SEED_SK seed_y, const SEED_SK seed_r,
                             const SEED_SK seed_sol, const uint8_t msg[],
                             size_t msg_len, SALT sig_r, SIG_S sig_s)
{
    uint8_t sd[QRUOV_tau_n2];
    uint8_t sdt[QRUOV_M * QRUOV_V * QRUOV_L];
    uint8_t y[QRUOV_V * QRUOV_L];   // vinegar
    uint8_t eqn[QRUOV_m * QRUOV_m]; // L
    uint8_t u[QRUOV_m];
    prepare_sd_from_seed(sd, seed_sk);
    prepare_sdt_from_sd(sdt, sd);
    prepare_y_from_seed(y, seed_y);
    sign_build_system(eqn, u, sdt, y, seed_pk);
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
    secure_zero(sdt, sizeof(sdt));
    secure_zero(eqn, sizeof(eqn));
    return ret;
}
