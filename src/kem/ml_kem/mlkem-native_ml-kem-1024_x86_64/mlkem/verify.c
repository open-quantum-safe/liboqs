/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include "verify.h"

#if !defined(MLK_USE_ASM_VALUE_BARRIER) && \
    !defined(MLK_MULTILEVEL_BUILD_NO_SHARED)
/*
 * Masking value used in constant-time functions from
 * verify.h to block the compiler's range analysis and
 * thereby reduce the risk of compiler-introduced branches.
 */
volatile uint64_t mlk_ct_opt_blocker_u64 = 0;

#else /* MLK_USE_ASM_VALUE_BARRIER && \
         !MLK_MULTILEVEL_BUILD_NO_SHARED */

MLK_EMPTY_CU(verify)

#endif /* MLK_USE_ASM_VALUE_BARRIER && \
          !MLK_MULTILEVEL_BUILD_NO_SHARED */
