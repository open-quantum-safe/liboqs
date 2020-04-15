#include <stdint.h>
#include <string.h>

#include "thashx4.h"
#include "address.h"
#include "params.h"

#include "fips202x4.h"

/**
 * 4-way parallel version of thash; takes 4x as much input and output
 */
#define thash_variant(name, inblocks)                                                   \
    void PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_thashx4_##name(unsigned char *out0,                                            \
            unsigned char *out1,                                                       \
            unsigned char *out2,                                                       \
            unsigned char *out3,                                                       \
            const unsigned char *in0,                                                  \
            const unsigned char *in1,                                                  \
            const unsigned char *in2,                                                  \
            const unsigned char *in3,                                                  \
            const unsigned char *pub_seed, uint32_t addrx4[4*8],                       \
            const hash_state *state_seeded)                                            \
    {                                                                                       \
        unsigned char buf0[PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N];                      \
        unsigned char buf1[PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N];                      \
        unsigned char buf2[PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N];                      \
        unsigned char buf3[PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N];                      \
        unsigned char bitmask0[(inblocks) * PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N];                                         \
        unsigned char bitmask1[(inblocks) * PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N];                                         \
        unsigned char bitmask2[(inblocks) * PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N];                                         \
        unsigned char bitmask3[(inblocks) * PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N];                                         \
        unsigned int i;                                                                     \
        \
        memcpy(buf0, pub_seed, PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N);                                                      \
        memcpy(buf1, pub_seed, PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N);                                                      \
        memcpy(buf2, pub_seed, PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N);                                                      \
        memcpy(buf3, pub_seed, PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N);                                                      \
        PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_addr_to_bytes(buf0 + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N, addrx4 + 0*8);                                      \
        PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_addr_to_bytes(buf1 + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N, addrx4 + 1*8);                                      \
        PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_addr_to_bytes(buf2 + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N, addrx4 + 2*8);                                      \
        PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_addr_to_bytes(buf3 + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N, addrx4 + 3*8);                                      \
        \
        PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_shake256x4(bitmask0, bitmask1, bitmask2, bitmask3, (inblocks) * PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N,          \
                buf0, buf1, buf2, buf3, PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES);                     \
        \
        for (i = 0; i < (inblocks) * PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N; i++) {                                          \
            buf0[PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES + i] = in0[i] ^ bitmask0[i];                        \
            buf1[PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES + i] = in1[i] ^ bitmask1[i];                        \
            buf2[PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES + i] = in2[i] ^ bitmask2[i];                        \
            buf3[PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES + i] = in3[i] ^ bitmask3[i];                        \
        }                                                                                   \
        \
        PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_shake256x4(                                                                     \
                out0, out1, out2, out3, PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N,                                                  \
                buf0, buf1, buf2, buf3, PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N + PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_N);             \
        \
        /* avoid unused parameter warning */                                                \
        (void)state_seeded;                                                                 \
    }


thash_variant(1, 1)
thash_variant(2, 2)
thash_variant(WOTS_LEN, PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_WOTS_LEN)
thash_variant(FORS_TREES, PQCLEAN_SPHINCSSHAKE256192FROBUST_AVX2_FORS_TREES)
