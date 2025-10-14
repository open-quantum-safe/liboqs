/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_NATIVE_X86_64_SRC_ARITH_NATIVE_X86_64_H
#define MLD_NATIVE_X86_64_SRC_ARITH_NATIVE_X86_64_H
#include "../../../common.h"

#include <immintrin.h>
#include <stdint.h>
#include "consts.h"

#define MLD_AVX2_REJ_UNIFORM_BUFLEN \
  (5 * 168) /* REJ_UNIFORM_NBLOCKS * SHAKE128_RATE */


/*
 * Sampling 256 coefficients mod 15 using rejection sampling from 4 bits.
 * Expected number of required bytes: (256 * (16/15))/2 = 136.5 bytes.
 * We sample 1 block (=136 bytes) of SHAKE256_RATE output initially.
 * Sampling 2 blocks initially results in slightly worse performance.
 */
#define MLD_AVX2_REJ_UNIFORM_ETA2_BUFLEN (1 * 136)

/*
 * Sampling 256 coefficients mod 9 using rejection sampling from 4 bits.
 * Expected number of required bytes: (256 * (16/9))/2 = 227.5 bytes.
 * We sample 2 blocks (=272 bytes) of SHAKE256_RATE output initially.
 */
#define MLD_AVX2_REJ_UNIFORM_ETA4_BUFLEN (2 * 136)

#define mld_rej_uniform_table MLD_NAMESPACE(mld_rej_uniform_table)
extern const uint8_t mld_rej_uniform_table[256][8];

#define mld_ntt_avx2 MLD_NAMESPACE(ntt_avx2)
void mld_ntt_avx2(__m256i *r, const __m256i *mld_qdata);

#define mld_invntt_avx2 MLD_NAMESPACE(invntt_avx2)
void mld_invntt_avx2(__m256i *r, const __m256i *mld_qdata);

#define mld_nttunpack_avx2 MLD_NAMESPACE(nttunpack_avx2)
void mld_nttunpack_avx2(__m256i *r);

#define mld_rej_uniform_avx2 MLD_NAMESPACE(mld_rej_uniform_avx2)
unsigned mld_rej_uniform_avx2(int32_t *r,
                              const uint8_t buf[MLD_AVX2_REJ_UNIFORM_BUFLEN]);

#define mld_rej_uniform_eta2_avx2 MLD_NAMESPACE(mld_rej_uniform_eta2_avx2)
unsigned mld_rej_uniform_eta2_avx2(
    int32_t *r, const uint8_t buf[MLD_AVX2_REJ_UNIFORM_ETA2_BUFLEN]);

#define mld_rej_uniform_eta4_avx2 MLD_NAMESPACE(mld_rej_uniform_eta4_avx2)
unsigned mld_rej_uniform_eta4_avx2(
    int32_t *r, const uint8_t buf[MLD_AVX2_REJ_UNIFORM_ETA4_BUFLEN]);

#define mld_poly_decompose_32_avx2 MLD_NAMESPACE(mld_poly_decompose_32_avx2)
void mld_poly_decompose_32_avx2(__m256i *a1, __m256i *a0, const __m256i *a);

#define mld_poly_decompose_88_avx2 MLD_NAMESPACE(mld_poly_decompose_88_avx2)
void mld_poly_decompose_88_avx2(__m256i *a1, __m256i *a0, const __m256i *a);

#endif /* !MLD_NATIVE_X86_64_SRC_ARITH_NATIVE_X86_64_H */
