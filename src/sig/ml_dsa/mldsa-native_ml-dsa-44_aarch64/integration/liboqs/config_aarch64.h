/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_INTEGRATION_LIBOQS_CONFIG_AARCH64_H
#define MLD_INTEGRATION_LIBOQS_CONFIG_AARCH64_H

/* Enable valgrind-based assertions in mldsa-native through macro
 * from libOQS. */
#if !defined(__ASSEMBLER__)
#include <oqs/common.h>
#if defined(OQS_ENABLE_TEST_CONSTANT_TIME)
#define MLD_CONFIG_CT_TESTING_ENABLED
#endif
#endif /* !__ASSEMBLER__ */


#define MLD_RANDOMIZED_SIGNING

/* Use OQS's FIPS202 via glue headers */
#define MLD_CONFIG_FIPS202_CUSTOM_HEADER "../integration/liboqs/fips202_glue.h"
#define MLD_CONFIG_FIPS202X4_CUSTOM_HEADER \
  "../integration/liboqs/fips202x4_glue.h"

#ifndef MLDSA_MODE
#define MLDSA_MODE 2
#endif

#if MLDSA_MODE == 2
#define MLD_NAMESPACETOP PQCP_MLDSA_NATIVE_MLDSA44_AARCH64_
#define MLD_NAMESPACE(s) PQCP_MLDSA_NATIVE_MLDSA44_AARCH64_##s
#elif MLDSA_MODE == 3
#define MLD_NAMESPACETOP PQCP_MLDSA_NATIVE_MLDSA65_AARCH64_
#define MLD_NAMESPACE(s) PQCP_MLDSA_NATIVE_MLDSA65_AARCH64_##s
#elif MLDSA_MODE == 5
#define MLD_NAMESPACETOP PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_
#define MLD_NAMESPACE(s) PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_##s
#endif

/******************************************************************************
 * Name:        MLD_CONFIG_FILE
 *
 * Description: If defined, this is a header that will be included instead
 *              of this default configuration file mldsa/config.h.
 *
 *              When you need to build mldsa-native in multiple configurations,
 *              using varying MLD_CONFIG_FILE can be more convenient
 *              then configuring everything through CFLAGS.
 *
 *              To use, MLD_CONFIG_FILE _must_ be defined prior
 *              to the inclusion of any mldsa-native headers. For example,
 *              it can be set by passing `-DMLD_CONFIG_FILE="..."`
 *              on the command line.
 *
 *****************************************************************************/
/* #define MLD_CONFIG_FILE "config.h" */

/******************************************************************************
 * Name:        MLD_CONFIG_ARITH_BACKEND_FILE
 *
 * Description: The arithmetic backend to use.
 *
 *              If MLD_CONFIG_USE_NATIVE_BACKEND_ARITH is unset, this option
 *              is ignored.
 *
 *              If MLD_CONFIG_USE_NATIVE_BACKEND_ARITH is set, this option must
 *              either be undefined or the filename of an arithmetic backend.
 *              If unset, the default backend will be used.
 *
 *              This can be set using CFLAGS.
 *
 *****************************************************************************/
#define MLD_CONFIG_USE_NATIVE_BACKEND_ARITH
#define MLD_CONFIG_ARITH_BACKEND_FILE "native/meta.h"

/******************************************************************************
 * Name:        MLD_CONFIG_FIPS202_BACKEND_FILE
 *
 * Description: The FIPS-202 backend to use.
 *
 *              If MLD_CONFIG_USE_NATIVE_BACKEND_FIPS202 is set, this option
 *              must either be undefined or the filename of a FIPS202 backend.
 *              If unset, the default backend will be used.
 *
 *              This can be set using CFLAGS.
 *
 *****************************************************************************/
/*
 * #define MLD_CONFIG_USE_NATIVE_BACKEND_FIPS202
 * #define MLD_CONFIG_FIPS202_BACKEND_FILE "fips202/native/auto.h"
 */

/******************************************************************************
 * Name:        MLD_CONFIG_CUSTOM_ZEROIZE
 *
 * Description: In compliance with FIPS 204 Section 3.6.3, mldsa-native zeroizes
 *              intermediate stack buffers before returning from function calls.
 *
 *              Set this option and define `mld_zeroize_native` if you want to
 *              use a custom method to zeroize intermediate stack buffers.
 *              The default implementation uses SecureZeroMemory on Windows
 *              and a memset + compiler barrier otherwise. If neither of those
 *              is available on the target platform, compilation will fail,
 *              and you will need to use MLD_CONFIG_CUSTOM_ZEROIZE to provide
 *              a custom implementation of `mld_zeroize_native()`.
 *
 *              WARNING:
 *              The explicit stack zeroization conducted by mldsa-native
 *              reduces the likelihood of data leaking on the stack, but
 *              does not eliminate it! The C standard makes no guarantee about
 *              where a compiler allocates structures and whether/where it makes
 *              copies of them. Also, in addition to entire structures, there
 *              may also be potentially exploitable leakage of individual values
 *              on the stack.
 *
 *              If you need bullet-proof zeroization of the stack, you need to
 *              consider additional measures instead of what this feature
 *              provides. In this case, you can set mld_zeroize_native to a
 *              no-op.
 *
 *****************************************************************************/
