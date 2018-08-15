/**
 * \file common.h
 * \brief Utility functions for use in liboqs.
 */

#ifndef __OQS_COMMON_H
#define __OQS_COMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <oqs/config.h>

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
	/** Used to indicate errors occurred during BIKE protocol. */
	OQS_ERR_KEM_BIKE_DECODING_FAILURE = 100,
	OQS_ERR_KEM_BIKE_AES_SET_KEY,
	OQS_ERR_KEM_BIKE_WEIGHT_IS_NOT_T,
	OQS_ERR_KEM_BIKE_AES_CTR_PRF_INIT,
	OQS_ERR_KEM_BIKE_AES_OVER_USED
} OQS_STATUS;

/**
 * Prints a byte array to stdout as a hexadecimal.
 *
 * @param[in] label A label to display alongside the value
 * @param[in] str The byte array to print
 * @param[in] len The number of bytes to print; the memory allocated for `str` must be at least `len` bytes
 */
void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len);

/**
 * Prints part of a byte array to stdout as a hexadecimal string, specifically the first `sub_len`
 * and last `sub_len` bytes.
 * @param[in] label A label to display alongside the value
 * @param[in] str The byte array to print
 * @param[in] len The length of the byte array `str`; the memory allocated for `str` must be at least `len` bytes
 * @param[in] sub_len The number of bytes to print
 */
void OQS_print_part_hex_string(const char *label, const uint8_t *str, size_t len, size_t sub_len);

/**
 * Zeros out `len` bytes of memory starting at `ptr`.
 *
 * Designed to be protected against optimizing compilers which try to remove "unnecessary"
 * operations.  Should be used for all buffers containing secret data.
 *
 * @param[in] ptr The start of the memory to zero out.
 * @param[in] len The number of bytes to zero out.
 */
void OQS_MEM_cleanse(void *ptr, size_t len);

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
void OQS_MEM_secure_free(void *ptr, size_t len);

/**
 * Frees `ptr`.
 *
 * Can be called with `ptr = NULL`, in which case no operation is performed.
 *
 * Should only be used on non-secret data.
 *
 * @param[in] ptr The start of the memory to free.
 */
void OQS_MEM_insecure_free(void *ptr);

/**
 * Function to print a message to standard error.  Workaround on Android to print
 * directly to stdout.
 */
#if __ANDROID__
#define eprintf(...) printf(__VA_ARGS__);
#else
#define eprintf(...) fprintf(stderr, __VA_ARGS__);
#endif

/**
 * Macros that indicates a function argument may be unused.  Used to comply with
 * an API specification but when an implementation doesn't actually use the argument
 * and we'd get a compiler warning otherwise.
 */
#if defined(_WIN32)
#define UNUSED
// __attribute__ not supported in VS
#else
#define UNUSED __attribute__((unused))
#endif

/**
 * Defines which functions should be exposed outside the LibOQS library
 *
 * By default the visibility of all the symbols is defined to "hidden"
 * Only the library API should be marked as default
 *
 * Example: OQS_API return_value function_name(void);
 */
#define OQS_API __attribute__((visibility("default")))

/** Internal macro used for function renaming. */
#define PASTER(x, y) x##_##y

/** Internal macro used for function renaming. */
#define EVALUATOR(x, y) PASTER(x, y)

/**
 * Used for renaming functions.
 *
 * Some implementations are structured so that binaries for multiple parameter
 * sets are generated from the same source code but with different macros set.
 * This causes problems when trying to assemble a single binary.
 *
 * To apply this technique to an implementation, do the following in that
 * implementation's directory:
 * 1. Define the appropriate prefix as a compiler flag -DFUNC_PREFIX=whatever
 *    in Makefile.am
 * 2. Create a functions_renaming.h file that uses RENAME_FUNC_NAME to combine
 *    the specified prefix with a basename.
 * 3. include "functions_renaming.h" in all files in that subdirectory that
 *    need to have their functions renamed during compilation.
 *
 * See src/kem/bike for an example usage.
 */
#define RENAME_FUNC_NAME(fname) EVALUATOR(FUNC_PREFIX, fname)

#endif // __OQS_COMMON_H
