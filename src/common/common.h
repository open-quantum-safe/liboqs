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
#include <stdio.h>

#include <oqs/oqsconfig.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Macro for terminating the program if x is
 * a null pointer.
 */
#define OQS_EXIT_IF_NULLPTR(x, loc)                                                   \
    do {                                                                              \
        if ( (x) == (void*)0 ) {                                                      \
            fprintf(stderr, "Unexpected NULL returned from %s API. Exiting.\n", loc); \
            exit(EXIT_FAILURE);                                                       \
        }                                                                             \
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
#ifdef OQS_USE_OPENSSL
#ifdef OPENSSL_NO_STDIO
#define OQS_OPENSSL_GUARD(x)                                                           \
    do {                                                                               \
        if( 1 != (x) ) {                                                               \
            fprintf(stderr, "Error return value from OpenSSL API: %d. Exiting.\n", x); \
            exit(EXIT_FAILURE);                                                        \
        }                                                                              \
    } while (0)
#else // OPENSSL_NO_STDIO
#define OQS_OPENSSL_GUARD(x)                                                           \
    do {                                                                               \
        if( 1 != (x) ) {                                                               \
            fprintf(stderr, "Error return value from OpenSSL API: %d. Exiting.\n", x); \
            OSSL_FUNC(ERR_print_errors_fp)(stderr);                                    \
            exit(EXIT_FAILURE);                                                        \
        }                                                                              \
    } while (0)
#endif // OPENSSL_NO_STDIO
#endif // OQS_USE_OPENSSL

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

#if defined(OQS_SYS_UEFI)
#undef OQS_API
#define OQS_API
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
 * CPU runtime detection flags
 */
typedef enum {
	OQS_CPU_EXT_INIT, /* Must be first */
	/* Start extension list */
	OQS_CPU_EXT_ADX,
	OQS_CPU_EXT_AES,
	OQS_CPU_EXT_AVX,
	OQS_CPU_EXT_AVX2,
	OQS_CPU_EXT_AVX512,
	OQS_CPU_EXT_BMI1,
	OQS_CPU_EXT_BMI2,
	OQS_CPU_EXT_PCLMULQDQ,
	OQS_CPU_EXT_VPCLMULQDQ,
	OQS_CPU_EXT_POPCNT,
	OQS_CPU_EXT_SSE,
	OQS_CPU_EXT_SSE2,
	OQS_CPU_EXT_SSE3,
	OQS_CPU_EXT_ARM_AES,
	OQS_CPU_EXT_ARM_SHA2,
	OQS_CPU_EXT_ARM_SHA3,
	OQS_CPU_EXT_ARM_NEON,
	/* End extension list */
	OQS_CPU_EXT_COUNT, /* Must be last */
} OQS_CPU_EXT;

/**
 * Checks if the CPU supports a given extension
 *
 * \return 1 if the given CPU extension is available, 0 otherwise.
 */
OQS_API int OQS_CPU_has_extension(OQS_CPU_EXT ext);

/**
 * This currently sets the values in the OQS_CPU_EXTENSIONS
 * and prefetches the OpenSSL objects if necessary.
 */
OQS_API void OQS_init(void);

/**
 * This function frees prefetched OpenSSL objects
 */
OQS_API void OQS_destroy(void);

/**
 * Return library version string.
 */
OQS_API const char *OQS_version(void);

/**
 * Constant time comparison of byte sequences `a` and `b` of length `len`.
 * Returns 0 if the byte sequences are equal or if `len`=0.
 * Returns 1 otherwise.
 *
 * @param[in] a A byte sequence of length at least `len`.
 * @param[in] b A byte sequence of length at least `len`.
 * @param[in] len The number of bytes to compare.
 */
OQS_API int OQS_MEM_secure_bcmp(const void *a, const void *b, size_t len);

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
 * Allocates memory of a specified size and checks for successful allocation.
 *
 * This function attempts to allocate a block of memory of the specified size.
 * If the allocation is successful, it returns a pointer to the beginning of the
 * memory block. If the allocation fails, it prints an error message to stderr
 * and terminates the program.
 *
 * @param[in] len The size of the memory block to allocate, in bytes.
 *
 * @return A pointer to the allocated memory block if the allocation is successful.
 *
 * @note This function is intended to be used when the allocation must succeed,
 *       and failure to allocate memory is considered a fatal error. As such,
 *       it does not return if the allocation fails, but instead terminates the
 *       program with an exit status indicating failure.
 *
 * @note The memory block returned by this function is not initialized. The caller
 *       is responsible for initializing the memory if required.
 *
 * @note The allocated memory should be freed using the standard `free` function
 *       when it is no longer needed.
 */
void *OQS_MEM_checked_malloc(size_t len);

/**
 * Allocates memory of a specified size and alignment and checks for successful allocation.
 *
 * This function attempts to allocate a block of memory with the specified size
 * and alignment. If the allocation is successful, it returns a pointer to the
 * memory block. If the allocation fails, it prints an error message to stderr
 * and terminates the program.
 *
 * Alignment must be a power of two and a multiple of sizeof(void *).
 *
 * @param[in] alignment The alignment of the memory block to allocate.
 * @param[in] size The size of the memory block to allocate, in bytes.
 *
 * @return A pointer to the allocated memory block if the allocation is successful.
 *
 * @note This function is intended to be used when the allocation must succeed,
 *       and failure to allocate memory is considered a fatal error. As such,
 *       it does not return if the allocation fails, but instead terminates the
 *       program with an exit status indicating failure.
 *
 * @note The memory block returned by this function is not initialized. The caller
 *       is responsible for initializing the memory if required.
 *
 * @note The allocated memory should be freed with `OQS_MEM_aligned_free` when it
 *       is no longer needed.
 */
void *OQS_MEM_checked_aligned_alloc(size_t alignment, size_t size);

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
 * Internal implementation of C11 aligned_alloc to work around compiler quirks.
 *
 * Allocates size bytes of uninitialized memory with a base pointer that is
 * a multiple of alignment. Alignment must be a power of two and a multiple
 * of sizeof(void *). Size must be a multiple of alignment.
 * @note The allocated memory should be freed with `OQS_MEM_aligned_free` when it
 *       is no longer needed.
 */
void *OQS_MEM_aligned_alloc(size_t alignment, size_t size);

/**
 * Free memory allocated with OQS_MEM_aligned_alloc.
 */
void OQS_MEM_aligned_free(void *ptr);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // OQS_COMMON_H