/* #define MLD_CONFIG_CUSTOM_ZEROIZE
   #if !defined(__ASSEMBLER__)
   #include <stdint.h>
   #include "sys.h"
   static MLD_INLINE void mld_zeroize_native(void *ptr, size_t len)
   {
       ... your implementation ...
   }
   #endif
*/

/******************************************************************************
 * Name:        MLD_CONFIG_CUSTOM_RANDOMBYTES
 *
 * Description: mldsa-native does not provide a secure randombytes
 *              implementation. Such an implementation has to provided by the
 *              consumer.
 *
 *              If this option is not set, mlkem-native expects a function
 *              void randombytes(uint8_t *out, size_t outlen).
 *
 *              Set this option and define `mlk_randombytes` if you want to
 *              use a custom method to sample randombytes with a different name
 *              or signature.
 *
 *****************************************************************************/
#define MLD_CONFIG_CUSTOM_RANDOMBYTES
#if !defined(__ASSEMBLER__)
#include <oqs/rand.h>
#include <stdint.h>
#include "../../mldsa/sys.h"
static MLD_INLINE void mld_randombytes(uint8_t *ptr, size_t len)
{
  OQS_randombytes(ptr, len);
}
#endif

/******************************************************************************
 * Name:        MLD_CONFIG_KEYGEN_PCT
 *
 * Description: Compliance with @[FIPS140_3_IG, p.87] requires a
 *              Pairwise Consistency Test (PCT) to be carried out on a freshly
 *              generated keypair before it can be exported.
 *
 *              Set this option if such a check should be implemented.
 *              In this case, crypto_sign_keypair_internal and
 *              crypto_sign_keypair will return a non-zero error code if the
 *              PCT failed.
 *
 *              NOTE: This feature will drastically lower the performance of
 *              key generation.
 *
 *****************************************************************************/
/* #define MLD_CONFIG_KEYGEN_PCT */

/******************************************************************************
 * Name:        MLD_CONFIG_KEYGEN_PCT_BREAKAGE_TEST
 *
 * Description: If this option is set, the user must provide a runtime
 *              function `static inline int mld_break_pct() { ... }` to
 *              indicate whether the PCT should be made fail.
 *
 *              This option only has an effect if MLD_CONFIG_KEYGEN_PCT is set.
 *
 *****************************************************************************/
/* #define MLD_CONFIG_KEYGEN_PCT_BREAKAGE_TEST
   #if !defined(__ASSEMBLER__)
   #include "sys.h"
   static MLD_INLINE int mld_break_pct(void)
   {
       ... return 0/1 depending on whether PCT should be broken ...
   }
   #endif
*/

/******************************************************************************
 * Name:        MLD_CONFIG_NO_ASM
 *
 * Description: If this option is set, mldsa-native will be built without
 *              use of native code or inline assembly.
 *
 *              By default, inline assembly is used to implement value barriers.
 *              Without inline assembly, mldsa-native will use a global volatile
 *              'opt blocker' instead; see ct.h.
 *
 *              Inline assembly is also used to implement a secure zeroization
 *              function on non-Windows platforms. If this option is set and
 *              the target platform is not Windows, you MUST set
 *              MLD_CONFIG_CUSTOM_ZEROIZE and provide a custom zeroization
 *              function.
 *
 *              If this option is set, MLD_CONFIG_USE_NATIVE_BACKEND_FIPS202 and
 *              and MLD_CONFIG_USE_NATIVE_BACKEND_ARITH will be ignored, and no
 *              native backends will be used.
 *
 *****************************************************************************/
/* #define MLD_CONFIG_NO_ASM */

/******************************************************************************
 * Name:        MLD_CONFIG_NO_ASM_VALUE_BARRIER
 *
 * Description: If this option is set, mldsa-native will be built without
 *              use of native code or inline assembly for value barriers.
 *
 *              By default, inline assembly (if available) is used to implement
 *              value barriers.
 *              Without inline assembly, mldsa-native will use a global volatile
 *              'opt blocker' instead; see ct.h.
 *
 *****************************************************************************/
/* #define MLD_CONFIG_NO_ASM_VALUE_BARRIER */

/******************************************************************************
 * Name:        MLD_CONFIG_EXTERNAL_API_QUALIFIER
 *
 * Description: If set, this option provides an additional function
 *              qualifier to be added to declarations of mldsa-native's
 *              public API.
 *
 *              The primary use case for this option are single-CU builds
 *              where the public API exposed by mldsa-native is wrapped by
 *              another API in the consuming application. In this case,
 *              even mldsa-native's public API can be marked `static`.
 *
 *****************************************************************************/
#if !defined(__ASSEMBLER__)
#include <oqs/common.h>
#define MLD_CONFIG_EXTERNAL_API_QUALIFIER OQS_API
#endif /* !__ASSEMBLER__ */

#endif /* !MLD_INTEGRATION_LIBOQS_CONFIG_AARCH64_H */
