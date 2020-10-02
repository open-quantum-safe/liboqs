/**
 * \file common.h
 * \brief Utility functions for use in liboqs.
 *
 * SPDX-License-Identifier: MIT
 */


#ifndef OQS_COMMON_H
#define OQS_COMMON_H

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#include <oqs/oqsconfig.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Macro for terminating the program if x is
 * a null pointer.
 */
#define OQS_EXIT_IF_NULLPTR(x)  \
    do {                        \
        if ( (x) == (void*)0 )  \
            exit(EXIT_FAILURE); \
    } while (0)

/**
 * This macro is intended to replace those assert()s
 * involving side-effecting statements in aes/aes_ossl.c.
 *
 * assert() becomes a no-op when -DNDEBUG is defined,
 * which causes compilation failures when the statement
 * being checked also results in side-effects.
 *
 * This is a temporary workaround until a better error
 * handling strategy is developed.
 */
#define OQS_OPENSSL_GUARD(x)    \
    do {                        \
        if( 1 != (x) ) {        \
            exit(EXIT_FAILURE); \
        }                       \
    } while (0)

/**
 * Certain functions (such as OQS_randombytes_openssl in
 * src/rand/rand.c) take in a size_t parameter, but can
 * only handle values up to INT_MAX for those parameters.
 * This macro is a temporary workaround for such functions.
 */
#define SIZE_T_TO_INT_OR_EXIT(size_t_var_name, int_var_name)  \
    int int_var_name = 0;                                     \
    if (size_t_var_name <= INT_MAX) {                         \
        int_var_name = (int)size_t_var_name;                  \
    } else {                                                  \
        exit(EXIT_FAILURE);                                   \
    }

/**
 * Defines which functions should be exposed outside the LibOQS library
 *
 * By default the visibility of all the symbols is defined to "hidden"
 * Only the library API should be marked as default
 *
 * Example: OQS_API return_value function_name(void);
 */
#if defined(_WIN32)
#define OQS_API __declspec(dllexport)
#else
#define OQS_API __attribute__((visibility("default")))
#endif

/**
 * Represents return values from functions.
 *
 * Callers should compare with the symbol rather than the individual value.
 * For example,
 *
 *     ret = OQS_KEM_encaps(...);
 *     if (ret == OQS_SUCCESS) { ... }
 *
 * rather than
 *
 *     if (!OQS_KEM_encaps(...) { ... }
 *
 */
typedef enum {
	/** Used to indicate that some undefined error occurred. */
	OQS_ERROR = -1,
	/** Used to indicate successful return from function. */
	OQS_SUCCESS = 0,
	/** Used to indicate failures in external libraries (e.g., OpenSSL). */
	OQS_EXTERNAL_LIB_ERROR_OPENSSL = 50,
} OQS_STATUS;

#if (defined(OQS_USE_CPU_EXTENSIONS) && defined(OQS_PORTABLE_BUILD))

/**
 * Architecture macros.
 */
#if (defined(_M_X64) || defined(__x86_64__))
#define ARCH_X86_64
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
#define ARCH_ARM_ANY
#endif

/**
 * CPU runtime detection flags
 */
#if defined(ARCH_X86_64)
typedef struct {
	unsigned int AES_ENABLED;
	unsigned int AVX_ENABLED;
	unsigned int AVX2_ENABLED;
	unsigned int AVX512_ENABLED;
	unsigned int BMI1_ENABLED;
	unsigned int BMI2_ENABLED;
	unsigned int PCLMUL_ENABLED;
	unsigned int POPCNT_ENABLED;
	unsigned int SSE_ENABLED;
	unsigned int SSE2_ENABLED;
	unsigned int SSE3_ENABLED;
} OQS_CPU_EXTENSIONS;
#elif defined(ARCH_ARM_ANY)
typedef struct {
	unsigned int NEON_ENABLED;
} OQS_CPU_EXTENSIONS;
#endif
/* Keep in sync with name array in common.c ! */

/**
 * Returns a list of available CPU extensions.
 *
 * \return Struct of type OQS_CPU_EXTENSIONS containing flags for runtime CPU extension detection.
 */
OQS_API OQS_CPU_EXTENSIONS OQS_get_available_CPU_extensions(void);

/**
 * Returns name of available CPU extension.
 *
 * \return name of extension indexed by struct entry number
 */
OQS_API const char *OQS_get_cpu_extension_name(unsigned int i);

#endif /* OQS_USE_CPU_EXTENSIONS && OQS_PORTABLE_BUILD */

/**
 * This currently only sets the values in the OQS_CPU_EXTENSIONS,
 * and so has effect only when OQS_USE_CPU_EXTENSIONS and
 * OQS_PORTABLE_BUILD are set.
 */
OQS_API void OQS_init(void);

/**
 * Zeros out `len` bytes of memory starting at `ptr`.
 *
 * Designed to be protected against optimizing compilers which try to remove
 * "unnecessary" operations.  Should be used for all buffers containing secret
 * data.
 *
 * @param[in] ptr The start of the memory to zero out.
 * @param[in] len The number of bytes to zero out.
 */
OQS_API void OQS_MEM_cleanse(void *ptr, size_t len);

/**
 * Zeros out `len` bytes of memory starting at `ptr`, then frees `ptr`.
 *
 * Can be called with `ptr = NULL`, in which case no operation is performed.
 *
 * Designed to be protected against optimizing compilers which try to remove
 * "unnecessary" operations.  Should be used for all buffers containing secret
 * data.
 *
 * @param[in] ptr The start of the memory to zero out and free.
 * @param[in] len The number of bytes to zero out.
 */
OQS_API void OQS_MEM_secure_free(void *ptr, size_t len);

/**
 * Frees `ptr`.
 *
 * Can be called with `ptr = NULL`, in which case no operation is performed.
 *
 * Should only be used on non-secret data.
 *
 * @param[in] ptr The start of the memory to free.
 */
OQS_API void OQS_MEM_insecure_free(void *ptr);

/**
 * Macros that indicates a function argument may be unused.  Used to comply with
 * an API specification but when an implementation doesn't actually use the
 * argument and we'd get a compiler warning otherwise.
 */
#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
// __attribute__ not supported in VS
#define UNUSED
#endif

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // OQS_COMMON_H
