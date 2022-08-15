#ifndef XMSS_COMMONS_H
#define XMSS_COMMONS_H

#include <stdint.h>
#include "params.h"

/**
 * Computes the leaf at a given address. First generates the WOTS key pair,
 * then computes leaf using l_tree. As this happens position independent, we
 * only require that addr encodes the right ltree-address.
 */
void gen_leaf_wots(const xmss_params *params, uint8_t *leaf,
                   const uint8_t *sk_seed, const uint8_t *pub_seed,
                   uint32_t ltree_addr[8], uint32_t ots_addr[8]);

/**
 * Used for pseudo-random key generation.
 * Generates the seed for the WOTS key pair at address 'addr'.
 *
 * Takes n-byte sk_seed and returns n-byte seed using 32 byte address 'addr'.
 */
void get_seed(const xmss_params *params, uint8_t *seed,
              const uint8_t *sk_seed, uint32_t addr[8]);

/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
int xmss_core_verify(const xmss_params *params,
                        uint8_t *m, unsigned long long *mlen,
                        const uint8_t *sm, unsigned long long smlen,
                        const uint8_t *pk);

/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
int xmssmt_core_verify(const xmss_params *params,
                          uint8_t *m, unsigned long long *mlen,
                          const uint8_t *sm, unsigned long long smlen,
                          const uint8_t *pk);
#endif
