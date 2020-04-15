#ifndef PQCLEAN_SPHINCSSHAKE256256SROBUST_AVX2_HASHX4_H
#define PQCLEAN_SPHINCSSHAKE256256SROBUST_AVX2_HASHX4_H

#include <stdint.h>

#include "hash_state.h"

void PQCLEAN_SPHINCSSHAKE256256SROBUST_AVX2_prf_addrx4(unsigned char *out0,
        unsigned char *out1,
        unsigned char *out2,
        unsigned char *out3,
        const unsigned char *key,
        const uint32_t addrx4[4 * 8],
        const hash_state *state_seeded);

#endif
