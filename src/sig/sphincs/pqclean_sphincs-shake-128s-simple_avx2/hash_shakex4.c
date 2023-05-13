#include <stdint.h>
#include <string.h>

#include "hashx4.h"

#include "address.h"
#include "fips202x4.h"
#include "params.h"

extern void KeccakP1600times4_PermuteAll_24rounds(__m256i *s);

/*
 * 4-way parallel version of prf_addr; takes 4x as much input and output
 */
void prf_addrx4(unsigned char *out0,
                unsigned char *out1,
                unsigned char *out2,
                unsigned char *out3,
                const spx_ctx *ctx,
                const uint32_t addrx4[4 * 8]) {
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
    for (int i = 0; i < SPX_N / 8; i++) {
        state[SPX_N / 8 + i + 4] = _mm256_set1_epi64x(((int64_t *)ctx->sk_seed)[i]);
    }

    /* SHAKE domain separator and padding. */
    state[SPX_N / 4 + 4] = _mm256_set1_epi64x(0x1f);
    for (int i = SPX_N / 4 + 5; i < 16; i++) {
        state[i] = _mm256_set1_epi64x(0);
    }
    // shift unsigned and then cast to avoid UB
    state[16] = _mm256_set1_epi64x((long long)(0x80ULL << 56));

    for (int i = 17; i < 25; i++) {
        state[i] = _mm256_set1_epi64x(0);
    }

    KeccakP1600times4_PermuteAll_24rounds(&state[0]);

    for (int i = 0; i < SPX_N / 8; i++) {
        ((int64_t *)out0)[i] = _mm256_extract_epi64(state[i], 0);
        ((int64_t *)out1)[i] = _mm256_extract_epi64(state[i], 1);
        ((int64_t *)out2)[i] = _mm256_extract_epi64(state[i], 2);
        ((int64_t *)out3)[i] = _mm256_extract_epi64(state[i], 3);
    }
}
