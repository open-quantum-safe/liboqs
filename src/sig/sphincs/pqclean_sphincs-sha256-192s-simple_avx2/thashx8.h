#ifndef PQCLEAN_SPHINCSSHA256192SSIMPLE_AVX2_THASHX8_H
#define PQCLEAN_SPHINCSSHA256192SSIMPLE_AVX2_THASHX8_H

#include <stdint.h>

#include "hash_state.h"
#include "sha256avx.h"


#define thashx8_variant(name)                \
    void PQCLEAN_SPHINCSSHA256192SSIMPLE_AVX2_thashx8_##name(                 \
            unsigned char *out0,             \
            unsigned char *out1,             \
            unsigned char *out2,             \
            unsigned char *out3,             \
            unsigned char *out4,             \
            unsigned char *out5,             \
            unsigned char *out6,             \
            unsigned char *out7,             \
            const unsigned char *in0,        \
            const unsigned char *in1,        \
            const unsigned char *in2,        \
            const unsigned char *in3,        \
            const unsigned char *in4,        \
            const unsigned char *in5,        \
            const unsigned char *in6,        \
            const unsigned char *in7,        \
            const unsigned char *pub_seed,   \
            uint32_t addrx8[8*8],            \
            const hash_state *state_seeded)


thashx8_variant(1);
thashx8_variant(2);
thashx8_variant(WOTS_LEN);
thashx8_variant(FORS_TREES);

#undef thashx8_variant
#endif
