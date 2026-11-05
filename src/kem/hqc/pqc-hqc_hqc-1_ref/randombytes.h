/**
 * @file randombytes.h
 * @brief Generic RNG interface for the HQC scheme.
 */

#ifndef HQC_RANDOMBYTES_H
#define HQC_RANDOMBYTES_H

#include <stddef.h>
#include <stdint.h>

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
#ifdef USE_OQS_RANDOMBYTES
#include <oqs/rand.h>
static inline int randombytes(uint8_t *out, size_t outlen) {
    OQS_randombytes(out, outlen);
    return 0;
}
#else
int randombytes(uint8_t *out, size_t outlen);
#endif

#endif  // HQC_RANDOMBYTES_H
