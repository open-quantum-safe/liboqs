/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLD_NATIVE_X86_64_META_H
#define MLD_NATIVE_X86_64_META_H

/* Identifier for this backend so that source and assembly files
 * in the build can be appropriately guarded. */
#define MLD_ARITH_BACKEND_X86_64_DEFAULT

#define MLD_USE_NATIVE_NTT_CUSTOM_ORDER
#define MLD_USE_NATIVE_NTT
#define MLD_USE_NATIVE_INTT
#define MLD_USE_NATIVE_REJ_UNIFORM
#define MLD_USE_NATIVE_REJ_UNIFORM_ETA2
#define MLD_USE_NATIVE_REJ_UNIFORM_ETA4
#define MLD_USE_NATIVE_POLY_DECOMPOSE_32
#define MLD_USE_NATIVE_POLY_DECOMPOSE_88

#if !defined(__ASSEMBLER__)
#include <string.h>
#include "../../common.h"
#include "src/arith_native_x86_64.h"

static MLD_INLINE void mld_poly_permute_bitrev_to_custom(int32_t data[MLDSA_N])
{
  mld_nttunpack_avx2((__m256i *)(data));
}

static MLD_INLINE void mld_ntt_native(int32_t data[MLDSA_N])
{
  mld_ntt_avx2((__m256i *)data, mld_qdata.vec);
}
static MLD_INLINE void mld_intt_native(int32_t data[MLDSA_N])
{
  mld_invntt_avx2((__m256i *)data, mld_qdata.vec);
}

static MLD_INLINE int mld_rej_uniform_native(int32_t *r, unsigned len,
                                             const uint8_t *buf,
                                             unsigned buflen)
{
  /* AVX2 implementation assumes specific buffer lengths */
  if (len != MLDSA_N || buflen != MLD_AVX2_REJ_UNIFORM_BUFLEN)
  {
    return -1;
  }

  return (int)mld_rej_uniform_avx2(r, buf);
}

static MLD_INLINE int mld_rej_uniform_eta2_native(int32_t *r, unsigned len,
                                                  const uint8_t *buf,
                                                  unsigned buflen)
{
  int outlen;
  /* AVX2 implementation assumes specific buffer lengths */
  if (len != MLDSA_N || buflen != MLD_AVX2_REJ_UNIFORM_ETA2_BUFLEN)
  {
    return -1;
  }

  /* Constant time: Inputs and outputs to this function are secret.
   * It is safe to leak which coefficients are accepted/rejected.
   * The assembly implementation must not leak any other information about the
   * accepted coefficients. Constant-time testing cannot cover this, and we
   * hence have to manually verify the assembly.
   * We declassify prior the input data and mark the outputs as secret.
   */
  MLD_CT_TESTING_DECLASSIFY(buf, buflen);
  outlen = (int)mld_rej_uniform_eta2_avx2(r, buf);
  MLD_CT_TESTING_SECRET(r, sizeof(int32_t) * outlen);
  return outlen;
}

static MLD_INLINE int mld_rej_uniform_eta4_native(int32_t *r, unsigned len,
                                                  const uint8_t *buf,
                                                  unsigned buflen)
{
  int outlen;
  /* AVX2 implementation assumes specific buffer lengths */
  if (len != MLDSA_N || buflen != MLD_AVX2_REJ_UNIFORM_ETA4_BUFLEN)
  {
    return -1;
  }

  /* Constant time: Inputs and outputs to this function are secret.
   * It is safe to leak which coefficients are accepted/rejected.
   * The assembly implementation must not leak any other information about the
   * accepted coefficients. Constant-time testing cannot cover this, and we
   * hence have to manually verify the assembly.
   * We declassify prior the input data and mark the outputs as secret.
   */
  MLD_CT_TESTING_DECLASSIFY(buf, buflen);
  outlen = (int)mld_rej_uniform_eta4_avx2(r, buf);
  MLD_CT_TESTING_SECRET(r, sizeof(int32_t) * outlen);
  return outlen;
}

static MLD_INLINE void mld_poly_decompose_32_native(int32_t *a1, int32_t *a0,
                                                    const int32_t *a)
{
  mld_poly_decompose_32_avx2((__m256i *)a1, (__m256i *)a0, (const __m256i *)a);
}

static MLD_INLINE void mld_poly_decompose_88_native(int32_t *a1, int32_t *a0,
                                                    const int32_t *a)
{
  mld_poly_decompose_88_avx2((__m256i *)a1, (__m256i *)a0, (const __m256i *)a);
}

#endif /* !__ASSEMBLER__ */

#endif /* !MLD_NATIVE_X86_64_META_H */
