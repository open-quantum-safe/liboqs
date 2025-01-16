/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include "verify.h"

#if !defined(MLKEM_USE_ASM_VALUE_BARRIER)
/*
 * Masking value used in constant-time functions from
 * verify.h to block the compiler's range analysis and
 * thereby reduce the risk of compiler-introduced branches.
 */
volatile uint64_t ct_opt_blocker_u64 = 0;

#else /* MLKEM_USE_ASM_VALUE_BARRIER */

#define empty_cu_verify MLKEM_NAMESPACE(empty_cu_verify)
int empty_cu_verify;

#endif /* MLKEM_USE_ASM_VALUE_BARRIER */
