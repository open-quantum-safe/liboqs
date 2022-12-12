#include "crypto_decode_761x3.h"
#include <immintrin.h>
#define uint8 uint8_t

#define p 761
#define loops 6
#define overshoot 2

void PQCLEAN_SNTRUP761_AVX2_crypto_decode_761x3(void *v, const unsigned char *s) {
    uint8 *f = v;
    int loop;
    uint8 *nextf = f + 128 - 4 * overshoot;
    const unsigned char *nexts = s + 32 - overshoot;

    for (loop = loops; loop > 0; --loop) {
        __m256i s0 = _mm256_loadu_si256((const __m256i *) s);
        s = nexts;
        nexts += 32;

        __m256i s1 = _mm256_srli_epi16(s0 & _mm256_set1_epi8(-16), 4);
        s0 &= _mm256_set1_epi8(15);

        __m256i a0 = _mm256_unpacklo_epi8(s0, s1);
        /* 0 0>>4 1 1>>4 2 2>>4 3 3>>4 4 4>>4 5 5>>4 6 6>>4 7 7>>4 */
        /* 16 16>>4 ... */
        __m256i a1 = _mm256_unpackhi_epi8(s0, s1);
        /* 8 8>>4 9 9>>4 10 10>>4 ... */
        /* 24 24>>4 ... */

        __m256i a2 = _mm256_srli_epi16(a0 & _mm256_set1_epi8(12), 2);
        __m256i a3 = _mm256_srli_epi16(a1 & _mm256_set1_epi8(12), 2);
        a0 &= _mm256_set1_epi8(3);
        a1 &= _mm256_set1_epi8(3);

        __m256i b0 = _mm256_unpacklo_epi8(a0, a2);
        /* 0 0>>2 0>>4 0>>6 1 1>>2 1>>4 1>>6 */
        /* 2 2>>2 2>>4 2>>6 3 3>>2 3>>4 3>.6 */
        /* 16 16>>2 16>>4 16>>6 ... */
        __m256i b2 = _mm256_unpackhi_epi8(a0, a2);
        /* 4 4>>2 ... */
        __m256i b1 = _mm256_unpacklo_epi8(a1, a3);
        /* 8 8>>2 ... */
        __m256i b3 = _mm256_unpackhi_epi8(a1, a3);
        /* 12 12>>2 ... */

        __m256i f0 = _mm256_permute2x128_si256(b0, b2, 0x20);
        __m256i f2 = _mm256_permute2x128_si256(b0, b2, 0x31);
        __m256i f1 = _mm256_permute2x128_si256(b1, b3, 0x20);
        __m256i f3 = _mm256_permute2x128_si256(b1, b3, 0x31);

        f0 = _mm256_add_epi8(f0, _mm256_set1_epi8(-1));
        f1 = _mm256_add_epi8(f1, _mm256_set1_epi8(-1));
        f2 = _mm256_add_epi8(f2, _mm256_set1_epi8(-1));
        f3 = _mm256_add_epi8(f3, _mm256_set1_epi8(-1));

        _mm256_storeu_si256((__m256i *) (f + 0), f0);
        _mm256_storeu_si256((__m256i *) (f + 32), f1);
        _mm256_storeu_si256((__m256i *) (f + 64), f2);
        _mm256_storeu_si256((__m256i *) (f + 96), f3);
        f = nextf;
        nextf += 128;
    }

    *f = (uint8) ((*s & 3) - 1);
}
