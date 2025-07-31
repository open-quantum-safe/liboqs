/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLD_NATIVE_AARCH64_META_H
#define MLD_NATIVE_AARCH64_META_H

/* Set of primitives that this backend replaces */
#define MLD_USE_NATIVE_NTT
#define MLD_USE_NATIVE_INTT
#define MLD_USE_NATIVE_REJ_UNIFORM
#define MLD_USE_NATIVE_REJ_UNIFORM_ETA2
#define MLD_USE_NATIVE_REJ_UNIFORM_ETA4

/* Identifier for this backend so that source and assembly files
 * in the build can be appropriately guarded. */
#define MLD_ARITH_BACKEND_AARCH64


#if !defined(__ASSEMBLER__)
#include "src/arith_native_aarch64.h"

static MLD_INLINE void mld_ntt_native(int32_t data[MLDSA_N])
{
  mld_ntt_asm(data, mld_aarch64_ntt_zetas_layer123456,
              mld_aarch64_ntt_zetas_layer78);
}

static MLD_INLINE void mld_intt_native(int32_t data[MLDSA_N])
{
  mld_intt_asm(data, mld_aarch64_intt_zetas_layer78,
               mld_aarch64_intt_zetas_layer123456);
}

static MLD_INLINE int mld_rej_uniform_native(int32_t *r, unsigned len,
                                             const uint8_t *buf,
                                             unsigned buflen)
{
  if (len != MLDSA_N || buflen % 24 != 0)
  {
    return -1;
  }
  return (int)mld_rej_uniform_asm(r, buf, buflen, mld_rej_uniform_table);
}

static MLD_INLINE int mld_rej_uniform_eta2_native(int32_t *r, unsigned len,
                                                  const uint8_t *buf,
                                                  unsigned buflen)
{
  /* AArch64 implementation assumes specific buffer lengths */
  if (len != MLDSA_N || buflen != MLD_AARCH64_REJ_UNIFORM_ETA2_BUFLEN)
  {
    return -1;
  }

  return (int)mld_rej_uniform_eta2_asm(r, buf, buflen,
                                       mld_rej_uniform_eta_table);
}

static MLD_INLINE int mld_rej_uniform_eta4_native(int32_t *r, unsigned len,
                                                  const uint8_t *buf,
                                                  unsigned buflen)
{
  /* AArch64 implementation assumes specific buffer lengths */
  if (len != MLDSA_N || buflen != MLD_AARCH64_REJ_UNIFORM_ETA4_BUFLEN)
  {
    return -1;
  }

  return (int)mld_rej_uniform_eta4_asm(r, buf, buflen,
                                       mld_rej_uniform_eta_table);
}

#endif /* !__ASSEMBLER__ */

#endif /* !MLD_NATIVE_AARCH64_META_H */
