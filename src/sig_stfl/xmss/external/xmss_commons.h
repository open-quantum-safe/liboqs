#ifndef XMSS_COMMONS_H
#define XMSS_COMMONS_H

#include <stdint.h>
#include "params.h"

/**
 * Computes the leaf at a given address. First generates the WOTS key pair,
 * then computes leaf using l_tree. As this happens position independent, we
 * only require that addr encodes the right ltree-address.
 */
#define gen_leaf_wots XMSS_INNER_NAMESPACE(gen_leaf_wots)
void gen_leaf_wots(const xmss_params *params, unsigned char *leaf,
                   const unsigned char *sk_seed, const unsigned char *pub_seed,
                   uint32_t ltree_addr[8], uint32_t ots_addr[8]);

/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
#define xmss_core_sign_open XMSS_INNER_NAMESPACE(xmss_core_sign_open)
int xmss_core_sign_open(const xmss_params *params,
                        const unsigned char *m, unsigned long long mlen,
                        const unsigned char *sm, unsigned long long smlen,
                        const unsigned char *pk);

/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
#define xmssmt_core_sign_open XMSS_INNER_NAMESPACE(xmssmt_core_sign_open)
int xmssmt_core_sign_open(const xmss_params *params,
                          const unsigned char *m, unsigned long long mlen,
                          const unsigned char *sm, unsigned long long smlen,
                          const unsigned char *pk);
#endif
