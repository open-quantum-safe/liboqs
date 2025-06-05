/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [FIPS140_3_IG]
 *   Implementation Guidance for FIPS 140-3 and the Cryptographic Module
 *   Validation Program National Institute of Standards and Technology
 *   https://csrc.nist.gov/projects/cryptographic-module-validation-program/fips-140-3-ig-announcements
 */

#ifndef MLK_INTEGRATION_LIBOQS_CONFIG_X86_64_H
#define MLK_INTEGRATION_LIBOQS_CONFIG_X86_64_H

/******************************************************************************
 * Name:        MLK_CONFIG_PARAMETER_SET
 *
 * Description: Specifies the parameter set for ML-KEM
 *              - MLK_CONFIG_PARAMETER_SET=512 corresponds to ML-KEM-512
 *              - MLK_CONFIG_PARAMETER_SET=768 corresponds to ML-KEM-768
 *              - MLK_CONFIG_PARAMETER_SET=1024 corresponds to ML-KEM-1024
 *
 *              This can also be set using CFLAGS.
 *
 *****************************************************************************/
#ifndef MLK_CONFIG_PARAMETER_SET
#define MLK_CONFIG_PARAMETER_SET \
  768 /* Change this for different security strengths */
#endif

/******************************************************************************
 * Name:        MLK_CONFIG_NAMESPACE_PREFIX
 *
 * Description: The prefix to use to namespace global symbols from mlkem/.
 *
 *              In a multi-level build (that is, if either
 *              - MLK_CONFIG_MULTILEVEL_WITH_SHARED, or
 *              - MLK_CONFIG_MULTILEVEL_NO_SHARED,
 *              are set, level-dependent symbols will additionally be prefixed
 *              with the parameter set (512/768/1024).
 *
 *              This can also be set using CFLAGS.
 *
 *****************************************************************************/
#if MLK_CONFIG_PARAMETER_SET == 512
#define MLK_CONFIG_NAMESPACE_PREFIX PQCP_MLKEM_NATIVE_MLKEM512_X86_64
#elif MLK_CONFIG_PARAMETER_SET == 768
#define MLK_CONFIG_NAMESPACE_PREFIX PQCP_MLKEM_NATIVE_MLKEM768_X86_64
#elif MLK_CONFIG_PARAMETER_SET == 1024
#define MLK_CONFIG_NAMESPACE_PREFIX PQCP_MLKEM_NATIVE_MLKEM1024_X86_64
#endif

/******************************************************************************
 * Name:        MLK_CONFIG_USE_NATIVE_BACKEND_ARITH
 *
 * Description: Determines whether an native arithmetic backend should be used.
 *
 *              The arithmetic backend covers performance critical functions
 *              such as the number-theoretic transform (NTT).
 *
 *              If this option is unset, the C backend will be used.
 *
 *              If this option is set, the arithmetic backend to be use is
 *              determined by MLK_CONFIG_ARITH_BACKEND_FILE: If the latter is
 *              unset, the default backend for your the target architecture
 *              will be used. If set, it must be the name of a backend metadata
 *              file.
 *
 *              This can also be set using CFLAGS.
 *
 *****************************************************************************/
#define MLK_CONFIG_USE_NATIVE_BACKEND_ARITH

/******************************************************************************
 * Name:        MLK_CONFIG_ARITH_BACKEND_FILE
 *
 * Description: The arithmetic backend to use.
 *
 *              If MLK_CONFIG_USE_NATIVE_BACKEND_ARITH is unset, this option
 *              is ignored.
 *
 *              If MLK_CONFIG_USE_NATIVE_BACKEND_ARITH is set, this option must
 *              either be undefined or the filename of an arithmetic backend.
 *              If unset, the default backend will be used.
 *
 *              This can be set using CFLAGS.
 *
 *****************************************************************************/
#define MLK_CONFIG_ARITH_BACKEND_FILE "native/meta.h"

/******************************************************************************
 * Name:        MLK_CONFIG_FIPS202_CUSTOM_HEADER
 *
 * Description: Custom header to use for FIPS-202
 *
 *              This should only be set if you intend to use a custom
 *              FIPS-202 implementation, different from the one shipped
 *              with mlkem-native.
 *
 *              If set, it must be the name of a file serving as the
 *              replacement for mlkem/fips202/fips202.h, and exposing
 *              the same API (see FIPS202.md).
 *
 *****************************************************************************/
#define MLK_CONFIG_FIPS202_CUSTOM_HEADER \
  "../../integration/liboqs/fips202_glue.h"

/******************************************************************************
 * Name:        MLK_CONFIG_FIPS202X4_CUSTOM_HEADER
 *
 * Description: Custom header to use for FIPS-202-X4
 *
 *              This should only be set if you intend to use a custom
 *              FIPS-202 implementation, different from the one shipped
 *              with mlkem-native.
 *
 *              If set, it must be the name of a file serving as the
 *              replacement for mlkem/fips202/fips202x4.h, and exposing
 *              the same API (see FIPS202.md).
 *
 *****************************************************************************/
#define MLK_CONFIG_FIPS202X4_CUSTOM_HEADER \
  "../../integration/liboqs/fips202x4_glue.h"

