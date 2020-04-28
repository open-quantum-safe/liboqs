#include <stdint.h>
#include <string.h>

#include "address.h"
#include "params.h"
#include "thashx4.h"

#include "fips202x4.h"

/**
 * 4-way parallel version of thash; takes 4x as much input and output
 */
#define thashx4_variant(name, inblocks)                                                            \
    void PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_thashx4_##name(                                                                       \
            unsigned char *out0, unsigned char *out1, unsigned char *out2, unsigned char *out3,        \
            const unsigned char *in0, const unsigned char *in1, const unsigned char *in2,              \
            const unsigned char *in3, const unsigned char *pub_seed, uint32_t addrx4[4 * 8],           \
            const hash_state *state_seeded) {                                                          \
        unsigned char buf0[PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N];                             \
        unsigned char buf1[PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N];                             \
        unsigned char buf2[PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N];                             \
        unsigned char buf3[PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N];                             \
        \
        memcpy(buf0, pub_seed, PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N);                                                             \
        memcpy(buf1, pub_seed, PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N);                                                             \
        memcpy(buf2, pub_seed, PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N);                                                             \
        memcpy(buf3, pub_seed, PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N);                                                             \
        PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_addr_to_bytes(buf0 + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N, addrx4 + 0 * 8);                                           \
        PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_addr_to_bytes(buf1 + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N, addrx4 + 1 * 8);                                           \
        PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_addr_to_bytes(buf2 + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N, addrx4 + 2 * 8);                                           \
        PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_addr_to_bytes(buf3 + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N, addrx4 + 3 * 8);                                           \
        memcpy(buf0 + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_ADDR_BYTES, in0, (inblocks)*PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N);                              \
        memcpy(buf1 + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_ADDR_BYTES, in1, (inblocks)*PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N);                              \
        memcpy(buf2 + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_ADDR_BYTES, in2, (inblocks)*PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N);                              \
        memcpy(buf3 + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_ADDR_BYTES, in3, (inblocks)*PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N);                              \
        \
        PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_shake256x4(out0, out1, out2, out3, PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N, buf0, buf1, buf2, buf3,                      \
                PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_N);                                 \
        \
        /* Avoid unused parameter warning */                                                       \
        (void)state_seeded;                                                                        \
    }

thashx4_variant(1, 1)
thashx4_variant(2, 2)
thashx4_variant(WOTS_LEN, PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_WOTS_LEN)
thashx4_variant(FORS_TREES, PQCLEAN_SPHINCSSHAKE256128FSIMPLE_AVX2_FORS_TREES)
