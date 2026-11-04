#include <string.h>
#include "qrop.h"
#include "fastop.h"
#include "prim.h"
#include "util.h"

static void expand_transposed_matrix_MxV(uint8_t *mat, const uint8_t *vec)
{
    const uint8_t *ptr = vec;
    for (int i = 0; i < QRUOV_V; i++) {
        for (int j = 0; j < QRUOV_M; j++) {
            memcpy(&mat[(j * QRUOV_V + i) * QRUOV_L], ptr, QRUOV_L);
            ptr += QRUOV_L;
        }
    }
}

static void unpack_upper_to_symm(uint8_t *dst, const uint8_t *packed_upper, int dim)
{
    size_t offset = 0;
    for (int i = 0; i < dim; i++) {
        for (int j = i; j < dim; j++) {
            memcpy(&dst[(i * dim + j) * QRUOV_L], packed_upper + offset, QRUOV_L);
            memcpy(&dst[(j * dim + i) * QRUOV_L], packed_upper + offset, QRUOV_L);
            offset += QRUOV_L;
        }
    }
}

void p3_store_upper_MxM(uint8_t upper[P3_EQ_COEFF_LEN],
                        const uint8_t src[QRUOV_M * QRUOV_M * QRUOV_L])
{
    size_t offset = 0;
    for (int i = 0; i < QRUOV_M; i++) {
        for (int j = i; j < QRUOV_M; j++) {
            memcpy(upper + offset, &src[(i * QRUOV_M + j) * QRUOV_L], QRUOV_L);
            offset += QRUOV_L;
        }
    }
}

void unpack_upper_to_symm_VxV(uint8_t dst[QRUOV_V * QRUOV_V * QRUOV_L],
                              const uint8_t packed_upper[QRUOV_n1])
{
    unpack_upper_to_symm(dst, packed_upper, QRUOV_V);
}

void unpack_upper_to_symm_MxM(uint8_t dst[QRUOV_M * QRUOV_M * QRUOV_L],
                              const uint8_t packed_upper[P3_EQ_COEFF_LEN])
{
    unpack_upper_to_symm(dst, packed_upper, QRUOV_M);
}

int load_fq_checked(uint8_t *dst, const uint8_t *src, size_t dst_len)
{
    if (!check_fq_trailing_bits(src, dst_len)) return 0;
    load_fq(dst, src, dst_len);
    return memchr(dst, QRUOV_q, dst_len) == NULL;
}

void rejection_sample(uint8_t *dst, int tau, int length)
{
    fastop_rejection_sample(dst, tau, length);
}

void compute_mu_and_hash(uint8_t out[QRUOV_m], const SEED_PK seed_pk, const uint8_t *msg, size_t msg_len, const SALT sig_r)
{
    uint8_t mu[64];
    uint8_t sampled[QRUOV_tau_m];
    shake256 st;
    shake256_init(&st);
    shake256_update(&st, seed_pk, SEED_PK_LEN);
    shake256_update(&st, msg, msg_len);
    shake256_digestfinal(&st, mu, sizeof(mu));
    shake256_reset(&st);
    shake256_update(&st, mu, sizeof(mu));
    shake256_update(&st, sig_r, SALT_LEN);
    shake256_digestfinal(&st, sampled, sizeof(sampled));
    shake256_free(&st);
    rejection_sample(sampled, QRUOV_tau_m, QRUOV_m);
    memcpy(out, sampled, QRUOV_m);
}

void compute_mu(uint8_t mu[64], const SEED_PK seed_pk, const uint8_t *msg, size_t msg_len)
{
    shake256 st;
    shake256_init(&st);
    shake256_update(&st, seed_pk, SEED_PK_LEN);
    shake256_update(&st, msg, msg_len);
    shake256_digestfinal(&st, mu, 64);
    shake256_free(&st);
}

void compute_hash(uint8_t t[QRUOV_m], const uint8_t mu[64], const SALT sig_r)
{
    uint8_t sampled[QRUOV_tau_m];
    shake256 st;
    shake256_init(&st);
    shake256_update(&st, mu, 64);
    shake256_update(&st, sig_r, SALT_LEN);
    shake256_digestfinal(&st, sampled, sizeof(sampled));
    shake256_free(&st);
    rejection_sample(sampled, QRUOV_tau_m, QRUOV_m);
    memcpy(t, sampled, QRUOV_m);
}

