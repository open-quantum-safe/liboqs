#ifndef QRUOV_COMMON_H
#define QRUOV_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include "qruov.h"

void qruov_common_sig_s(SIG_S sig_s, const uint8_t sd[QRUOV_tau_n2],
                        const uint8_t y[QRUOV_V * QRUOV_L],
                        const uint8_t oil[QRUOV_m]);
int qruov_common_is_canonical_s(const SIG_S sig_s);
void qruov_common_canonicalize_s(SIG_S sig_s);

int qruov_common_sign_solve_oil(uint8_t oil_out[QRUOV_m], SALT sig_r_out,
                                uint8_t eqn[QRUOV_m * QRUOV_m],
                                const uint8_t u[QRUOV_m],
                                const SEED_PK seed_pk, const SEED_SK seed_r,
                                const SEED_SK seed_sol, const uint8_t *msg,
                                size_t msg_len);
int qruov_common_sign_scalar(const SEED_PK seed_pk, const SEED_SK seed_sk,
                             const SEED_SK seed_y, const SEED_SK seed_r,
                             const SEED_SK seed_sol, const uint8_t msg[],
                             size_t msg_len, SALT sig_r, SIG_S sig_s);

#endif // QRUOV_COMMON_H
