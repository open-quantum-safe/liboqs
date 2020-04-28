#ifndef PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_THASHX4_H
#define PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_THASHX4_H

#include <stdint.h>

#include "hash_state.h"

#define thashx4_header(inblocks)                                    \
    void PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thashx4_##inblocks(unsigned char *out0,                    \
            unsigned char *out1,                                   \
            unsigned char *out2,                                   \
            unsigned char *out3,                                   \
            const unsigned char *in0,                              \
            const unsigned char *in1,                              \
            const unsigned char *in2,                              \
            const unsigned char *in3,                              \
            const unsigned char *pub_seed, uint32_t addrx4[4*8],   \
            const hash_state *state_seeded)

thashx4_header(1);
thashx4_header(2);
thashx4_header(WOTS_LEN);
thashx4_header(FORS_TREES);

#endif
