// SPDX-License-Identifier: Apache-2.0

#ifndef rng_h
#define rng_h

#include <sqisign_namespace.h>

/**
 * Randombytes initialization.
 * Initialization may be needed for some random number generators (e.g. CTR-DRBG).
 *
 * @param[in] entropy_input 48 bytes entropy input
 * @param[in] personalization_string Personalization string
 * @param[in] security_strength Security string
 */
SQISIGN_API
void randombytes_init(unsigned char *entropy_input,
                      unsigned char *personalization_string,
                      int security_strength);

/**
 * Random byte generation using /dev/urandom.
 * The caller is responsible to allocate sufficient memory to hold x.
 *
 * @param[out] x Memory to hold the random bytes.
 * @param[in] xlen Number of random bytes to be generated
 * @return int 0 on success, -1 otherwise
 */
SQISIGN_API
int randombytes_select(unsigned char *x, unsigned long long xlen);

/**
 * Random byte generation.
 * The caller is responsible to allocate sufficient memory to hold x.
 *
 * @param[out] x Memory to hold the random bytes.
 * @param[in] xlen Number of random bytes to be generated
 * @return int 0 on success, -1 otherwise
 */
SQISIGN_API
int randombytes(unsigned char *x, unsigned long long xlen);

#endif /* rng_h */
