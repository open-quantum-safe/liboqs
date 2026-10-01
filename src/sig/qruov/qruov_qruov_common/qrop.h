#ifndef QROP_H
#define QROP_H

#include "qruov.h"
#include "fastop.h"
#include "prg.h"

#define P3_CHUNK_EQS 8
#define P3_CHUNK_COEFF_LEN (P3_CHUNK_EQS * P3_EQ_COEFF_LEN)

void rejection_sample(uint8_t *dst, int tau, int length);
void compute_mu_and_hash(uint8_t out[QRUOV_m], const SEED_PK seed_pk, const uint8_t *msg, size_t msg_len, const SALT sig_r);
void compute_mu(uint8_t mu[64], const SEED_PK seed_pk, const uint8_t *msg, size_t msg_len);
void compute_hash(uint8_t t[QRUOV_m], const uint8_t mu[64], const SALT sig_r);
void prepare_seed_pk_from_seed(SEED_PK seed_pk, const SEED_SK seed_sk);
void prepare_sd_from_seed(uint8_t sd[QRUOV_tau_n2], const SEED_SK seed_sk);
void prepare_sdt_from_sd(uint8_t *sdt_MxV, const uint8_t sd[QRUOV_tau_n2]);
void prepare_y_from_seed(uint8_t y[QRUOV_V * QRUOV_L], const SEED_SK seed_y);
void prepare_sol_from_seed(uint8_t free_oil[QRUOV_m], const SEED_SK seed_sol);
void prepare_sdt_from_seed(uint8_t *sdt_MxV, const SEED_SK seed_sk);
void prepare_pi1_raw_from_prg(uint8_t pi1_raw[QRUOV_tau_n1], prg *pk_prg, uint16_t eq);
void prepare_pi2_raw_from_prg(uint8_t pi2_raw[QRUOV_tau_n2], prg *pk_prg, uint16_t eq);
void prepare_pi1_coeffs_from_prg(uint8_t pi1_coeffs[QRUOV_tau_n1], prg *pk_prg, uint16_t eq);
void prepare_pi2_coeffs_from_prg(uint8_t pi2_coeffs[QRUOV_tau_n2], prg *pk_prg, uint16_t eq);
void prepare_pi1_from_prg(uint8_t *pi1_VxV, prg *pk_prg, uint16_t eq);
void prepare_pi2t_from_prg(uint8_t *pi2t_MxV, prg *pk_prg, uint16_t eq);
void p3_store_upper_MxM(uint8_t upper[P3_EQ_COEFF_LEN],
                        const uint8_t src[QRUOV_M * QRUOV_M * QRUOV_L]);
void unpack_upper_to_symm_VxV(uint8_t dst[QRUOV_V * QRUOV_V * QRUOV_L],
                              const uint8_t packed_upper[QRUOV_n1]);
void unpack_upper_to_symm_MxM(uint8_t dst[QRUOV_M * QRUOV_M * QRUOV_L],
                              const uint8_t packed_upper[P3_EQ_COEFF_LEN]);
int load_fq_checked(uint8_t *dst, const uint8_t *src, size_t dst_len);

// Checks that unused high bits in the last packed byte are zero.
static inline int check_fq_trailing_bits(const uint8_t *src, size_t dst_len)
{
    const unsigned used_bits = (unsigned)((QRUOV_q_LOG * dst_len) & 7u);
    if (used_bits == 0) return 1;
    const size_t src_len = BITS2BYTE(QRUOV_q_LOG * dst_len);
    const uint8_t mask = (uint8_t)((1u << used_bits) - 1u);
    return (src[src_len - 1] & (uint8_t)~mask) == 0;
}

// Requires: src[i] in [0, QRUOV_q). No validation/reduction for speed.
static inline void store_fq(uint8_t *dst, const uint8_t *src, size_t src_len)
{
    fastop_store_fq(dst, src, src_len);
}

// Requires: src is canonical packed Fq. No validation/reduction for speed.
static inline void load_fq(uint8_t *dst, const uint8_t *src, size_t dst_len)
{
    fastop_load_fq(dst, src, dst_len);
}

#endif // QROP_H
