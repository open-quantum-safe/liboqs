/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLD_FIPS202_NATIVE_AARCH64_X4_V8A_V84A_SCALAR_H
#define MLD_FIPS202_NATIVE_AARCH64_X4_V8A_V84A_SCALAR_H

#if !defined(__ARM_FEATURE_SHA3)
#error This backend can only be used if SHA3 extensions are available.
#endif

/* Part of backend API */
#define MLD_USE_FIPS202_X4_NATIVE
/* Guard for assembly file */
#define MLD_FIPS202_AARCH64_NEED_X4_V8A_V84A_SCALAR_HYBRID

#if !defined(__ASSEMBLER__)
#include "src/fips202_native_aarch64.h"
static MLD_INLINE void mld_keccak_f1600_x4_native(uint64_t *state)
{
  mld_keccak_f1600_x4_scalar_v8a_v84a_hybrid_asm(
      state, mld_keccakf1600_round_constants);
}
#endif /* !__ASSEMBLER__ */

#endif /* !MLD_FIPS202_NATIVE_AARCH64_X4_V8A_V84A_SCALAR_H */
