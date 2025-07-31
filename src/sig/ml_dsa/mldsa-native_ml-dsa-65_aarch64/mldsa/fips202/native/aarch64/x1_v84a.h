/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLD_FIPS202_NATIVE_AARCH64_X1_V84A_H
#define MLD_FIPS202_NATIVE_AARCH64_X1_V84A_H

#if !defined(__ARM_FEATURE_SHA3)
#error This backend can only be used if SHA3 extensions are available.
#endif

/* Part of backend API */
#define MLD_USE_FIPS202_X1_NATIVE
/* Guard for assembly file */
#define MLD_FIPS202_AARCH64_NEED_X1_V84A

#if !defined(__ASSEMBLER__)
#include "src/fips202_native_aarch64.h"
static MLD_INLINE void mld_keccak_f1600_x1_native(uint64_t *state)
{
  mld_keccak_f1600_x1_v84a_asm(state, mld_keccakf1600_round_constants);
}
#endif /* !__ASSEMBLER__ */

#endif /* !MLD_FIPS202_NATIVE_AARCH64_X1_V84A_H */
