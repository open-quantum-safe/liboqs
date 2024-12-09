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
#if defined(MLKEM_USE_NATIVE_BACKEND_ARITH)
#if defined(MLKEM_NATIVE_ARITH_BACKEND_FILE)
#include MLKEM_NATIVE_ARITH_BACKEND_FILE
#else
#error Bad configuration: MLKEM_USE_NATIVE_BACKEND_ARITH is set, but MLKEM_NATIVE_ARITH_BACKEND_FILE is not.
#endif
#endif

#if defined(MLKEM_USE_NATIVE_BACKEND_FIPS202)
#if defined(MLKEM_NATIVE_FIPS202_BACKEND_FILE)
#include MLKEM_NATIVE_FIPS202_BACKEND_FILE
#else
#error Bad configuration: MLKEM_USE_NATIVE_BACKEND_FIPS202 is set, but MLKEM_NATIVE_FIPS202_BACKEND_FILE is not.
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

#define MLKEM_NAMESPACE(s) \
  MLKEM_NATIVE_MAKE_NAMESPACE(MLKEM_NAMESPACE_PREFIX, s)

#if defined(MLKEM_NAMESPACE_PREFIX_ADD_LEVEL)
#define MLKEM_NATIVE_MAKE_NAMESPACE_K_(x1, x2, x3) x1##x2##_##x3
#define MLKEM_NATIVE_MAKE_NAMESPACE_K(x1, x2, x3) \
  MLKEM_NATIVE_MAKE_NAMESPACE_K_(x1, x2, x3)
#define MLKEM_NAMESPACE_K(s) \
  MLKEM_NATIVE_MAKE_NAMESPACE_K(MLKEM_NAMESPACE_PREFIX, MLKEM_LVL, s)
#else
#define MLKEM_NAMESPACE_K(s) MLKEM_NAMESPACE(s)
#endif

/* On Apple platforms, we need to emit leading underscore
 * in front of assembly symbols. We thus introducee a separate
 * namespace wrapper for ASM symbols. */
#if !defined(__APPLE__)
#define MLKEM_ASM_NAMESPACE(sym) MLKEM_NAMESPACE(sym)
#define MLKEM_ASM_NAMESPACE_K(sym) MLKEM_NAMESPACE_K(sym)
#else
#define PREFIX_UNDERSCORE_(sym) _##sym
#define PREFIX_UNDERSCORE(sym) PREFIX_UNDERSCORE_(sym)
#define MLKEM_ASM_NAMESPACE(sym) PREFIX_UNDERSCORE(MLKEM_NAMESPACE(sym))
#define MLKEM_ASM_NAMESPACE_K(sym) PREFIX_UNDERSCORE(MLKEM_NAMESPACE_K(sym))
#endif

/* We aim to simplify the user's life by supporting builds where
 * all source files are included, even those that are not needed.
 * Those files are appropriately guarded and will be empty when unneeded.
 * The following is to avoid compilers complaining about this. */
#define MLKEM_NATIVE_EMPTY_CU(s) extern int MLKEM_NAMESPACE_K(empty_cu_##s);

#endif /* MLKEM_NATIVE_COMMON_H */
