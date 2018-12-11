/** Public header */
#ifndef __THREE_BEARS_BABYBEAR_H__
#define __THREE_BEARS_BABYBEAR_H__

#include <stdint.h>
#include <stddef.h> /* for size_t */

#define BABYBEAR_KEYGEN_SEED_BYTES 40
#define BABYBEAR_PRIVATE_KEY_BYTES BABYBEAR_KEYGEN_SEED_BYTES
#define BABYBEAR_SHARED_SECRET_BYTES 32
#define BABYBEAR_ENC_SEED_AND_IV_BYTES 32
#define BABYBEAR_PUBLIC_KEY_BYTES 804
#define BABYBEAR_CAPSULE_BYTES 917

/**
 * Expand a secret seed to a public/private keypair.
 *
 * @param[out] pk The public key.
 * @param[in] sk The private key, which must be uniformly random.
 */
void BabyBear_get_pubkey (
    uint8_t pk[BABYBEAR_PUBLIC_KEY_BYTES],
    const uint8_t sk[BABYBEAR_PRIVATE_KEY_BYTES]
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
void BabyBear_encapsulate (
    uint8_t shared_secret[BABYBEAR_SHARED_SECRET_BYTES],
    uint8_t capsule[BABYBEAR_CAPSULE_BYTES],
    const uint8_t pk[BABYBEAR_PUBLIC_KEY_BYTES],
    const uint8_t seed[BABYBEAR_ENC_SEED_AND_IV_BYTES]
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
int __attribute__((warn_unused_result)) BabyBear_decapsulate ( 
    uint8_t shared_secret[BABYBEAR_SHARED_SECRET_BYTES],
    const uint8_t capsule[BABYBEAR_CAPSULE_BYTES],
    const uint8_t sk[BABYBEAR_PRIVATE_KEY_BYTES]
);

void secure_bzero (void *s,size_t size);

#endif /*__THREE_BEARS_BABYBEAR_H__*/

