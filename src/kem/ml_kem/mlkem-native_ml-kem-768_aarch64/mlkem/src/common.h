/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLK_COMMON_H
#define MLK_COMMON_H

#if defined(MLK_CONFIG_FILE)
#include MLK_CONFIG_FILE
#else
#include "config.h"
#endif

#include "params.h"
#include "sys.h"

/* Internal and public API have external linkage by default, but
 * this can be overwritten by the user, e.g. for single-CU builds. */
#if !defined(MLK_CONFIG_INTERNAL_API_QUALIFIER)
#define MLK_INTERNAL_API
#else
#define MLK_INTERNAL_API MLK_CONFIG_INTERNAL_API_QUALIFIER
#endif

#if !defined(MLK_CONFIG_EXTERNAL_API_QUALIFIER)
#define MLK_EXTERNAL_API
#else
#define MLK_EXTERNAL_API MLK_CONFIG_EXTERNAL_API_QUALIFIER
#endif

#if defined(MLK_CONFIG_MULTILEVEL_NO_SHARED) || \
    defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED)
#define MLK_MULTILEVEL_BUILD
#endif

#define MLK_CONCAT_(x1, x2) x1##x2
#define MLK_CONCAT(x1, x2) MLK_CONCAT_(x1, x2)

#if defined(MLK_MULTILEVEL_BUILD)
#define MLK_ADD_PARAM_SET(s) MLK_CONCAT(s, MLK_CONFIG_PARAMETER_SET)
#else
#define MLK_ADD_PARAM_SET(s) s
#endif

#define MLK_NAMESPACE_PREFIX MLK_CONCAT(MLK_CONFIG_NAMESPACE_PREFIX, _)
#define MLK_NAMESPACE_PREFIX_K \
  MLK_CONCAT(MLK_ADD_PARAM_SET(MLK_CONFIG_NAMESPACE_PREFIX), _)

/* Functions are prefixed by MLK_CONFIG_NAMESPACE_PREFIX.
 *
 * If multiple parameter sets are used, functions depending on the parameter
 * set are additionally prefixed with 512/768/1024. See config.h.
 *
 * Example: If MLK_CONFIG_NAMESPACE_PREFIX is mlkem, then
 * MLK_NAMESPACE_K(enc) becomes mlkem512_enc/mlkem768_enc/mlkem1024_enc.
 */
#define MLK_NAMESPACE(s) MLK_CONCAT(MLK_NAMESPACE_PREFIX, s)
#define MLK_NAMESPACE_K(s) MLK_CONCAT(MLK_NAMESPACE_PREFIX_K, s)

/* On Apple platforms, we need to emit leading underscore
 * in front of assembly symbols. We thus introducee a separate
 * namespace wrapper for ASM symbols. */
#if !defined(__APPLE__)
#define MLK_ASM_NAMESPACE(sym) MLK_NAMESPACE(sym)
#else
#define MLK_ASM_NAMESPACE(sym) MLK_CONCAT(_, MLK_NAMESPACE(sym))
#endif

/*
 * On X86_64 if control-flow protections (CET) are enabled (through
 * -fcf-protection=), we add an endbr64 instruction at every global function
 * label.  See sys.h for more details
 */
#if defined(MLK_SYS_X86_64)
#define MLK_ASM_FN_SYMBOL(sym) MLK_ASM_NAMESPACE(sym) : MLK_CET_ENDBR
#else
#define MLK_ASM_FN_SYMBOL(sym) MLK_ASM_NAMESPACE(sym) :
#endif

/* We aim to simplify the user's life by supporting builds where
 * all source files are included, even those that are not needed.
 * Those files are appropriately guarded and will be empty when unneeded.
 * The following is to avoid compilers complaining about this. */
#define MLK_EMPTY_CU(s) extern int MLK_NAMESPACE_K(empty_cu_##s);

