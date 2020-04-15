#include <stdint.h>
#include <string.h>

#include "address.h"
#include "hash_state.h"
#include "params.h"
#include "sha256.h"
#include "sha256avx.h"
#include "sha256x8.h"
#include "thashx8.h"
#include "utils.h"

/**
 * 8-way parallel version of thash; takes 8x as much input and output
 */
static void thashx8(unsigned char *out0,
                    unsigned char *out1,
                    unsigned char *out2,
                    unsigned char *out3,
                    unsigned char *out4,
                    unsigned char *out5,
                    unsigned char *out6,
                    unsigned char *out7,
                    const unsigned char *in0,
                    const unsigned char *in1,
                    const unsigned char *in2,
                    const unsigned char *in3,
                    const unsigned char *in4,
                    const unsigned char *in5,
                    const unsigned char *in6,
                    const unsigned char *in7, unsigned int inblocks,
                    const unsigned char *pub_seed, uint32_t addrx8[8 * 8],
                    uint8_t *bufx8,
                    const hash_state *state_seeded) {
    unsigned char outbufx8[8 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES];
    unsigned int i;
    sha256ctxx8 ctx;

    (void)pub_seed; /* Suppress an 'unused parameter' warning. */

    PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_sha256_clone_statex8(&ctx, &state_seeded->x8);

    for (i = 0; i < 8; i++) {
        PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_compress_address(bufx8 + i * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N),
                addrx8 + i * 8);
    }

    memcpy(bufx8 + PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES +
           0 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N), in0, inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(bufx8 + PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES +
           1 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N), in1, inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(bufx8 + PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES +
           2 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N), in2, inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(bufx8 + PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES +
           3 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N), in3, inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(bufx8 + PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES +
           4 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N), in4, inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(bufx8 + PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES +
           5 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N), in5, inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(bufx8 + PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES +
           6 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N), in6, inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(bufx8 + PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES +
           7 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N), in7, inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);

    PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_sha256_update8x(&ctx,
            bufx8 + 0 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N),
            bufx8 + 1 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N),
            bufx8 + 2 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N),
            bufx8 + 3 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N),
            bufx8 + 4 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N),
            bufx8 + 5 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N),
            bufx8 + 6 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N),
            bufx8 + 7 * (PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N),
            PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);

    PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_sha256_final8x(&ctx,
            outbufx8 + 0 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES,
            outbufx8 + 1 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES,
            outbufx8 + 2 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES,
            outbufx8 + 3 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES,
            outbufx8 + 4 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES,
            outbufx8 + 5 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES,
            outbufx8 + 6 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES,
            outbufx8 + 7 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES);

    memcpy(out0, outbufx8 + 0 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(out1, outbufx8 + 1 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(out2, outbufx8 + 2 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(out3, outbufx8 + 3 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(out4, outbufx8 + 4 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(out5, outbufx8 + 5 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(out6, outbufx8 + 6 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
    memcpy(out7, outbufx8 + 7 * PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N);
}

#define thashx8_variant_impl(name, size)                                      \
    void PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_thashx8_##name(unsigned char *out0,                              \
            unsigned char *out1,                                       \
            unsigned char *out2,                                       \
            unsigned char *out3,                                       \
            unsigned char *out4,                                       \
            unsigned char *out5,                                       \
            unsigned char *out6,                                       \
            unsigned char *out7,                                       \
            const unsigned char *in0,                                  \
            const unsigned char *in1,                                  \
            const unsigned char *in2,                                  \
            const unsigned char *in3,                                  \
            const unsigned char *in4,                                  \
            const unsigned char *in5,                                  \
            const unsigned char *in6,                                  \
            const unsigned char *in7,                                  \
            const unsigned char *pub_seed,                             \
            uint32_t addrx8[8*8],                                      \
            const hash_state *state_seeded)                            \
    {                                                                         \
        const unsigned int inblocks = (size);                                 \
        uint8_t bufx8[8*(PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_ADDR_BYTES + (size)*PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N)];              \
        thashx8(out0, out1, out2, out3, out4, out5, out6, out7,               \
                in0, in1, in2, in3, in4, in5, in6, in7, inblocks,             \
                pub_seed, addrx8, bufx8, state_seeded);                       \
    }

thashx8_variant_impl(1, 1)
thashx8_variant_impl(2, 2)
thashx8_variant_impl(WOTS_LEN, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_WOTS_LEN)
thashx8_variant_impl(FORS_TREES, PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_FORS_TREES)

#undef thashx8_variant_impl
