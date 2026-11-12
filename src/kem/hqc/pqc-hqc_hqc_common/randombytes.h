/**
 * @file randombytes.h
 * @brief Generic RNG interface for the HQC scheme.
 */

#ifndef HQC_RANDOMBYTES_H
#define HQC_RANDOMBYTES_H

#include <stddef.h>
#include <stdint.h>

#include "namespace.h"

#define randombytes HQC_NS(randombytes)

/**
 * @brief Fill a buffer with random bytes.
 *
 * @param[out] out    Output buffer to fill.
 * @param[in]  outlen Number of bytes to generate.
 *
 * @return 0 on success, nonzero on failure.
 *
 * @note This function must be backed by a secure RBG and return fresh bytes
 *       on every call.
 */
int randombytes(uint8_t *out, size_t outlen);

#endif  // HQC_RANDOMBYTES_H
