#ifndef __THREE_BEARS_PAPABEAR_H__
#define __THREE_BEARS_PAPABEAR_H__

#include <stddef.h> /* for size_t */
#include <stdint.h>

#define PAPABEAR_KEYGEN_SEED_BYTES 40
#define PAPABEAR_PRIVATE_KEY_BYTES PAPABEAR_KEYGEN_SEED_BYTES
#define PAPABEAR_SHARED_SECRET_BYTES 32
#define PAPABEAR_ENC_SEED_AND_IV_BYTES 32
#define PAPABEAR_PUBLIC_KEY_BYTES 1584
#define PAPABEAR_CAPSULE_BYTES 1697

/**
 * Expand a secret seed to a public/private keypair.
 *
 * @param[out] pk The public key.
 * @param[in] sk The private key, which must be uniformly random.
 */
void PQCLEAN_PAPABEAR_CLEAN_get_pubkey (
    uint8_t pk[PAPABEAR_PUBLIC_KEY_BYTES],
    const uint8_t sk[PAPABEAR_PRIVATE_KEY_BYTES]
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
void PQCLEAN_PAPABEAR_CLEAN_encapsulate (
    uint8_t shared_secret[PAPABEAR_SHARED_SECRET_BYTES],
    uint8_t capsule[PAPABEAR_CAPSULE_BYTES],
    const uint8_t pk[PAPABEAR_PUBLIC_KEY_BYTES],
    const uint8_t seed[PAPABEAR_ENC_SEED_AND_IV_BYTES]
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
 */
void PQCLEAN_PAPABEAR_CLEAN_decapsulate (
    uint8_t shared_secret[PAPABEAR_SHARED_SECRET_BYTES],
    const uint8_t capsule[PAPABEAR_CAPSULE_BYTES],
    const uint8_t sk[PAPABEAR_PRIVATE_KEY_BYTES]
);

#endif
