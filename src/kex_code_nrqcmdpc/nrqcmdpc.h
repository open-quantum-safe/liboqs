#ifndef OQS_KEX_CODE_NRQCMDPC_QCMDPC_H
#define OQS_KEX_CODE_NRQCMDPC_QCMDPC_H

#include <oqs/rand.h>

#include "types.h"

/**
 * Generate key-pair
 *
 * @param[out] pub_key   Public key
 * @param[out] priv_key  Private key
 * @param[in]  rand      OQS_RAND object for random number generation
 * @return 0
 */
int oqs_kex_code_nrqcmdpc_keypair(
    byte_t *pub_key,
    par_ch_t priv_key,
    OQS_RAND *rand);

/**
 * Generate random secret key and encrypt using KEM encryption
 *
 * @param[out] sec_key   Secret key (plaintext)
 * @param[out] ct        Ciphertext
 * @param[in]  pub_key   Public key
 * @param[in]  rand      OQS_RAND object for random number generation
 */
void oqs_kex_code_nrqcmdpc_encrypt(
    byte_t *sec_key,
    byte_t *ct,
    const byte_t *pub_key,
    OQS_RAND *rand);

/**
 * Verify/decrypt a secret key
 *
 * @param[out] sec_key   Secret key (plaintext)
 * @param[in]  ct        Ciphertext
 * @param[in]  priv_key  Private key
 *
 * @return 1 upon successful decryption, 0 otherwise
 */
int oqs_kex_code_nrqcmdpc_decrypt(
    byte_t *sec_key,
    const byte_t *ct,
    const par_ch_t priv_key);

#endif /* OQS_KEX_CODE_NRQCMDPC_QCMDPC_H */
