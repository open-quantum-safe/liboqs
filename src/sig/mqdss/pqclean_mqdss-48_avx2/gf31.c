#include "params.h"
#include "fips202.h"
#include "gf31.h"
#include <immintrin.h>
#include <stdint.h>
#include <string.h>

/* Given a vector of N elements in the range [0, 31], this reduces the elements
   to the range [0, 30] by mapping 31 to 0 (i.e reduction mod 31) */
void PQCLEAN_MQDSS48_AVX2_vgf31_unique(gf31 *out, gf31 *in) {
    __m256i x;
    __m256i _w31 = _mm256_set1_epi16(31);
    int i;

    for (i = 0; i < (N >> 4); ++i) {
        x = _mm256_loadu_si256((__m256i const *) (in + 16 * i));
        x = _mm256_xor_si256(x, _mm256_and_si256(_w31, _mm256_cmpeq_epi16(x, _w31)));
        _mm256_storeu_si256((__m256i *)(out + i * 16), x);
    }
}

/* This function acts on vectors with 64 gf31 elements.
It performs one reduction step and guarantees output in [0, 30],
but requires input to be in [0, 32768). */
void PQCLEAN_MQDSS48_AVX2_vgf31_shorten_unique(gf31 *out, gf31 *in) {
    __m256i x;
    __m256i _w2114 = _mm256_set1_epi32(2114 * 65536 + 2114);
    __m256i _w31 = _mm256_set1_epi16(31);
    int i;

    for (i = 0; i < (N >> 4); ++i) {
        x = _mm256_loadu_si256((__m256i const *) (in + 16 * i));
        x = _mm256_sub_epi16(x, _mm256_mullo_epi16(_w31, _mm256_mulhi_epi16(x, _w2114)));
        x = _mm256_xor_si256(x, _mm256_and_si256(_w31, _mm256_cmpeq_epi16(x, _w31)));
        _mm256_storeu_si256((__m256i *)(out + i * 16), x);
    }
}

/* Given a seed, samples len gf31 elements (in the range [0, 30]), and places
   them in a vector of 16-bit elements */
void PQCLEAN_MQDSS48_AVX2_gf31_nrand(gf31 *out, size_t len, const uint8_t *seed, size_t seedlen) {
    size_t i = 0, j;
    shake256ctx shakestate;
    uint8_t shakeblock[SHAKE256_RATE];

    shake256_absorb(&shakestate, seed, seedlen);

    while (i < len) {
        shake256_squeezeblocks(shakeblock, 1, &shakestate);
        for (j = 0; j < SHAKE256_RATE && i < len; j++) {
            if ((shakeblock[j] & 31) != 31) {
                out[i] = (shakeblock[j] & 31);
                i++;
            }
        }
    }
    shake256_ctx_release(&shakestate);
}

/* Given a seed, samples len gf31 elements, transposed into unsigned range,
   i.e. in the range [-15, 15], and places them in an array of 8-bit integers.
   This is used for the expansion of F, which wants packed elements. */
void PQCLEAN_MQDSS48_AVX2_gf31_nrand_schar(signed char *out, size_t len, const uint8_t *seed, size_t seedlen) {
    size_t i = 0, j;
    shake256ctx shakestate;
    uint8_t shakeblock[SHAKE256_RATE];

    shake256_absorb(&shakestate, seed, seedlen);

    while (i < len) {
        shake256_squeezeblocks(shakeblock, 1, &shakestate);
        for (j = 0; j < SHAKE256_RATE && i < len; j++) {
            if ((shakeblock[j] & 31) != 31) {
                out[i] = (signed char)((shakeblock[j] & 31) - 15);
                i++;
            }
        }
    }
    shake256_ctx_release(&shakestate);

}

/* Unpacks an array of packed GF31 elements to one element per gf31.
   Assumes that there is sufficient empty space available at the end of the
   array to unpack. Can perform in-place. */
void PQCLEAN_MQDSS48_AVX2_gf31_nunpack(gf31 *out, const uint8_t *in, size_t n) {
    size_t i;
    size_t j = ((n * 5) >> 3) - 1;
    unsigned int d = 0;

    for (i = n; i > 0; i--) {
        out[i - 1] = (gf31)((in[j] >> d) & 31);
        d += 5;
        if (d > 8) {
            d -= 8;
            j--;
            out[i - 1] = (gf31)(out[i - 1] ^ ((in[j] << (5 - d)) & 31));
        }
    }
}

/* Packs an array of GF31 elements from gf31's to concatenated 5-bit values.
   Assumes that there is sufficient space available to unpack.
   Can perform in-place. */
void PQCLEAN_MQDSS48_AVX2_gf31_npack(uint8_t *out, const gf31 *in, size_t n) {
    unsigned int i = 0;
    unsigned int j;
    int d = 3;

    /* There will be ceil(5n / 8) output blocks */
    memset(out, 0, (size_t)((5 * n + 7) & ~7U) >> 3);

    for (j = 0; j < n; j++) {
        if (d < 0) {
            d += 8;
            out[i] = (uint8_t)((out[i] & (255 << (d - 3))) |
                               ((in[j] >> (8 - d)) & ~(255 << (d - 3))));
            i++;
        }
        out[i] = (uint8_t)((out[i] & ~(31 << d)) | ((in[j] << d) & (31 << d)));
        d -= 5;
    }
}
