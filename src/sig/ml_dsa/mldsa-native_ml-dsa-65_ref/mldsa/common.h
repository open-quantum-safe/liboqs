/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLD_COMMON_H
#define MLD_COMMON_H

#if defined(MLD_CONFIG_FILE)
#include MLD_CONFIG_FILE
#else
#include "config.h"
#endif

#include "cbmc.h"
#include "params.h"
#include "sys.h"

/* Internal and public API have external linkage by default, but
 * this can be overwritten by the user, e.g. for single-CU builds. */
#if !defined(MLD_CONFIG_INTERNAL_API_QUALIFIER)
#define MLD_INTERNAL_API
#else
#define MLD_INTERNAL_API MLD_CONFIG_INTERNAL_API_QUALIFIER
#endif

#if !defined(MLD_CONFIG_EXTERNAL_API_QUALIFIER)
#define MLD_EXTERNAL_API
#else
#define MLD_EXTERNAL_API MLD_CONFIG_EXTERNAL_API_QUALIFIER
#endif


#if defined(MLD_CONFIG_USE_NATIVE_BACKEND_ARITH) && \
    !defined(MLD_CONFIG_ARITH_BACKEND_FILE)
#error Bad configuration: MLD_CONFIG_USE_NATIVE_BACKEND_ARITH is set, but MLD_CONFIG_ARITH_BACKEND_FILE is not.
#endif

#if defined(MLD_CONFIG_USE_NATIVE_BACKEND_ARITH)
#include MLD_CONFIG_ARITH_BACKEND_FILE
#endif

#define MLD_CONCAT_(x1, x2) x1##x2
#define MLD_CONCAT(x1, x2) MLD_CONCAT_(x1, x2)

/* On Apple platforms, we need to emit leading underscore
 * in front of assembly symbols. We thus introducee a separate
 * namespace wrapper for ASM symbols. */
#if !defined(__APPLE__)
#define MLD_ASM_NAMESPACE(sym) MLD_NAMESPACE(sym)
#else
#define MLD_ASM_NAMESPACE(sym) MLD_CONCAT(_, MLD_NAMESPACE(sym))
#endif

/*
 * On X86_64 if control-flow protections (CET) are enabled (through
 * -fcf-protection=), we add an endbr64 instruction at every global function
 * label.  See sys.h for more details
 */
#if defined(MLD_SYS_X86_64)
#define MLD_ASM_FN_SYMBOL(sym) MLD_ASM_NAMESPACE(sym) : MLD_CET_ENDBR
#else
#define MLD_ASM_FN_SYMBOL(sym) MLD_ASM_NAMESPACE(sym) :
#endif

/* We aim to simplify the user's life by supporting builds where
 * all source files are included, even those that are not needed.
 * Those files are appropriately guarded and will be empty when unneeded.
 * The following is to avoid compilers complaining about this. */
#define MLD_EMPTY_CU(s) extern int MLD_NAMESPACE(empty_cu_##s);

#if defined(MLD_CONFIG_USE_NATIVE_BACKEND_FIPS202)
#include MLD_CONFIG_FIPS202_BACKEND_FILE
#endif

#if !defined(__ASSEMBLER__)
#include <string.h>

/*************************************************
 * Name:        mld_zeroize
 *
 * Description: Force-zeroize a buffer.
 *              FIPS 204. Section 3.6.3 Destruction of intermediate values.
 *
 * Arguments:   void *ptr: pointer to buffer to be zeroed
 *              size_t len: Amount of bytes to be zeroed
 **************************************************/
static MLD_INLINE void mld_zeroize(void *ptr, size_t len)
__contract__(
  requires(memory_no_alias(ptr, len))
  assigns(memory_slice(ptr, len))
);

#if defined(MLD_CONFIG_CUSTOM_ZEROIZE)
static MLD_INLINE void mld_zeroize(void *ptr, size_t len)
{
  mld_zeroize_native(ptr, len);
}
#elif defined(MLD_SYS_WINDOWS)
#include <windows.h>
static MLD_INLINE void mld_zeroize(void *ptr, size_t len)
{
  SecureZeroMemory(ptr, len);
}
#elif defined(MLD_HAVE_INLINE_ASM)
static MLD_INLINE void mld_zeroize(void *ptr, size_t len)
{
  memset(ptr, 0, len);
  /* This follows OpenSSL and seems sufficient to prevent the compiler
   * from optimizing away the memset.
   *
   * If there was a reliable way to detect availability of memset_s(),
   * that would be preferred. */
  __asm__ __volatile__("" : : "r"(ptr) : "memory");
}
#else /* !MLD_CONFIG_CUSTOM_ZEROIZE && !MLD_SYS_WINDOWS && MLD_HAVE_INLINE_ASM \
       */
#error No plausibly-secure implementation of mld_zeroize available. Please provide your own using MLD_CONFIG_CUSTOM_ZEROIZE.
#endif /* !MLD_CONFIG_CUSTOM_ZEROIZE && !MLD_SYS_WINDOWS && \
          !MLD_HAVE_INLINE_ASM */

#endif /* !__ASSEMBLER__ */

#if !defined(MLD_CONFIG_FIPS202_CUSTOM_HEADER)
#define MLD_FIPS202_HEADER_FILE "fips202/fips202.h"
#else
#define MLD_FIPS202_HEADER_FILE MLD_CONFIG_FIPS202_CUSTOM_HEADER
#endif

#if !defined(MLD_CONFIG_FIPS202X4_CUSTOM_HEADER)
#define MLD_FIPS202X4_HEADER_FILE "fips202/fips202x4.h"
#else
#define MLD_FIPS202X4_HEADER_FILE MLD_CONFIG_FIPS202X4_CUSTOM_HEADER
#endif

/* Standard library function replacements */
#if !defined(__ASSEMBLER__)
#if !defined(MLD_CONFIG_CUSTOM_MEMCPY)
#include <string.h>
#define mld_memcpy memcpy
#endif

#if !defined(MLD_CONFIG_CUSTOM_MEMSET)
#include <string.h>
#define mld_memset memset
#endif
#endif /* !__ASSEMBLER__ */



#endif /* !MLD_COMMON_H */
