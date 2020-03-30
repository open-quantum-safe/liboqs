#ifndef __THREE_BEARS_MAMABEAREPHEM_H__
#define __THREE_BEARS_MAMABEAREPHEM_H__

#include <stddef.h> /* for size_t */
#include <stdint.h>

#define MAMABEAREPHEM_KEYGEN_SEED_BYTES 40
#define MAMABEAREPHEM_PRIVATE_KEY_BYTES MAMABEAREPHEM_KEYGEN_SEED_BYTES
#define MAMABEAREPHEM_SHARED_SECRET_BYTES 32
#define MAMABEAREPHEM_ENC_SEED_AND_IV_BYTES 32
#define MAMABEAREPHEM_PUBLIC_KEY_BYTES 1194
#define MAMABEAREPHEM_CAPSULE_BYTES 1307

/**
 * Expand a secret seed to a public/private keypair.
 *
 * @param[out] pk The public key.
 * @param[in] sk The private key, which must be uniformly random.
 */
void PQCLEAN_MAMABEAREPHEM_CLEAN_get_pubkey (
    uint8_t pk[MAMABEAREPHEM_PUBLIC_KEY_BYTES],
    const uint8_t sk[MAMABEAREPHEM_PRIVATE_KEY_BYTES]
);

/**
 * Create a shared secret using a random seed and another party's public key.
 *
 * Input and output parameters may not alias.
 *
 * @param[out] shared_secret The shared secret key.
 * @param[out] capsule A ciphertext to send to the other party.
 * @param[in] pk The other party's public key.
 * @param[in] seed A random seed.
 */
void PQCLEAN_MAMABEAREPHEM_CLEAN_encapsulate (
    uint8_t shared_secret[MAMABEAREPHEM_SHARED_SECRET_BYTES],
    uint8_t capsule[MAMABEAREPHEM_CAPSULE_BYTES],
    const uint8_t pk[MAMABEAREPHEM_PUBLIC_KEY_BYTES],
    const uint8_t seed[MAMABEAREPHEM_ENC_SEED_AND_IV_BYTES]
);

/**
 * Extract the shared secret from a capsule using the private key.
 * Has a negligible but nonzero probability of failure.
 *
 * Input and output parameters may not alias.
 *
 * @param[out] shared_secret The shared secret.
 * @param[in] capsule The capsule produced by encapsulate_cca2.
 * @param[in] sk The private key.
 * @return -1 on failure, 0 on success.
 * @warning The value of shared_secret must not be used on failure
 */
void PQCLEAN_MAMABEAREPHEM_CLEAN_decapsulate (
    uint8_t shared_secret[MAMABEAREPHEM_SHARED_SECRET_BYTES],
    const uint8_t capsule[MAMABEAREPHEM_CAPSULE_BYTES],
    const uint8_t sk[MAMABEAREPHEM_PRIVATE_KEY_BYTES]
);

#endif
