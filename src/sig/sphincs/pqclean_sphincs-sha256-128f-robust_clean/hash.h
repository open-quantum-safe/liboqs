#ifndef SPX_HASH_H
#define SPX_HASH_H

#include "hash_state.h"

#include <stddef.h>
#include <stdint.h>

void PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_initialize_hash_function(
    hash_state *hash_state_seeded,
    const unsigned char *pub_seed, const unsigned char *sk_seed);

void PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_prf_addr(
    unsigned char *out, const unsigned char *key, const uint32_t addr[8],
    const hash_state *hash_state_seeded);

void PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_gen_message_random(
    unsigned char *R,
    const unsigned char *sk_prf, const unsigned char *optrand,
    const unsigned char *m, size_t mlen,
    const hash_state *hash_state_seeded);

void PQCLEAN_SPHINCSSHA256128FROBUST_CLEAN_hash_message(
    unsigned char *digest, uint64_t *tree, uint32_t *leaf_idx,
    const unsigned char *R, const unsigned char *pk,
    const unsigned char *m, size_t mlen,
    const hash_state *hash_state_seeded);

#endif
