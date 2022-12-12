#include "crypto_encode_761x3.h"
#include <immintrin.h>
#define uint8 uint8_t

#define p 761
#define loops 6
#define overshoot 2

static const union {
    uint8 init[32];
    __m256i val;
} lobytes_buf = { .init = {
        255, 0, 255, 0, 255, 0, 255, 0,
        255, 0, 255, 0, 255, 0, 255, 0,
        255, 0, 255, 0, 255, 0, 255, 0,
        255, 0, 255, 0, 255, 0, 255, 0,
    }
};
#define lobytes (lobytes_buf.val)

void PQCLEAN_SNTRUP761_AVX2_crypto_encode_761x3(unsigned char *s, const void *v) {
    const uint8 *f = v;
    int loop;
    const uint8 *nextf = f + 128 - 4 * overshoot;
    unsigned char *nexts = s + 32 - overshoot;

    for (loop = loops; loop > 0; --loop) {
        __m256i f0 = _mm256_loadu_si256((const __m256i *) (f + 0));
        __m256i f1 = _mm256_loadu_si256((const __m256i *) (f + 32));
        __m256i f2 = _mm256_loadu_si256((const __m256i *) (f + 64));
        __m256i f3 = _mm256_loadu_si256((const __m256i *) (f + 96));
        f = nextf;
        nextf += 128;

        __m256i a0 = _mm256_packus_epi16(f0 & lobytes, f1 & lobytes);
        /* 0 2 4 6 8 10 12 14 32 34 36 38 40 42 44 46 */
        /* 16 18 20 22 24 26 28 30 48 50 52 54 56 58 60 62 */
        __m256i a1 = _mm256_packus_epi16(_mm256_srli_epi16(f0, 8), _mm256_srli_epi16(f1, 8));
        /* 1 3 ... */
        __m256i a2 = _mm256_packus_epi16(f2 & lobytes, f3 & lobytes);
        __m256i a3 = _mm256_packus_epi16(_mm256_srli_epi16(f2, 8), _mm256_srli_epi16(f3, 8));

        a0 = _mm256_add_epi8(a0, _mm256_slli_epi16(a1 & _mm256_set1_epi8(63), 2));
        a2 = _mm256_add_epi8(a2, _mm256_slli_epi16(a3 & _mm256_set1_epi8(63), 2));

        __m256i b0 = _mm256_packus_epi16(a0 & lobytes, a2 & lobytes);
        /* 0 4 8 12 32 36 40 44 64 68 72 76 96 100 104 108 */
        /* 16 20 24 28 48 52 56 60 80 84 88 92 112 116 120 124 */
        __m256i b2 = _mm256_packus_epi16(_mm256_srli_epi16(a0, 8), _mm256_srli_epi16(a2, 8));
        /* 2 6 ... */

        b0 = _mm256_add_epi8(b0, _mm256_slli_epi16(b2 & _mm256_set1_epi8(15), 4));

        b0 = _mm256_permutevar8x32_epi32(b0, _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0));

        b0 = _mm256_add_epi8(b0, _mm256_set1_epi8(85));

        _mm256_storeu_si256((__m256i *) s, b0);
        s = nexts;
        nexts += 32;
    }

    *s++ = *f++ + 1;
}
