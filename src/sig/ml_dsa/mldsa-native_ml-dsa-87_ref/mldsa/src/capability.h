/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_CAPABILITY_H
#define MLD_CAPABILITY_H

#include "sys.h"

#if !defined(MLD_CONFIG_CUSTOM_CAPABILITY_FUNC) && !defined(__ASSEMBLER__)
#include "cbmc.h"

MLD_MUST_CHECK_RETURN_VALUE
static MLD_INLINE int mld_sys_check_capability(mld_sys_cap cap)
__contract__(
  ensures(return_value == 0 || return_value == 1)
)
{
  /* By default, we rely on compile-time feature detection/specification:
   * If a feature is enabled at compile-time, we assume it is supported by
   * the host that the resulting library/binary will be built on.
   * If this assumption is not true, you MUST overwrite this function.
   * See the documentation of MLD_CONFIG_CUSTOM_CAPABILITY_FUNC in
   * mldsa_native_config.h for more information. */
  (void)cap;
  return 1;
}
#endif /* !MLD_CONFIG_CUSTOM_CAPABILITY_FUNC && !__ASSEMBLER__ */

#endif /* !MLD_CAPABILITY_H */
