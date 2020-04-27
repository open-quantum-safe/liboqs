#ifndef PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_THASH_H
#define PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_THASH_H

#include "hash_state.h"

#include <stdint.h>

void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_thash_1(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded);

void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_thash_2(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded);

void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_thash_WOTS_LEN(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded);

void PQCLEAN_SPHINCSSHA256256FROBUST_AVX2_thash_FORS_TREES(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded);

#endif
