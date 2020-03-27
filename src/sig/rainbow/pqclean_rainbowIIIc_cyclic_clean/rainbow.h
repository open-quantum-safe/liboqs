#ifndef _RAINBOW_H_
#define _RAINBOW_H_
/// @file rainbow.h
/// @brief APIs for rainbow.
///

#include "rainbow_config.h"
#include "rainbow_keypair.h"

#include <stdint.h>

///
/// @brief Signing function for classical secret key.
///
/// @param[out] signature - the signature.
/// @param[in]  sk        - the secret key.
/// @param[in]  digest    - the digest.
///
int PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_rainbow_sign(uint8_t *signature, const sk_t *sk, const uint8_t *digest);

///
/// @brief Verifying function.
///
/// @param[in]  digest    - the digest.
/// @param[in]  signature - the signature.
/// @param[in]  pk        - the public key.
/// @return 0 for successful verified. -1 for failed verification.
///
int PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_rainbow_verify(const uint8_t *digest, const uint8_t *signature, const pk_t *pk);


///
/// @brief Verifying function for cyclic public keys.
///
/// @param[in]  digest    - the digest.
/// @param[in]  signature - the signature.
/// @param[in]  pk        - the public key of cyclic rainbow.
/// @return 0 for successful verified. -1 for failed verification.
///
int PQCLEAN_RAINBOWIIICCYCLIC_CLEAN_rainbow_verify_cyclic(const uint8_t *digest, const uint8_t *signature, const cpk_t *pk);

#endif // _RAINBOW_H_
