#include "qruov.h"
#include "qrop.h"
#include "prg.h"
#include "util.h"
#include "linsys.h"
#include "gf.h"
#include "fql.h"
#include "qruov_common.h"

void qruov_init(void)
{
}

static void fql_neg_upper_mul_MxV_MxVt(uint8_t *pi3, const uint8_t *A_MxV, const uint8_t *B_MxV)
{
    uint8_t zero[QRUOV_L];
    fql_zero(zero);
    // Only the upper triangle of pi3 is defined in ref keygen.
    for (int i = 0; i < QRUOV_M; i++) {
        for (int j = i; j < QRUOV_M; j++) {
            uint8_t tmp[QRUOV_L];
            fql_dot_V(tmp, &A_MxV[i * QRUOV_V * QRUOV_L], &B_MxV[j * QRUOV_V * QRUOV_L]);
            fql_sub(&pi3[(i * QRUOV_M + j) * QRUOV_L], zero, tmp);
        }
    }
}

static void fql_add_upper_symmetrized_MxM(uint8_t *pi3, const uint8_t *src)
{
    for (int i = 0; i < QRUOV_M; i++) {
        for (int j = i; j < QRUOV_M; j++) {
            uint8_t symm[QRUOV_L];
            fql_add(symm, &src[(i * QRUOV_M + j) * QRUOV_L], &src[(j * QRUOV_M + i) * QRUOV_L]);
            fql_add(&pi3[(i * QRUOV_M + j) * QRUOV_L], &pi3[(i * QRUOV_M + j) * QRUOV_L], symm);
        }
    }
}

void qruov_keygen(const SEED_PK seed_pk, const SEED_SK seed_sk, uint8_t pk_p3[P3_SERIALIZED_LEN])
{
    uint8_t sdt[QRUOV_M * QRUOV_V * QRUOV_L];
    uint8_t pi1[QRUOV_V * QRUOV_V * QRUOV_L];
    uint8_t pi2t[QRUOV_M * QRUOV_V * QRUOV_L];
    uint8_t sdt_pi1[QRUOV_M * QRUOV_V * QRUOV_L];
    uint8_t sdt_pi2[QRUOV_M * QRUOV_M * QRUOV_L];
    uint8_t pi3[QRUOV_M * QRUOV_M * QRUOV_L];
    uint8_t p3_chunk[P3_CHUNK_COEFF_LEN];
    int chunk_eqs = 0;
    size_t p3_offset = 0;
    prepare_sdt_from_seed(sdt, seed_sk);
    prg pk_prg;
    prg_init(&pk_prg, seed_pk);
    for (int eq = 0; eq < QRUOV_m; eq++) {
        prepare_pi1_from_prg(pi1, &pk_prg, (uint16_t)eq);
        prepare_pi2t_from_prg(pi2t, &pk_prg, (uint16_t)eq);
        fql_mul_MxV_VxVt(sdt_pi1, sdt, pi1);
        // The lower triangle is intentionally left uninitialized: the helpers
        // below update only the upper triangle, and only that upper triangle is packed.
        fql_neg_upper_mul_MxV_MxVt(pi3, sdt_pi1, sdt);
        fql_mul_MxV_MxVt(sdt_pi2, sdt, pi2t);
        fql_add_upper_symmetrized_MxM(pi3, sdt_pi2);
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
    secure_zero(sdt_pi1, sizeof(sdt_pi1));
    secure_zero(sdt_pi2, sizeof(sdt_pi2));
}

int qruov_sign(const SEED_PK seed_pk, const SEED_SK seed_sk, const SEED_SK seed_y, const SEED_SK seed_r, const SEED_SK seed_sol, const uint8_t msg[], size_t msg_len, SALT sig_r, SIG_S sig_s)
{
    return qruov_common_sign_scalar(seed_pk, seed_sk, seed_y, seed_r, seed_sol,
                                    msg, msg_len, sig_r, sig_s);
}

int qruov_verify(const SEED_PK seed_pk, const uint8_t pk_p3[P3_SERIALIZED_LEN], const uint8_t msg[], size_t msg_len, const SALT sig_r, const SIG_S sig_s)
{
    uint8_t t[QRUOV_m];
    const int perm0 = QRUOV_perm(0);
    const uint8_t *vinegar = sig_s;
    const uint8_t *oil = sig_s + QRUOV_V * QRUOV_L;
    uint8_t p3_chunk[P3_CHUNK_COEFF_LEN];
    prg pk_prg;
    if (!qruov_common_is_canonical_s(sig_s)) return 0;
    compute_mu_and_hash(t, seed_pk, msg, msg_len, sig_r);
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
            uint8_t pi1[QRUOV_V * QRUOV_V * QRUOV_L];
            uint8_t pi2[QRUOV_tau_n2];
            uint8_t pi3[QRUOV_M * QRUOV_M * QRUOV_L];
            uint8_t pi1_v[QRUOV_V * QRUOV_L];
            uint8_t pi2_o[QRUOV_V * QRUOV_L];
            uint8_t pi3_o[QRUOV_M * QRUOV_L];
            uint8_t v_pi1_v[QRUOV_L];
            uint8_t v_pi2_o[QRUOV_L];
            uint8_t o_pi3_o[QRUOV_L];
            uint8_t lhs[QRUOV_L];
            prepare_pi1_from_prg(pi1, &pk_prg, (uint16_t)eq);
            prepare_pi2_coeffs_from_prg(pi2, &pk_prg, (uint16_t)eq);
            fql_mul_VxV_V(pi1_v, pi1, vinegar);
            fql_dot_V(v_pi1_v, vinegar, pi1_v);
            fql_mul_VxM_M(pi2_o, pi2, oil);
            fql_dot_V(v_pi2_o, vinegar, pi2_o);
            unpack_upper_to_symm_MxM(pi3, &p3_chunk[chunk_eq * P3_EQ_COEFF_LEN]);
            fql_mul_MxM_M(pi3_o, pi3, oil);
            fql_dot_M(o_pi3_o, oil, pi3_o);
            fql_add(lhs, v_pi1_v, v_pi2_o);
            fql_add(lhs, lhs, v_pi2_o);
            fql_add(lhs, lhs, o_pi3_o);
            ok &= (lhs[perm0] == t[eq]);
        }
    }
    prg_free(&pk_prg);
    return ok;
}
