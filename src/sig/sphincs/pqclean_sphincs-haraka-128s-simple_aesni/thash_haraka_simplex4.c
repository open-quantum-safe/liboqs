#include <stdint.h>
#include <string.h>

#include "address.h"
#include "haraka.h"
#include "params.h"
#include "thashx4.h"

/**
 * 4-way parallel version of thash; takes 4x as much input and output
 */
#define thashx4_variant(name, inblocks)                                                            \
    void PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_thashx4_##name(unsigned char *out0, unsigned char *out1, unsigned char *out2,         \
            unsigned char *out3, const unsigned char *in0,                         \
            const unsigned char *in1, const unsigned char *in2,                    \
            const unsigned char *in3, const unsigned char *pub_seed,               \
            uint32_t addrx4[4 * 8], const harakactx *state) {                      \
        unsigned char buf0[PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N];                                     \
        unsigned char buf1[PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N];                                     \
        unsigned char buf2[PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N];                                     \
        unsigned char buf3[PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N];                                     \
        unsigned char outbuf[32 * 4];                                                              \
        unsigned char buf_tmp[64 * 4];                                                             \
        \
        (void)pub_seed; /* Suppress an 'unused parameter' warning. */                              \
        \
        if ((inblocks) == 1) {                                                                     \
            memset(buf_tmp, 0, 64 * 4);                                                            \
            \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_addr_to_bytes(buf_tmp, addrx4 + 0 * 8);                                            \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_addr_to_bytes(buf_tmp + 64, addrx4 + 1 * 8);                                       \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_addr_to_bytes(buf_tmp + 128, addrx4 + 2 * 8);                                      \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_addr_to_bytes(buf_tmp + 192, addrx4 + 3 * 8);                                      \
            \
            memcpy(buf_tmp + PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES, in0, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                          \
            memcpy(buf_tmp + PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES + 64, in1, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                     \
            memcpy(buf_tmp + PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES + 128, in2, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                    \
            memcpy(buf_tmp + PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES + 192, in3, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                    \
            \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_haraka512x4(outbuf, buf_tmp, state);                                               \
            \
            memcpy(out0, outbuf, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                                           \
            memcpy(out1, outbuf + 32, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                                      \
            memcpy(out2, outbuf + 64, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                                      \
            memcpy(out3, outbuf + 96, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                                      \
        } else {                                                                                   \
            /* All other tweakable hashes*/                                                        \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_addr_to_bytes(buf0, addrx4 + 0 * 8);                                               \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_addr_to_bytes(buf1, addrx4 + 1 * 8);                                               \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_addr_to_bytes(buf2, addrx4 + 2 * 8);                                               \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_addr_to_bytes(buf3, addrx4 + 3 * 8);                                               \
            \
            memcpy(buf0 + PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES, in0, (inblocks)*PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                  \
            memcpy(buf1 + PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES, in1, (inblocks)*PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                  \
            memcpy(buf2 + PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES, in2, (inblocks)*PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                  \
            memcpy(buf3 + PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES, in3, (inblocks)*PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N);                                  \
            \
            PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_haraka_Sx4(out0, out1, out2, out3, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N, buf0, buf1, buf2, buf3,                  \
                    PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_ADDR_BYTES + (inblocks)*PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_N, state);                              \
        }                                                                                          \
    }

thashx4_variant(1, 1)
thashx4_variant(2, 2)
thashx4_variant(WOTS_LEN, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_WOTS_LEN)
thashx4_variant(FORS_TREES, PQCLEAN_SPHINCSHARAKA128SSIMPLE_AESNI_FORS_TREES)
