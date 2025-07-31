/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLD_NATIVE_AARCH64_SRC_ARITH_NATIVE_AARCH64_H
#define MLD_NATIVE_AARCH64_SRC_ARITH_NATIVE_AARCH64_H

#include <stdint.h>
#include "../../../common.h"

#define mld_aarch64_ntt_zetas_layer123456 \
  MLD_NAMESPACE(mld_aarch64_ntt_zetas_layer123456)
#define mld_aarch64_ntt_zetas_layer78 \
  MLD_NAMESPACE(mld_aarch64_ntt_zetas_layer78)

#define mld_aarch64_intt_zetas_layer78 \
  MLD_NAMESPACE(mld_aarch64_intt_zetas_layer78)
#define mld_aarch64_intt_zetas_layer123456 \
  MLD_NAMESPACE(mld_aarch64_intt_zetas_layer123456)

extern const int32_t mld_aarch64_ntt_zetas_layer123456[];
extern const int32_t mld_aarch64_ntt_zetas_layer78[];

extern const int32_t mld_aarch64_intt_zetas_layer78[];
extern const int32_t mld_aarch64_intt_zetas_layer123456[];

extern const uint8_t mld_rej_uniform_table[];
extern const uint8_t mld_rej_uniform_eta_table[];


/*
 * Sampling 256 coefficients mod 15 using rejection sampling from 4 bits.
 * Expected number of required bytes: (256 * (16/15))/2 = 136.5 bytes.
 * We sample 1 block (=136 bytes) of SHAKE256_RATE output initially.
 * Sampling 2 blocks initially results in slightly worse performance.
 */
#define MLD_AARCH64_REJ_UNIFORM_ETA2_BUFLEN (1 * 136)
/*
 * Sampling 256 coefficients mod 9 using rejection sampling from 4 bits.
 * Expected number of required bytes: (256 * (16/9))/2 = 227.5 bytes.
 * We sample 2 blocks (=272 bytes) of SHAKE256_RATE output initially.
 */
#define MLD_AARCH64_REJ_UNIFORM_ETA4_BUFLEN (2 * 136)

#define mld_ntt_asm MLD_NAMESPACE(ntt_asm)
void mld_ntt_asm(int32_t *, const int32_t *, const int32_t *);

#define mld_intt_asm MLD_NAMESPACE(intt_asm)
void mld_intt_asm(int32_t *, const int32_t *, const int32_t *);

#define mld_rej_uniform_asm MLD_NAMESPACE(rej_uniform_asm)
uint64_t mld_rej_uniform_asm(int32_t *r, const uint8_t *buf, unsigned buflen,
                             const uint8_t *table);

#define mld_rej_uniform_eta2_asm MLD_NAMESPACE(rej_uniform_eta2_asm)
unsigned mld_rej_uniform_eta2_asm(int32_t *r, const uint8_t *buf,
                                  unsigned buflen, const uint8_t *table);

#define mld_rej_uniform_eta4_asm MLD_NAMESPACE(rej_uniform_eta4_asm)
unsigned mld_rej_uniform_eta4_asm(int32_t *r, const uint8_t *buf,
                                  unsigned buflen, const uint8_t *table);

#endif /* !MLD_NATIVE_AARCH64_SRC_ARITH_NATIVE_AARCH64_H */
