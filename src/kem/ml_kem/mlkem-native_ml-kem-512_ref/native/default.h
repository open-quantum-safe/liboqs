/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLKEM_NATIVE_ARITH_BACKEND_DEFAULT_H
#define MLKEM_NATIVE_ARITH_BACKEND_DEFAULT_H

/*
 * Default arithmetic backend
 */
#include "../sys.h"

#ifdef SYS_AARCH64
#include "aarch64/opt.h"
#endif /* SYS_AARCH64 */

#ifdef SYS_X86_64_AVX2
#include "x86_64/default.h"
#endif /* SYS_X86_64 */

#endif /* MLKEM_NATIVE_ARITH_BACKEND_DEFAULT_H */
