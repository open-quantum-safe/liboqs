/**
 * \file rand.h
 * \brief Random number generator.
 */

#ifndef __OQS_RANDOM_H
#define __OQS_RANDOM_H

#include <stddef.h>
#include <stdint.h>

/**
 * Fills the given memory with the requested number of random bytes.
 *
 * This implementation reads the bytes directly from `/dev/urandom` and will
 * block until the requested number of bytes have been read.
 *
 * The caller is responsible for providing a buffer allocated with sufficient room.
 *
 * @param[out] random_array Pointer to the memory to fill with random bytes
 * @param[in] bytes_to_read The number of random bytes to read into memory
 */
void OQS_randombytes(uint8_t *random_array, size_t bytes_to_read);

#endif // __OQS_RANDOM_H
