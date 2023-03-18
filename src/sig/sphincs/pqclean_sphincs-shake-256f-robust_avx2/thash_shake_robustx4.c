#include <stdint.h>
#include <string.h>

#include "thashx4.h"

#include "address.h"
#include "params.h"
#include "utils.h"

#include "fips202x4.h"

extern void KeccakP1600times4_PermuteAll_24rounds(__m256i *s);

/**
 * 4-way parallel version of thash; takes 4x as much input and output
 */
void thashx4(unsigned char *out0,
             unsigned char *out1,
             unsigned char *out2,
             unsigned char *out3,
             const unsigned char *in0,
             const unsigned char *in1,
             const unsigned char *in2,
             const unsigned char *in3, unsigned int inblocks,
             const spx_ctx *ctx, uint32_t addrx4[4 * 8]) {
    if (inblocks == 1 || inblocks == 2) {
        /* As we write and read only a few quadwords, it is more efficient to
         * build and extract from the fourway SHAKE256 state by hand. */
        __m256i state[25];
        for (int i = 0; i < SPX_N / 8; i++) {
            state[i] = _mm256_set1_epi64x(((int64_t *)ctx->pub_seed)[i]);
        }
        for (int i = 0; i < 4; i++) {
            state[SPX_N / 8 + i] = _mm256_set_epi32(
                                       (int)addrx4[3 * 8 + 1 + 2 * i],
                                       (int)addrx4[3 * 8 + 2 * i],
                                       (int)addrx4[2 * 8 + 1 + 2 * i],
                                       (int)addrx4[2 * 8 + 2 * i],
                                       (int)addrx4[8 + 1 + 2 * i],
                                       (int)addrx4[8 + 2 * i],
                                       (int)addrx4[1 + 2 * i],
                                       (int)addrx4[2 * i]
                                   );
        }

        /* SHAKE domain separator and padding */
        state[SPX_N / 8 + 4] = _mm256_set1_epi64x(0x1f);
        for (int i = SPX_N / 8 + 5; i < 16; i++) {
            state[i] = _mm256_set1_epi64x(0);
        }
        state[16] = _mm256_set1_epi64x((long long)(0x80ULL << 56));

        for (int i = 17; i < 25; i++) {
            state[i] = _mm256_set1_epi64x(0);
        }

        /* We will permutate state2 with f1600x4 to compute the bitmask,
         * but first we'll copy it to state2 which will be used to compute
         * the final output, as its input is alsmost identical. */
        __m256i state2[25];
        memcpy(state2, state, 800);

        KeccakP1600times4_PermuteAll_24rounds(&state[0]);

        /* By copying from state, state2 already contains the pub_seed
         * and addres.  We just need to copy in the input blocks xorred with
         * the bitmask we just computed. */
        for (unsigned int i = 0; i < (SPX_N / 8) * inblocks; i++) {
            state2[SPX_N / 8 + 4 + i] = _mm256_xor_si256(
                                            state[i],
                                            _mm256_set_epi64x(
                                                ((int64_t *)in3)[i],
                                                ((int64_t *)in2)[i],
                                                ((int64_t *)in1)[i],
                                                ((int64_t *)in0)[i]
                                            )
                                        );
        }

        /* Domain separator and start of padding.  Note that the quadwords
         * around are already zeroed for state from which we copied.
         * We do a XOR instead of a set as this might be the 16th quadword
         * when N=32 and inblocks=2, which already contains the end
         * of the padding. */
        state2[(SPX_N / 8) * (1 + inblocks) + 4] = _mm256_xor_si256(
                    state2[(SPX_N / 8) * (1 + inblocks) + 4],
                    _mm256_set1_epi64x(0x1f)
                );

        KeccakP1600times4_PermuteAll_24rounds(&state2[0]);

        for (int i = 0; i < SPX_N / 8; i++) {
            ((int64_t *)out0)[i] = _mm256_extract_epi64(state2[i], 0);
            ((int64_t *)out1)[i] = _mm256_extract_epi64(state2[i], 1);
            ((int64_t *)out2)[i] = _mm256_extract_epi64(state2[i], 2);
            ((int64_t *)out3)[i] = _mm256_extract_epi64(state2[i], 3);
        }
    } else {
        PQCLEAN_VLA(unsigned char, buf0, SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N);
        PQCLEAN_VLA(unsigned char, buf1, SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N);
        PQCLEAN_VLA(unsigned char, buf2, SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N);
        PQCLEAN_VLA(unsigned char, buf3, SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N);
        PQCLEAN_VLA(unsigned char, bitmask0, inblocks * SPX_N);
        PQCLEAN_VLA(unsigned char, bitmask1, inblocks * SPX_N);
        PQCLEAN_VLA(unsigned char, bitmask2, inblocks * SPX_N);
        PQCLEAN_VLA(unsigned char, bitmask3, inblocks * SPX_N);
        unsigned int i;

        memcpy(buf0, ctx->pub_seed, SPX_N);
        memcpy(buf1, ctx->pub_seed, SPX_N);
        memcpy(buf2, ctx->pub_seed, SPX_N);
        memcpy(buf3, ctx->pub_seed, SPX_N);
        memcpy(buf0 + SPX_N, addrx4 + 0 * 8, SPX_ADDR_BYTES);
        memcpy(buf1 + SPX_N, addrx4 + 1 * 8, SPX_ADDR_BYTES);
        memcpy(buf2 + SPX_N, addrx4 + 2 * 8, SPX_ADDR_BYTES);
        memcpy(buf3 + SPX_N, addrx4 + 3 * 8, SPX_ADDR_BYTES);

        shake256x4(bitmask0, bitmask1, bitmask2, bitmask3, inblocks * SPX_N,
                   buf0, buf1, buf2, buf3, SPX_N + SPX_ADDR_BYTES);

        for (i = 0; i < inblocks * SPX_N; i++) {
            buf0[SPX_N + SPX_ADDR_BYTES + i] = in0[i] ^ bitmask0[i];
            buf1[SPX_N + SPX_ADDR_BYTES + i] = in1[i] ^ bitmask1[i];
            buf2[SPX_N + SPX_ADDR_BYTES + i] = in2[i] ^ bitmask2[i];
            buf3[SPX_N + SPX_ADDR_BYTES + i] = in3[i] ^ bitmask3[i];
        }

        shake256x4(out0, out1, out2, out3, SPX_N,
                   buf0, buf1, buf2, buf3, SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N);
    }
}
