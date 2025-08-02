/**
 * \file rand_nist.h
 * \brief NIST DRBG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef OQS_RAND_NIST_H
#define OQS_RAND_NIST_H

#include <stddef.h>
#include <stdint.h>

#include <oqs/common.h>

typedef struct {
	unsigned char Key[32];
	unsigned char V[16];
	unsigned long reseed_counter;
} OQS_NIST_DRBG_struct;

/* SP 800-90Ar1, Table 3 */
/* ctr_len = 32 bits */
#define OQS_RAND_NIST_MAX_REQUEST_SIZE (1UL << 16)
#define OQS_RAND_NIST_RESEED_INTERVAL  (1UL << 45)

/**
 * Initializes the NIST DRBG with a given seed and with 256-bit security.
 *
 * @param[in] entropy_input The seed; must be exactly 48 bytes
 * @param[in] personalization_string An optional personalization string;
 * may be NULL; if not NULL, must be at least 48 bytes long
 */
void OQS_randombytes_nist_kat_init_256bit(const uint8_t *entropy_input, const uint8_t *personalization_string);

/**
 * Fills the given memory with the requested number of pseudorandom bytes using the NIST DRBG.
 *
 * @warning The caller must ensure that `bytes_to_read` is not larger than
 * `OQS_RAND_NIST_MAX_REQUEST_SIZE`, and this function is not called more than
 * `OQS_RAND_NIST_RESEED_INTERVAL` times between successive reseeds using
 * `OQS_randombytes_nist_kat_init_256bit()`.
 *
 * @param[out] random_array Pointer to the memory to fill with (pseudo)random bytes
 * @param[in] bytes_to_read The number of random bytes to read into memory
 * @return OQS_SUCCESS on success, OQS_ERROR if the DBRG needs to be reseeded.
 */
void OQS_randombytes_nist_kat(uint8_t *random_array, size_t bytes_to_read);

/**
 * Writes the current state of the NIST DRBG into the provided memory.
 */
void OQS_randombytes_nist_kat_get_state(void *out);

/**
 * Overwrites the current state of the NIST DRBG from the provided memory.
 */
void OQS_randombytes_nist_kat_set_state(const void *in);

#endif // OQS_RAND_NIST_H