/* MLK_CONFIG_NO_ASM takes precedence over MLK_USE_NATIVE_XXX */
#if defined(MLK_CONFIG_NO_ASM)
#undef MLK_CONFIG_USE_NATIVE_BACKEND_ARITH
#undef MLK_CONFIG_USE_NATIVE_BACKEND_FIPS202
#endif

#if defined(MLK_CONFIG_USE_NATIVE_BACKEND_ARITH) && \
    !defined(MLK_CONFIG_ARITH_BACKEND_FILE)
#error Bad configuration: MLK_CONFIG_USE_NATIVE_BACKEND_ARITH is set, but MLK_CONFIG_ARITH_BACKEND_FILE is not.
#endif

#if defined(MLK_CONFIG_USE_NATIVE_BACKEND_FIPS202) && \
    !defined(MLK_CONFIG_FIPS202_BACKEND_FILE)
#error Bad configuration: MLK_CONFIG_USE_NATIVE_BACKEND_FIPS202 is set, but MLK_CONFIG_FIPS202_BACKEND_FILE is not.
#endif

#if defined(MLK_CONFIG_USE_NATIVE_BACKEND_ARITH)
#include MLK_CONFIG_ARITH_BACKEND_FILE
/* Include to enforce consistency of API and implementation,
 * and conduct sanity checks on the backend.
 *
 * Keep this _after_ the inclusion of the backend; otherwise,
 * the sanity checks won't have an effect. */
#if defined(MLK_CHECK_APIS) && !defined(__ASSEMBLER__)
#include "native/api.h"
#endif
#endif /* MLK_CONFIG_USE_NATIVE_BACKEND_ARITH */

#if defined(MLK_CONFIG_USE_NATIVE_BACKEND_FIPS202)
#include MLK_CONFIG_FIPS202_BACKEND_FILE
/* Include to enforce consistency of API and implementation,
 * and conduct sanity checks on the backend.
 *
 * Keep this _after_ the inclusion of the backend; otherwise,
 * the sanity checks won't have an effect. */
#if defined(MLK_CHECK_APIS) && !defined(__ASSEMBLER__)
#include "fips202/native/api.h"
#endif
#endif /* MLK_CONFIG_USE_NATIVE_BACKEND_FIPS202 */

#if !defined(MLK_CONFIG_FIPS202_CUSTOM_HEADER)
#define MLK_FIPS202_HEADER_FILE "fips202/fips202.h"
#else
#define MLK_FIPS202_HEADER_FILE MLK_CONFIG_FIPS202_CUSTOM_HEADER
#endif

#if !defined(MLK_CONFIG_FIPS202X4_CUSTOM_HEADER)
#define MLK_FIPS202X4_HEADER_FILE "fips202/fips202x4.h"
#else
#define MLK_FIPS202X4_HEADER_FILE MLK_CONFIG_FIPS202X4_CUSTOM_HEADER
#endif

/* Just in case we want to include mlkem_native.h, set the configuration
 * for that header in accordance with the configuration used here. */

/* Double-check that this is not conflicting with pre-existing definitions. */
#if defined(MLK_CONFIG_API_PARAMETER_SET) ||    \
    defined(MLK_CONFIG_API_NAMESPACE_PREFIX) || \
    defined(MLK_CONFIG_API_NO_SUPERCOP) ||      \
    defined(MLK_CONFIG_API_CONSTANTS_ONLY)
#error Pre-existing MLK_CONFIG_API_XXX configuration is neither useful nor allowed during an mlkem-native build
#endif /* MLK_CONFIG_API_PARAMETER_SET || MLK_CONFIG_API_NAMESPACE_PREFIX || \
          MLK_CONFIG_API_NO_SUPERCOP || MLK_CONFIG_API_CONSTANTS_ONLY */

#define MLK_CONFIG_API_PARAMETER_SET MLK_CONFIG_PARAMETER_SET
#define MLK_CONFIG_API_NAMESPACE_PREFIX \
  MLK_ADD_PARAM_SET(MLK_CONFIG_NAMESPACE_PREFIX)

#endif /* !MLK_COMMON_H */