void prepare_y_from_seed(uint8_t y[QRUOV_V * QRUOV_L], const SEED_SK seed_y)
{
    uint8_t y_buf[QRUOV_tau_v];
    shake256 st;
    shake256_init(&st);
    shake256_update(&st, seed_y, SEED_SK_LEN);
    shake256_digestfinal(&st, y_buf, QRUOV_tau_v);
    shake256_free(&st);
    rejection_sample(y_buf, QRUOV_tau_v, QRUOV_v);
    memcpy(y, y_buf, QRUOV_V * QRUOV_L);
    secure_zero(y_buf, sizeof(y_buf));
}

void prepare_sol_from_seed(uint8_t free_oil[QRUOV_m], const SEED_SK seed_sol)
{
    uint8_t free_oil_buf[QRUOV_tau_m];
    shake256 st;
    shake256_init(&st);
    shake256_update(&st, seed_sol, SEED_SK_LEN);
    shake256_digestfinal(&st, free_oil_buf, QRUOV_tau_m);
    shake256_free(&st);
    rejection_sample(free_oil_buf, QRUOV_tau_m, QRUOV_m);
    memcpy(free_oil, free_oil_buf, QRUOV_m);
    secure_zero(free_oil_buf, sizeof(free_oil_buf));
}

void prepare_seed_pk_from_seed(SEED_PK seed_pk, const SEED_SK seed_sk)
{
    uint8_t idx_be[2];
    shake256 st;
    shake256_init(&st);
    shake256_update(&st, seed_sk, SEED_SK_LEN);
    store_u16_be(idx_be, 1);
    shake256_update(&st, idx_be, sizeof(idx_be));
    shake256_digestfinal(&st, seed_pk, SEED_PK_LEN);
    shake256_free(&st);
}

void prepare_sd_from_seed(uint8_t sd[QRUOV_tau_n2], const SEED_SK seed_sk)
{
    uint8_t idx_be[2];
    shake256 st;
    shake256_init(&st);
    shake256_update(&st, seed_sk, SEED_SK_LEN);
    store_u16_be(idx_be, 0);
    shake256_update(&st, idx_be, sizeof(idx_be));
    shake256_digestfinal(&st, sd, QRUOV_tau_n2);
    shake256_free(&st);
    rejection_sample(sd, QRUOV_tau_n2, QRUOV_n2);
}

void prepare_sdt_from_sd(uint8_t *sdt_MxV, const uint8_t sd[QRUOV_tau_n2])
{
    expand_transposed_matrix_MxV(sdt_MxV, sd);
}

void prepare_sdt_from_seed(uint8_t *sdt_MxV, const SEED_SK seed_sk)
{
    uint8_t sd[QRUOV_tau_n2];
    prepare_sd_from_seed(sd, seed_sk);
    prepare_sdt_from_sd(sdt_MxV, sd);
    secure_zero(sd, sizeof(sd));
}

void prepare_pi1_raw_from_prg(uint8_t pi1_raw[QRUOV_tau_n1], prg *pk_prg, uint16_t eq)
{
    prg_yield(pk_prg, pi1_raw, (uint16_t)(2 * eq), QRUOV_tau_n1);
}

void prepare_pi1_coeffs_from_prg(uint8_t pi1_coeffs[QRUOV_tau_n1], prg *pk_prg, uint16_t eq)
{
    prepare_pi1_raw_from_prg(pi1_coeffs, pk_prg, eq);
    rejection_sample(pi1_coeffs, QRUOV_tau_n1, QRUOV_n1);
}

void prepare_pi1_from_prg(uint8_t *pi1_VxV, prg *pk_prg, uint16_t eq)
{
    uint8_t pi1_coeffs[QRUOV_tau_n1];
    prepare_pi1_coeffs_from_prg(pi1_coeffs, pk_prg, eq);
    unpack_upper_to_symm_VxV(pi1_VxV, pi1_coeffs);
}

void prepare_pi2_raw_from_prg(uint8_t pi2_raw[QRUOV_tau_n2], prg *pk_prg, uint16_t eq)
{
    prg_yield(pk_prg, pi2_raw, (uint16_t)(2 * eq + 1), QRUOV_tau_n2);
}

void prepare_pi2_coeffs_from_prg(uint8_t pi2_coeffs[QRUOV_tau_n2], prg *pk_prg, uint16_t eq)
{
    prepare_pi2_raw_from_prg(pi2_coeffs, pk_prg, eq);
    rejection_sample(pi2_coeffs, QRUOV_tau_n2, QRUOV_n2);
}

void prepare_pi2t_from_prg(uint8_t *pi2t_MxV, prg *pk_prg, uint16_t eq)
{
    uint8_t pi2_coeffs[QRUOV_tau_n2];
    prepare_pi2_coeffs_from_prg(pi2_coeffs, pk_prg, eq);
    expand_transposed_matrix_MxV(pi2t_MxV, pi2_coeffs);
}
