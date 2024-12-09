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
/*
 * For AArch64, we currently we have one clean and one opt profile.
 * We default to the opt profile.
 *
 * In the future, this may branch further depending on the microarchitecture.
 */
#include "aarch64/opt.h"
#endif /* SYS_AARCH64 */

#ifdef SYS_X86_64_AVX2
/*
 * For now, there's only one x86_64 profile, based on
 * the AVX2 code from the Kyber repository.
 * https://github.com/pq-crystals/kyber
 */
#include "x86_64/default.h"
#endif /* SYS_X86_64 */

#endif /* MLKEM_NATIVE_ARITH_BACKEND_DEFAULT_H */
