/**
 * \file common.h
 * \brief Utility functions for use in liboqs.
 */

#ifndef __OQS_COMMON_H
#define __OQS_COMMON_H

#include <stdint.h>
#include <stdlib.h>

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
	/** Used to indicate that some error occurred. */
	OQS_ERROR = -1,
	/** Used to indicate successful return from function. */
	OQS_SUCCESS = 0
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

#endif // __OQS_COMMON_H
