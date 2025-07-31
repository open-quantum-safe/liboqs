/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLD_FIPS202_NATIVE_X86_64_XKCP_H
#define MLD_FIPS202_NATIVE_X86_64_XKCP_H

#include "../../../common.h"

#define MLD_FIPS202_X86_64_XKCP

#if !defined(__ASSEMBLER__)
#include <stdint.h>
#include "src/KeccakP_1600_times4_SIMD256.h"

#define MLD_USE_FIPS202_X4_NATIVE
static MLD_INLINE void mld_keccak_f1600_x4_native(uint64_t *state)
{
  mld_keccakf1600x4_permute24(state);
}
#endif /* !__ASSEMBLER__ */

#endif /* !MLD_FIPS202_NATIVE_X86_64_XKCP_H */
