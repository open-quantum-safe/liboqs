/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLKEM_NATIVE_COMMON_H
#define MLKEM_NATIVE_COMMON_H

#if defined(MLKEM_NATIVE_CONFIG_FILE)
#include MLKEM_NATIVE_CONFIG_FILE
#else
#include "config.h"
#endif /* MLKEM_NATIVE_CONFIG_FILE */

#include "params.h"
#include "sys.h"

/* Include backend metadata */
#if defined(MLKEM_USE_NATIVE)
#if defined(MLKEM_NATIVE_ARITH_BACKEND)
#include MLKEM_NATIVE_ARITH_BACKEND
#endif
#if defined(MLKEM_NATIVE_FIPS202_BACKEND)
#include MLKEM_NATIVE_FIPS202_BACKEND
#endif
#endif

#if !defined(MLKEM_NATIVE_ARITH_BACKEND_NAME)
#define MLKEM_NATIVE_ARITH_BACKEND_NAME C
#endif

#if !defined(MLKEM_NATIVE_FIPS202_BACKEND_NAME)
#define MLKEM_NATIVE_FIPS202_BACKEND_NAME C
#endif

/* For a monobuild (where all compilation units are merged into one), mark
 * all non-public API as static since they don't need external linkage. */
#if !defined(MLKEM_NATIVE_MONOBUILD)
#define MLKEM_NATIVE_INTERNAL_API
#else
#define MLKEM_NATIVE_INTERNAL_API static
#endif

#define MLKEM_NATIVE_MAKE_NAMESPACE_(x1, x2) x1##_##x2
#define MLKEM_NATIVE_MAKE_NAMESPACE(x1, x2) MLKEM_NATIVE_MAKE_NAMESPACE_(x1, x2)

#define FIPS202_NAMESPACE(s) \
  MLKEM_NATIVE_MAKE_NAMESPACE(FIPS202_NAMESPACE_PREFIX, s)

#define MLKEM_NAMESPACE(s) \
  MLKEM_NATIVE_MAKE_NAMESPACE(MLKEM_NAMESPACE_PREFIX, s)

/* On Apple platforms, we need to emit leading underscore
 * in front of assembly symbols. We thus introducee a separate
 * namespace wrapper for ASM symbols. */
#if !defined(__APPLE__)
#define MLKEM_ASM_NAMESPACE(sym) MLKEM_NAMESPACE(sym)
#define FIPS202_ASM_NAMESPACE(sym) FIPS202_NAMESPACE(sym)
#else
#define PREFIX_UNDERSCORE_(sym) _##sym
#define PREFIX_UNDERSCORE(sym) PREFIX_UNDERSCORE_(sym)
#define MLKEM_ASM_NAMESPACE(sym) PREFIX_UNDERSCORE(MLKEM_NAMESPACE(sym))
#define FIPS202_ASM_NAMESPACE(sym) PREFIX_UNDERSCORE(FIPS202_NAMESPACE(sym))
#endif

#endif /* MLKEM_NATIVE_COMMON_H */
