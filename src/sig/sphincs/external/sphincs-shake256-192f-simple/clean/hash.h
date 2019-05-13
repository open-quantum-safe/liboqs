#ifndef SPX_HASH_H
#define SPX_HASH_H

#include <stdint.h>

void PQCLEAN_SPHINCSSHAKE256192FSIMPLE_CLEAN_initialize_hash_function(
    const unsigned char *pub_seed, const unsigned char *sk_seed);

void PQCLEAN_SPHINCSSHAKE256192FSIMPLE_CLEAN_prf_addr(
    unsigned char *out, const unsigned char *key, const uint32_t addr[8]);

void PQCLEAN_SPHINCSSHAKE256192FSIMPLE_CLEAN_gen_message_random(
    unsigned char *R,
    const unsigned char *sk_prf, const unsigned char *optrand,
    const unsigned char *m, size_t mlen);

void PQCLEAN_SPHINCSSHAKE256192FSIMPLE_CLEAN_hash_message(
    unsigned char *digest, uint64_t *tree, uint32_t *leaf_idx,
    const unsigned char *R, const unsigned char *pk,
    const unsigned char *m, size_t mlen);

#endif
