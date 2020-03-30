/**
 * \file common.h
 * \brief Utility functions for use in liboqs.
 */

#ifndef OQS_COMMON_H
#define OQS_COMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#include <oqs/oqsconfig.h>

#if defined(__cplusplus)
extern "C" {
#endif

/** 
 * Runtime CPU feature flag indicating AES support
 */
 int OQS_RT_AES_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating AVX support
 */
 int OQS_RT_AVX_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating AVX2 support
 */
 int OQS_RT_AVX2_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating AVX512 support
 */
 int OQS_RT_AVX512_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating BMI support
 */
 int OQS_RT_BMI_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating BMI2 support
 */
 int OQS_RT_BMI2_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating FMA support
 */
 int OQS_RT_FMA_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating FMA4 support
 */
 int OQS_RT_FMA4_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating MMX support
 */
 int OQS_RT_MMX_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating POPCNT support
 */
 int OQS_RT_POPCNT_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating SSE support
 */
 int OQS_RT_SSE_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating SSE2 support
 */
 int OQS_RT_SSE2_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating SSE3 support
 */
 int OQS_RT_SSE3_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating SSE4A support
 */
 int OQS_RT_SSE4A_INSTRUCTIONS ;

/** 
 * Runtime CPU feature flag indicating NEON support
 */
 int OQS_RT_NEON_INSTRUCTIONS;


/**
 * Certain functions (such as OQS_randombytes_openssl in
 * src/rand/rand.c) take in a size_t parameter, but can
 * only handle values up to INT_MAX for those parameters.
 * This macro is a temporary workaround for such functions.
 */
#define SIZE_T_TO_INT_OR_ABORT(size_t_var_name, int_var_name) \
    int int_var_name = 0;                            \
    if(size_t_var_name <= INT_MAX) {                 \
        int_var_name = (int)size_t_var_name;         \
    } else {                                         \
        abort();                                     \
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

/**
 * Initialization function.
 * For now, it sets the CPU runtime feature flags
 */
OQS_API void OQS_init(void);

/**
 * Zeros out `len` bytes of memory starting at `ptr`.
 *
 * Designed to be protected against optimizing compilers which try to remove "unnecessary"
 * operations.  Should be used for all buffers containing secret data.
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
 * Designed to be protected against optimizing compilers which try to remove "unnecessary"
 * operations.  Should be used for all buffers containing secret data.
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
 * an API specification but when an implementation doesn't actually use the argument
 * and we'd get a compiler warning otherwise.
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