/******************************************************************************
 * Name:        MLK_CONFIG_CUSTOM_ZEROIZE
 *
 * Description: In compliance with FIPS 203 Section 3.3, mlkem-native zeroizes
 *              intermediate stack buffers before returning from function calls.
 *
 *              Set this option and define `mlk_zeroize` if you want to
 *              use a custom method to zeroize intermediate stack buffers.
 *              The default implementation uses SecureZeroMemory on Windows
 *              and a memset + compiler barrier otherwise. If neither of those
 *              is available on the target platform, compilation will fail,
 *              and you will need to use MLK_CONFIG_CUSTOM_ZEROIZE to provide
 *              a custom implementation of `mlk_zeroize()`.
 *
 *              WARNING:
 *              The explicit stack zeroization conducted by mlkem-native
 *              reduces the likelihood of data leaking on the stack, but
 *              does not eliminate it! The C standard makes no guarantee about
 *              where a compiler allocates structures and whether/where it makes
 *              copies of them. Also, in addition to entire structures, there
 *              may also be potentially exploitable leakage of individual values
 *              on the stack.
 *
 *              If you need bullet-proof zeroization of the stack, you need to
 *              consider additional measures instead of of what this feature
 *              provides. In this case, you can set mlk_zeroize to a no-op.
 *
 *****************************************************************************/
/* #define MLK_CONFIG_CUSTOM_ZEROIZE
   #if !defined(__ASSEMBLER__)
   #include <stdint.h>
   #include "sys.h"
   static MLK_INLINE void mlk_zeroize(void *ptr, size_t len)
   {
       ... your implementation ...
   }
   #endif
*/

/******************************************************************************
 * Name:        MLK_CONFIG_CUSTOM_RANDOMBYTES
 *
 * Description: mlkem-native does not provide a secure randombytes
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
#define MLK_CONFIG_CUSTOM_RANDOMBYTES
#if !defined(__ASSEMBLER__)
#include <oqs/rand.h>
#include <stdint.h>
#include "../../mlkem/src/sys.h"
static MLK_INLINE void mlk_randombytes(uint8_t *ptr, size_t len)
{
  OQS_randombytes(ptr, len);
}
#endif /* !__ASSEMBLER__ */


/******************************************************************************
 * Name:        MLK_CONFIG_NO_ASM
 *
 * Description: If this option is set, mlkem-native will be built without
 *              use of native code or inline assembly.
 *
 *              By default, inline assembly is used to implement value barriers.
 *              Without inline assembly, mlkem-native will use a global volatile
 *              'opt blocker' instead; see verify.h.
 *
 *              Inline assembly is also used to implement a secure zeroization
 *              function on non-Windows platforms. If this option is set and
 *              the target platform is not Windows, you MUST set
 *              MLK_CONFIG_CUSTOM_ZEROIZE and provide a custom zeroization
 *              function.
 *
 *              If this option is set, MLK_CONFIG_USE_NATIVE_BACKEND_FIPS202 and
 *              and MLK_CONFIG_USE_NATIVE_BACKEND_ARITH will be ignored, and no
 *native backends will be used.
 *
 *****************************************************************************/
/* #define MLK_CONFIG_NO_ASM */

/******************************************************************************
 * Name:        MLK_CONFIG_KEYGEN_PCT
 *
 * Description: Compliance with @[FIPS140_3_IG, p.87] requires a
 *              Pairwise Consistency Test (PCT) to be carried out on a freshly
 *              generated keypair before it can be exported.
 *
 *              Set this option if such a check should be implemented.
 *              In this case, crypto_kem_keypair_derand and crypto_kem_keypair
 *              will return a non-zero error code if the PCT failed.
 *
 *              NOTE: This feature will drastically lower the performance of
 *              key generation.
 *
 *****************************************************************************/
/* #define MLK_CONFIG_KEYGEN_PCT */

/******************************************************************************
 * Name:        MLK_CONFIG_KEYGEN_PCT_BREAKAGE_TEST
 *
 * Description: If this option is set, the user must provide a runtime
 *              function `static inline int mlk_break_pct() { ... }` to
 *              indicate whether the PCT should be made fail.
 *
 *              This option only has an effect if MLK_CONFIG_KEYGEN_PCT is set.
 *
 *****************************************************************************/
/* #define MLK_CONFIG_KEYGEN_PCT_BREAKAGE_TEST
   #if !defined(__ASSEMBLER__)
   #include "sys.h"
   static MLK_INLINE int mlk_break_pct(void)
   {
       ... return 0/1 depending on whether PCT should be broken ...
   }
   #endif
*/

/* Enable valgrind-based assertions in mlkem-native through macro
 * from libOQS. */
#if !defined(__ASSEMBLER__)
#include <oqs/common.h>
#if defined(OQS_ENABLE_TEST_CONSTANT_TIME)
#define MLK_CONFIG_CT_TESTING_ENABLED
#endif
#endif /* !__ASSEMBLER__ */

#endif /* !MLK_INTEGRATION_LIBOQS_CONFIG_X86_64_H */
